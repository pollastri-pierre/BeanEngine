/*
 * BFreetypeFont.cpp
 *
 *  Created on: 5 f�vr. 2012
 *      Author: NC
 */

#include "BFreetypeFont.h"
#include <cmath>
#include <utf8.h>

static wchar_t* CHARSET = 	NULL;

BFreetypeFont::BFreetypeFont(BString name, BString path) : BFont(name, path)
{

}

void BFreetypeFont::loadCharset()
{
	BReader* reader = BReader::create();
	reader->open("charsets/main.charset");
	reader->readAll();
	CHARSET = new wchar_t[reader->dataSize()];

	utf8_to_wchar(reader->data(), reader->dataSize(), CHARSET, reader->dataSize(), 0);
	delete reader;
}

lua_Number BFreetypeFont::maxHeight() const
{
	return _maxHeight;
}

static inline int next_p2 (int a)
{
    int rval = 1;
    while (rval < a) rval<<=1;
    return rval;
}

static inline GLubyte* createEmptyBitmap(FT_Face face, unsigned int* _width, unsigned int* _height,
		unsigned int* _maxRows, unsigned int* _maxWidth, unsigned int* _range)
{
	size_t len = wcslen(CHARSET);

	int range = (int) ceil(sqrt(len));

	unsigned int rows = 0;
	unsigned int width = 0;

	int row, index = 0;
	unsigned int currentRowWidth = 0;

	*_maxWidth = 0;

	for (int i = 0; i < len; i++)
	{
		if (FT_Load_Glyph(face, FT_Get_Char_Index(face, CHARSET[i]), FT_LOAD_RENDER))
		{
			continue;
		}

		FT_GlyphSlot g = face->glyph;

		//width += g->bitmap.width;
		if (g->bitmap.rows > rows)
			rows = g->bitmap.rows;
		if (g->bitmap.width > *_maxWidth)
			*_maxWidth = g->bitmap.width;
		currentRowWidth += g->bitmap.width + 20;

		index += 1;

		if (index >= range)
		{
			if (currentRowWidth > width)
				width = currentRowWidth;
			currentRowWidth = 0;
			index = 0;
			row += 1;
		}

	}

	*_maxRows = rows;

	width = next_p2(width);
	rows = next_p2(rows * range);

	*_width = width;
	*_height = rows;
	*_range = range;

	return new GLubyte[2 * *_width * *_height];
}

static inline void writeFontAtlas(FT_Face face,  GLubyte* bitmap, BFreetypeFont::FontAtlas& atlas,unsigned int width, unsigned int height,
		unsigned int _maxRows, unsigned int _maxWidth, unsigned int _range)
{
    if (_range == 0) return ;
	size_t len = wcslen(CHARSET);
	int x = 0;

	int y = 0;
	int rowIndex = 0;

	int marginX, marginY = 0;

	marginX = (width - _range * _maxWidth) / _range;
	if (width < (_range * _maxWidth))
		marginX = 0;
	marginY = (height - _range * _maxRows) / _range;
	if (height < (_range * _maxRows))
		marginY = 0;
	for (int index = 0; index < len; index++)
	{
		if (FT_Load_Glyph(face, FT_Get_Char_Index(face, CHARSET[index]), FT_LOAD_RENDER))
			continue;

		FT_GlyphSlot g = face->glyph;

		atlas[CHARSET[index]].width = g->bitmap.width;
		atlas[CHARSET[index]].height = g->bitmap.rows;
		atlas[CHARSET[index]].y = y;
		atlas[CHARSET[index]].x = x;
		atlas[CHARSET[index]].left = g->bitmap_left;
		atlas[CHARSET[index]].top = g->bitmap_top;

		//Log::debug("LETTER = %c, TOP = %g, HEIGHT = %u", CHARSET[index], atlas[CHARSET[index]].top, atlas[CHARSET[index]].height);




		for (int j = 0; j < _maxRows ; j++)
		{
		  for (int i = 0; i < g->bitmap.width; i++)
		  {
		    bitmap[2 * ((i + x) + (j + y) * width)] = 255;
		    bitmap[2 * ((i + x) + (j + y) * width) + 1] =
		        (i >= g->bitmap.width || j >= g->bitmap.rows) ?
		        0 : g->bitmap.buffer[i + g->bitmap.width * (j)];
		  }
		}

		x += g->bitmap.width + marginX;
		rowIndex += 1;

		if (rowIndex >= _range)
		{
			rowIndex = 0;
			y += (_maxRows + marginY);
			x = 0;
		}

	}

	/*for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width - x; i++)
		{
			bitmap[2 * ((i + x) + j * width)] = 255;
			bitmap[2 * ((i + x) + j * width) + 1] = 0;
		}
	}*/

}

void BFreetypeFont::load(unsigned int h)
{
	if (CHARSET == NULL)
		loadCharset();
	/*
	if (_atlas.size() != 0)
		return ;
	*/
	BFont::load(h);

	FT_Library library;

	int error = FT_Init_FreeType(&library);

	if (error)
		return ;

	FT_Face face;
	error = FT_New_Memory_Face(library, (FT_Byte*)_reader->data(), _reader->dataSize(), 0, &face);
	if (error)
		return ;

	FT_Set_Char_Size(face, h << 6, h << 6, 96, 96);

	unsigned int _maxRows, _maxWidth, _range = 0;
	GLubyte* bitmap = createEmptyBitmap(face, &_width, &_height, &_maxRows, &_maxWidth, &_range);
	writeFontAtlas(face, bitmap, _atlas, _width, _height, _maxRows, _maxWidth, _range);
	_maxHeight = _maxRows;
	glGenTextures(1, &_texture[0]);
	glBindTexture(GL_TEXTURE_2D, _texture[0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, bitmap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, _width, _height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, bitmap);
	//Log::debug("Bind font %s to OpenGL (Error %i)", _name.c_str(), glGetError());
	BFont::unload();
	delete [] bitmap;
	FT_Done_Face(face);
	FT_Done_FreeType(library);
}

BFreetypeFont::FontAtlas& BFreetypeFont::atlas()
{
	return _atlas;
}

void BFreetypeFont::unload()
{
	if (_atlas.size() == 0)
		return ;
	glDeleteTextures(1, _texture);
	_atlas.clear();
}

GLuint BFreetypeFont::texture() const
{
	return _texture[0];
}

unsigned int BFreetypeFont::width() const
{
	return _width;
}

unsigned int BFreetypeFont::height() const
{
	return _height;
}

BFreetypeFont::~BFreetypeFont()
{
	unload();
}
