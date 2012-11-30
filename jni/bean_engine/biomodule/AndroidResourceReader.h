/*
 * AndroidResourceReader.h
 *
 *  Created on: 25 mars 2012
 *      Author: Pierre Pollastri
 *      mail:   pollastri.p@gmail.com
 */

#ifdef __ANDROID__
#ifndef ANDROIDRESOURCEREADER_H_
#define ANDROIDRESOURCEREADER_H_

#include <application/AndroidApplication.h>
#include <application/BString.h>
#include <android/asset_manager.h>

class AndroidResourceReader {

	AAsset*	_asset;

public:

	AndroidResourceReader();
	void open(const BString& path);
	size_t read(void* buffer, size_t size);
	void close();
	virtual ~AndroidResourceReader();

};

#endif /* ANDROIDRESOURCEREADER_H_ */

#endif