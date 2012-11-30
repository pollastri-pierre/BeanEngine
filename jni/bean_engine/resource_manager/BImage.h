/*
 * BImage.h
 *
 *  Created on: 22 mars 2012
 *      Author: NC
 */

#ifndef BIMAGE_H_
#define BIMAGE_H_

#include <application/BString.h>
#include <bobjects/BSize.h>
class BImage {


protected:
	BString _name;
	BString _path;
	BSize	_size;

public:

	static BImage* create(const BString& name, const BString& path);

	BImage(const BString& name, const BString& path);
	virtual void load(const BString& path);
	const BSize& size() const;
	virtual void unload();
	~BImage();

};

#endif /* BIMAGE_H_ */
