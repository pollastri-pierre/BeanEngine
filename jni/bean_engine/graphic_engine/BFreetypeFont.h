/*
 * BFreetypeFont.h
 *
 *  Created on: 5 févr. 2012
 *      Author: NC
 */

#ifndef BFREETYPEFONT_H_
#define BFREETYPEFONT_H_

#include "BFont.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>
#ifdef __ANDROID__
#include <GLES/gl.h>
#include <GLES/glext.h>
#elif defined (__SFML__)
#include <SFML/OpenGL.hpp>
#endif

#include <wchar.h>

class BFreetypeFont : public BFont
{
public:

	struct BFreetypeChar
	{
		unsigned int x;
		unsigned int y;
		lua_Number	 top;
		lua_Number	 left;
		unsigned int width;
		unsigned int height;
	};

	typedef std::map<wchar_t, BFreetypeChar> FontAtlas;

private:

	static FT_Library 	_lib;
	FontAtlas 			_atlas;
	unsigned int		_width, _height;
	lua_Number			_maxHeight;
	GLuint 				_texture[1];

public:

	BFreetypeFont(BString name, BString path);
	virtual void load(unsigned int h);
	virtual void unload();
	virtual GLuint texture() const;
	virtual FontAtlas& atlas();
	virtual unsigned int width() const;
	virtual unsigned int height() const;
	virtual lua_Number maxHeight() const;
	~BFreetypeFont();

private:
	static void loadCharset();

};

#endif /* BFREETYPEFONT_H_ */