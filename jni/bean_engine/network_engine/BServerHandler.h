//
//  BServerHandler.h
//  NetworkTest
//
//  Created by Pierre POLLASTRI on 10/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef NetworkTest_BServerHandler_h
#define NetworkTest_BServerHandler_h

#include "BMultiNetworkEventHandler.h"
#include "BMulticastSocket.h"
#include "BPackage.h"
#include "BNetworkBuffer.h"
#include "BTcpSocket.h"
#include "BNetworkGame.h"
#include "BNetworkPlayer.h"
#include "BClientHandler.h"
#include "BUdpSocket.h"

#include <map>

typedef std::map<BSocket*, BNetworkPlayer /* Player */> BNetworkPlayerMap;
typedef std::map<BSocket*,  std::pair<BNetworkBuffer*, BPackage*> > BClientMap;

class BNetworkEngine;

class BServerHandler : public BMultiNetworkEventHandler<BServerHandler> {

private:
    BNetworkEngine*         _engine;
    
    // Multicast Room data
    BMulticastSocket*       _broadcast;
    BNetworkBuffer          _broadcastBuffer;
    BPackage                _broadcastPackage;
    
    // TCP data
    BTcpSocket*             _tcp;
    BNetworkBuffer          _tcpBuffer;
    BPackage                _tcpPackage;
   
    // UDP data
    BUdpSocket*             _udp;
    BNetworkBuffer          _udpBuffer;
    BPackage                _udpPackage;
    
    // Game
    BNetworkGame            _game;
    BNetworkPlayerMap       _players;
    
    boost::recursive_mutex  _access;
    
    BClientMap              _clients;
    
public:
    BServerHandler(BNetworkEngine* engine, const BNetworkGame& game);
    void startBroadcastGameService();
    void startTcpService();
    void startUdpService();
    void setGame(const BString& name, int maxPlayers, bool pwd);
    void startGame();
    BInGamePlayerMap players();
    void startSynchronizePlayers();
    void sendReady();
    void sendTcpMessage(BPackage& package);
    void sendUdpMessage(BPackage& package);
    virtual ~BServerHandler();
    
private:
    void onBroadcastPackage(BSocket* sender, BPackage& package);
    void onTcpPackage(BSocket* sender, BPackage& package);
    void onUdpPackage(BSocket* sender, BPackage& package);
    void onConnect(BSocket* client, const BNetworkError& error);
    void dispatchTCPSend(BSocket* sender, BPackage& package);
    void onDisconnect(BSocket* client);
    void sendDisconnectGame();
    
    unsigned int getAvailablePlayerNumber() const;
    
    inline void sendPlayerInfoOnConnect(BSocket* sender, BPackage& package);
    inline void dispatchPlayerInfoOnConnect(BSocket* sender);
    inline void dispatchPlayerDisconnected(BSocket* sender);
    inline void dispatchSendReady();
    inline void dispatchTcpMessage(BSocket* sender, BPackage& package);
    inline void dispatchUdpMessage(BUdpSocket* sender, BPackage& package);
};

#endif
