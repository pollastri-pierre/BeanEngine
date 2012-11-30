//
//  BTcpSocket.cpp
//  NetworkTest
//
//  Created by Pierre POLLASTRI on 11/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "BTcpSocket.h"
#include "BNetworkEngine.h"
#include <application/Log.h>

using namespace boost::asio::ip;

BTcpSocket::BTcpSocket(boost::asio::io_service& io_service) : BSocket(io_service)
{
    _server = false;
    _acceptor = NULL;
    _endpoint = NULL;
    _socket = NULL;
}

void BTcpSocket::accept()
{
    _server = true;
    if (!_endpoint)
        _endpoint = new tcp::endpoint(tcp::v4(), BNetworkEngine::instance()->tcpPort());
    if (!_acceptor)
    {
        _acceptor = new tcp::acceptor(_io_service, *_endpoint);
        _acceptor->listen();
        std::cout << _acceptor->local_endpoint().address().to_string() << std::endl;
    }
    std::cout << _endpoint->address().to_string() << std::endl;
    BTcpSocket* client = new BTcpSocket(_io_service);
    client->_socket = new tcp::socket(_io_service);
    _acceptor->async_accept(*client->_socket,
                            boost::bind(&BTcpSocket::onAccept, this, client,
                                       boost::asio::placeholders::error));
}

BString BTcpSocket::GetLocalAddress()
{
    BString out;
    try
    {
        boost::asio::io_service io_service;
        
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(boost::asio::ip::host_name(),"");
        tcp::resolver::iterator it = resolver.resolve(query);
        
        while(it != tcp::resolver::iterator())
        {
            boost::asio::ip::address addr = (it++)->endpoint().address();
            if(addr.is_v6())
                continue;
            out = addr.to_string();
        }
    }
    catch(std::exception &e)
    {
        
    }
    return out;
}

void BTcpSocket::connect(const BString& addr, short port)
{
    if (_socket->is_open())
        return ;
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(addr), port);
    _socket->async_connect
            (
                endpoint,
                boost::bind(&BTcpSocket::onConnect, this,
                            boost::asio::placeholders::error)
            );
}

void BTcpSocket::send(BPackage& package)
{
    BNetworkRawData data = package.toRawData();
    _socket->async_send(
                           boost::asio::buffer(data.c_str(), data.size()),
                           boost::bind(&BTcpSocket::onSend, this,
                                       boost::asio::placeholders::error)); 
}

void BTcpSocket::start()
{
    if (_socket == NULL)
    {
        _server = false;
        _socket = new tcp::socket(_io_service);
    }
}

void BTcpSocket::receive()
{
    _socket->async_receive
            (
                boost::asio::buffer(_buffer.data(), _buffer.size()),
                boost::bind(&BTcpSocket::onReceive, this,
                         boost::asio::placeholders::error,
                         boost::asio::placeholders::bytes_transferred)
            );
}

void BTcpSocket::onReceive(const BNetworkError& error, size_t received)
{
    BSocket::onReceive(error, received);
    if (_socket->is_open())
        receive();
    else delete this;
}

void BTcpSocket::onSend(const BNetworkError& error)
{
    BSocket::onSend(error);
}

void BTcpSocket::onAccept(BTcpSocket* client, const boost::system::error_code& error)
{
    BSocket::onAccept(client, error);
    client->receive();
    accept();
}

void BTcpSocket::onConnect(const BNetworkError &error)
{
    BSocket::onConnect(error);
    receive();
}

void BTcpSocket::close()
{
    if (_socket && _socket->is_open())
    {
        _socket->close();
        try
        {
            _socket->cancel();
        }
        catch (...)
        {
            
        }
    }
}

tcp::endpoint BTcpSocket::remoteEndpoint() const
{
    return _socket->remote_endpoint();
}

BTcpSocket::~BTcpSocket()
{
    close();
    delete _acceptor;
    delete _socket;
}
