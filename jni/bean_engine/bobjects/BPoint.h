/*
 * BPoint.h
 *
 *  Created on: 27 janv. 2012
 *      Author: NC
 */

#ifndef BPOINT_H_
#define BPOINT_H_

#include "BObject.h"
#include "BClass.hpp"

class BPoint : public BObject
{

private:
	lua_Number _x, _y;

public:
	BPoint();
	BPoint(lua_Number x, lua_Number y);
	virtual void init(lua_State* state);
	virtual std::string toString() const;
	virtual void setX(lua_Number x);
	virtual void setY(lua_Number y);
	virtual lua_Number x() const;
	virtual lua_Number y() const;

	static int ___x(lua_State* state);
	static int ___y(lua_State* state);
	static int ___setX(lua_State* state);
	static int ___setY(lua_State* state);
};

#endif /* BPOINT_H_ */
