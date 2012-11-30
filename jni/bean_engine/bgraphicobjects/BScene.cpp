/*
 * BScene.cpp
 *
 *  Created on: 31 janv. 2012
 *      Author: NC
 */

#include "BScene.h"
#include <game_engine/BGameEngine.h>

#define CLASS BScene

B_DEFINE_CLASS("Scene", BGraphicObject)

B_NO_FUNCTION

B_METHODS_BEGIN

B_METHODS_END

BScene::BScene() {
	_userInteractionEnabled = true;
}

void BScene::onDraw(BCanvas * const canvas, const BCanvasContext& context)
{
	float tX = context.rect.x() + _rotationCenter.x() * canvas->scalerX();
	float tY = context.rect.y() + _rotationCenter.y() * canvas->scalerY();
	BColor c = backgroundColor();
	c.setAlpha(c.alpha() * context.opacity);
	canvas->setColor(c);
	canvas->translate(tX, tY);
	canvas->rotate(_rotation);
	canvas->translate(-tX, -tY);
	//canvas->drawChar('c', _FONT);
	canvas->setRect(context.rect);
	canvas->fillRect();
}

B_CONSTRUCTOR
(void)self;
	int args = lua_gettop(state);

	switch (args)
	{
	case 0:
	{
		setSize(BGameEngine::instance()->defaultSceneSize());
	}
	break;
	case 1:
	{
		BSize* size = BClass<BSize>::self(state, 1);
		setSize(*size);
	}
	break;
	case 2:
	{
		lua_Number width = luaL_checknumber(state, 1);
		lua_Number height = luaL_checknumber(state, 2);
		setSize(BSize(width, height));
	}
	break;
	}

B_END

BScene::~BScene() {
	// TODO Auto-generated destructor stub
}
