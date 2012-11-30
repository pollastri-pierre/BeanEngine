/*
 * AndroidSoundEngine.cpp
 *
 *  Created on: 8 juin 2012
 *      Author: NC
 */

#ifdef __ANDROID__
#include "AndroidSoundEngine.h"
#include <application/Log.h>
#include <biomodule/BReader.h>
#include <iostream>

#define MINIMUM_VOLUME -2400

// Check an SLES error and return immediately with a STATUS_KO if      an
// error is detected, dumping an error message to the log.
#define slCheckErrorWithStatus(pResult, pMessage, ...) \
   if (pResult != SL_RESULT_SUCCESS) { \
	   Log::error(pMessage, __VA_ARGS__); \
	   Log::error("at line %d in file %s.", __LINE__, __FILE__); \
	   return; \
   }

// Check an SLES error and return immediately if an error is
// detected, dumping an error message to the log.
#define slCheckError(pResult, pMessage, ...) \
   if (pResult != SL_RESULT_SUCCESS) { \
	   Log::error(pMessage, __VA_ARGS__); \
	   Log::error("at line %d in file %s.", __LINE__, __FILE__);\
	   return; \
}

AndroidSoundEngine::AndroidSoundEngine() {
	// TODO Auto-generated constructor stub

}

void AndroidSoundEngine::start()
{
	Log::info("Starting SoundEngine.");
	startBGMService();
	startSoundEffectService();
	Log::info("SoundEngine started.");
}

void AndroidSoundEngine::startBGMService()
{
	Log::info("Starting BGM Service.");

	SLresult result = slCreateEngine(&_engineObj, 0, NULL, 0, NULL, NULL);
	slCheckErrorWithStatus(result, "Problem creating SLESengine (Error %d).", result);
	result = (*_engineObj)->Realize(_engineObj, SL_BOOLEAN_FALSE);
	slCheckErrorWithStatus(result, "Problem realizing SLES engine (Error %d).", result);
	result = (*_engineObj)->GetInterface(_engineObj, SL_IID_ENGINE, &_engine);
	slCheckErrorWithStatus(result, "Problem getting SL_IID_ENGINE for engine (Error %d).", result);

	// Creates audio output.
	const SLuint32 outputMixIIDCount = 0;
	const SLInterfaceID outputMixIIDs[] = {};
	const SLboolean outputMixReqs[] = {};
	result = (*_engine)->CreateOutputMix(_engine, &_outputMix, outputMixIIDCount, outputMixIIDs, outputMixReqs);
	slCheckErrorWithStatus(result, "Problem creating audio output (Error %d).", result);
	result = (*_outputMix)->Realize(_outputMix, SL_BOOLEAN_FALSE);
	slCheckErrorWithStatus(result, "Problem realizing audio output (Error %d).", result);

	slCheckErrorWithStatus(result, "Problem while getting DeviceVolume (Error %d).", result);

	Log::info("BGM Service ........ OK");
}

