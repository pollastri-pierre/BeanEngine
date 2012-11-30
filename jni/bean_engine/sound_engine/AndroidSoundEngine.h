/*
 * AndroidSoundEngine.h
 *
 *  Created on: 8 juin 2012
 *      Author: NC
 */

#ifdef __ANDROID__
#ifndef ANDROIDSOUNDENGINE_H_
#define ANDROIDSOUNDENGINE_H_

#include "BSoundEngine.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>
#include <map>
#include "BSoundEffect.h"

class AndroidSoundEngine : public BSoundEngine {

private:

	struct BGMInfo {
		BString		name;
		BString		path;
		int			fd;
		off_t		start;
		off_t		length;
		SLObjectItf BGMPlayerObj;
		SLPlayItf 	BGMPlayer;
		SLSeekItf 	BGMPlayerSeek;
		SLVolumeItf	BGMPlayerVolume;
		short int	volume;
		bool		loop;
		int			state;
		int			seek;

		BGMInfo() : fd(-1) {}

	};

	typedef std::map<int, BGMInfo> BGMMap;

	SLObjectItf 		_engineObj;
	SLEngineItf 		_engine;
	SLObjectItf 		_outputMix;
	SLDeviceVolumeItf	_deviceVolume;

	SLObjectItf 		_soundEffectObject;
	SLPlayItf 			_soundEffectPlayer;
	SLBufferQueueItf 	_soundEffectPlayerQueue;
	SLVolumeItf			_soundEffectVolume;

	BGMMap _bgms;


public:
	AndroidSoundEngine();

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

	virtual ~AndroidSoundEngine();

private:
	void startBGMService();
	void startSoundEffectService();

};

#endif /* ANDROIDSOUNDENGINE_H_ */
#endif
