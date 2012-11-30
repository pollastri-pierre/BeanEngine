/*
 * BNetworkEngine.h
 *
 *  Created on: 9 juil. 2012
 *      Author: NC
 */

#ifndef BNETWORKENGINE_H_
#define BNETWORKENGINE_H_

#include <boost/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <application/Log.h>
#include <application/BString.h>
#include <boost/asio.hpp>
#include "BMulticastSocket.h"
#include "BServerHandler.h"
#include "BClientHandler.h"
#include "BNetworkPlayer.h"
#include <queue>

typedef std::queue<BPackage> BPackageQueue;

class BNetworkEngine
{

public:
    enum
    {
        BROADCAST_GAME  =   0x01,
        UDP             =   0x02,
        TCP             =   0x03
    };
    
private:

	enum State
	{
		INIT,
		RUN,
		PAUSED
	};
    
    enum Mode
    {
        SERVER,
        CLIENT
    };
    
	static BNetworkEngine*		_instance;

	boost::thread*				_thread;
	boost::recursive_mutex		_access;
	State						_state;
    Mode                        _mode;
    boost::asio::io_service*     _io_service;
    
    // Network info
    unsigned int                _magic;
    BString                     _broadcastAddr;
    short                       _broadcastPort;
    short                       _tcpPort;
    short                       _udpPort;
    
    // 
    unsigned int                _services;
    BServerHandler*             _server;
    BClientHandler*             _client;
    
    
    // Player
    BNetworkPlayer              _me;
    BNetworkGame                _game;
    
    //Messages
    BPackageQueue               _tcpMessages;
    BPackageQueue               _udpMessages;
    
    // Common
    bool                        _playerListChanged;
    bool                        _gameReady;
    bool                        _keepConnected;
    bool                        _connected;
    
public:
	static BNetworkEngine* instance();
	void pause();
	void stop();
    void startAsClient(unsigned int services);
    void startAsServer(const BNetworkGame& game, unsigned int services);
    void setPlayer(const BString& name);
    void setMagic(unsigned int magic);
    void setBroadcastPort(short port);
    void setTcpPort(short port);
    void setUdpPort(short port);
    void setBroadcastAddress(const BString addr);
    void changePlayerList();
    
    unsigned int magic() const;
    short broadcastPort() const;
    short tcpPort() const;
    short udpPort() const;
    const BString& broadCastAddress() const;
    
    BNetworkPlayer& me();
    BNetworkGame& game();
    
    size_t tcpMessagesCount() const;
    size_t udpMessagesCount() const;
    BPackage popTcpMessage();
    BPackage popUdpMessage();
    void pushTcpMessage(BPackage package);
    void pushUdpMessage(BPackage package);
    
    void sendTcpMessage(BPackage& package);
    void sendUdpMessage(BPackage& package);
    
    BClientHandler* clientHandler() const;
    BServerHandler* serverHandler() const;
    void setGameReady(bool value);
    bool isGameReady() const;
    bool keepConnectedWhenClientLeave() const;
    void setKeepConnection(bool value);
    bool isConnected() const;
    void setConnected(bool value);
    
    BInGamePlayerMap players();
    bool playerListChanged() const;
    void startSynchronizePlayers();
    void sendReady();
    
    boost::asio::io_service& io_service();
    
private:
	BNetworkEngine();
    void start();
	static void startThread();
	void run(void);
	void exit();
    void initServer();
    void initClient();
	virtual ~BNetworkEngine();
};

#endif /* BNETWORKENGINE_H_ */
