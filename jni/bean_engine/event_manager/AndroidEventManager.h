/*
 * AndroidEventManager.h
 *
 *  Created on: 12 févr. 2012
 *      Author: NC
 */

#ifdef __ANDROID__

#ifndef ANDROIDEVENTMANAGER_H_
#define ANDROIDEVENTMANAGER_H_

#include "BEventManager.h"
#include "BTouchEvents.h"
#include <BApplication.h>
#include <boost/thread/recursive_mutex.hpp>
#include <android_native_app_glue.h>


class AndroidEventManager : public BEventManager {

private:
	BTouchEvents _events;
	BApplication* _application;
	boost::recursive_mutex _mutex;

public:
	AndroidEventManager(BApplication* application);
	virtual BTouchEvents getEvents();
	virtual ~AndroidEventManager();
	int32_t processTouchEvent(AInputEvent* event);
};

#endif /* ANDROIDEVENTMANAGER_H_ */
#endif