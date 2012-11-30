/*
 * BFont.cpp
 *
 *  Created on: 5 fŽvr. 2012
 *      Author: NC
 */

#include "BFont.h"
#include "BFreetypeFont.h"

BFont* BFont::create(BString name, BString path)
{
	return new BFreetypeFont(name, path);
}

BFont::BFont(BString name, BString path)
{
	_name = name;
	_path = path;
	_reader = NULL;
}

const BString& BFont::name() const
{
	return _name;
}

float BFont::size() const
{
	return _h;
}

char* BFont::rawData() const
{
	return _reader->data();
}

size_t BFont::rawDataSize() const
{
	return _reader->dataSize();
}

void BFont::load(unsigned int h)
{
	_h = h;
	if (_reader == NULL)
		_reader = BReader::create();
	_reader->open(_path);
	_reader->readAll();
}

void BFont::unload()
{
	_reader->close();
	_reader->deleteData();
	delete _reader;
	_reader = NULL;
}