void AndroidSoundEngine::startSoundEffectService()
{
	Log::info("Starting SoundEffect Service.");
	SLresult result;

	// Set-up sound audio source.
	SLDataLocator_AndroidSimpleBufferQueue dataLocatorIn;
	dataLocatorIn.locatorType = SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE;

	// At most one buffer in the queue.
	dataLocatorIn.numBuffers = 1;

	SLDataFormat_PCM dataFormat;
	dataFormat.formatType = SL_DATAFORMAT_PCM;
	dataFormat.numChannels = 1; // Mono sound.
	dataFormat.samplesPerSec = SL_SAMPLINGRATE_44_1;
	dataFormat.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
	dataFormat.containerSize = SL_PCMSAMPLEFORMAT_FIXED_16;
	dataFormat.channelMask = SL_SPEAKER_FRONT_CENTER;
	dataFormat.endianness = SL_BYTEORDER_LITTLEENDIAN;

	SLDataSource dataSource;
	dataSource.pLocator = &dataLocatorIn;
	dataSource.pFormat = &dataFormat;
	SLDataLocator_OutputMix dataLocatorOut;
	dataLocatorOut.locatorType = SL_DATALOCATOR_OUTPUTMIX;
	dataLocatorOut.outputMix = _outputMix;

	SLDataSink dataSink;
	dataSink.pLocator = &dataLocatorOut;
	dataSink.pFormat = NULL;

	// Creates the sounds player and retrieves its interfaces.
	const SLuint32 soundPlayerIIDCount = 3;
	const SLInterfaceID soundPlayerIIDs[] = { SL_IID_PLAY , SL_IID_ANDROIDSIMPLEBUFFERQUEUE, SL_IID_VOLUME };
	const SLboolean soundPlayerReqs[] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };

	result = (*_engine)->CreateAudioPlayer(_engine, &_soundEffectObject, &dataSource, &dataSink, soundPlayerIIDCount, soundPlayerIIDs, soundPlayerReqs);
	slCheckErrorWithStatus(result, "Problem creating sound player (Error %d).", result);
	result = (*_soundEffectObject)->Realize(_soundEffectObject, SL_BOOLEAN_FALSE);
	slCheckErrorWithStatus(result, "Problem realizing sound player (Error %d).", result);
	result = (*_soundEffectObject)->GetInterface(_soundEffectObject, SL_IID_PLAY, &_soundEffectPlayer);
	slCheckErrorWithStatus(result, "Problem getting SL_IID_PLAY for sound player (Error %d).", result);
	result = (*_soundEffectObject)->GetInterface(_soundEffectObject, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, &_soundEffectPlayerQueue);
	slCheckErrorWithStatus(result, "Problem getting SL_IID_ANDROIDSIMPLEBUFFERQUEUE for sound player (Error %d).", result);

	 result = (*_soundEffectObject)->GetInterface(_soundEffectObject, SL_IID_VOLUME, &_soundEffectVolume);
	 slCheckErrorWithStatus(result, "Problem getting SL_IID_VOLUME for sound player player (Error %d).", result);
	 result = (*_soundEffectVolume)->SetVolumeLevel(_soundEffectVolume, 0);

	// Starts the sound player. Nothing can be heard while the // sound queue remains empty.
	result = (*_soundEffectPlayer)->SetPlayState(_soundEffectPlayer, SL_PLAYSTATE_PLAYING);
	slCheckErrorWithStatus(result, "Problem starting player (Error %d).", result);
	Log::info("SoundEffect Service ........ OK");
}

void AndroidSoundEngine::stop()
{
	Log::info("Stopping SoundService.");

	if (_outputMix != NULL)
	{
	   (*_outputMix)->Destroy(_outputMix);
	   _outputMix = NULL;
	   //_outputMixReverb = NULL;
	}
	if (_engineObj != NULL)
	{
	   (*_engineObj)->Destroy(_engineObj);
	   _engineObj = NULL;
	   _engine = NULL;
	}
}

