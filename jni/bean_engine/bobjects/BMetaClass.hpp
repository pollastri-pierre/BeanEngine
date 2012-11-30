/*
 * BMetaClass.hpp
 *
 *  Created on: 27 janv. 2012
 *      Author: NC
 */

#ifndef BMETACLASS_HPP_
#define BMETACLASS_HPP_

class BMetaClass
{
public:
	BMetaClass() {};
	virtual const struct luaL_Reg* getFunctionTable() const = 0;
	virtual const struct luaL_Reg* getMethodTable() const = 0;
	virtual const char* getClassName() const = 0;
	virtual const BMetaClass* super() const = 0;
};

#endif /* BMETACLASS_HPP_ */
