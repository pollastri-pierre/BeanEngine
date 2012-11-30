//
//  BNetworkVariant.h
//  NetworkTest
//
//  Created by Pierre POLLASTRI on 10/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef NetworkTest_BNetworkVariant_h
#define NetworkTest_BNetworkVariant_h

#include <string>
#include <boost/serialization/string.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <application/Log.h>
#include <boost/cstdint.hpp>

class BNetworkVariant
{
    
public:
    
    enum type_t
    {
        BOOLEAN = 16,
        STRING = 42,
        NUMBER = 84,
        INTEGER = 168
    };
    
    
private:
    
    union __data_t
    {
        boost::int32_t        boolean;
        std::string* string;
        double      number;
        boost::int32_t         integer;
    };
    
    __data_t _data;
    type_t _type;
    
public:
    BNetworkVariant();
    BNetworkVariant(bool boolean);
    BNetworkVariant(const std::string& string);
    BNetworkVariant(double number);
    BNetworkVariant(boost::int32_t integer);
    BNetworkVariant(const char* string);
    BNetworkVariant(const BNetworkVariant& cpy);
    BNetworkVariant& operator=(boost::int32_t integer);
    BNetworkVariant& operator=(bool boolean);
    BNetworkVariant& operator=(std::string string);
    BNetworkVariant& operator=(double number);
    BNetworkVariant& operator=(const char* string);
    BNetworkVariant& operator=(const BNetworkVariant& cpy);
    bool toBool() const;
    int toInteger() const;
    std::string toString() const;
    double toNumber() const;
    type_t type() const;
    
private:
    friend class boost::serialization::access;
    template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
        ar & _type;
        switch (_type)
        {
            case BOOLEAN:
                ar & _data.boolean;
                break;
            case STRING:
                ar & *_data.string;
                break;
            case NUMBER:
                ar & _data.number;
                break;
            case INTEGER:
                ar & _data.integer;
                break;
        }
    }
    template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
        ar & _type;
        switch (_type)
        {
            case BOOLEAN:
                ar & _data.boolean;
                break;
            case STRING:
                _data.string = new std::string();
                ar & (*_data.string);
                break;
            case NUMBER:
                ar & _data.number;
                break;
            case INTEGER:
                ar & _data.integer;
                break;
        }
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

#endif
