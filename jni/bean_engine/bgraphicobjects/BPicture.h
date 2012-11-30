/*
 * BPicture.h
 *
 *  Created on: 25 mars 2012
 *      Author: NC
 */

#ifndef BPICTURE_H_
#define BPICTURE_H_

#include "BRectangle.h"
#include <resource_manager/BImage.h>
#include <resource_manager/BResourceManager.h>

class BPicture : public BRectangle {

protected:
	BImage* _image;
	BString _imageFile;
	BRect	_subRect;
	BPoint	_subRectPosition;

public:
	BPicture();
	void setImage(BImage* image);
	void setImage(const char* image);
	BString imageFile() const;
	virtual void onDraw(BCanvas * const canvas, const BCanvasContext& context);
	virtual void setSubRect(BRect rect);
	virtual void setSubRectPosition(BPoint point);
	virtual BRect subRect() const;
	virtual BPoint subRectPosition() const;
	virtual void onResourceDestroyed();
	virtual void setStyle(BStyle& style);
	virtual ~BPicture();


	B_DECLARATION(setSubRect)
	B_DECLARATION(setSubRectPosition)
	B_DECLARATION(subRect)
	B_DECLARATION(setImage)
	B_DECLARATION(subRectPosition)
	B_DECLARATION(image)

};

#endif /* BPICTURE_H_ */
