//
//  BPackage.cpp
//  NetworkTest
//
//  Created by Pierre POLLASTRI on 10/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <sstream>
#include "BPackage.h"
#include "BNetworkEngine.h"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <exception>

BNetworkData& BPackage::data()
{
    return _data;
}

BNetworkData& BPackage::operator*()
{
    return _data;
}

void BPackage::setValid(bool valid)
{
    _valid = valid;
}

bool BPackage::isValid() const
{
    return _valid;
}

BNetworkHeader& BPackage::header()
{
    return _header;
}

static BString IntToStr(int nbr, BString* out = NULL)
{
    std::ostringstream os;
    os << nbr;
    return os.str();
}

BString BPackage::dump()
{
    std::ostringstream os;
    std::string raw = toRawData();
    os << "Client : " << _header.isClientHeader() << std::endl;
    os << "Type : " << ((int)(_header.type) & (BNetworkHeader::SERVER | BNetworkHeader::SERVER)) << std::endl;
    for (int i = 0; i < raw.length(); i++)
    {
        if ((raw[i] >= ' ' && raw[i] <= '{'))
            os << raw[i];
        else
            os << (BString() + "[" + IntToStr(raw[i]) + "]");
    }
    return os.str();
}

std::string BPackage::toRawData()
{
    std::ostringstream os;
    boost::archive::text_oarchive ar(os, boost::archive::no_header);
    ar << _data;

    
    _header.magic = BNetworkEngine::instance()->magic();
    _header.checksum = BNetworkHeader::HEADER_SIZE + os.str().size();
    std::string h(_header.toNetwork().get(), BNetworkHeader::HEADER_SIZE);
        
    std::ostringstream out;
    out << h << os.str();
    
    return out.str();
}

void BPackage::fromRawData(char* data)
{
    _header = BNetworkHeader::fromNetwork(data);
    std::stringstream os;
    os.write(data + BNetworkHeader::HEADER_SIZE, _header.dataSize());
    boost::archive::text_iarchive ar(os, boost::archive::no_header);
    try {
        ar >> _data;
    } catch (std::exception& e) {
        Log::error("Network : parse package error '%s'.", e.what());
    }
}