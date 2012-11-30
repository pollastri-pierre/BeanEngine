//
//  BNetworkBuffer.h
//  NetworkTest
//
//  Created by Pierre POLLASTRI on 10/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef NetworkTest_BNetworkBuffer_h
#define NetworkTest_BNetworkBuffer_h

#include "BNetworkHeader.h"
#include "BPackage.h"

class BNetworkBuffer
{
private:
    char*       _data;
    size_t      _allocSize;
    size_t      _dataSize;
    
public:
    BNetworkBuffer(size_t size = 1024);
    void fill(char* data, size_t size);
    void trimToNextHeader();
    bool consumePackage(BPackage * const package);
    size_t size() const;
    ~BNetworkBuffer();
};

#endif
