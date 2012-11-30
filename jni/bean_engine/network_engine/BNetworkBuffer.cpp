//
//  File.cpp
//  NetworkTest
//
//  Created by Pierre POLLASTRI on 10/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "BNetworkBuffer.h"
#include "BNetworkEngine.h"
#include <application/Log.h>

BNetworkBuffer::BNetworkBuffer(size_t size)
{
    _allocSize = size;
    _data = new char[_allocSize];
    _dataSize = 0;
}

void BNetworkBuffer::fill(char* data, size_t size)
{
    while (size + _dataSize > _allocSize)
    {
        _allocSize = _allocSize * 2;
        _data = (char*)::realloc(_data, _allocSize);
    }
    ::memmove(_data + _dataSize , data, size);
    _dataSize += size;
    //Log::debug("Size = %i", _dataSize);
}

void BNetworkBuffer::trimToNextHeader()
{
    //Log::debug("Trim Size = %i", _dataSize);
    if (_dataSize < BNetworkHeader::HEADER_SIZE)
        return ;
    off_t offset = 0;
    BNetworkHeader header;
    size_t dataSize = _dataSize - BNetworkHeader::HEADER_SIZE;
    unsigned int magic = BNetworkEngine::instance()->magic();
    
    do {
        header = BNetworkHeader::fromNetwork(_data + offset);
    } while (offset < dataSize && header.magic != magic && ++offset);
    
    if (offset > 0)
    {
        Log::debug("Size = %i, %i bytes trimmed",  _dataSize, offset);
        _dataSize -= offset;
        ::memmove(_data, _data + offset, _dataSize);
    }
}

bool BNetworkBuffer::consumePackage(BPackage * const package)
{
    if (package == NULL)
        return false;
    trimToNextHeader();
    if (_dataSize < BNetworkHeader::HEADER_SIZE)
        goto invalid;
    BNetworkHeader header;
    header = BNetworkHeader::fromNetwork(_data);
    
    if (header.magic != BNetworkEngine::instance()->magic())
        goto invalid;
    
    if (header.checksum > _dataSize)
        goto invalid;
    package->setValid(true);
    package->fromRawData(_data);
    
    _dataSize -= header.checksum;
    ::memmove(_data, _data + header.checksum, _dataSize);
    goto end;
    
invalid:
    package->setValid(false);
    
end:
    return package->isValid();
}

size_t BNetworkBuffer::size() const
{
    return _dataSize;
}

BNetworkBuffer::~BNetworkBuffer()
{
    delete [] _data;
}
