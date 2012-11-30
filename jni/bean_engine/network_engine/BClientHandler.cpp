//
//  BClientHandler.cpp
//  NetworkTest
//
//  Created by Pierre POLLASTRI on 10/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "BClientHandler.h"
#include "BNetworkEngine.h"

using namespace boost::asio::ip;

BClientHandler::BClientHandler(BNetworkEngine* engine) : _engine(engine), BMultiNetworkEventHandler<BClientHandler>(this)
{
    _broadcast = NULL;
    _tcp = NULL;
    _udp = NULL;
    _connected = false;
    _connectionFailed = false;
    _waitingForConnection = false;
}

void BClientHandler::startBroadcastGameService()
{
    _broadcastAccess.lock();
    _broadcast = new BMulticastSocket(_engine->io_service());
    _broadcast->setEventHandler(this);
    _broadcast->start();
    registerPackageListener(_broadcast, &BClientHandler::onBroadcastPackage, &_broadcastBuffer, &_broadcastPackage);
    _broadcastAccess.unlock();
}

void BClientHandler::startTcpService()
{
    _tcp = new BTcpSocket(_engine->io_service());
    _tcp->setEventHandler(this);
    _tcp->start();
    registerConnectListener(_tcp, &BClientHandler::onConnect);
    registerPackageListener(_tcp, &BClientHandler::onTcpPackage, &_tcpBuffer, &_tcpPackage);
    registerDisconnectListener(_tcp, &BClientHandler::onDisconnect);
}

void BClientHandler::startUdpService()
{
    _udp = new BUdpSocket(_engine->io_service());
    _udp->setEventHandler(this);
    _udp->start();
    registerPackageListener(_udp, &BClientHandler::onUdpPackage, &_udpBuffer, &_udpPackage);
}

void BClientHandler::askForGames()
{
    if (_broadcast == NULL)
        return ;
    _broadcastAccess.lock();
    BPackage p;
    p.header().setType(BNetworkHeader::CLIENT, BNetworkHeader::GAME_INFO);
    _broadcast->send(p);
    _broadcastAccess.unlock();
    _gamesAccess.lock();
    _lastAsk = ::time(NULL);
    
    {
        BNetworkGameAttemptsMap::iterator it = _attempts.begin();
        BNetworkGameAttemptsMap::iterator ite = _attempts.end();
        
        while (it != ite)
        {
            BNetworkGameAttemptsMap::iterator a = it;
            ++it;
            if (_lastAsk - a->second > B_MAX_ATTEMPT)
            {
                eraseGame(a->first);
                _attempts.erase(a);
            }
        }
    }
    
    _gamesAccess.unlock();
}

bool BClientHandler::isConnected() const
{
    return _connected;
}

bool BClientHandler::connectionFailed() const
{
    return _connectionFailed;
}

void BClientHandler::connect(const BString &host, const BString &pwd)
{
    _gamePassword = pwd;
    _serverEndpoint = udp::endpoint(address_v4::from_string(host), _engine->udpPort());
    _tcp->connect(host, _engine->tcpPort());
    _waitingForConnection = true;
}

void BClientHandler::onBroadcastPackage(BSocket *sender, BPackage &package)
{
    if (package.header().isServerHeader())
    {
        BString addr = _broadcast->senderEndpoint().address().to_string();
        if (package.header().type & BNetworkHeader::GAME_INFO)
        {
            _gamesAccess.lock();
            _games[addr] << package.data();
            _games[addr].setHostIp(addr);
            _attempts[addr] = ::time(NULL);
            _gamesAccess.unlock();
        }
        else if (package.header().type & BNetworkHeader::DISCONNECT)
            eraseGame(addr);
        else if (!_udp && package.header().type & BNetworkHeader::MESSAGE && package.data().find("senderId") != package.data().end())
            _engine->pushUdpMessage(package);
    }
}

