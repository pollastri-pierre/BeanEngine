/*
 * BTouchEvents.cpp
 *
 *  Created on: 12 févr. 2012
 *      Author: NC
 */

#include "BTouchEvents.h"

BTouchEvents::BTouchEvents() {
	// TODO Auto-generated constructor stub

}

void BTouchEvents::add(lua_Number x, lua_Number y, lua_Number index, BTouchEvent::Type type)
{
	push_back(new BTouchEvent(x, y, index, type));
}

void BTouchEvents::remove(int index)
{
	if (index < size())
	{
		delete (*this)[index];
		erase(begin() + index);
	}
}

void BTouchEvents::release()
{
	while (size())
	{
		delete back();
		pop_back();
	}
}

BTouchEvents::~BTouchEvents() {
	// TODO Auto-generated destructor stub
}
