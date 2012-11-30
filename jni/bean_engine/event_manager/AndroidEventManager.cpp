/*
 * AndroidEventManager.cpp
 *
 *  Created on: 12 févr. 2012
 *      Author: NC
 */

#ifdef __ANDROID__
#include "AndroidEventManager.h"
#include <AndroidApplication.h>
#include <android/input.h>

static AndroidEventManager* eventManager = NULL;

AndroidEventManager::AndroidEventManager(BApplication* application) {
	_application = application;
	delete eventManager;
	eventManager = this;
}

BTouchEvents AndroidEventManager::getEvents()
{
	_mutex.lock();
	BTouchEvents events = _events;
	_events.clear();
	_mutex.unlock();
	return events;
}

int32_t AndroidEventManager::processTouchEvent(AInputEvent* event)
{
	for (lua_Number i = 0; i < AMotionEvent_getPointerCount(event); i++)
	{
		lua_Number x, y;
		x = AMotionEvent_getX(event, i);
		y = AMotionEvent_getY(event, i);
		BTouchEvent::Type type;
		int32_t action = AMotionEvent_getAction(event);
		int32_t actionCode = action & AMOTION_EVENT_ACTION_MASK;
		int32_t actionIndex = action >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
		if (actionCode == AMOTION_EVENT_ACTION_UP || actionCode == AMOTION_EVENT_ACTION_OUTSIDE || actionCode == AMOTION_EVENT_ACTION_CANCEL)
			type = BTouchEvent::UP;
		else if (action == AMOTION_EVENT_ACTION_DOWN)
			type = BTouchEvent::DOWN;
		else if (actionCode == AMOTION_EVENT_ACTION_POINTER_DOWN && actionIndex == i)
			type = BTouchEvent::DOWN;
		else if (actionCode == AMOTION_EVENT_ACTION_POINTER_UP && actionIndex == i)
			type = BTouchEvent::UP;
		else
			type = BTouchEvent::MOVE;
		//if (!(type == BTouchEvent::MOVE && x == AMotionEvent_getHistoricalX(event, i, i) && y == AMotionEvent_getHistoricalY(event, i, i)))
		_events.add(x, y, i, type);
	}
	return 1;
}

AndroidEventManager::~AndroidEventManager() {
	// TODO Auto-generated destructor stub
	eventManager = NULL;
}
#endif