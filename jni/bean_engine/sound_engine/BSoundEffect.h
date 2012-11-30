/*
 * BSoundEffect.h
 *
 *  Created on: 13 juin 2012
 *      Author: NC
 */

#ifndef BSOUNDEFFECT_H_
#define BSOUNDEFFECT_H_

#include <resource_manager/BSound.h>

typedef int	SoundID;

class BSoundEffect : public BSound {

	uint8_t*	_buffer;
	off_t		_length;

public:
	BSoundEffect(const BString& name, const BString& path);
	virtual void load(const BString& path);
	virtual void unload();
	uint8_t* buffer() const;
	off_t	length() const;
	virtual ~BSoundEffect();
};


#endif /* BSOUNDEFFECT_H_ */
