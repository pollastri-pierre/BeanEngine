/*
 * JAndroidSoundEngine.cpp
 *
 *  Created on: 9 sept. 2012
 *      Author: pollas_p
 */

#include "JAndroidSoundEngine.h"

JAndroidSoundEngine::JAndroidSoundEngine()
{

}

JAndroidSoundEngine::~JAndroidSoundEngine()
{
	stop();
}

void JAndroidSoundEngine::start()
{
	getSoundEngine();
}

void JAndroidSoundEngine::stop()
{
	stopSoundEngine();
}

void JAndroidSoundEngine::playBGM(const BString& name, const BString& path, int level, short int volume, bool isLooping)
{
	JNIEnv* jni;
	AndroidApplication* app = (AndroidApplication*)AndroidApplication::instance();
	Context* c = app->context();
	jobject obj = getSoundEngine(&jni);
	jclass clazz = jni->GetObjectClass(obj);
	jmethodID playBGM = jni->GetMethodID(clazz, "playBGM", "(Ljava/lang/String;Ljava/lang/String;ISZZ)V");
	if (playBGM == 0) {
		Log::error("playBGM not found");
		return ;
	}
	jstring N = jni->NewStringUTF(name.c_str());
	jstring P = jni->NewStringUTF(path.c_str());
	jint L = level;
	jshort V = volume;
	jni->CallVoidMethod(obj, playBGM, N, P, L, V, false, isLooping);
	c->activity->vm->DetachCurrentThread();
}

void JAndroidSoundEngine::playSoundEffect(BSoundEffect* effect, int volume)
{

}

void JAndroidSoundEngine::playBGM(int level)
{

}

void JAndroidSoundEngine::pauseBGM(int level)
{

}

void JAndroidSoundEngine::stopBGM(int level)
{

}

void JAndroidSoundEngine::setBGMVolume(int level, short int volume)
{

}

void JAndroidSoundEngine::setBGMLoops(int level, bool isLooping)
{

}

short int JAndroidSoundEngine::getBGMVolume(int level)
{

}

BString JAndroidSoundEngine::getBGMName(int level)
{

}

bool JAndroidSoundEngine::getBGMLoops(int level)
{

}

bool JAndroidSoundEngine::isBGMPlaying(int level)
{

}

void JAndroidSoundEngine::destroyBGM(int level)
{

}

void JAndroidSoundEngine::destroyBGM(const BString& path)
{

}

jobject JAndroidSoundEngine::getSoundEngine(JNIEnv** out)
{
	JNIEnv *jni;
	AndroidApplication* app = (AndroidApplication*)AndroidApplication::instance();
	Context* c = app->context();
	c->activity->vm->AttachCurrentThread(&jni, NULL);
	jclass activityClass = jni->GetObjectClass(c->activity->clazz);
	jmethodID getSoundEngine = jni->GetMethodID(activityClass,"getSoundEngine", "()Lcom/bean_engine/sound_engine/BSoundEngine;");
	jobject obj = jni->CallObjectMethod(c->activity->clazz, getSoundEngine);
	if (out == NULL)
		c->activity->vm->DetachCurrentThread();
	else
		*out = jni;
	return obj;
}

void JAndroidSoundEngine::stopSoundEngine()
{

}
