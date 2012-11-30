/*
 * BSoundBGM.cpp
 *
 *  Created on: 11 juin 2012
 *      Author: NC
 */

#include "BSoundBGM.h"
#ifdef __SFML__
#include <application/SFMLApplication.h>
#endif


BSoundBGM::BSoundBGM(const BString& name, const BString& path) : BSound(name, path)
{

}

void BSoundBGM::load(const BString& path)
{
	_path = path;
#ifdef __SFML__
    SFMLApplication* app = (SFMLApplication*)BApplication::instance();
    BString p = app->context()->path + path;
    Log::debug("Open [%s]", p.c_str());
    //_music.openFromFile(app->context()->path + path);
#endif
}

#ifdef __SFML__
sf::Music& BSoundBGM::music()
{
    return _music;
}
#endif

void BSoundBGM::unload()
{
	_path = "";
}

BSoundBGM::~BSoundBGM()
{

}
