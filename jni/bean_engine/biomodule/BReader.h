/*
 * BReader.h
 *
 *  Created on: 5 févr. 2012
 *      Author: NC
 */

#ifndef BREADER_H_
#define BREADER_H_

#include <application/BString.h>

class BReader
{
public:

	enum BufferMode {
		AUTO_DELETE,
		NO_DELETE
	};

	static BReader* create(BufferMode mode = AUTO_DELETE);
	virtual char* 	data() const = 0;
	virtual bool 	open(const BString& path, const char* absolute_path = NULL) = 0;
	virtual int		read(void* buffer, size_t size) = 0;
	virtual void 	readAll() = 0;
	virtual void 	close() = 0;
	virtual size_t 	dataSize() const = 0;
	virtual int		fd() = 0;
	virtual off_t	start() = 0;
	virtual off_t	length() = 0;
	virtual void 	deleteData() = 0;
    virtual ~BReader() {};

};

#endif /* BREADER_H_ */

#include "BAndroidReader.h"
