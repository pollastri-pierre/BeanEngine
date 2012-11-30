/*
 * BSoundEffect.cpp
 *
 *  Created on: 13 juin 2012
 *      Author: NC
 */

#include "BSoundEffect.h"
#include <biomodule/BReader.h>
#include <application/Log.h>

BSoundEffect::BSoundEffect(const BString& name, const BString& path) : BSound(name, path) {
	// TODO Auto-generated constructor stub
	_buffer = NULL;
	_length = 0;
}

void BSoundEffect::load(const BString& path)
{
	BReader* reader = BReader::create(BReader::NO_DELETE);
	if (!reader->open(path))
	{
		delete reader;
		Log::error("Error: SoundEffect <%s> cannot be opened", path.c_str());
		return ;
	}
	reader->readAll();
	_length = reader->dataSize();
	_buffer = (uint8_t* )reader->data();


	//Log::debug("SoundEffect <%s> loaded with %i bytes", path.c_str(), _length);
	delete reader;
}

uint8_t* BSoundEffect::buffer() const
{
	return _buffer;
}

off_t BSoundEffect::length() const
{
	return _length;
}

void BSoundEffect::unload()
{
	delete [] _buffer;
	_buffer = NULL;
	_length = 0;
}

BSoundEffect::~BSoundEffect() {
	unload();
}

