/*
 * BObject.cpp
 *
 *  Created on: 27 janv. 2012
 *      Author: NC
 */

#include "BObject.h"
#include "BClass.hpp"
#include <game_engine/BGameEngine.h>

template<>
const char* BClass<BObject>::_className = "Object";

template<>
const struct luaL_Reg BClass<BObject>::_functions[] = {
		{NULL, NULL}
};

template<>
const struct luaL_Reg BClass<BObject>::_methods[] = {
		{"release", BObject::___release},
		{"autorelease", BObject::___auto_realease},
		{"__tostring", BObject::___toString},
		{"toString", BObject::___toString},
		{"__gc", BObject::___release},
		{NULL, NULL}
};

template<>
const BMetaClass*BClass<BObject>:: _super = NULL;

std::list<BObject*> BObject::___instances;

BObject::BObject()
{
	___ref = 0;
}

void BObject::dealloc()
{

}

void BObject::___lua_instance()
{
	if (!___findInstance(this))
		___instances.push_back(this);
}

bool BObject::release()
{
	___ref--;
	if (___ref < 0)
	{
		dealloc();
		//Log::debug("Je delete ca [%s]", this->toString().c_str());
		___instances.remove(this);
		delete this;
		return true;
	}
	return false;
}

void BObject::retain()
{
	___ref++;
}

void BObject::___gc_collect()
{
	return ;
	if (___instances.size() == 0)
		return ;
	std::list<BObject*> stack;
	while (___instances.size() > 0)
	{
		BObject* object = *___instances.begin();
		if (!object->release())
		{
			___instances.pop_front();
			stack.push_back(object);
		}
	}
	___instances = stack;
}

std::string BObject::toString() const
{
	return "BObject";
}

void BObject::init(lua_State* state)
{

}

int BObject::___toString(lua_State* state)
{
	 BObject *self = BClass<BObject>::self(state);

	 lua_pushstring(state, self->toString().c_str());
	 return 1;
}

bool BObject::___findInstance(BObject* object)
{
	std::list<BObject*>::iterator it = ___instances.begin();
	std::list<BObject*>::iterator ite = ___instances.end();

	for (; it != ite; ++it)
	{
		if (*it == object)
			return true;
	}
	return false;
}

int BObject::___release(lua_State* state)
{
	if (___findInstance(BClass<BObject>::self(state)))
	{
		//Log::debug("LUA RELEASE [%s]", BClass<BObject>::self(state)->toString().c_str());
		BClass<BObject>::self(state)->release();
	}
	return 0;
}

int BObject::___auto_realease(lua_State* state)
{
	BObject* object = BClass<BObject>::self(state);
	BGameEngine::release(object);
	return 1;
}
