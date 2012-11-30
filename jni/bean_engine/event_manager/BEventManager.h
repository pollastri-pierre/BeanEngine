/*
 * BEventManager.h
 *
 *  Created on: 12 févr. 2012
 *      Author: NC
 */

#ifndef BEVENTMANAGER_H_
#define BEVENTMANAGER_H_

#include "BTouchEvents.h"

struct BEventContext
{
	float* 	scalers;
	BRect	rect;
	float scalerX() const { return scalers[0]; };
	float scalerY() const { return scalers[1]; };
};

class BEventManager
{
public:
	virtual BTouchEvents getEvents() = 0;
};

#endif /* BEVENTMANAGER_H_ */
