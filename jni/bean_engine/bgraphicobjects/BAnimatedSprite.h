/*
 * BAnimatedSprite.h
 *
 *  Created on: 29 mars 2012
 *      Author: NC
 */

#ifndef BANIMATEDSPRITE_H_
#define BANIMATEDSPRITE_H_

#include <game_engine/BAutoupdateObject.h>
#include "BSprite.h"

class BAnimatedSprite : public BSprite, public BAutoupdateObject {

private:
	bool _autodelete;
	bool _ended;

public:
	BAnimatedSprite();
	void start();
	void stop();
	void pause();
	void setFrameInterval(int frame);
	virtual bool onUpdate();
	virtual void onDelete();
	virtual void setStyle(BStyle& style);
	void setSingleShot(bool value);
	virtual bool release();
	virtual ~BAnimatedSprite();

	B_DECLARATION(start)
	B_DECLARATION(stop)
	B_DECLARATION(pause)
	B_DECLARATION(setFrameInterval)
	B_DECLARATION(frameInterval)
	B_DECLARATION(setSingleShot)

};

#endif /* BANIMATEDSPRITE_H_ */
