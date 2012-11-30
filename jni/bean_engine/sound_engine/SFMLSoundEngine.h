//
//  SFMLSoundEngine.h
//  BeanEngine
//
//  Created by Pierre POLLASTRI on 20/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef BeanEngine_SFMLSoundEngine_h
#define BeanEngine_SFMLSoundEngine_h

#include <SFML/Audio.hpp>
#include "BSoundEngine.h"
#include <map>

class SFMLSoundEngine : public BSoundEngine
{
private:
    struct pair
    {
        BString name;
        sf::Music* music;
        pair(const BString& _name) : name(_name) {music = NULL;};
        pair(){music = NULL;};
    };
    
    typedef std::map<int, pair > BMusicVector;
    
    BMusicVector    _bgms;
    
public:
  	virtual void start();
	virtual void stop();
    
	// SoundEffect
	virtual void playSoundEffect(BSoundEffect* effect, int volume);
    
	// BGMs
	void startBGM();
	virtual void playBGM(const BString& name, const BString& path, int level, short int volume, bool isLooping);
	virtual void playBGM(int level);
	virtual void pauseBGM(int level);
	virtual void stopBGM(int level);
	virtual void destroyBGM(int level);
	virtual void destroyBGM(const BString& path);
	virtual void setBGMVolume(int level, short int volume);
	virtual void setBGMLoops(int level, bool isLooping);
	virtual short int getBGMVolume(int level);
	virtual bool getBGMLoops(int level);
	virtual BString getBGMName(int level);
	virtual bool isBGMPlaying(int level);
  
};

#endif
