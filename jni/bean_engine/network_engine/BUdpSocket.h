//
//  BUdpSocket.h
//  NetworkTest
//
//  Created by Pierre POLLASTRI on 12/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef NetworkTest_BUdpSocket_h
#define NetworkTest_BUdpSocket_h

#include "BSocket.h"
#include <boost/asio.hpp>

class BUdpSocket : public BSocket
{
private:
    boost::asio::ip::udp::socket        _socket;
    
    boost::asio::ip::udp::endpoint      _toSend;
    boost::asio::ip::udp::endpoint      _senderEndpoint;
    
public:
    BUdpSocket(boost::asio::io_service& service);
    void send(BPackage& package, const boost::asio::ip::udp::endpoint& endpoint);
    void send(BPackage& package);
    void start();
    void onReceive(const boost::system::error_code& error, size_t received);
    const boost::asio::ip::udp::endpoint& senderEndpoint() const;
    ~BUdpSocket();
};

#endif
