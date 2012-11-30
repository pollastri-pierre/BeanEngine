//
//  BServerHandler.cpp
//  NetworkTest
//
//  Created by Pierre POLLASTRI on 10/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "BServerHandler.h"
#include "BNetworkEngine.h"
#include "BNetworkBuffer.h"
#include <application/Log.h> 

using namespace boost::asio::ip;

BServerHandler::BServerHandler(BNetworkEngine* engine, const BNetworkGame& game) : _engine(engine), BMultiNetworkEventHandler<BServerHandler>(this), _game(game)
{
    _broadcast = NULL;
    _tcp = NULL;
    _udp = NULL;
    Log::debug("Game for %i", game.getMaximumPlayerAllowed());
    _engine->setConnected(true);
}

void BServerHandler::startBroadcastGameService()
{
    _broadcast = new BMulticastSocket(_engine->io_service());
    _broadcast->setEventHandler(this);
    _broadcast->start();
    registerPackageListener(_broadcast, &BServerHandler::onBroadcastPackage, &_broadcastBuffer, &_broadcastPackage);
}

void BServerHandler::startTcpService()
{
    _tcp = new BTcpSocket(_engine->io_service());
    _tcp->setEventHandler(this);
    _players[_tcp] = _engine->me();
    _engine->me().setIp("127.0.0.1");
    _engine->me().setPlayerNumber(1);
    _game.setNbrPlayer(1);
    _tcp->accept();
    registerAcceptListener(_tcp, &BServerHandler::onConnect);
}

void BServerHandler::startUdpService()
{
    _udp = new BUdpSocket(_engine->io_service());
    _udp->setEventHandler(this);
    registerPackageListener(_udp, &BServerHandler::onUdpPackage, &_udpBuffer, &_udpPackage);
    _udp->start();
}

void BServerHandler::onBroadcastPackage(BSocket* sender, BPackage& package)
{
    if (package.header().isClientHeader())
    {
        if (package.header().type & BNetworkHeader::GAME_INFO && _game.getNbrPlayers() < _game.getMaximumPlayerAllowed())
        {
            BPackage p;
            p.header().setType(BNetworkHeader::SERVER, BNetworkHeader::GAME_INFO);
            _game.setNbrPlayer(_players.size());
            _game >> p.data();
            _broadcast->send(p);
        }
        else if (!_udp && package.header().type & BNetworkHeader::MESSAGE && package.data().find("senderId") != package.data().end())
        {
            //dispatchUdpMessage(dynamic_cast<BUdpSocket*>(sender), package);
        }
    }
}

void BServerHandler::dispatchUdpMessage(BUdpSocket *sender, BPackage &package)
{
    if (!sender)
        return ;
    package.header().setType(BNetworkHeader::SERVER, BNetworkHeader::MESSAGE);
    BNetworkPlayerMap::iterator it = _players.begin();
    BNetworkPlayerMap::iterator ite = _players.end();
    unsigned int id = package.data()["senderId"].toInteger();
    for (; it != ite; ++it)
    {
        if (it->second.playerNumber() == id || it->second.playerNumber() == 1)
            continue;
        _udp->send(package, it->second.udpEndpoint());
    }
    _broadcast->send(package);
    if (id != 1)
        _engine->pushUdpMessage(package);
}

void BServerHandler::onUdpPackage(BSocket *sender, BPackage &package)
{
    if (package.header().isClientHeader())
    {
        if (package.header().type & BNetworkHeader::MESSAGE && package.data().find("senderId") != package.data().end())
        {
            dispatchUdpMessage(dynamic_cast<BUdpSocket*>(sender), package);
        }
    }
}

