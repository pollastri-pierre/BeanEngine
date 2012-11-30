/*
 * BSprite.cpp
 *
 *  Created on: 29 mars 2012
 *      Author: NC
 */

#include "BSprite.h"

#define CLASS BSprite

B_DEFINE_CLASS("Sprite", BPicture)

B_NO_FUNCTION

B_METHODS_BEGIN

	B_METHOD(setTileIndex)
	B_METHOD(setTileCount)
	B_METHOD(nextTile)
	B_METHOD(previousTile)
	B_METHOD(beginTile)
	B_METHOD(tileCount)
	B_METHOD(endTile)

B_METHODS_END

BSprite::BSprite() {
	// TODO Auto-generated constructor stub
	setTileCount(1);
	setTileIndex(0);
}

void BSprite::setStyle(BStyle& style)
{
	BPicture::setStyle(style);
	if (keyExists(style, "tilecount"))
		setTileCount(stringToNumber(style["tilecount"]));
	if (keyExists(style, "tileindex"))
		setTileIndex(stringToNumber(style["tileindex"]));
}

void BSprite::setTileIndex(int index)
{
	if (_subRect.size().width() == 0.0 || _subRect.size().height() == 0.0)
	{
		_index = 0;
		return ;
	}
	if (index >= 0)
		_index = index % _count;
	else
		_index = _count + index % _count;

	if (!_image)
		return ;
	BPoint p;
	p.setX((int)(_subRect.x() + _index * _subRect.size().width()) % (int)_image->size().width() / (int)_subRect.size().width());
	p.setY(((int)(_subRect.x() + _index * _subRect.size().width()) / (int)_image->size().width()));
	setSubRectPosition(p);

}

int BSprite::tileCount() const
{
	return _count;
}

void BSprite::nextTile()
{
	setTileIndex(_index + 1);
}

void BSprite::previousTile()
{
	setTileIndex(_index - 1);
}

void BSprite::beginTile()
{
	setTileIndex(0);
}

void BSprite::endTile()
{
	setTileCount(_count - 1);
}

void BSprite::setTileCount(int nbr)
{
	_count = nbr;
	if (_count <= 0)
		_count = 1;
}

void BSprite::onDraw(BCanvas * const canvas, const BCanvasContext& context)
{
	setTileIndex(_index);
	BPicture::onDraw(canvas, context);
}

B_IMPLEMENTATION(setTileIndex)

	self->setTileIndex(luaL_checkinteger(state, 2));
	return 0;

B_END

B_IMPLEMENTATION(setTileCount)

	self->setTileCount(luaL_checkinteger(state, 2));
	return 0;

B_END

B_IMPLEMENTATION(nextTile)

	self->nextTile();
	return 0;

B_END

B_IMPLEMENTATION(previousTile)

	self->previousTile();
	return 0;

B_END

B_IMPLEMENTATION(beginTile)

	self->beginTile();
	return 0;

B_END

B_IMPLEMENTATION(endTile)

	self->endTile();
	return 0;

B_END

B_IMPLEMENTATION(tileCount)

	lua_pushnumber(state, self->tileCount());
	return 1;

B_END

BSprite::~BSprite() {
	// TODO Auto-generated destructor stub
}

