/*
 * BCanvas.h
 *
 *  Created on: 3 févr. 2012
 *      Author: NC
 */

#ifndef BCANVAS_H_
#define BCANVAS_H_

#include <bobjects/BColor.h>
#include <bobjects/BRect.h>
#include <application/BString.h>
#include <bobjects/BSize.h>
#include <resource_manager/BImage.h>

class BFont;

struct BCanvasContext
{
	BRect rect;
	lua_Number opacity;
};

class BCanvas
{

public:
	virtual void clear() = 0;
	virtual void setColor(const BColor& color) = 0;
	virtual void setRect(const BRect& rect) = 0;
	virtual void fillRect() = 0;
	virtual bool stretched() const = 0;
	virtual float scalerX() const = 0;
	virtual float scalerY() const = 0;
	virtual void pushMatrix() = 0;
	virtual void popMatrix() = 0;
	virtual void translate(float x, float y) = 0;
    virtual void translate(float value, float x, float y, float z) = 0;
	virtual void rotate(float angle) = 0;
    virtual void rotate(float angle, float x, float y, float z) = 0;
	virtual void drawChar(BChar c, BFont* font) = 0;
	virtual void drawImage(BImage* image, BRect subRect) = 0;
	virtual bool sizeofChar(BChar c, BFont* font, int size, lua_Number* width, lua_Number* height, lua_Number* left, lua_Number* top) = 0;
	virtual void clipRect(const BRect& rect) = 0;
	virtual void unclip() = 0;
    
    virtual ~BCanvas() {}
};

#endif /* BCANVAS_H_ */
