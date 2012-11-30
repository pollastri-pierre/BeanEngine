//
//  BMultiNetworkEventHandler.h
//  NetworkTest
//
//  Created by Pierre POLLASTRI on 10/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef NetworkTest_BMultiNetworkEventHandler_h
#define NetworkTest_BMultiNetworkEventHandler_h

#include "BNetworkEventHandler.h"
#include "BSocket.h"
#include "BPackage.h"
#include "BNetworkBuffer.h"

#include <map>

template<typename T>
class BMultiNetworkEventHandler : public BNetworkEventHandler {
    
public:
    typedef void (T::*OnReceiveListener)(BSocket* socket, const BNetworkError& error, const BSocket::Buffer& buffer, size_t received);
    typedef void (T::*OnSendListener)(BSocket* socket, const BNetworkError& error);
    typedef void (T::*OnPackageListener)(BSocket*, BPackage& package);
    typedef void (T::*OnAcceptListener)(BSocket*, const BNetworkError& error);
    typedef void (T::*OnConnectListener)(const BNetworkError& error);
    typedef void (T::*OnDisconnectListener)(BSocket* socket);

    
private:
    struct Object
    {
        OnPackageListener   listener;
        BNetworkBuffer*     buffer;
        BPackage*           package;
        
        Object()
        {
            
        }
        
        Object(OnPackageListener _listener, BNetworkBuffer* _buffer, BPackage* _package)
        {
            listener = _listener;
            buffer = _buffer;
            package = _package;
        };
    };
    
    typedef std::map<BSocket*, OnReceiveListener>   ReceiveListenerMap;
    typedef std::map<BSocket*, OnSendListener>      SendListenerMap;
    typedef std::map<BSocket*, OnAcceptListener>    AcceptListenerMap;
    typedef std::map<BSocket*, OnConnectListener>   ConnectListenerMap;
    typedef std::map<BSocket*, OnDisconnectListener>   DisconnectListenerMap;
    typedef std::map<BSocket*, Object>              PackageListenerMap;
    
    
    T*                      _instance;
    ReceiveListenerMap      _rListeners;
    SendListenerMap         _sListeners;
    PackageListenerMap      _pListeners;
    AcceptListenerMap       _aListeners;
    ConnectListenerMap      _cListeners;
    DisconnectListenerMap  _dListeners;
    
public:
    
    BMultiNetworkEventHandler(T* instance) : _instance(instance) {}
    
    void registerReceiveListener(BSocket* socket, OnReceiveListener listener)
    {
        _rListeners[socket] = listener;
    }
    
    void registerAcceptListener(BSocket* socket, OnAcceptListener listener)
    {
        _aListeners[socket] = listener;
    }
    
    void registerConnectListener(BSocket* socket, OnConnectListener listener)
    {
        _cListeners[socket] = listener;
    }
    
    void registerDisconnectListener(BSocket* socket, OnDisconnectListener listener)
    {
        _dListeners[socket] = listener;
    }
    
    void registerPackageListener(BSocket* socket, OnPackageListener listener, BNetworkBuffer* buffer, BPackage* package)
    {
        _pListeners[socket] = Object(listener, buffer, package);
    }
    
    void unregisterPackageListener(BSocket* socket)
    {
        _pListeners.erase(socket);
    }
    
    void registerSendListener(BSocket* socket, OnSendListener listener)
    {
        _sListeners[socket] = listener;   
    }
    
    void unregisterReceiveListener(BSocket* socket)
    {
        _rListeners.erase(socket);   
    }
    
    void unregisterAcceptListener(BSocket* socket)
    {
        _aListeners.erase(socket);   
    }
    
    void unregisterConnectListener(BSocket* socket)
    {
        _cListeners.erase(socket);   
    }
    
    void unregisterDisconnectListener(BSocket* socket)
    {
        _dListeners.erase(socket);   
    }
    
    void unregisterSendListener(BSocket* socket)
    {
        _sListeners.erase(socket);  
    }
    
    void onReceived(BSocket* socket, const BNetworkError& error, const BSocket::Buffer& buffer, size_t received)
    {
        if (_rListeners.find(socket) != _rListeners.end())
            (_instance->*_rListeners[socket])(socket, error, buffer, received);
        if (_pListeners.find(socket) != _pListeners.end())
        {
            Object& object = _pListeners[socket];
            object.buffer->fill(buffer.data(), received);
            if (object.buffer->consumePackage(object.package))
                (_instance->*object.listener)(socket, *object.package);
        }
    }
    
    void onSend(BSocket* socket, const BNetworkError& error)
    {
        if (_sListeners.find(socket) != _sListeners.end())
            (_instance->*_sListeners[socket])(socket, error);
    }
    
    virtual void onAccept(BSocket* socket, BSocket* client, const BNetworkError& error)
    {
        if (_aListeners.find(socket) != _aListeners.end())
            (_instance->*_aListeners[socket])(client, error);
    }
    
    virtual void onConnect(BSocket* socket, const BNetworkError& error)
    {
        if (_cListeners.find(socket) != _cListeners.end())
            (_instance->*_cListeners[socket])(error);
    }
    
    virtual void onDisconnect(BSocket* socket)
    {
        if (_dListeners.find(socket) != _dListeners.end())
            (_instance->*_dListeners[socket])(socket);
    }
};

#endif
