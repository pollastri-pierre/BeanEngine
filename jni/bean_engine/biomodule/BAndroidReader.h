/*
 * BAndroidReader.h
 *
 *  Created on: 5 févr. 2012
 *      Author: NC
 */

#ifdef __ANDROID__
#ifndef BANDROIDREADER_H_
#define BANDROIDREADER_H_

#include "BReader.h"
#include <android/asset_manager.h>
#include <BApplication.h>

class BAndroidReader : public BReader
{

	char* _data;
	size_t _size;
	AAsset* _asset;
	off_t	_start;
	off_t	_length;
	int		_fd;
	BReader::BufferMode _mode;

public:

	BAndroidReader(BReader::BufferMode mode = BReader::AUTO_DELETE);
	virtual char* 	data() const;
	virtual bool 	open(const BString& path, const char* absolute_path = NULL);
	virtual int 	read(void* buf, size_t size);
	virtual void 	readAll();
	virtual size_t 	dataSize() const;
	virtual void 	close();
	virtual void 	deleteData();
	virtual int		fd();
	virtual off_t	start();
	virtual off_t	length();
	~BAndroidReader();

};

#endif /* BANDROIDREADER_H_ */
#endif
