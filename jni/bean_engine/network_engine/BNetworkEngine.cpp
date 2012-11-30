/*
 * BNetworkEngine.cpp
 *
 *  Created on: 9 juil. 2012
 *      Author: NC
 */

#include "BNetworkEngine.h"
#include "BNetworkConfig.h"

BNetworkEngine* BNetworkEngine::_instance = NULL;

BNetworkEngine::BNetworkEngine()
{
    _magic = B_MAGIC;
    _broadcastAddr = B_BROADCAST_ADDR;
    _broadcastPort = B_BROADCAST_PORT;
    _tcpPort = B_TCP_PORT;
    _udpPort = B_UDP_PORT;
    _mode = SERVER;
    _state = INIT;
    _server = NULL;
    _thread = NULL;
    _client = NULL;
    _me.setName("John Doe");
    _keepConnected = true;
    _connected = false;
    _io_service = NULL;
}

BNetworkEngine* BNetworkEngine::instance()
{
	return (!_instance) ? _instance = new BNetworkEngine() : _instance;
}

void BNetworkEngine::startThread()
{
	_instance->run();
}

void BNetworkEngine::start()
{
    stop();
    _io_service = new boost::asio::io_service();
    if (_mode == SERVER)
        initServer();
    else
        initClient();
    _thread = new boost::thread(startThread);
}

void BNetworkEngine::startAsClient(unsigned int services)
{
    _services = services;
    _mode = CLIENT;
    start();
}

void BNetworkEngine::startAsServer(const BNetworkGame &game, unsigned int services)
{
    _services = services;
    _mode = SERVER;
    _game = game;
    _game.setAuthor(_me.getName());
    start();
}

void BNetworkEngine::setPlayer(const BString &name)
{
    _me.setName(name);
}

void BNetworkEngine::pause()
{
	_access.lock();
	_state = PAUSED;
	_access.unlock();
}

void BNetworkEngine::stop()
{
    if (_thread && _io_service)
    {
        _io_service->stop();
        _thread->join();
        _io_service->reset();
        delete _thread;
        delete _server;
        delete _client;
        _server = NULL;
        _client = NULL;
        _thread = NULL;
    }
}

BNetworkGame& BNetworkEngine::game()
{
    return _game;
}

BNetworkPlayer& BNetworkEngine::me()
{
    return _me;
}

boost::asio::io_service& BNetworkEngine::io_service()
{
    return *_io_service;
}

void BNetworkEngine::run()
{
    _io_service->run();
    _io_service->reset();
}

void BNetworkEngine::initClient()
{
    if (_server)
    {
        delete _server;
        _server = NULL;
    }
    else if (!_server)
    {
        _client = new BClientHandler(this);
        if (_services & BROADCAST_GAME)
            _client->startBroadcastGameService();
        if (_services & TCP)
            _client->startTcpService();
        if (_services & UDP)
            _client->startUdpService();
    }
}

void BNetworkEngine::initServer()
{
    if (_client)
    {
        delete _client;
        _client = NULL;
    }
    else if (!_server)
    {
        _server = new BServerHandler(this, _game);
        if (_services & BROADCAST_GAME)
            _server->startBroadcastGameService();
        if (_services & TCP)
            _server->startTcpService();
        if (_services & UDP)
           _server->startUdpService();
    }
}

void BNetworkEngine::exit()
{
    if (_io_service)
    {
        _io_service->stop();
        _io_service->reset();
    }
}

BServerHandler* BNetworkEngine::serverHandler() const
{
    return _server;
}

BClientHandler* BNetworkEngine::clientHandler() const
{
    return _client;
}

BInGamePlayerMap BNetworkEngine::players()
{
    BInGamePlayerMap map;
    _playerListChanged = false;
    if (_server) {
        map = _server->players();
    }
    else {
        map = _client->players();
    }
    return map;
}

bool BNetworkEngine::playerListChanged() const
{
    return _playerListChanged;
}

void BNetworkEngine::changePlayerList()
{
    _playerListChanged = true;
}

void BNetworkEngine::setMagic(unsigned int magic)
{
    _magic = magic;
}

void BNetworkEngine::setBroadcastPort(short port)
{
    _broadcastPort = port;
}

void BNetworkEngine::setTcpPort(short port)
{
    _tcpPort = port;
}

void BNetworkEngine::setUdpPort(short port)
{
    _udpPort = port;
}

void BNetworkEngine::setBroadcastAddress(const BString addr)
{
    _broadcastAddr = addr;
}

unsigned int BNetworkEngine::magic() const
{
    return _magic;
}

short BNetworkEngine::broadcastPort() const
{
    return _broadcastPort;
}

short BNetworkEngine::tcpPort() const
{
    return _tcpPort;
}

short BNetworkEngine::udpPort() const
{
    return _udpPort;
}

const BString& BNetworkEngine::broadCastAddress() const
{
    return _broadcastAddr;
}

BPackage BNetworkEngine::popTcpMessage()
{
    BPackage out = _tcpMessages.front();
    _tcpMessages.pop();
    return out;
}

BPackage BNetworkEngine::popUdpMessage()
{
    BPackage out = _udpMessages.front();
    _udpMessages.pop();
    return out;
}

size_t BNetworkEngine::tcpMessagesCount() const
{
    return _tcpMessages.size();
}

size_t BNetworkEngine::udpMessagesCount() const
{
    return _udpMessages.size();
}

void BNetworkEngine::pushTcpMessage(BPackage package)
{
    _tcpMessages.push(package);
}

void BNetworkEngine::pushUdpMessage(BPackage package)
{
    _udpMessages.push(package);
}

void BNetworkEngine::sendTcpMessage(BPackage &package)
{
   if (_server)
        _server->sendTcpMessage(package);
    else
        _client->sendTcpMessage(package);
}

void BNetworkEngine::sendUdpMessage(BPackage &package)
{
    if (_server)
        _server->sendUdpMessage(package);
    else
        _client->sendUdpMessage(package);
}

void BNetworkEngine::startSynchronizePlayers()
{
    setGameReady(false);
    if (_client)
        _client->startSynchronizePlayers();
    else
        _server->startSynchronizePlayers();
}

void BNetworkEngine::sendReady()
{
    if (_client)
        _client->sendReady();
    else
        _server->sendReady();
}

void BNetworkEngine::setGameReady(bool value)
{
    _gameReady = value;
}

bool BNetworkEngine::isGameReady() const
{
    return _gameReady;
}

bool BNetworkEngine::keepConnectedWhenClientLeave() const
{
    return _keepConnected;
}

void BNetworkEngine::setKeepConnection(bool value)
{
    _keepConnected = value;
}

void BNetworkEngine::setConnected(bool value)
{
    _connected = value;
}

bool BNetworkEngine::isConnected() const
{
    return _connected;
}

BNetworkEngine::~BNetworkEngine()
{
	if (_state != INIT)
	{
		exit();
		_thread->join();
		delete _thread;
	}
    
}
