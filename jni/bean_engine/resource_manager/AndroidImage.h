/*
 * AndroidImage.h
 *
 *  Created on: 25 mars 2012
 *      Author: NC
 */

#ifdef __ANDROID__

#ifndef ANDROIDIMAGE_H_
#define ANDROIDIMAGE_H_

#include "BImage.h"
#include <png.h>
#include <biomodule/AndroidResourceReader.h>
#include <GLES/gl.h>

class AndroidImage : public BImage
{

	AndroidResourceReader 	_reader;
	GLuint					_textureId;
	int32_t					_format;
	int32_t					_type;
	BSize					_rSize;

public:

	AndroidImage(const BString& name, const BString& path);
	void load(const BString& path);
	void unload();
	BSize rSize() const;
	int32_t textureId() const;
	virtual ~AndroidImage();
};

#endif /* ANDROIDIMAGE_H_ */
#endif