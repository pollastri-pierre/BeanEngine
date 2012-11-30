//
//  SFMLCanvas.cpp
//  BeanEngine
//
//  Created by Pierre POLLASTRI on 24/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "SFMLCanvas.h"
#include "SFMLImage.h"
#include "BFreetypeFont.h"

SFMLCanvas::SFMLCanvas(sf::RenderWindow& window, float scalerX, float scalerY, bool fullscreen) : _window(window), _scalerX(scalerX), _scalerY(scalerY), _fullscreen(fullscreen)
{
    
}

void SFMLCanvas::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SFMLCanvas::setRect(const BRect& rect)
{
    _rect = rect;
}

void SFMLCanvas::setColor(const BColor &color)
{
    _color = color;
    glColor4f(color.red() / 255.f, color.green() / 255.f, color.blue() / 255.f, color.alpha() / 255.f);
}

void SFMLCanvas::fillRect()
{
    glBegin(GL_QUADS);
    
    glVertex2f(_rect.x(), _rect.y());
    glVertex2f(_rect.x(),  _rect.y() + _rect.height());
    glVertex2f(_rect.x() + _rect.width(), _rect.y() + _rect.height());
    glVertex2f(_rect.x() + _rect.width(), _rect.y());
    
    glEnd();
}

bool SFMLCanvas::stretched() const
{
    return _fullscreen;
}

float SFMLCanvas::scalerX() const
{
    return _scalerX;
}

float SFMLCanvas::scalerY() const
{
    return _scalerY;
}

void SFMLCanvas::pushMatrix()
{
    glPushMatrix(); 
}

void SFMLCanvas::popMatrix()
{
    glPopMatrix();
}

void SFMLCanvas::translate(float x, float y)
{
    glTranslatef(x, y, 0);
}

void SFMLCanvas::translate(float value, float x, float y, float z)
{
    glRotatef(value, x, y, z);
}

void SFMLCanvas::rotate(float angle)
{
    glRotatef(angle, 0.f, 0.f, 1.f);
}

void SFMLCanvas::rotate(float angle, float x, float y, float z)
{
    glRotatef(angle, x, y, z);
}

void SFMLCanvas::drawChar(BChar c, BFont *f)
{
    BFreetypeFont* font = dynamic_cast<BFreetypeFont*>(f);
    
	if (font == NULL)
		return ;
	BFreetypeFont::FontAtlas& atlas = font->atlas();
    
	if (atlas.find(c) == atlas.end())
		return ;
	

    glEnable(GL_TEXTURE_2D);
    
	glBindTexture(GL_TEXTURE_2D, font->texture());
    
    glBegin(GL_QUADS);
    glTexCoord2f((float) ((((float) atlas[c].x) + 0.5f) / (float) font->width()),
                 (float) ((((float) atlas[c].y) + 0.5f) / (float) font->height()));
    glVertex2f(_rect.x(), _rect.y());
    glTexCoord2f(((((float) atlas[c].x) + 0.5f) / (float) font->width()),
                    (float) (((float)((float) atlas[c].height + (float) atlas[c].y) - 0.5f) / (float) font->height()));
    glVertex2f(_rect.x(),  _rect.y() + _rect.height());
    glTexCoord2f((float) (( (float)((float) atlas[c].x + (float) atlas[c].width) - 0.5f) / (float) font->width()),
                 (float) ((((float) atlas[c].height + (float) atlas[c].y) - 0.5f) / (float) font->height()));
    glVertex2f(_rect.x() + _rect.width(), _rect.y() + _rect.height());
    glTexCoord2f((float) (((float)((float)atlas[c].x + (float) atlas[c].width) - 0.5f) / (float) font->width()),
                (float) (((float)((float) atlas[c].y) + 0.5f) / (float) font->height()));
    glVertex2f(_rect.x() + _rect.width(), _rect.y());
    glEnd();
    
	glDisable(GL_TEXTURE_2D);
}

void SFMLCanvas::drawImage(BImage *image, BRect subrect)
{
    SFMLImage* img = (SFMLImage*)image;
    if (img == NULL || img->size().width() <= 0 || img->size().height() <= 0)
		return ;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, img->textureId());
    
    
	if (subrect.width() == 0 || subrect.height() == 0)
	{
		subrect.set(0, 0, image->size().width(), image->size().height());
	}
    
    glBegin(GL_QUADS);
   
    glTexCoord2f((subrect.x() + 0.5) / img->size().width(), (subrect.y() + 0.5) / img->size().height());
    glVertex2f(_rect.x(), _rect.y());
    
    glTexCoord2f((subrect.x() + 0.5) / img->size().width(), (subrect.y() + subrect.height() - 0.5) / img->size().height());
    glVertex2f(_rect.x(),  _rect.y() + _rect.height());
    
    glTexCoord2f((subrect.x() + subrect.width() - 0.5) / img->size().width(), (subrect.y() + subrect.height() - 0.5) / img->size().height());
    glVertex2f(_rect.x() + _rect.width(), _rect.y() + _rect.height());
    
    glTexCoord2f((subrect.x() + subrect.width() - 0.5) / img->size().width(), (subrect.y() + 0.5) / img->size().height());
    glVertex2f(_rect.x() + _rect.width(), _rect.y());
    
    glEnd();
    
    
    
	//float h = (float)(0.5 * ((1.0f / 1) / pixelsPerTile));;
    
	glDisable(GL_TEXTURE_2D);
}

bool SFMLCanvas::sizeofChar(BChar c, BFont *f, int size, lua_Number *width, lua_Number *height, lua_Number *left, lua_Number *top)
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

void SFMLCanvas::clipRect(const BRect &rect)
{
    
}

void SFMLCanvas::unclip()
{
    
}
