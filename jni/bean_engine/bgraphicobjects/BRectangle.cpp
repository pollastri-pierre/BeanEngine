/*
 * BRectangle.cpp
 *
 *  Created on: 3 fŽvr. 2012
 *      Author: NC
 */

#include "BRectangle.h"

#define CLASS BRectangle

B_DEFINE_CLASS("Rectangle", BGraphicObject)

B_NO_FUNCTION

B_METHODS_BEGIN

B_METHODS_END

BRectangle::BRectangle()
{

}

void BRectangle::onDraw(BCanvas * const canvas, const BCanvasContext& context)
{
	float tX = context.rect.x() + _rotationCenter.x() * canvas->scalerX();
	float tY = context.rect.y() + _rotationCenter.y() * canvas->scalerY();
	BColor c = backgroundColor();
	c.setAlpha(c.alpha() * context.opacity);
	canvas->setColor(c);
	canvas->translate(tX, tY);
	canvas->rotate(_rotation);
	canvas->translate(-tX, -tY);
	canvas->setRect(context.rect);
	canvas->fillRect();
}


