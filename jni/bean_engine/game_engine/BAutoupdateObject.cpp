/*
 * BAutoupdateObject.cpp
 *
 *  Created on: 29 mars 2012
 *      Author: NC
 */

#include "BAutoupdateObject.h"
#include <application/Log.h>

BAutoupdateObject::ObjectList BAutoupdateObject::_objects;

BAutoupdateObject::BAutoupdateObject() {
	// TODO Auto-generated constructor stub
	_curFrame = 0;
	_frameToUpdate = 0;
	_unregister = false;
}

bool BAutoupdateObject::update()
{
	_curFrame++;
	if (_curFrame >= _frameToUpdate)
	{
		if (!_unregister)
			_curFrame = 0;
		return onUpdate();
	}
	return true;
}

int BAutoupdateObject::frameToUpdate() const
{
	return _frameToUpdate;
}

void BAutoupdateObject::registerObject()
{
	_objects.remove(this);
	_objects.push_front(this);
}

void BAutoupdateObject::unregisterObject()
{
	_objects.remove(this);
}

void BAutoupdateObject::setFrameToUpdate(int f)
{
	_frameToUpdate = f;
}

void BAutoupdateObject::unregisterAtTheEnd(bool value)
{
	_unregister = value;
}

void BAutoupdateObject::updateObjects()
{
	ObjectList::iterator it = _objects.begin();
	ObjectList::iterator ite = _objects.end();

	while (it != ite)
	{

		if (!(*it)->update() || (*it)->_unregister)
		{
			ObjectList::iterator It = it;
			++it;
			BAutoupdateObject* obj = *It;
			_objects.erase(It);
			obj->onDelete();
		}
		else
			++it;
	}

}

BAutoupdateObject::~BAutoupdateObject() {
	unregisterObject();
}

