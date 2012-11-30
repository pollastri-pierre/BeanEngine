/*
 * BTouchEvent.h
 *
 *  Created on: 12 févr. 2012
 *      Author: NC
 */

#ifndef BTOUCHEVENT_H_
#define BTOUCHEVENT_H_

#include <bobjects/BRect.h>
#include <vector>

struct BEventContext;

class BTouchEvent {

public:
	enum Type
	{
		UP,
		DOWN,
		MOVE
	};

private:
	lua_Number 	_x;
	lua_Number	_y;
	lua_Number	_index;
	Type		_type;
	bool 		_isMapCoordinate;

public:
	BTouchEvent(lua_Number x, lua_Number y, lua_Number index, Type type);
	void toMapCoordinates(BEventContext& context);
	lua_Number x() const;
	lua_Number y() const;
	lua_Number index() const;
	Type type() const;
	~BTouchEvent();
};

#endif /* BTOUCHEVENT_H_ */
