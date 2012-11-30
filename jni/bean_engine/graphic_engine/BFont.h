/*
 * BFont.h
 *
 *  Created on: 5 févr. 2012
 *      Author: NC
 */

#ifndef BFONT_H_
#define BFONT_H_

#include <biomodule/BReader.h>
#include <lua.h>
#include <application/Log.h>
#include <resource_manager/BData.h>

class BFont : public BData
{

protected:

	BReader* _reader;
	BString _name;
	BString _path;
	float 	_h;

public:

	static BFont* create(BString name, BString path);
	BFont(BString name, BString path);
	virtual const BString& name() const;
	virtual char* rawData() const;
	virtual size_t rawDataSize() const;
	virtual void load(unsigned int h);
	virtual void unload();
	virtual float size() const;

};


#endif /* BFONT_H_ */
