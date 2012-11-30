//
//  BNetworkHeader.h
//  NetworkTest
//
//  Created by Pierre POLLASTRI on 10/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef NetworkTest_BNetworkHeader_h
#define NetworkTest_BNetworkHeader_h

#include <iostream>
#include <string>
#include <boost/shared_array.hpp>
#include <application/Log.h>

struct BNetworkHeader
{
    
    enum Sender
    {
        SERVER      = 0x000001,
        CLIENT      = 0x000002
    };
    
    enum PackageType
    {
        GAME_INFO   = 0x000004,
        JOIN        = 0x000008,
        DISCONNECT  = 0x000010,
        READY       = 0x000020,
        MESSAGE     = 0x000080
    };
    
    
    
    unsigned int    type;
    unsigned int    magic;
    size_t          checksum;
    
    static const size_t HEADER_SIZE = 12;
    
    size_t dataSize() const 
    {
        return checksum - HEADER_SIZE;
    };
    
    void setType(Sender sender, PackageType type)
    {
        this->type = sender | type;
    };
    
    bool isServerHeader() const
    {
        return  type & SERVER;
    };
    
    bool isClientHeader() const
    {
        return type & CLIENT;
    };
    
    static BNetworkHeader fromNetwork(const char* data) {
        BNetworkHeader header;
        ::memset(&header, 0, sizeof(BNetworkHeader));
        if (isBigEndian()) {
            readBigEndian((char*)&header.type, 4, 0, data);
            readBigEndian((char*)&header.magic, 4, 4, data);
            readBigEndian((char*)&header.checksum, 4, 8, data);
        }
        else {
            readLittleEndian((char*)&header.type, 4, 0, data);
            readLittleEndian((char*)&header.magic, 4, 4, data);
            readLittleEndian((char*)&header.checksum, 4, 8, data);
        }
        Log::debug("MAGIC %u", header.magic);
        return header;
    };
    
    boost::shared_array<char> toNetwork() {
        boost::shared_array<char> out(new char[HEADER_SIZE]);
        if (isBigEndian()) {
            copyBigEndian((char*)&type, 4, 0, out);
            copyBigEndian((char*)&magic, 4, 4, out);
            copyBigEndian((char*)&checksum, 4, 8, out);
        }
        else {
            copyLittleEndian((char*)&type, 4, 0, out);
            copyLittleEndian((char*)&magic, 4, 4, out);
            copyLittleEndian((char*)&checksum, 4, 8, out);
        }
        return out;
    };
    
    
private:
    static bool isBigEndian() {
        int n = 0x01;
        return ((char*)&n)[0] == 0;
    }
    
    static void copyLittleEndian(char* data, size_t size, off_t offset, boost::shared_array<char>& array) {
        for (int index = 0; index < size; index++)
            array[index + offset] = data[size - index - 1];
    }
    
    static void copyBigEndian(char* data, size_t size, off_t offset, boost::shared_array<char>& array) {
        for (int index = 0; index < size; index++)
            array[index + offset] = data[index];
    }
    
    static void readLittleEndian(char* data, size_t size, off_t offset, const char* array) {
        for (int index = 0; index < size; index++)
            data[index] = array[offset + size - index - 1];
    }
    
    static void readBigEndian(char* data, size_t size, off_t offset, const char* array) {
        for (int index = 0; index < size; index++)
            data[index] = array[offset + index];
    }
    
};

#endif
