/*
 * BObject.h
 *
 *  Created on: 27 janv. 2012
 *      Author: NC
 */

#ifndef BOBJECT_H_
#define BOBJECT_H_

#include <application/Log.h>

extern "C" {

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

}

#include <string>
#include <list>

class BObject
{

private:
	int ___ref;
	static std::list<BObject*> ___instances;
    
public:
	BObject();
	virtual void init(lua_State* state);
	virtual	std::string toString() const;
	virtual void retain();
	virtual bool release();
	virtual void dealloc();
	static bool ___findInstance(BObject*);
	void ___lua_instance();


	static int ___toString(lua_State* state);
	static int ___release(lua_State* state);
	static int ___auto_realease(lua_State* state);
	static void ___gc_collect();
};

#endif /* BOBJECT_H_ */
