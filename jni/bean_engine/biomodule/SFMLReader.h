//
//  SFMLReader.h
//  BeanEngine
//
//  Created by Pierre POLLASTRI on 24/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef BeanEngine_SFMLReader_h
#define BeanEngine_SFMLReader_h

#include "BReader.h"
#include <iostream>
#include <fstream>

class SFMLReader : public BReader {
    
private:
    BReader::BufferMode _mode;
    char*               _data;
    off_t               _size;
    std::ifstream       _is;
    
public:
    SFMLReader(BReader::BufferMode mode = BReader::AUTO_DELETE);
    virtual char* 	data() const;
	virtual bool 	open(const BString& path, const char* absolute_path = NULL);
	virtual int		read(void* buffer, size_t size);
	virtual void 	readAll();
	virtual void 	close();
	virtual size_t 	dataSize() const;
	virtual int		fd();
	virtual off_t	start();
	virtual off_t	length();
	virtual void 	deleteData();
    virtual ~SFMLReader();
};

#endif
