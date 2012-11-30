/*
 * BSize.h
 *
 *  Created on: 29 janv. 2012
 *      Author: NC
 */

#ifndef BSIZE_H_
#define BSIZE_H_

#include "BObject.h"
#include "BClass.hpp"

class BSize : public BObject {

private:
	lua_Number _width, _height;

public:
	BSize();
	BSize(lua_Number width, lua_Number height);
	virtual void setWidth(lua_Number width);
	virtual void setHeight(lua_Number height);
	virtual lua_Number width() const;
	virtual lua_Number height() const;
	virtual void init(lua_State* state);
	virtual std::string toString() const;
	virtual ~BSize();

	static int ___setWidth(lua_State* state);
	static int ___setHeight(lua_State* state);
	static int ___width(lua_State* state);
	static int ___height(lua_State* state);

};

#endif /* BSIZE_H_ */
