/*
 * BSprite.h
 *
 *  Created on: 29 mars 2012
 *      Author: NC
 */

#ifndef BSPRITE_H_
#define BSPRITE_H_

#include "BPicture.h"

class BSprite : public BPicture {

protected:
	int _index;
	int _count;

public:
	BSprite();
	virtual void nextTile();
	virtual void previousTile();
	virtual void beginTile();
	virtual void endTile();
	virtual void setTileIndex(int index);
	virtual void setTileCount(int nbr);
	virtual void setStyle(BStyle& style);
	virtual void onDraw(BCanvas * const canvas, const BCanvasContext& context);
	virtual int tileCount() const;
	virtual ~BSprite();

	B_DECLARATION(setTileIndex)
	B_DECLARATION(setTileCount)
	B_DECLARATION(nextTile)
	B_DECLARATION(previousTile)
	B_DECLARATION(beginTile)
	B_DECLARATION(endTile)
	B_DECLARATION(tileCount)

};

#endif /* BSPRITE_H_ */
