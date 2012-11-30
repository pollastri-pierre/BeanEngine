/*
 * AndroidCanvas.h
 *
 *  Created on: 3 févr. 2012
 *      Author: NC
 */


#ifdef __ANDROID__

#ifndef ANDROIDCANVAS_H_
#define ANDROIDCANVAS_H_

#include <queue>
#include <resource_manager/AndroidImage.h>
#include "BCanvas.h"

class AndroidCanvas : public BCanvas
{

private:
	BRect 				_rect;
	BColor				_color;
	float*				_scalers;
	bool				_stretched;
	int					_screenWidth, _screenHeight;
	float				_vertexPointer[8];
	float				_texturePointer[8];
	std::list<BRect> 	_clipping;

public:
	AndroidCanvas(float* scalers, int _screenWidth, int _screenHeight, bool stretched);
	virtual void clear();
	virtual void setColor(const BColor& color);
	virtual void setRect(const BRect& rect);
	virtual void fillRect();
	virtual bool stretched() const;
	virtual float scalerX() const;
	virtual float scalerY() const;
	virtual void pushMatrix();
	virtual void popMatrix();
	virtual void translate(float x, float y);
	virtual void translate(float value, float x, float y, float z);
	virtual void rotate(float angle);
	virtual void rotate(float angle, float x, float y, float z);
	virtual void drawChar(BChar c, BFont* font);
	virtual void drawImage(BImage* image, BRect subrect);
	virtual bool sizeofChar(BChar c, BFont* font, int size, lua_Number* width, lua_Number* height, lua_Number* left, lua_Number* top);
	virtual void clipRect(const BRect& rect);
	virtual void unclip();
	virtual ~AndroidCanvas();

};

#endif /* ANDROIDCANVAS_H_ */
#endif
