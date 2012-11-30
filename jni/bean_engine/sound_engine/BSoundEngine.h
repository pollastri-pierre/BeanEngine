/*
 * BSoundEngine.h
 *
 *  Created on: 8 juin 2012
 *      Author: NC
 */

#ifndef BSOUNDENGINE_H_
#define BSOUNDENGINE_H_

#include <application/BString.h>
#include <sound_engine/BSoundEffect.h>

class BSoundEngine {

	static BSoundEngine* _instance;

public:

	static BSoundEngine* instance();
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void playBGM(const BString& name, const BString& path, int level, short int volume, bool isLooping) = 0;
	virtual void playBGM(int level) = 0;
	virtual void pauseBGM(int level) = 0;
	virtual void stopBGM(int level) = 0;
	virtual void destroyBGM(int level) = 0;
	virtual void destroyBGM(const BString& path) = 0;
	virtual void setBGMVolume(int level, short int volume) = 0;
	virtual void setBGMLoops(int level, bool isLooping) = 0;
	virtual short int getBGMVolume(int level) = 0;
	virtual bool getBGMLoops(int level) = 0;
	virtual bool isBGMPlaying(int level) = 0;
	virtual BString getBGMName(int level) = 0;
	virtual void playSoundEffect(BSoundEffect* effect, int volume) = 0;

};

#endif /* BSOUNDENGINE_H_ */