void BClientHandler::onUdpPackage(BSocket *sender, BPackage &package)
{
    if (package.header().isServerHeader() && package.header().type & BNetworkHeader::MESSAGE)
    {
        _engine->pushUdpMessage(package);
    }
}

void BClientHandler::onTcpPackage(BSocket *sender, BPackage &package)
{
    if (package.header().isServerHeader())
    {
        if (package.header().type & BNetworkHeader::JOIN && package.data()["source"].toInteger() == 0)
        {
            _waitingForConnection = false;
            _connectionFailed = false;
            _connected = true;
            _engine->me().setPlayerNumber(package.data()["player"].toInteger());
            //Log::debug("Player NBR = %i", _engine->me().playerNumber());
            _currentGame.setName(package.data()["game_name"].toString());
            _currentGame.setAuthor(package.data()["game_author"].toString());
            _currentGame.setNbrPlayer(package.data()["player_count"].toInteger());
            _currentGame.setMaximumPlayerAllowed(package.data()["max_player"].toInteger());
            unsigned count = _currentGame.getNbrPlayers();
            for (int i = 0; i < count; ++i)
            {
                BString key;
                key << i;
                BNetworkPlayer player;
                player = BNetworkPlayer::fromNetwork(package.data()[key].toString());
                _players[player.playerNumber()] = player;
            }
            _engine->setConnected(true);
        }
        else if (package.header().type & BNetworkHeader::JOIN && package.data()["source"].toInteger() != 0)
        {
            BNetworkPlayer player;
            player = BNetworkPlayer::fromNetwork(package.data()["source"].toString());
            _players[player.playerNumber()] = player;
        }
        else if (package.header().type & BNetworkHeader::READY)
        {
            _engine->setGameReady(true);
        }
        else if (package.header().type & BNetworkHeader::MESSAGE)
        {
            _engine->pushTcpMessage(package);
        }
    }
}

void BClientHandler::onConnect(const BNetworkError &error)
{
    if (!error)
    {
        _engine->me().setReady(false);
        BPackage p;
        p.header().setType(BNetworkHeader::CLIENT, BNetworkHeader::JOIN);
        p.data()["name"] = _engine->me().getName();
        p.data()["pwd"] = _gamePassword;
        _tcp->send(p);
    }
    else 
    {
        _connectionFailed = true;
    }
}

void BClientHandler::sendTcpMessage(BPackage &package)
{
    if (_tcp)
    {
        package.header().setType(BNetworkHeader::CLIENT, BNetworkHeader::MESSAGE);
        _tcp->send(package);
    }
}

void BClientHandler::sendUdpMessage(BPackage &package)
{
    if (_udp)
    {
        package.header().setType(BNetworkHeader::CLIENT, BNetworkHeader::MESSAGE);
        package.data()["senderId"] = (int)_engine->me().playerNumber();
        _udp->send(package, _serverEndpoint);
        _broadcast->send(package);
    }
}

bool BClientHandler::isWaitingForConnection() const
{
    return _waitingForConnection;
}

void BClientHandler::onDisconnect(BSocket *socket)
{
    _connected = false;
    _connectionFailed = true;
    _waitingForConnection = false;
    _engine->setConnected(false);
     _tcp = NULL;
    socket->close();
}

const BNetworkGameMap& BClientHandler::games() const
{
    return _games;
}

void BClientHandler::eraseGame(const BString& addr)
{
    _gamesAccess.lock();
    _games.erase(addr);
    _gamesAccess.unlock();
}

BInGamePlayerMap BClientHandler::players() const
{
    return _players;
}

void BClientHandler::startSynchronizePlayers()
{
    _engine->me().setReady(false);
}

void BClientHandler::sendReady()
{
    if (_tcp)
    {
        _engine->me().setReady(true);
        BPackage p;
        p.header().setType(BNetworkHeader::CLIENT, BNetworkHeader::READY);
        _tcp->send(p);
    }
}

BClientHandler::~BClientHandler()
{
    delete _broadcast;
    delete _udp;
    delete _tcp;
}
