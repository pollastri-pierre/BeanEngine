/*
 * AndroidImage.cpp
 *
 *  Created on: 25 mars 2012
 *      Author: NC
 */


#ifdef __ANDROID__
#include "AndroidImage.h"

static void png_read_callback(png_structp png, png_bytep data, png_size_t size)
{
	AndroidResourceReader* reader = (AndroidResourceReader*)png_get_io_ptr(png);
	reader->read(data, size);
}

AndroidImage::AndroidImage(const BString& name, const BString& path) : BImage(name, path)
{
	// TODO Auto-generated constructor stub
	_textureId = 0;
}

static inline int next_p2 (int a)
{
    int rval = 1;
    while (rval < a) rval<<=1;
    return rval;
}

void AndroidImage::load(const BString& path)
{
	BImage::load(path);
	png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	AndroidResourceReader* reader = new AndroidResourceReader();
	reader->open(_path);
	// Signature check
	png_byte header[8];
	if (reader->read(header, sizeof(header)) == -1)
	{
		Log::error("AndroidImage load error: Cannot read Header of %s", _path.c_str());
		delete reader;
		return ;
	}
	if (png_sig_cmp(header, 0, 8) != 0)
	{
		Log::error("AndroidImage load error: %s is not a PNG file", _path.c_str());
		delete reader;
		return ;
	}
	// End Signature check
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		Log::error("AndroidImage load error: unable to create png_struct");
		delete reader;
		return ;
	}
	png_infop info_ptr = png_create_info_struct(png_ptr);
	png_infop end_info = png_create_info_struct(png_ptr);

	 if (setjmp(png_jmpbuf(png_ptr)) || !info_ptr || !end_info)
	 {
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		Log::error("Error during setjmp");
		delete reader;
		return;
	}

	 png_set_read_fn(png_ptr, reader, png_read_callback);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	png_int_32 lDepth;
	png_int_32 lColorType;
	png_uint_32 lWidth, lHeight;
	png_get_IHDR(png_ptr, info_ptr, &lWidth, &lHeight,&lDepth, &lColorType, NULL, NULL, NULL);
	png_read_update_info(png_ptr, info_ptr);

	png_int_32 lRowSizeBytes = png_get_rowbytes(png_ptr, info_ptr);

	png_int_32 _width, _height, rWidth, rHeight, pixelSize;
	_height = lHeight;
	_width = lWidth;
	rWidth = next_p2(_width);
	rHeight = next_p2(_height);
	pixelSize = lRowSizeBytes / _width;

	//Log::debug("Size %i x %i", _width, _height);
	png_byte* lImageBuffer = new png_byte[rWidth * rHeight * pixelSize];
	if (!lImageBuffer)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		delete reader;
		return ;
	}

	// Allocates a table of pointers, each pointing to a row
	// the final image buffer.
	png_bytep* lRowPointers = new png_bytep[lHeight];
	if (!lRowPointers)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		delete[] lImageBuffer;
		delete reader;
		return ;
	}

	for (int i = 0; i < lHeight; ++i)
	{
		lRowPointers[lHeight - 1 - i] = lImageBuffer + (lHeight -1 - i) * rWidth * pixelSize;
	}
	// Reads the image into the image buffer.
	png_read_image(png_ptr, lRowPointers);
	// Loads image into OpenGL.

	glGenTextures(1, &_textureId);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	_format = (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGBA) ? GL_RGBA : GL_RGB;
	_type = GL_UNSIGNED_BYTE;
	glTexImage2D(GL_TEXTURE_2D, 0, _format, rWidth, rHeight, 0, _format, _type, lImageBuffer);
	//Log::debug("Bind image %s to OpenGL (Error %i)", _name.c_str(), glGetError());
	// Frees memory and resources.
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	delete[] lRowPointers;
	delete reader;

	_size.setWidth(_width);
	_size.setHeight(_height);
	_rSize.setWidth(rWidth);
	_rSize.setHeight(rHeight);
	//Log::debug("Loaded %s", _path.c_str());

	return ;
}

int32_t AndroidImage::textureId() const { return _textureId; }

BSize AndroidImage::rSize() const { return _rSize; }

void AndroidImage::unload()
{
	glDeleteTextures(1, &_textureId);
}

AndroidImage::~AndroidImage() {
	// TODO Auto-generated destructor stub
}

#endif

