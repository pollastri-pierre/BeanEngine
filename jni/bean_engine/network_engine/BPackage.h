//
//  BPackage.h
//  NetworkTest
//
//  Created by Pierre POLLASTRI on 10/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef NetworkTest_BPackage_h
#define NetworkTest_BPackage_h

#include "BNetworkConfig.h"
#include "BNetworkHeader.h"
#include <application/BString.h>
#include <boost/variant.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include "BNetworkVariant.h"

typedef std::string BNetworkRawData;
typedef std::map<BString, BNetworkVariant> BNetworkData;

class BPackage
{
    BNetworkHeader  _header;
    BNetworkData    _data;
    bool            _valid;
    
public:
    BNetworkHeader& header();
    BNetworkData& data();
    BNetworkData& operator*();
    BNetworkRawData toRawData();
    void fromRawData(char* data);
    void setValid(bool valid);
    bool isValid() const;
    BString dump();
};

#endif
