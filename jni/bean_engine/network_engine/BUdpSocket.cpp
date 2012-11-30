//
//  BUdpSocket.cpp
//  NetworkTest
//
//  Created by Pierre POLLASTRI on 12/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "BUdpSocket.h"
#include "BNetworkEngine.h"

using namespace boost::asio::ip;

BUdpSocket::BUdpSocket(boost::asio::io_service& service)
    :   BSocket(service),
        _socket(service)
{
    _socket.open(udp::v4());
}

void BUdpSocket::send(BPackage &package, const udp::endpoint& endpoint)
{
    BNetworkRawData data = package.toRawData();
    _toSend = endpoint;
    _socket.async_send_to(
                           boost::asio::buffer(data.c_str(), data.size()), _toSend,
                           boost::bind(&BUdpSocket::onSend, this,
                                       boost::asio::placeholders::error));
}

void BUdpSocket::send(BPackage &package)
{
    send(package, _toSend);
}

const udp::endpoint& BUdpSocket::senderEndpoint() const
{
    return _senderEndpoint;
}

void BUdpSocket::start()
{
   _socket.async_receive_from(
                                boost::asio::buffer(_buffer.data(), _buffer.size()), _senderEndpoint,
                                boost::bind(
                                            &BUdpSocket::onReceive, this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred
                                            )
                                );
}


void BUdpSocket::onReceive(const boost::system::error_code &error, size_t received)
{
    BSocket::onReceive(error, received);
    start();
}

BUdpSocket::~BUdpSocket()
{
   
}