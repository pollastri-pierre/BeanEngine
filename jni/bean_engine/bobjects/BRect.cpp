/*
 * BRect.cpp
 *
 *  Created on: 29 janv. 2012
 *      Author: NC
 */

#include "BRect.h"
#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#define CLASS BRect

#define ABS(x) ((x) < 0) ? -(x) : x

B_DEFINE_CLASS("Rect", BObject)

B_NO_FUNCTION

B_METHODS_BEGIN
	B_METHOD(setPosition)
	{"x", B_NAME(x)},
	{"y", B_NAME(y)},
	{"width", B_NAME(width)},
	{"height", B_NAME(height)},
	{"size", B_NAME(size)},
	{"position", B_NAME(position)},
	{"setX", B_NAME(setX)},
	{"setY", B_NAME(setY)},
	{"setWidth", B_NAME(setWidth)},
	{"setHeight", B_NAME(setHeight)},
	{"setOrigin", B_NAME(setOrigin)},
	{"setSize", B_NAME(setSize)},
	{"isPointInRect", B_NAME(isPointInRect)},
B_METHODS_END


BRect::BRect() {
	// TODO Auto-generated constructor stub

}

BRect::BRect(lua_Number x, lua_Number y, lua_Number widtht, lua_Number height)
{
	set(x, y, x + widtht, y + height);
}

std::string BRect::toString() const
{
	return "Rect {" + _position.toString() + ", " + _size.toString() + "}";
}

lua_Number BRect::x() const
{
	return _position.x();
}

lua_Number BRect::y() const
{
	return _position.y();
}

lua_Number BRect::width() const
{
	return _size.width();
}

lua_Number BRect::height() const
{
	return _size.height();
}

BSize BRect::size() const
{
	return _size;
}

BPoint BRect::position() const
{
	return _position;
}

void BRect::setX(lua_Number x)
{
	_position.setX(x);
}

void BRect::setY(lua_Number y)
{
	_position.setY(y);
}

void BRect::setWidth(lua_Number width)
{
	_size.setWidth(width);
}

void BRect::set(lua_Number left, lua_Number top, lua_Number right, lua_Number bottom)
{
	setX(left);
	setY(top);
	setWidth(ABS(left - right));
	setHeight(ABS(top - bottom));
}

void BRect::setPosition(const lua_Number x, const lua_Number y)
{
	setX(x);
	setY(y);
}

void BRect::setHeight(lua_Number height)
{
	_size.setHeight(height);
}

void BRect::setSize(const BSize& size)
{
	_size = size;
}

void BRect::setOrigin(const BPoint& origin)
{
	_position = origin;
}

bool BRect::intersect(const BRect& rect) const
{
	return 	x() <= ((rect).x() + (rect).width()) && (rect).x() <= (x() + width()) &&
			y() <= ((rect).y() + (rect).height()) && (rect).y() <= (y() + height());
}

lua_Number max(lua_Number a, lua_Number b)
{
	return (::abs(a) < abs(b)) ? b : a;
}

lua_Number min(lua_Number a, lua_Number b)
{
	return (::abs(a) > abs(b)) ? b : a;
}

BRect BRect::operator &(const BRect& other) const
{
	if(intersect(other))
	{
		BRect r;

		r.set(max(x(), other.x()), max(y(), other.y()),
		min(x() + width(), other.x() + other.width()), min(y() + height(), other.y() + other.height()));
		return r;
	}

	return BRect();
}

bool BRect::isPointInRect(const BPoint& point)
{
	return point.x() >= x() && point.x() <= x() + width() && point.y() >= y() && point.y() <= y() + height();
}

B_CONSTRUCTOR

	if (lua_gettop(state) == 0)
		return ;

	int argc = lua_gettop(state);
	lua_Number x = (argc == 2) ? 0 : luaL_checknumber(state, 1);
	lua_Number y = (argc == 2) ? 0 : luaL_checknumber(state, 2);
	lua_Number width = (argc == 2) ? luaL_checknumber(state, 1) : luaL_checknumber(state, 3);
	lua_Number height = (argc == 2) ? luaL_checknumber(state, 2) : luaL_checknumber(state, 4);

	setX(x);
	setY(y);
	setWidth(width);
	setHeight(height);

B_END

B_IMPLEMENTATION(x)

	lua_pushnumber(state, self->x());
	return 1;

B_END

B_IMPLEMENTATION(y)

	lua_pushnumber(state, self->y());
	return 1;

B_END

B_IMPLEMENTATION(width)

	lua_pushnumber(state, self->width());
	return 1;

B_END

B_IMPLEMENTATION(height)

	lua_pushnumber(state, self->height());
	return 1;

B_END

B_IMPLEMENTATION(size)
	return BClass<BSize>::newInstance(state, new BSize(self->size()));
B_END

B_IMPLEMENTATION(position)
	return BClass<BPoint>::newInstance(state, new BPoint(self->position()));
B_END

B_IMPLEMENTATION(setX)

	lua_Number value = luaL_checknumber(state, 2);
	self->setX(value);
	return 0;

B_END

B_IMPLEMENTATION(setY)

	lua_Number value = luaL_checknumber(state, 2);
	self->setY(value);
	return 0;

B_END

B_IMPLEMENTATION(setWidth)

	lua_Number value = luaL_checknumber(state, 2);
	self->setWidth(value);
	return 0;

B_END

B_IMPLEMENTATION(setHeight)

	lua_Number value = luaL_checknumber(state, 2);
	self->setHeight(value);
	return 0;

B_END

B_IMPLEMENTATION(setSize)

	switch(lua_gettop(state))
	{
	case 2:
	{
		BSize* value = BClass<BSize>::self(state, 2);
		if (value)
			self->setSize(*value);
	}
	break;
	case 3:
	{
		lua_Number width = luaL_checknumber(state, 2);
		lua_Number height = luaL_checknumber(state, 3);
		self->setSize(BSize(width, height));
	}
	break;
	}
	return 0;

B_END

B_IMPLEMENTATION(setOrigin)

	BPoint* value = BClass<BPoint>::self(state, 2);
	if (value)
		self->setOrigin(*value);
	return 0;

B_END

B_IMPLEMENTATION(isPointInRect)

	BPoint* point = BClass<BPoint>::self(state, 2);
	if (point)
		lua_pushboolean(state, self->isPointInRect(*point));
	return 1;
B_END

B_IMPLEMENTATION(setPosition)

	switch (lua_gettop(state))
	{
	case 2:
	{
		BPoint* point = BClass<BPoint>::self(state, 2);
		self->setOrigin(*point);
	}
		break;
	case 3:
	{
		lua_Number x = luaL_checknumber(state, 2);
		lua_Number y = luaL_checknumber(state, 3);
		self->setPosition(x, y);
	}
		break;
	}

	return 0;

B_END

BRect::~BRect() {
	// TODO Auto-generated destructor stub
}
