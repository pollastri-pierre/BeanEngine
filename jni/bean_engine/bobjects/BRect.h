/*
 * BRect.h
 *
 *  Created on: 29 janv. 2012
 *      Author: NC
 */

#ifndef BRECT_H_
#define BRECT_H_

#include "BClass.hpp"
#include "BObject.h"
#include "BSize.h"
#include "BPoint.h"

class BRect : public BObject {

protected:
	BPoint _position;
	BSize _size;

public:

	B_LUA_CLASS

	BRect();
	BRect(lua_Number x, lua_Number y, lua_Number widtht, lua_Number height);
	virtual lua_Number x() const;
	virtual void setX(lua_Number x);
	virtual void setY(lua_Number y);
	virtual void setWidth(lua_Number width);
	virtual void setHeight(lua_Number height);
	virtual	void setSize(const BSize& size);
	virtual void setOrigin(const BPoint& origin);
	virtual void setPosition(const lua_Number x, const lua_Number y);
	virtual void set(lua_Number left, lua_Number top, lua_Number right, lua_Number bottom);
	virtual lua_Number y() const;
	virtual lua_Number width() const;
	virtual lua_Number height() const;
	virtual BPoint position() const;
	virtual BSize size() const;
	virtual bool isPointInRect(const BPoint& point);
	virtual std::string toString() const;
	virtual bool intersect(const BRect& rect) const;
	virtual BRect operator&(const BRect& rect) const;

	~BRect();

	B_DECLARATION(x)
	B_DECLARATION(y)
	B_DECLARATION(position)
	B_DECLARATION(width)
	B_DECLARATION(height)
	B_DECLARATION(size)
	B_DECLARATION(setX)
	B_DECLARATION(setY)
	B_DECLARATION(setOrigin)
	B_DECLARATION(setWidth)
	B_DECLARATION(setHeight)
	B_DECLARATION(setSize)
	B_DECLARATION(isPointInRect)
	B_DECLARATION(setPosition)
};

#endif /* BRECT_H_ */
