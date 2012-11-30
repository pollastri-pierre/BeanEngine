//
//  SFMLReader.cpp
//  BeanEngine
//
//  Created by Pierre POLLASTRI on 24/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "SFMLReader.h"
#include <application/SFMLApplication.h>

SFMLReader::SFMLReader(BReader::BufferMode mode)
{
    _mode = mode;
    _data = NULL;
}

char* SFMLReader::data() const
{
    return _data;
}

void SFMLReader::deleteData()
{
    delete _data;
    _data = NULL;
}

void SFMLReader::readAll()
{
    if (!_is.is_open())
        return ;
    delete _data;
    _size = 0;
    // get length of file:
    _is.seekg (0, std::ios::end);
    _size = (int)_is.tellg();
    _is.seekg (0, std::ios::beg);
    
    // allocate memory:
    _data = new char [_size + 1];
    memset(_data, 0, _size + 1);
    // read data as a block:
    _is.read (_data, _size);
}

bool SFMLReader::open(const BString &path, const char* absolute_path)
{
    SFMLApplication* app = dynamic_cast<SFMLApplication*>(BApplication::instance());
    BString final;
    if (absolute_path != NULL)
        final = (BString(absolute_path) + "/" + path);
    else
        final = app->context()->path + path;
    _is.open(final.c_str(), std::ios::binary);
    Log::debug("Try to open %s", final.c_str());
    return _is.is_open();
}

void SFMLReader::close()
{
    _is.close();
}

size_t SFMLReader::dataSize() const
{
    return _size;
}

int SFMLReader::fd()
{
    return -1;
}

off_t SFMLReader::start()
{
    return 0;
}

off_t SFMLReader::length()
{
    return _size;
}

int SFMLReader::read(void *buffer, size_t size)
{
    return 0;
}

SFMLReader::~SFMLReader()
{
    close();
    if (_mode != NO_DELETE)
        deleteData();
}
