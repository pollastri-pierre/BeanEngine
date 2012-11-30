/*
 * BScene.h
 *
 *  Created on: 31 janv. 2012
 *      Author: NC
 */

#ifndef BSCENE_H_
#define BSCENE_H_

#include "BGraphicObject.h"

class BScene : public BGraphicObject
{
public:

	B_LUA_CLASS

	BScene();
	virtual void onDraw(BCanvas * const canvas, const BCanvasContext& context);
	~BScene();



};

#endif /* BSCENE_H_ */
