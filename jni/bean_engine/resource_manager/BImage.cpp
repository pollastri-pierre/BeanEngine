/*
 * BImage.cpp
 *
 *  Created on: 22 mars 2012
 *      Author: NC
 */

#include "BImage.h"

#ifdef __ANDROID__
#include "AndroidImage.h"
#else
#include "SFMLImage.h"
#endif

BImage::BImage(const BString& name, const BString& path)
{
	_path = path;
	_name = name;

}

BImage* BImage::create(const BString& name, const BString& path)
{
#ifdef __ANDROID__
	return new AndroidImage(name, path);
#else
    return new SFMLImage(name, path);
#endif
}

void BImage::load(const BString& path)
{
	_path = path;
}

void BImage::unload()
{

}



const BSize& BImage::size() const { return _size; }

BImage::~BImage() {
	// TODO Auto-generated destructor stub
}
