/*
 * BColor.cpp
 *
 *  Created on: 29 janv. 2012
 *      Author: NC
 */

#include "BColor.h"
#include <application/BString.h>

#define CLASS BColor

std::map<std::string, BColor>* BColor::_keyColors = NULL;

B_DEFINE_CLASS("Color", BObject)

B_NO_FUNCTION

B_METHODS_BEGIN
	{"red", B_NAME(red)},
	{"green", B_NAME(green)},
	{"blue", B_NAME(blue)},
	{"alpha", B_NAME(alpha)},
	{"setAlpha", B_NAME(setAlpha)},
	B_METHOD(setRed)
	B_METHOD(setGreen)
	B_METHOD(setBlue)
B_METHODS_END

B_CONSTRUCTOR

	int args;
	if ((args = lua_gettop(state)) < 1)
		return ;
	if (args == 1)
	{
		const char* key = luaL_checklstring(state, 1, NULL);
		fromString(key);
		return ;
	}
	this->setRed(luaL_checknumber(state, 1));
	this->setGreen(luaL_checknumber(state, 2));
	this->setBlue(luaL_checknumber(state, 3));
	if (args > 3)
		this->setAlpha(luaL_checknumber(state, 4));
	else
		this->setAlpha(255);

B_END

void BColor::fromString(std::string key)
{
	if (_keyColors->find(key) != _keyColors->end())
		*this = (*_keyColors)[key];
}

void BColor::fromLua(lua_State* state, int begin)
{
	int args = lua_gettop(state) - begin;

	if (lua_isstring(state, 1 + begin) && args == 1)
	{
		const char* str = luaL_checkstring(state, 1 + begin);
		fromString(str);
		return ;
	}

	if (args == 1)
	{
		BColor* c = BClass<BColor>::self(state, 1 + begin);
		*this = *c;
		return ;
	}

	if (args >= 3)
	{
		_red = luaL_checknumber(state, 1 + begin);
		_green = luaL_checknumber(state, 2 + begin);
		_blue = luaL_checknumber(state, 3 + begin);
		_alpha = 255;
		if (args == 4)
			_alpha = luaL_checknumber(state, 4 + begin);
		return ;
	}
}

BColor::BColor()
{
	// TODO Auto-generated constructor stub
	_red = 0;
	_green = 0;
	_blue = 0;
	_alpha  = 255;

	if (_keyColors == NULL)
	{
		_keyColors = new std::map<std::string, BColor>();
		(*_keyColors)["red"] = BColor(255, 0, 0);
		(*_keyColors)["green"] = BColor(0, 255, 0);
		(*_keyColors)["blue"] = BColor(0, 0, 255);
		(*_keyColors)["black"] = BColor(0, 0, 0);
		(*_keyColors)["white"] = BColor(255, 255, 255);
	}
}

BColor::BColor(lua_Number red, lua_Number green, lua_Number blue, lua_Number alpha)
{
	_red = red;
	_green = green;
	_blue = blue;
	_alpha = alpha;
}

std::string BColor::toString() const
{
	BString out;
	out << "Color {red = " << _red << ", green = "  << _green << ", blue = " << _blue << ", alpha = " << _alpha << "}";
	return out.c_str();
}

void BColor::setRed(lua_Number red)
{
	_red = red;
}

void BColor::setGreen(lua_Number green)
{
	_green = green;
}

void BColor::setBlue(lua_Number blue)
{
	_blue = blue;
}

void BColor::setAlpha(lua_Number alpha)
{
	_alpha = alpha;
}

lua_Number BColor::red() const
{
	return _red;
}

lua_Number BColor::green() const
{
	return _green;
}

lua_Number BColor::blue() const
{
	return _blue;
}

lua_Number BColor::alpha() const
{
	return _alpha;
}

BColor::~BColor()
{
	// TODO Auto-generated destructor stub
}

B_IMPLEMENTATION(red)

	lua_pushnumber(state, self->red());
	return 1;

B_END

B_IMPLEMENTATION(green)

	lua_pushnumber(state, self->green());
	return 1;

B_END

B_IMPLEMENTATION(blue)

	lua_pushnumber(state, self->blue());
	return 1;

B_END

B_IMPLEMENTATION(alpha)

	lua_pushnumber(state, self->alpha());
	return 1;

B_END

B_IMPLEMENTATION(setRed)

	self->setRed(luaL_checknumber(state, 2));
	return 0;

B_END

B_IMPLEMENTATION(setGreen)

	self->setGreen(luaL_checknumber(state, 2));
	return 0;

B_END

B_IMPLEMENTATION(setBlue)

	self->setBlue(luaL_checknumber(state, 2));
	return 0;

B_END

B_IMPLEMENTATION(setAlpha)

	self->setAlpha(luaL_checknumber(state, 2));
	return 0;

B_END
