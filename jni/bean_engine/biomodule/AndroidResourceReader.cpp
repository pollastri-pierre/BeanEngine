/*
 * AndroidResourceReader.cpp
 *
 *  Created on: 25 mars 2012
 *      Author: NC
 */

#include "AndroidResourceReader.h"

#ifdef __ANDROID__

AndroidResourceReader::AndroidResourceReader() {
	// TODO Auto-generated constructor stub
	_asset = NULL;
}

void AndroidResourceReader::open(const BString& path)
{
	_asset = AAssetManager_open(((AndroidApplication*)BApplication::instance())->context()->activity->assetManager,
			path.c_str(), AASSET_MODE_BUFFER);
}

size_t AndroidResourceReader::read(void* buffer, size_t size)
{
	if (!_asset) return 0;
	return AAsset_read(_asset, buffer, size);
}

void AndroidResourceReader::close()
{
	if (!_asset) return ;
	AAsset_close(_asset);
	_asset = NULL;
}

AndroidResourceReader::~AndroidResourceReader() {
	close();
}

#endif