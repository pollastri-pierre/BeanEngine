/*
 * BRectangle.h
 *
 *  Created on: 3 févr. 2012
 *      Author: NC
 */

#ifndef BRECTANGLE_H_
#define BRECTANGLE_H_

#include "BGraphicObject.h"

class BRectangle : public BGraphicObject {
public:

	BRectangle();
	virtual void onDraw(BCanvas * const canvas, const BCanvasContext& context);

	//B_LUA_CLASS

};

#endif /* BRECTANGLE_H_ */
