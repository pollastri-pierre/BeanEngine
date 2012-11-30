//
//  BNetworkVariant.cpp
//  NetworkTest
//
//  Created by Pierre POLLASTRI on 10/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "BNetworkVariant.h"
#include <sstream>

BNetworkVariant::BNetworkVariant()
{
    _type = NUMBER;
    _data.number = 0.0;
}

BNetworkVariant::BNetworkVariant(bool boolean)
{
    *this = (bool)boolean;
}

BNetworkVariant::BNetworkVariant(boost::int32_t integer)
{
    *this = (int)integer;
}

BNetworkVariant::BNetworkVariant(const std::string& string)
{
    *this = string;
}

BNetworkVariant::BNetworkVariant(const char* string)
{
    *this = std::string(string);
}

BNetworkVariant::BNetworkVariant(double number)
{
    *this = number;
}

BNetworkVariant::BNetworkVariant(const BNetworkVariant& variant)
{
    _type = NUMBER;
    _data.number = 0.0;
    *this = variant;
}

BNetworkVariant& BNetworkVariant::operator=(const BNetworkVariant &cpy)
{
    switch (cpy._type)
    {
        case BOOLEAN:
            *this = cpy._data.boolean;
            break;
        case STRING:
            *this = *cpy._data.string;
            break;
        case NUMBER:
            *this = cpy._data.number;
            break;
        case INTEGER:
            *this = (int)cpy._data.integer;
            break;
    }

    return *this;
}

BNetworkVariant& BNetworkVariant::operator=(const char* str)
{
    return (*this = std::string(str));
}

BNetworkVariant& BNetworkVariant::operator=(bool boolean)
{
    if (_type == STRING)
        delete _data.string;
    _type = BOOLEAN;
    _data.boolean = boolean;
    return *this;
}

BNetworkVariant& BNetworkVariant::operator=(boost::int32_t integer)
{
    if (_type == STRING)
        delete _data.string;
    _type = INTEGER;
    _data.integer = integer;
    return *this;
}

BNetworkVariant& BNetworkVariant::operator=(std::string string)
{
    if (_type != STRING)
        _data.string = new std::string();
    _type = STRING;
    *_data.string = string;
    return *this;
}

BNetworkVariant& BNetworkVariant::operator=(double number)
{
    if (_type == STRING)
        delete _data.string;
    _type = NUMBER;
    _data.boolean = number;
    return *this;
}

bool BNetworkVariant::toBool() const
{
    return _data.boolean;
}

std::string BNetworkVariant::toString() const
{
    if (_type == NUMBER || _type == INTEGER)
    {
        std::stringstream os;
        os << _data.number;
        return os.str();
    }
    if (_type == BOOLEAN)
    {
        if (_data.boolean)
            return "true";
        else
            return "false";
    }
    return *_data.string;
}

double BNetworkVariant::toNumber() const
{
    return _data.number;
}

boost::int32_t BNetworkVariant::toInteger() const
{
    return _data.integer;
}

BNetworkVariant::type_t BNetworkVariant::type() const
{
    return _type;
}
