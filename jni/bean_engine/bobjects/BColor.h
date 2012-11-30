/*
 * BColor.h
 *
 *  Created on: 29 janv. 2012
 *      Author: NC
 */

#ifndef BCOLOR_H_
#define BCOLOR_H_

#include "BObject.h"
#include "BClass.hpp"
#include <map>

class BColor : public BObject {

private:
	lua_Number _red, _green, _blue, _alpha;

	static std::map<std::string, BColor>* _keyColors;

public:

	B_LUA_CLASS

	BColor(); //-- @lua
	BColor(lua_Number red, lua_Number green, lua_Number blue, lua_Number alpha = 255);
	void setRed(lua_Number red);
	void fromString(std::string str);
	void setGreen(lua_Number green);
	void setBlue(lua_Number blue);
	void setAlpha(lua_Number alpha);
	lua_Number red() const;
	lua_Number green() const;
	lua_Number blue() const;
	lua_Number alpha() const;
	virtual std::string toString() const;
	virtual void fromLua(lua_State* state, int begin);
	virtual ~BColor();

	B_DECLARATION(setRed)
	B_DECLARATION(setGreen)
	B_DECLARATION(setBlue)
	B_DECLARATION(setAlpha)
	B_DECLARATION(red)
	B_DECLARATION(green)
	B_DECLARATION(blue)
	B_DECLARATION(alpha)

};

#endif /* BCOLOR_H_ */
