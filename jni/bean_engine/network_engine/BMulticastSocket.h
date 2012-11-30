//
//  BMulticastSocket.h
//  NetworkTest
//
//  Created by Pierre POLLASTRI on 10/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef NetworkTest_BMulticastSocket_h
#define NetworkTest_BMulticastSocket_h

#include "BSocket.h"
#include "BPackage.h"
#include "BnetworkConfig.h"
#include <boost/asio.hpp>

class BMulticastSocket : public BSocket
{
private:
    boost::asio::ip::udp::endpoint      _multicast_endpoint;
    boost::asio::ip::udp::socket        _send;
    boost::asio::ip::udp::socket        _receiver;
    
    boost::asio::ip::udp::endpoint      _sender_endpoint;
    boost::asio::ip::udp::endpoint      _endpoint;
    
public:
    BMulticastSocket(boost::asio::io_service& io_service);
    void send(BPackage& package);
    void start();
    void onReceive(const boost::system::error_code& error, size_t received);
    const boost::asio::ip::udp::endpoint& senderEndpoint() const;
    ~BMulticastSocket();
};

#endif
