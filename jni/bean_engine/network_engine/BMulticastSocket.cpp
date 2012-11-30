//
//  BMulticasSocket.cpp
//  NetworkTest
//
//  Created by Pierre POLLASTRI on 10/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <sstream>
#include "BMulticastSocket.h"
#include "BNetworkEngine.h"
#include <boost/bind.hpp>

using namespace boost::asio::ip;

BMulticastSocket::BMulticastSocket(boost::asio::io_service& service) :
        BSocket(service),
        _multicast_endpoint(boost::asio::ip::address::from_string("224.0.0.255"), 8686),
        _send(service, _multicast_endpoint.protocol()),
        _receiver(service)
{
    _receiver.open(_multicast_endpoint.protocol());
    _receiver.set_option(boost::asio::ip::udp::socket::reuse_address(true));
    _receiver.bind(_multicast_endpoint);
    
    // Join the multicast group.
    _receiver.set_option(
                       boost::asio::ip::multicast::join_group(boost::asio::ip::address::from_string("224.0.0.255")));
    

}

void BMulticastSocket::send(BPackage &package)
{
    BNetworkRawData data = package.toRawData();
    Log::debug("SENT %i", data.size());
    _send.async_send_to(
                          boost::asio::buffer(data, data.size()), _multicast_endpoint,
                          boost::bind(&BMulticastSocket::onSend, this,
                                      boost::asio::placeholders::error));
}

const udp::endpoint& BMulticastSocket::senderEndpoint() const
{
    return _sender_endpoint;
}

void BMulticastSocket::start()
{
    _receiver.async_receive_from(
                                boost::asio::buffer(_buffer.data(), _buffer.size()), _sender_endpoint,
                                boost::bind(
                                                &BMulticastSocket::onReceive, this,
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred
                                            )
                                );
}


void BMulticastSocket::onReceive(const boost::system::error_code &error, size_t received)
{
    BSocket::onReceive(error, received);
    start();
}

BMulticastSocket::~BMulticastSocket()
{
   
}