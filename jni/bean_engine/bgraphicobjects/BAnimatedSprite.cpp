/*
 * BAnimatedSprite.cpp
 *
 *  Created on: 29 mars 2012
 *      Author: NC
 */

#include "BAnimatedSprite.h"
#include <game_engine/BGameEngine.h>

#define CLASS BAnimatedSprite

B_DEFINE_CLASS("AnimatedSprite", BSprite)
B_NO_FUNCTION

B_METHODS_BEGIN

	B_METHOD(start)
	B_METHOD(stop)
	B_METHOD(pause)
	B_METHOD(setFrameInterval)
	B_METHOD(setSingleShot)
	B_METHOD(frameInterval)

B_METHODS_END

BAnimatedSprite::BAnimatedSprite() {
	// TODO Auto-generated constructor stub
	setFrameToUpdate(1);
	_autodelete = false;
	_ended = false;
}

bool BAnimatedSprite::release()
{
	unregisterObject();
	return BSprite::release();
}

void BAnimatedSprite::setStyle(BStyle& style)
{
	BSprite::setStyle(style);
	if (keyExists(style, "frameinterval"))
		setFrameInterval(stringToNumber(style["frameinterval"]));
}

void BAnimatedSprite::start()
{
	registerObject();
}

void BAnimatedSprite::stop()
{
	unregisterObject();
	beginTile();
	if (_autodelete)
		delete this;
}

void BAnimatedSprite::pause()
{
	unregisterObject();
}

void BAnimatedSprite::setFrameInterval(int frame)
{
	setFrameToUpdate(frame);
}

void BAnimatedSprite::setSingleShot(bool value)
{
	_autodelete = value;
}

void BAnimatedSprite::onDelete()
{
	BGameEngine::release(this);
}

bool BAnimatedSprite::onUpdate()
{
	nextTile();
	if (_index == 0)
		_ended = true;
	if (_ended && _index == 0 && _autodelete)
		return false;
	return true;
}

B_IMPLEMENTATION(start)

	self->start();
	return 0;

B_END

B_IMPLEMENTATION(stop)

	self->stop();
	return 0;

B_END

B_IMPLEMENTATION(pause)

	self->pause();
	return 0;

B_END

B_IMPLEMENTATION(setFrameInterval)

	self->setFrameInterval(luaL_checkint(state, 2));
	return 0;

B_END

B_IMPLEMENTATION(setSingleShot)

	if (lua_isboolean(state, 2))
		self->setSingleShot(lua_toboolean(state, 2));
	return 0;

B_END

B_IMPLEMENTATION(frameInterval)

	lua_pushnumber(state, self->frameToUpdate());
	return 1;

B_END

BAnimatedSprite::~BAnimatedSprite() {
	unregisterObject();

}

