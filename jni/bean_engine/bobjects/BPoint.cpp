/*
 * BPoint.cpp
 *
 *  Created on: 27 janv. 2012
 *      Author: NC
 */

#include "BPoint.h"
#include <application/BString.h>

template<>
const char* BClass<BPoint>::_className = "Point";

template<>
const struct luaL_Reg BClass<BPoint>::_functions[] = {
		{NULL, NULL}
};

template<>
const struct luaL_Reg BClass<BPoint>::_methods[] = {
		{"x", BPoint::___x},
		{"y", BPoint::___y},
		{"setX", BPoint::___setX},
		{"setY", BPoint::___setY},
		{NULL, NULL}
};

template<>
const BMetaClass* BClass<BPoint>::_super = new BClass<BObject>;

BPoint::BPoint()
{
	_x = 0.0;
	_y = 0.0;
}

BPoint::BPoint(lua_Number x, lua_Number y)
{
	_x = x;
	_y = y;
}

std::string BPoint::toString() const
{
	BString out;
	out << "Point {x = " << _x << ", y = "  << _y << "}";
	return out.c_str();
}

int BPoint::___x(lua_State* state)
{
	BPoint* self = BClass<BPoint>::self(state);
	if (self)
		lua_pushnumber(state, self->x());
	else
		lua_pushnil(state);
	return 1;
}

int BPoint::___y(lua_State* state)
{
	BPoint* self = BClass<BPoint>::self(state);
	if (self)
		lua_pushnumber(state, self->y());
	else
		lua_pushnil(state);
	return 1;
}

int BPoint::___setX(lua_State* state)
{
	BPoint* self = BClass<BPoint>::self(state);
	lua_Number value = luaL_checknumber(state, 2);
	if (self)
		self->setX(value);
	return 0;
}

int BPoint::___setY(lua_State* state)
{
	BPoint* self = BClass<BPoint>::self(state);
	lua_Number value = luaL_checknumber(state, 2);
	if (self)
		self->setY(value);
	return 0;
}

void BPoint::setX(lua_Number x)
{
	_x = x;
}

void BPoint::setY(lua_Number y)
{
	_y = y;
}

lua_Number BPoint::x() const
{
	return _x;
}

lua_Number BPoint::y() const
{
	return _y;
}

void BPoint::init(lua_State* state)
{
	int nbrArgs = lua_gettop(state);
	if (nbrArgs == 2)
	{
		setX(luaL_checknumber(state, 1));
		setY(luaL_checknumber(state, 2));
	}
}