void AndroidSoundEngine::playBGM(const BString& name, const BString& path, int level, short int volume, bool isLooping)
{

	if (_bgms.find(level) != _bgms.end())
	{
		if (_bgms[level].path == path && _bgms[level].BGMPlayer != NULL)
		{
			playBGM(level);
			return ;
		}
		destroyBGM(level);
	}

	SLresult result;

	BReader* reader = BReader::create();

	if (!reader->open(path))
	{
		Log::error("Error cannot open audio file <%s>", path.c_str());
		return ;
	}

	BGMInfo info;

	info.name = name;
	info.path = path;
	info.fd = reader->fd();
	info.start = reader->start();
	info.length = reader->length();
	info.volume = volume;
	info.loop = isLooping;
	info.seek = 0;

	reader->close();

	SLDataLocator_AndroidFD _dataLocatorIn;
	_dataLocatorIn.locatorType = SL_DATALOCATOR_ANDROIDFD;
	_dataLocatorIn.fd = info.fd;
	_dataLocatorIn.offset = info.start;
	_dataLocatorIn.length = info.length;
	SLDataFormat_MIME _dataFormat;
	_dataFormat.formatType = SL_DATAFORMAT_MIME;
	_dataFormat.mimeType = NULL;
	_dataFormat.containerType = SL_CONTAINERTYPE_UNSPECIFIED;

	SLDataSource _dataSource;
   _dataSource.pLocator = &_dataLocatorIn;
   _dataSource.pFormat = &_dataFormat;
   SLDataLocator_OutputMix _dataLocatorOut;
   _dataLocatorOut.locatorType = SL_DATALOCATOR_OUTPUTMIX;
   _dataLocatorOut.outputMix = _outputMix;
   SLDataSink _dataSink;
   _dataSink.pLocator = &_dataLocatorOut;
   _dataSink.pFormat = NULL;
   Log::debug("A1");
   // Creates BGM player and retrieves its interfaces.
	const SLuint32 _BGMPlayerIIDCount = 3;
	const SLInterfaceID _BGMPlayerIIDs[] = { SL_IID_PLAY, SL_IID_SEEK, SL_IID_VOLUME };
	const SLboolean _BGMPlayerReqs[] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };
	result = (*_engine)->CreateAudioPlayer(_engine, &info.BGMPlayerObj, &_dataSource, &_dataSink,
	 _BGMPlayerIIDCount, _BGMPlayerIIDs, _BGMPlayerReqs);
	 Log::debug("A2");
	slCheckErrorWithStatus(result, "Problem creating BGM player (Error %d).", result);
	result = (*info.BGMPlayerObj)->Realize(info.BGMPlayerObj, SL_BOOLEAN_FALSE);
	slCheckErrorWithStatus(result, "Problem realizing BGM player (Error %d).", result);

	result = (*info.BGMPlayerObj)->GetInterface(info.BGMPlayerObj, SL_IID_PLAY, &info.BGMPlayer);
	slCheckErrorWithStatus(result, "Problem getting SL_IID_PLAY for BGM player (Error %d).", result);

	result = (*info.BGMPlayerObj)->GetInterface(info.BGMPlayerObj, SL_IID_SEEK, &info.BGMPlayerSeek);
	slCheckErrorWithStatus(result, "Problem getting SL_IID_SEEK for BGM player (Error %d).", result);
	// Enables looping and starts playing.
	if (isLooping)
	{
		result = (*info.BGMPlayerSeek)->SetLoop(info.BGMPlayerSeek, SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);
		slCheckErrorWithStatus(result, "Problem setting BGM player loop (Error %d).", result);
	}
    result = (*info.BGMPlayer)->SetPlayState(info.BGMPlayer, SL_PLAYSTATE_PLAYING);
    info.state = SL_PLAYSTATE_PLAYING;
    slCheckErrorWithStatus(result, "Problem starting BGMplayer (Error %d).", result);

    result = (*info.BGMPlayerObj)->GetInterface(info.BGMPlayerObj, SL_IID_VOLUME, &info.BGMPlayerVolume);
    slCheckErrorWithStatus(result, "Problem getting SL_IID_VOLUME for BGM player (Error %d).", result);
    result = (*info.BGMPlayerVolume)->SetVolumeLevel(info.BGMPlayerVolume, (float)volume / 100.f * -MINIMUM_VOLUME + MINIMUM_VOLUME);
    slCheckErrorWithStatus(result, "Problem setting VOLUME for BGM player (Error %d).", result);
    _bgms[level] = info;
}

void AndroidSoundEngine::playSoundEffect(BSoundEffect* effect, int volume)
{
	if (!effect)
		return;
	Log::debug("TEST SOUND EFFECT");
	SLresult lResult;
// Removes any sound from the queue.
	lResult = (*_soundEffectPlayerQueue)->Clear(_soundEffectPlayerQueue);
	slCheckError(lResult, "Problem clearing sound queue (Error %d).", lResult);
// Plays the new sound.
	int16_t* buffer = (int16_t*)effect->buffer();
	off_t length = effect->length();
	lResult = (*_soundEffectVolume)->SetVolumeLevel(_soundEffectVolume, (float)volume / 100.f * -MINIMUM_VOLUME + MINIMUM_VOLUME);
	lResult = (*_soundEffectPlayerQueue)->Enqueue(_soundEffectPlayerQueue, (char*)buffer + 44, length - 44);
}

void AndroidSoundEngine::playBGM(int level)
{
	if (_bgms.find(level) != _bgms.end())
	{
		BGMInfo& info = _bgms[level];
		SLresult result;
		if (info.BGMPlayer != NULL)
		{
			result = (*info.BGMPlayer)->SetPlayState(info.BGMPlayer, SL_PLAYSTATE_PLAYING);
			info.state = SL_PLAYSTATE_PLAYING;
			slCheckErrorWithStatus(result, "Problem starting BGMplayer (Error %d).", result);
		}
		else
			playBGM(info.name, info.path, level, info.volume, info.loop);
	}
}

void AndroidSoundEngine::pauseBGM(int level)
{
	if (_bgms.find(level) != _bgms.end())
		{
			BGMInfo& info = _bgms[level];
			SLresult result;
			if (info.BGMPlayer == NULL)
			{
				playBGM(info.name, info.path, level, info.volume, info.loop);
			}
			result = (*info.BGMPlayer)->SetPlayState(info.BGMPlayer, SL_PLAYSTATE_PAUSED);
			info.state = SL_PLAYSTATE_PAUSED;
			slCheckErrorWithStatus(result, "Problem pausing BGMplayer (Error %d).", result);
		}
}

