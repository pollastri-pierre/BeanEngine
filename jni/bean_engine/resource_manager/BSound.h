/*
 * BSound.h
 *
 *  Created on: 8 juin 2012
 *      Author: NC
 */

#ifndef BSOUND_H_
#define BSOUND_H_

#include <application/BString.h>

class BSound {
private:
	BString _name;
	BString _path;

public:
	BSound(const BString& name, const BString& path);
 	virtual BString path() const;
	virtual BString name() const;
	virtual void load(const BString& path) = 0;
	virtual void unload() = 0;
	virtual ~BSound() {};
private:
    BSound(const BSound&) {};
};

#endif /* BSOUND_H_ */
