/*
 * BGraphicEngine.h
 *
 *  Created on: 31 janv. 2012
 *      Author: NC
 */

#ifndef BGRAPHICENGINE_H_
#define BGRAPHICENGINE_H_

#include "BCanvas.h"
#include <bgraphicobjects/BScene.h>

class BGraphicEngine
{
public:
	virtual void setSurfaceCreated(bool surface) = 0;
	virtual void init() = 0;
	virtual void draw(BScene* scene, float* scaler, BRect& rect, bool fulscreen = false) = 0;
	virtual void stop() = 0;
	virtual void adjustToRatio(BScene* scene, BRect* rect, float* scalers, bool fullscreen = false) = 0;
	virtual bool isInitialized() const = 0;
	virtual bool isSurfaceCreated() const = 0;
    virtual long screenWidth() const = 0;
    virtual long screenHeight() const = 0;
	//virtual void display();
};

#endif /* BGRAPHICENGINE_H_ */
