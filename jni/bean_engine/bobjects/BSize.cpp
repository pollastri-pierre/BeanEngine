/*
 * BSize.cpp
 *
 *  Created on: 29 janv. 2012
 *      Author: NC
 */

#include "BSize.h"
#include <application/BString.h>

template<>
const char* BClass<BSize>::_className = "Size";

template<>
const struct luaL_Reg BClass<BSize>::_functions[] = {
		{NULL, NULL}
};

template<>
const struct luaL_Reg BClass<BSize>::_methods[] = {
		{"width", BSize::___width},
		{"height", BSize::___height},
		{"setWidth", BSize::___setWidth},
		{"setHeight", BSize::___setHeight},
		{NULL, NULL}
};

template<>
const BMetaClass* BClass<BSize>::_super = new BClass<BObject>;

BSize::BSize() {
	// TODO Auto-generated constructor stub
	_width = 0;
	_height = 0;
}

BSize::BSize(lua_Number width, lua_Number height)
{
	_width = width;
	_height = height;
}

std::string BSize::toString() const
{
	BString out;
	out << "Size {width = " << _width << ", height = "  << _height << "}";
	return out.c_str();
}

void BSize::setWidth(lua_Number width)
{
	_width = width;
}

void BSize::setHeight(lua_Number height)
{
	_height = height;
}

lua_Number BSize::width() const
{
	return _width;
}

lua_Number BSize::height() const
{
	return _height;
}

int BSize::___setWidth(lua_State* state)
{
	BSize* self = BClass<BSize>::self(state);
	lua_Number value = luaL_checknumber(state, 2);
	if (self)
		self->setWidth(value);
	return 0;
}

int BSize::___setHeight(lua_State* state)
{
	BSize* self = BClass<BSize>::self(state);
	lua_Number value = luaL_checknumber(state, 2);
	if (self)
		self->setHeight(value);
	return 0;
}

int BSize::___width(lua_State* state)
{
	BSize* self = BClass<BSize>::self(state);
	if (self)
		lua_pushnumber(state, self->width());
	else
		lua_pushnil(state);
	return 1;
}

int BSize::___height(lua_State* state)
{
	BSize* self = BClass<BSize>::self(state);
	if (self)
		lua_pushnumber(state, self->height());
	else
		lua_pushnil(state);
	return 1;
}

void BSize::init(lua_State* state)
{
	int nbrArgs = lua_gettop(state);
	if (nbrArgs == 2)
	{
		setWidth(luaL_checknumber(state, 1));
		setHeight(luaL_checknumber(state, 2));
	}
}

BSize::~BSize() {
	// TODO Auto-generated destructor stub
}
