/*
 * AndroidCanvas.cpp
 *
 *  Created on: 3 févr. 2012
 *      Author: NC
 */


#ifdef __ANDROID__
#include "AndroidCanvas.h"

#include <GLES/gl.h>
#include <GLES/glext.h>
#include "BFont.h"
#include "BFreetypeFont.h"



AndroidCanvas::AndroidCanvas(float* scalers, int width, int height,  bool stretched)
{
	_screenWidth = width;
	_screenHeight = height;
	_scalers = scalers;
	_stretched = stretched;
	glVertexPointer(2, GL_FLOAT, 0, _vertexPointer);
}

void AndroidCanvas::clear()
{

}

void AndroidCanvas::setColor(const BColor& color)
{
	glColor4f(color.red() / 255.f, color.green() / 255.f, color.blue() / 255.f, color.alpha() / 255.f);
}

void AndroidCanvas::setRect(const BRect& rect)
{
	_vertexPointer[0] = rect.x(); _vertexPointer[1] = rect.y();
	_vertexPointer[2] = rect.x(); _vertexPointer[3] = rect.y() + rect.height();
	_vertexPointer[4] = rect.x() + rect.width(); _vertexPointer[5] = rect.y() + rect.height();
	_vertexPointer[6] = rect.x() + rect.width(); _vertexPointer[7] = rect.y();
}

void AndroidCanvas::clipRect(const BRect& rect)
{
	return ;
	_clipping.push_back(rect);

	BRect r;

	std::list<BRect>::iterator it = _clipping.begin();
	std::list<BRect>::iterator ite = _clipping.end();

	r = *it;
	for (; it != ite; ++it)
	{
		r = r & *it;
		if (r.width() == 0 || r.height() == 0)
		{
			glScissor(0, 0, 0, 0);
			return ;
		}
	}

	glScissor((int)r.x(), _screenHeight - (r.y() + r.height()), (int)r.width(), _screenHeight - r.y());
}

void AndroidCanvas::unclip()
{
	return ;
	_clipping.pop_back();
	BRect r;

	std::list<BRect>::iterator it = _clipping.begin();
	std::list<BRect>::iterator ite = _clipping.end();

	r = *it;
	for (; it != ite; ++it)
	{
		r = r & *it;
	}
	if (r.width() == 0 || r.height() == 0)
	{
		glScissor(0, 0, 0, 0);
		return ;
	}
	glScissor((int)r.x(), _screenHeight - (r.y() + r.height()), (int)r.width(), _screenHeight - r.y());
}

void AndroidCanvas::fillRect()
{
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

bool AndroidCanvas::stretched() const
{
	return _stretched;
}

float AndroidCanvas::scalerX() const
{
	return _scalers[0];
}

float AndroidCanvas::scalerY() const
{
	return _scalers[1];
}

void AndroidCanvas::pushMatrix()
{
	glPushMatrix();
}

void AndroidCanvas::popMatrix()
{
	glPopMatrix();
}

void AndroidCanvas::rotate(float angle)
{
	glRotatef(angle, 0.f, 0.f, 1.f);
}

void AndroidCanvas::rotate(float angle, float x, float y, float z)
{
	glRotatef(angle, x, y, z);
}

void AndroidCanvas::translate(float x, float y)
{
	glTranslatef(x, y, 0);
}

void AndroidCanvas::translate(float value, float x, float y, float z)
{
	glTranslatef(x * value, y * value, z * value);
}

void AndroidCanvas::drawChar(BChar c, BFont* f)
{
	BFreetypeFont* font = dynamic_cast<BFreetypeFont*>(f);

	if (font == NULL)
		return ;

	BFreetypeFont::FontAtlas& atlas = font->atlas();

	if (atlas.find(c) == atlas.end())
		return ;
	glActiveTexture(GL_TEXTURE0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, font->texture());

	_texturePointer[0] = (float) ((((float) atlas[c].x) + 0.5f) / (float) font->width());
	_texturePointer[1] = (float) ((((float) atlas[c].y) + 0.5f) / (float) font->height()); // Haut gauche
	_texturePointer[2] = (float) ((((float) atlas[c].x) + 0.5f) / (float) font->width());
	_texturePointer[3] = (float) (((float)((float) atlas[c].height + (float) atlas[c].y) - 0.5f) / (float) font->height()); // bas gauche
	_texturePointer[4] = (float) (((float)((float) atlas[c].x + (float) atlas[c].width) - 0.5f) / (float) font->width());
	_texturePointer[5] = (float) ((((float) atlas[c].height + (float) atlas[c].y) - 0.5f) / (float) font->height()); // bas droit
	_texturePointer[6] = (float) (((float)((float)atlas[c].x + (float) atlas[c].width) - 0.5f) / (float) font->width());
	_texturePointer[7] = (float) (((float)((float) atlas[c].y) + 0.5f) / (float) font->height()); // haut droit

	glTexCoordPointer(2, GL_FLOAT, 0, _texturePointer);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void AndroidCanvas::drawImage(BImage* image, BRect subrect)
{
	AndroidImage* img = dynamic_cast<AndroidImage*>(image);
	if (img == NULL || img->rSize().width() <= 0 || img->rSize().height() <= 0)
		return ;
	glActiveTexture(GL_TEXTURE0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, img->textureId());


	if (subrect.width() == 0 || subrect.height() == 0)
	{
		subrect.set(0, 0, image->size().width(), image->size().height());
	}

	//float h = (float)(0.5 * ((1.0f / 1) / pixelsPerTile));;

	_texturePointer[0] = (subrect.x() + 0.5f) / img->rSize().width();
	_texturePointer[1] = (subrect.y() + 0.5f) / img->rSize().height(); // Haut gauche
	_texturePointer[2] = (subrect.x() + 0.5f) / img->rSize().width();
	_texturePointer[3] = (subrect.y() + subrect.height() - 0.5f) / img->rSize().height(); // bas gauche
	_texturePointer[4] = (subrect.x() + subrect.width() - 0.5f) / img->rSize().width();
	_texturePointer[5] = (subrect.y() + subrect.height() - 0.5f) / img->rSize().height();// bas droit
	_texturePointer[6] = (subrect.x() + subrect.width() - 0.5f) / img->rSize().width();
	_texturePointer[7] = (subrect.y() + 0.5f) / img->rSize().height(); // haut droit
	//Log::debug("SUBR", _texturePointer[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, _texturePointer);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);


}

bool AndroidCanvas::sizeofChar(BChar c, BFont* f, int size, lua_Number* width, lua_Number* height, lua_Number* left, lua_Number* top)
{
	BFreetypeFont* font = dynamic_cast<BFreetypeFont*>(f);

	if (font == NULL)
		return false;

	BFreetypeFont::FontAtlas& atlas = font->atlas();

	if (atlas.find(c) == atlas.end())
		return false;

	lua_Number w = atlas[c].width;
	lua_Number h = atlas[c].height;

	lua_Number scaler = (float)size / font->maxHeight();

	*height = scaler * (double)atlas[c].height;
	*width = scaler * w;
	*left = scaler * (double)atlas[c].left;
	*top = scaler * (double)atlas[c].top;

	return true;
}

AndroidCanvas::~AndroidCanvas() {
	// TODO Auto-generated destructor stub
}

#endif
