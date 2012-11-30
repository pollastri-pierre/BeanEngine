//
//  BNetworkEventHandler.h
//  NetworkTest
//
//  Created by Pierre POLLASTRI on 10/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef NetworkTest_BNetworkEventHandler_h
#define NetworkTest_BNetworkEventHandler_h

#include "BSocket.h"

class BNetworkEventHandler
{
public:
    virtual void onReceived(BSocket* socket, const BNetworkError& error, const BSocket::Buffer& buffer, size_t received) = 0;
    virtual void onSend(BSocket* socket, const BNetworkError& error) = 0;
    virtual void onAccept(BSocket* socket, BSocket* client, const BNetworkError& error) = 0;
    virtual void onConnect(BSocket* socket, const BNetworkError& eventHandler) = 0;
    virtual void onDisconnect(BSocket* socket) = 0;
};


#endif
