/*
 * JAndroidSoundEngine.h
 *
 *  Created on: 9 sept. 2012
 *      Author: pollas_p
 */

#ifndef JANDROIDSOUNDENGINE_H_
#define JANDROIDSOUNDENGINE_H_

#include "BSoundEngine.h"
#include <application/AndroidApplication.h>

#include <jni.h>

class JAndroidSoundEngine: public BSoundEngine
{
public:
	JAndroidSoundEngine();
	virtual void start();
	virtual void stop();
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
	virtual bool isBGMPlaying(int level);
	virtual BString getBGMName(int level);
	virtual void playSoundEffect(BSoundEffect* effect, int volume);
	virtual ~JAndroidSoundEngine();

private:
	jobject getSoundEngine(JNIEnv** out = NULL);
	void stopSoundEngine();
};

#endif /* JANDROIDSOUNDENGINE_H_ */
