/*
 * BPicture.cpp
 *
 *  Created on: 25 mars 2012
 *      Author: NC
 */

#include "BPicture.h"

#define CLASS BPicture

B_DEFINE_CLASS("Picture", BRectangle)

B_NO_FUNCTION

B_METHODS_BEGIN

	B_METHOD(setSubRect)
	B_METHOD(setImage)
	B_METHOD(setSubRectPosition)
	B_METHOD(image)

B_METHODS_END

BPicture::BPicture() {
	// TODO Auto-generated constructor stub
	_backgroundColor = BColor(255, 255, 255, 255);
	setWidth(0.0);
	setHeight(0.0);
	_image = NULL;
}

void BPicture::setStyle(BStyle& style)
{
	if (keyExists(style, "image"))
		setImage(style["image"].c_str());
	if (keyExists(style, "subrectposition"))
		setSubRectPosition((stringToPoint(style["subrectposition"])));
	if (keyExists(style, "subrectsize"))
	{
		_subRect.setPosition(0, 0);
		_subRect.setSize(stringToSize(style["subrectsize"]));
	}
	if (keyExists(style, "subrectpadding"))
		_subRect.setOrigin(stringToPoint(style["subrectpadding"]));
	BRectangle::setStyle(style);
}

void BPicture::onDraw(BCanvas * const canvas, const BCanvasContext& context)
{
	//Log::debug("Avant %i %i %u", width(), height(), _image);

	if (_image != NULL)
	{
		float tX = context.rect.x() + _rotationCenter.x() * canvas->scalerX();
		float tY = context.rect.y() + _rotationCenter.y() * canvas->scalerY();
		BColor c = backgroundColor();
		c.setAlpha(c.alpha() * context.opacity);
		canvas->setColor(c);
		canvas->translate(tX, tY);
		canvas->rotate(_rotation);
		canvas->translate(-tX, -tY);
		canvas->setRect(context.rect);

		BRect s = _subRect;
		s.setX(s.x() + _subRectPosition.x() * s.width());
		s.setY(s.y() + _subRectPosition.y() * s.height());
		canvas->drawImage(_image, s);

		//setSize(_image->size());

	}
	else
		BRectangle::onDraw(canvas, context);
}

void BPicture::setSubRect(BRect rect)
{
	_subRect = rect;
	_subRectPosition = BPoint(0, 0);
	setSize(rect.size());
}

void BPicture::setImage(const char* image)
{
	_imageFile = image;
	_image = BResourceManager::instance()->getImage(_imageFile);
	if (_image != NULL && (height() == 1.0 || width() == 1.0 || height() == 0.0 || width() == 0.0))
	{
        setSize(_image->size());
	}
}

BString	BPicture::imageFile() const
{
	return _imageFile;
}

void BPicture::onResourceDestroyed()
{
	_image = NULL;
}

void BPicture::setSubRectPosition(BPoint point)
{
	_subRectPosition = point;
}

BRect BPicture::subRect() const { return _subRect; }

BPoint BPicture::subRectPosition() const { return _subRectPosition; }

B_IMPLEMENTATION(subRect)

	BClass<BRect>::pushObject(state, new BRect(self->subRect()));
	return 1;

B_END

B_IMPLEMENTATION(subRectPosition)

	BClass<BPoint>::pushObject(state, new BPoint(self->subRectPosition()));
	return 1;

B_END

B_IMPLEMENTATION(setSubRect)

	BRect rect;
	if (lua_isnil(state, 2))
		rect.set(0, 0, 0, 0);
	else
	{
		int argc = lua_gettop(state) - 1;
		lua_Number x = (argc == 2) ? 0 : luaL_checknumber(state, 2);
		lua_Number y = (argc == 2) ? 0 : luaL_checknumber(state, 3);
		lua_Number width = (argc == 2) ? luaL_checknumber(state, 2) : luaL_checknumber(state, 4);
		lua_Number height = (argc == 2) ? luaL_checknumber(state, 3) : luaL_checknumber(state, 5);

		rect.setX(x);
		rect.setY(y);
		rect.setWidth(width);
		rect.setHeight(height);
	}
	self->setSubRect(rect);
	return 0;

B_END

B_IMPLEMENTATION(setImage)

	self->setImage(luaL_checkstring(state, 2));
	return 0;

B_END

B_IMPLEMENTATION(setSubRectPosition)

	BPoint point;
	int nbrArgs = lua_gettop(state);
	if (nbrArgs == 3)
	{
		point.setX(luaL_checknumber(state, 2));
		point.setY(luaL_checknumber(state, 3));
	}
	self->setSubRectPosition(point);
	return 0;

B_END

B_IMPLEMENTATION(image)

	lua_pushstring(state, self->imageFile().c_str());
	return 1;

B_END

BPicture::~BPicture() {

}

