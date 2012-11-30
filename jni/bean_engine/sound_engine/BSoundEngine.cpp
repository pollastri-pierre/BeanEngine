/*
 * BSoundEngine.cpp
 *
 *  Created on: 11 juin 2012
 *      Author: NC
 */


#include "BSoundEngine.h"
#if defined (__ANDROID__) && defined (__USE_OPEN_SL_ENGINE__)
#   include "AndroidSoundEngine.h"
#elif defined (__ANDROID__) && defined (__USE_ANDROID_JAVA_SOUND_ENGINE__)
#	include "JAndroidSoundEngine.h"
#elif defined (__SFML__)
#   include "SFMLSoundEngine.h"
#endif

BSoundEngine* BSoundEngine::_instance = NULL;

BSoundEngine* BSoundEngine::instance()
{
	if (!_instance)
#if defined (__ANDROID__) && defined (__USE_OPEN_SL_ENGINE__)
		_instance = new AndroidSoundEngine();
#elif defined (__ANDROID__) && defined (__USE_ANDROID_JAVA_SOUND_ENGINE__)
		_instance = new JAndroidSoundEngine();
#elif defined (__SFML__)
        _instance = new SFMLSoundEngine();
#endif
	return _instance;
}

