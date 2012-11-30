/*
 * BTouchEvents.h
 *
 *  Created on: 12 févr. 2012
 *      Author: NC
 */

#ifndef BTOUCHEVENTS_H_
#define BTOUCHEVENTS_H_

#include <vector>
#include "BTouchEvent.h"

class BTouchEvents : public std::vector<BTouchEvent*> {
public:
	BTouchEvents();
	void add(lua_Number x, lua_Number y, lua_Number index, BTouchEvent::Type type);
	void remove(int index);
	void release();
	virtual ~BTouchEvents();
};

#endif /* BTOUCHEVENTS_H_ */