void BServerHandler::sendPlayerInfoOnConnect(BSocket* sender, BPackage& package)
{
    BNetworkPlayer player = BNetworkPlayer(package.data()["player_name"].toString());
    player.setReady(false);
    player.setOnline(true);
    player.setIp(((BTcpSocket*)sender)->remoteEndpoint().address().to_string());
    player.setPlayerNumber((int)getAvailablePlayerNumber());
    Log::debug("Connection of player %i", player.playerNumber());
    player.setUdpEndpoint(udp::endpoint(address::from_string(player.getIp()), _engine->udpPort()));
    _players[sender] = player;
    _game.setNbrPlayer(_players.size());
    BPackage p;
    p.header().setType(BNetworkHeader::SERVER, BNetworkHeader::JOIN);
    p.data()["source"] = 0;
    p.data()["player"] = (int)player.playerNumber();
    p.data()["game_name"] = _game.getName();
    p.data()["game_author"] = _game.getAuthor();
    p.data()["player_count"] = (int)_game.getNbrPlayers();
    p.data()["max_player"] = (int)_game.getMaximumPlayerAllowed();
    BNetworkPlayerMap::iterator it = _players.begin();
    BNetworkPlayerMap::iterator ite = _players.end();
    
    for (int i = 0; it != ite; ++it, ++i)
    {
        //if (it->first == sender)
        //    continue;
        BString key;
        key << i;
        p.data()[key] = it->second.toNetwork();
    }
    
    sender->send(p);
}

void BServerHandler::dispatchPlayerInfoOnConnect(BSocket *sender)
{
    BPackage p;
    _access.lock();
    BNetworkPlayer& player = _players[sender];
    _access.unlock();
    p.header().setType(BNetworkHeader::SERVER, BNetworkHeader::JOIN);
    p.data()["source"] =  (int)player.playerNumber();
    p.data()["player"] = player.toNetwork();
   
    BNetworkPlayerMap::iterator it = _players.begin();
    BNetworkPlayerMap::iterator ite = _players.end();
    
    for (; it != ite; ++it)
    {
        if (it->first == sender || it->first == _tcp)
            continue;
        it->first->send(p);
    }
    _engine->changePlayerList();
}

void BServerHandler::dispatchPlayerDisconnected(BSocket* sender)
{
    BPackage p;
    BNetworkPlayer& player = _players[sender];
    p.header().setType(BNetworkHeader::SERVER, BNetworkHeader::DISCONNECT);
    p.data()["source"] =  (int)player.playerNumber();
    p.data()["player"] = player.toNetwork();
    
    BNetworkPlayerMap::iterator it = _players.begin();
    BNetworkPlayerMap::iterator ite = _players.end();
    
    for (; it != ite; ++it)
    {
        if (it->first == sender || it->first == _tcp)
            continue;
        it->first->send(p);
    }
    _access.lock();
    _players.erase(sender);
    _access.unlock();
    BNetworkEngine::instance()->changePlayerList();
}

void BServerHandler::dispatchSendReady()
{
    bool ready = true;
    
    {
        BNetworkPlayerMap::iterator it = _players.begin();
        BNetworkPlayerMap::iterator ite = _players.end();
        
        for (; it != ite; ++it)
        {
            if (!it->second.isReady())
            {
                ready = false;
                break;
            }
        }
    }
    
    if (ready)
    {
        BPackage p;
        p.header().setType(BNetworkHeader::SERVER, BNetworkHeader::READY);
    
        BNetworkPlayerMap::iterator it = _players.begin();
        BNetworkPlayerMap::iterator ite = _players.end();
    
        for (; it != ite; ++it)
        {
            if (it->first == _tcp)
                continue;
            it->first->send(p);
        }
        _engine->setGameReady(true);
    }
}

void BServerHandler::dispatchTcpMessage(BSocket *sender, BPackage &package)
{
    BNetworkPlayer& player = _players[sender];
    package.header().setType(BNetworkHeader::SERVER, BNetworkHeader::MESSAGE);
    package.data()["senderId"] = (int)player.playerNumber();
    _access.lock();
    BNetworkPlayerMap::iterator it = _players.begin();
    BNetworkPlayerMap::iterator ite = _players.end(); 
    
    for (; it != ite; ++it)
    {
        if (it->first == sender || it->first == _tcp)
            continue;
        it->first->send(package);
    }
    _access.unlock();
    if (sender != _tcp)
        _engine->pushTcpMessage(package);
}

void BServerHandler::dispatchTCPSend(BSocket *sender, BPackage &package)
{
    BNetworkPlayerMap::iterator it = _players.begin();
    BNetworkPlayerMap::iterator ite = _players.end();
    
    for (; it != ite; ++it)
    {
        if (it->first == sender || it->first == _tcp)
            continue;
        it->first->send(package);
    }
    
}

