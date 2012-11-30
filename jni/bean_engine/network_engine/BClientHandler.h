//
//  BClientHandler.h
//  NetworkTest
//
//  Created by Pierre POLLASTRI on 10/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef NetworkTest_BClientHandler_h
#define NetworkTest_BClientHandler_h

#include "BMultiNetworkEventHandler.h"
#include "BMulticastSocket.h"
#include "BTcpSocket.h"
#include <boost/thread/recursive_mutex.hpp>
#include <application/BString.h> 
#include "BNetworkGame.h"
#include "BNetworkPlayer.h"
#include <map>
#include <time.h>
#include "BUdpSocket.h"

class BNetworkEngine;
class BNetworkBuffer;

typedef std::map<BString /* IP */, BNetworkGame> BNetworkGameMap;
typedef std::map<BString, time_t> BNetworkGameAttemptsMap;
typedef std::map<unsigned int, BNetworkPlayer> BInGamePlayerMap;

#define B_MAX_ATTEMPT 2

class BClientHandler : public BMultiNetworkEventHandler<BClientHandler> {
    
private:
    BNetworkEngine*                 _engine;
    
    // Broadcast
    BMulticastSocket*               _broadcast;
    BNetworkBuffer                  _broadcastBuffer;
    BPackage                        _broadcastPackage;
    boost::recursive_mutex          _broadcastAccess;
    
    // TCP
    BTcpSocket*                     _tcp;
    BNetworkBuffer                  _tcpBuffer;
    BPackage                        _tcpPackage;
    
    // UDP
    BUdpSocket*                     _udp;
    BNetworkBuffer                  _udpBuffer;
    BPackage                        _udpPackage;
    
    // Game info
    BNetworkGameMap                 _games;
    BNetworkGameAttemptsMap         _attempts;
    boost::recursive_mutex          _gamesAccess;
    time_t                          _lastAsk;
    
    bool                            _connected;
    bool                            _connectionFailed;
    bool                            _waitingForConnection;
    BString                         _gamePassword;

    BInGamePlayerMap                _players;
    BNetworkGame                    _currentGame;
    
    boost::asio::ip::udp::endpoint  _serverEndpoint;
    
public:
    BClientHandler(BNetworkEngine* engine);
    void startBroadcastGameService();
    void startTcpService();
    void startUdpService();
    void askForGames();
    void eraseGame(const BString& addr);
    const BNetworkGameMap& games() const;
    bool isConnected() const;
    bool connectionFailed() const;
    bool isWaitingForConnection() const;
    void connect(const BString& host, const BString& pwd);
    void sendTcpMessage(BPackage& package);
    void sendUdpMessage(BPackage& package);
    void sendReady();
    const BNetworkGame& currentGame() const;
    BInGamePlayerMap players() const;
    void startSynchronizePlayers();
    ~BClientHandler();
    
private:
    void onBroadcastPackage(BSocket* sender, BPackage& package);
    void onTcpPackage(BSocket* sender, BPackage& package);
    void onUdpPackage(BSocket* sender, BPackage& package);
    void onConnect(const BNetworkError& error);
    void onDisconnect(BSocket* socket);
};

#endif
