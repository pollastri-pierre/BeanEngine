/*
 * BSoundBGM.h
 *
 *  Created on: 8 juin 2012
 *      Author: NC
 */

#ifndef BSOUNDBGM_H_
#define BSOUNDBGM_H_

#include <application/BString.h>
#include <resource_manager/BSound.h>
#ifdef __SFML__
#   include <SFML/Audio.hpp>
#endif

class BSoundBGM : public BSound {

	BString _path;
#ifdef __SFML__
    sf::Music _music;
#endif
    
public:
	BSoundBGM(const BString& name, const BString& path);
	virtual void load(const BString& path);
	virtual void unload();
#ifdef __SFML__
    virtual sf::Music& music();
#endif
	virtual ~BSoundBGM();
};

#endif /* BSOUNDBGM_H_ */
