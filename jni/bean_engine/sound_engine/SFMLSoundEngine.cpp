//
//  SFMLSoundEngine.cpp
//  BeanEngine
//
//  Created by Pierre POLLASTRI on 20/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "SFMLSoundEngine.h"
#include <application/SFMLApplication.h>

void SFMLSoundEngine::start()
{
    
}

void SFMLSoundEngine::stop()
{
    
}

void SFMLSoundEngine::playSoundEffect(BSoundEffect* effect, int volume)
{
    
}

void SFMLSoundEngine::startBGM()
{
    
}

void SFMLSoundEngine::playBGM(const BString& name, const BString& path, int level, short int volume, bool isLooping)
{
    _bgms[level] = pair(name);
    pair& bgm = _bgms[level];
    bgm.music = new sf::Music();
    SFMLApplication* app = (SFMLApplication*)BApplication::instance();
    bgm.music->openFromFile(app->context()->path + path);
    bgm.music->setVolume(volume);
    bgm.music->setLoop(isLooping);
    bgm.music->play();
}

void SFMLSoundEngine::playBGM(int level)
{
    if (_bgms.find(level) != _bgms.end())
    {
        _bgms[level].music->play();
    }
}

void SFMLSoundEngine::pauseBGM(int level)
{
    if (_bgms.find(level) != _bgms.end())
    {
        _bgms[level].music->pause();
    }

}

void SFMLSoundEngine::stopBGM(int level)
{
    if (_bgms.find(level) != _bgms.end())
    {
        _bgms[level].music->stop();
    }

}

void SFMLSoundEngine::destroyBGM(int level)
{
    if (_bgms.find(level) != _bgms.end())
    {
        delete _bgms[level].music; 
    }

}

void SFMLSoundEngine::destroyBGM(const BString& path)
{
    
}

void SFMLSoundEngine::setBGMVolume(int level, short int volume)
{
    if (_bgms.find(level) != _bgms.end())
    {
        _bgms[level].music->setVolume(volume);
    }
}

void SFMLSoundEngine::setBGMLoops(int level, bool isLooping)
{
    if (_bgms.find(level) != _bgms.end())
    {
        _bgms[level].music->setLoop(isLooping);
    }
}

short int SFMLSoundEngine::getBGMVolume(int level)
{
    int vol = 0;
    if (_bgms.find(level) != _bgms.end())
    {
        vol = _bgms[level].music->getVolume(); 
    }
    return vol;
}

bool SFMLSoundEngine::getBGMLoops(int level)
{
    
}

BString SFMLSoundEngine::getBGMName(int level)
{
    
}

bool SFMLSoundEngine::isBGMPlaying(int level)
{
    
}
