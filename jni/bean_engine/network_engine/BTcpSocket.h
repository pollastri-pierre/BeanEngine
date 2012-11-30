//
//  BTcpSocket.h
//  NetworkTest
//
//  Created by Pierre POLLASTRI on 11/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef NetworkTest_BTcpSocket_h
#define NetworkTest_BTcpSocket_h

#include "BSocket.h"
#include <application/BString.h>
#include <boost/asio.hpp>

class BTcpSocket : public BSocket
{
private:
    bool                                _server;
    boost::asio::ip::tcp::endpoint*     _endpoint;
    boost::asio::ip::tcp::acceptor*     _acceptor;
    boost::asio::ip::tcp::socket*       _socket;
    boost::asio::ip::tcp::resolver*     _resolver;
    
public:
    BTcpSocket(boost::asio::io_service& io_service);
    boost::asio::ip::tcp::endpoint*  endpoint() const;
    virtual void accept();
    virtual void connect(const BString& addr, short port);
    virtual void send(BPackage& package);
    virtual void start();
    virtual void onReceive(const BNetworkError& error, size_t received);
    virtual void onSend(const BNetworkError& error);
    virtual void onAccept(BTcpSocket* socket, const boost::system::error_code& error);
    virtual void onConnect(const BNetworkError& error);
    virtual void close();
    virtual boost::asio::ip::tcp::endpoint remoteEndpoint() const;
    static BString GetLocalAddress();
    ~BTcpSocket();
    
private:
    void receive();
};

#endif