void BServerHandler::onTcpPackage(BSocket *sender, BPackage &package)
{
    if (package.header().isClientHeader())
    {
        if (package.header().type & BNetworkHeader::JOIN)
        {
            if (_game.getNbrPlayers() >= _game.getMaximumPlayerAllowed() || (_game.needPassword() && package.data()["pwd"].toString() != _game.getPassword()))
                goto error;
            sendPlayerInfoOnConnect(sender, package);
            dispatchPlayerInfoOnConnect(sender);
            BNetworkEngine::instance()->changePlayerList();
        }
        else if (package.header().type & BNetworkHeader::READY)
        {
            Log::debug("Player #%i is ready", _players[sender].playerNumber());
            _access.lock();
            _players[sender].setReady(true);
            _access.unlock();
            dispatchPlayerInfoOnConnect(sender);
            dispatchSendReady();
        }
        else if (package.header().type & BNetworkHeader::MESSAGE)
            dispatchTcpMessage(sender, package);
    }
    
    return ;
error:
    sender->close();
    onDisconnect(sender);
}

void BServerHandler::startGame()
{
    _game.setMaximumPlayerAllowed(_game.getNbrPlayers());
}

void BServerHandler::sendDisconnectGame()
{
    if (_broadcast)
    {
        BPackage p;
        p.header().setType(BNetworkHeader::SERVER, BNetworkHeader::DISCONNECT);
        _broadcast->send(p);
    }
}

void BServerHandler::onConnect(BSocket *client, const BNetworkError &error)
{
    if (!error)
    {
        _clients[client].first = new BNetworkBuffer();
        _clients[client].second = new BPackage();
        client->setEventHandler(this);
        registerPackageListener(client, &BServerHandler::onTcpPackage, _clients[client].first, _clients[client].second);
        registerDisconnectListener(client, &BServerHandler::onDisconnect);
    }
}

void BServerHandler::onDisconnect(BSocket* client)
{
    if (_players.find(client) != _players.end())
    {
        dispatchPlayerDisconnected(client);
        _engine->changePlayerList();
        if (!_engine->keepConnectedWhenClientLeave())
            _engine->setConnected(false);
    }
    unregisterPackageListener(client);
    unregisterDisconnectListener(client);
    delete  _clients[client].first;
    delete _clients[client].second;
    _clients.erase(client);
    client->close();
}

unsigned int BServerHandler::getAvailablePlayerNumber() const
{
    unsigned int out = 2;
    bool found = true;
    
    while (found)
    {
        found = false;
        BNetworkPlayerMap::const_iterator it = _players.begin();
        BNetworkPlayerMap::const_iterator ite = _players.end();
        for (; it != ite; ++it)
        {
            Log::debug("Player #%i", it->second.playerNumber());
            if (it->second.playerNumber() == out)
            {
                out++;
                found = true;
            }
        }
    }
    
    return out;
}

void BServerHandler::sendTcpMessage(BPackage &package)
{
    dispatchTcpMessage(_tcp, package);
}

void BServerHandler::sendUdpMessage(BPackage &package)
{
    if (_udp)
    {
        package.data()["senderId"] = 1;
        dispatchUdpMessage(_udp, package);
    }
}

void BServerHandler::startSynchronizePlayers()
{
    BNetworkPlayerMap::iterator it = _players.begin();
    BNetworkPlayerMap::iterator ite = _players.end();
    
    for (; it != ite; ++it)
    {
        it->second.setReady(false);
    }
}

void BServerHandler::sendReady()
{
    _engine->me().setReady(true);
    _players[_tcp].setReady(true);
    dispatchPlayerInfoOnConnect(_tcp);
}

BInGamePlayerMap BServerHandler::players()
{
    BInGamePlayerMap out;
    _access.lock();
    
    BNetworkPlayerMap::iterator it = _players.begin();
    BNetworkPlayerMap::iterator ite = _players.end();
    
    for (; it != ite; ++it)
        out[it->second.playerNumber()] = it->second;
    
    _access.unlock();
    return out;
}

BServerHandler::~BServerHandler()
{
    sendDisconnectGame();
    BNetworkPlayerMap::iterator it = _players.begin();
    BNetworkPlayerMap::iterator ite = _players.end();
    
    for (; it != ite; ++it)
        delete it->first;
    
    delete _udp;
    delete _broadcast;
}