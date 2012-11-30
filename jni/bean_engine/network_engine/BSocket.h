//
//  BSocket.h
//  NetworkTest
//
//  Created by Pierre POLLASTRI on 10/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef NetworkTest_BSocket_h
#define NetworkTest_BSocket_h

#include <boost/asio.hpp>
#include "BNetworkConfig.h"
#include "BPackage.h"
#include <application/Log.h>

#define BSOCKET_BUFFER_SIZE 1024

typedef boost::system::error_code BNetworkError;

class BNetworkEventHandler;

class BSocket {
  
public:
    class Buffer {
        char*       _data;
        size_t      _size;
        
    public:
        Buffer(size_t size) {
            _size = size;
            _data = new char[_size];
        }
        char*  data() const {
            return _data;  
        };
        size_t size() const {
            return _size;
        };
        char& operator[](int index){
            return _data[index];
        }
        ~Buffer() {
            delete [] _data;
        };
    };
    
protected:
    
    Buffer                      _buffer;
    boost::asio::io_service&    _io_service;
    BNetworkEventHandler*       _handler;
    
public:
    BSocket(boost::asio::io_service& io_service);
    virtual void send(BPackage& package) = 0;
    virtual void start() = 0;
    virtual void onReceive(const BNetworkError& error, size_t received);
    virtual void onSend(const BNetworkError& error);
    virtual void onConnect(const BNetworkError& error);
    virtual void onAccept(BSocket* socket, const BNetworkError& error);
    virtual void setEventHandler(BNetworkEventHandler* eventHandler);
    virtual void close() {};
    virtual ~BSocket() {};
};

#endif