void AndroidSoundEngine::stopBGM(int level)
{
	if (_bgms.find(level) != _bgms.end())
	{
		BGMInfo& info = _bgms[level];
		SLresult result;
		if (info.BGMPlayer == NULL)
		{
			playBGM(info.name, info.path, level, info.volume, info.loop);
		}
		result = (*info.BGMPlayer)->SetPlayState(info.BGMPlayer, SL_PLAYSTATE_STOPPED);
		info.state = SL_PLAYSTATE_STOPPED;
		slCheckErrorWithStatus(result, "Problem stopping BGMplayer (Error %d).", result);
	}
}

void AndroidSoundEngine::setBGMVolume(int level, short int volume)
{
	if (_bgms.find(level) != _bgms.end())
	{
		BGMInfo& info = _bgms[level];
		SLresult result;
		info.volume = volume;
		if (info.BGMPlayer == NULL)
		{
			playBGM(info.name, info.path, level, info.volume, info.loop);
		}
		 result = (*info.BGMPlayerVolume)->SetVolumeLevel(info.BGMPlayerVolume, (float)volume / 100.f * -MINIMUM_VOLUME + MINIMUM_VOLUME);
		    slCheckErrorWithStatus(result, "Problem setting VOLUME for BGM player (Error %d).", result);

	}
}

void AndroidSoundEngine::setBGMLoops(int level, bool isLooping)
{
	if (_bgms.find(level) != _bgms.end())
	{
		BGMInfo& info = _bgms[level];
		SLresult result;
		info.loop = isLooping;
		if (info.BGMPlayer == NULL)
		{
			playBGM(info.name, info.path, level, info.volume, info.loop);
		}
		if (isLooping)
			result = (*info.BGMPlayerSeek)->SetLoop(info.BGMPlayerSeek, SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);
		else
			result = (*info.BGMPlayerSeek)->SetLoop(info.BGMPlayerSeek, SL_BOOLEAN_FALSE, 0, SL_TIME_UNKNOWN);
		slCheckErrorWithStatus(result, "Problem setting loops BGMplayer (Error %d).", result);
	}
}

short int AndroidSoundEngine::getBGMVolume(int level)
{
	if (_bgms.find(level) != _bgms.end())
	{
		BGMInfo& info = _bgms[level];
		return info.volume;
	}
	return -1;
}

BString AndroidSoundEngine::getBGMName(int level)
{
	if (_bgms.find(level) != _bgms.end())
	{
		BGMInfo& info = _bgms[level];
		return info.name;
	}
	return "";
}

bool AndroidSoundEngine::getBGMLoops(int level)
{
	if (_bgms.find(level) != _bgms.end())
	{
		BGMInfo& info = _bgms[level];
		return info.loop;
	}
	return false;
}

bool AndroidSoundEngine::isBGMPlaying(int level)
{
	if (_bgms.find(level) != _bgms.end())
	{
		BGMInfo& info = _bgms[level];
		return info.state = SL_PLAYSTATE_PLAYING;
	}
	return false;
}

void AndroidSoundEngine::destroyBGM(int level)
{
	if (_bgms.find(level) != _bgms.end() && _bgms[level].BGMPlayer != NULL)
	{
		BGMInfo& info = _bgms[level];
		SLresult result = (*info.BGMPlayer)->SetPlayState(info.BGMPlayer, SL_PLAYSTATE_PAUSED);
		info.state = SL_PLAYSTATE_STOPPED;
		slCheckError(result, "Problem pausing BGM player(Error %d).", result);
		(*info.BGMPlayerObj)->Destroy(info.BGMPlayerObj);
		info.BGMPlayerObj = NULL;
		info.BGMPlayer = NULL;
		info.BGMPlayerSeek = NULL;
		info.BGMPlayerVolume = NULL;
	}
}

void AndroidSoundEngine::destroyBGM(const BString& path)
{
	BGMMap::iterator it = _bgms.begin();
	BGMMap::iterator ite = _bgms.end();

	for (;it != ite; ++it)
	{
		if (it->second.path == path)
		{
			destroyBGM(it->first);
			_bgms.erase(it);
			return ;
		}
	}
}

AndroidSoundEngine::~AndroidSoundEngine() {
	// TODO Auto-generated destructor stub
}

#endif
