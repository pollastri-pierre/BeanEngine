#include "AndroidApplication.h"
#include "Log.h"
#include <android_native_app_glue.h>

// Module inclusion
#include <game_engine/BGameEngine.h>
#include <event_manager/AndroidEventManager.h>
#include <sound_engine/BSoundEngine.h>
#include <jni.h>

#include <iostream>
#include <dirent.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

AndroidApplication* AndroidApplication::_instance = NULL;

static void application_callback(Context* context, int32_t command);
static int32_t	input_eventcallback(Context* context, AInputEvent* event);

AndroidApplication::AndroidApplication(Context* context)
{
	_context = context;
	_applicationEnabled = false;

	callJNI();
	createFilesystem();
	_dirMap[B_PROFILES_DIR].setPath(_storageDirectory);
	_dirMap["scripts"].setPath(_storageDirectory);
	_profileManager = BProfileManager::createProfileManager(_dirMap[B_PROFILES_DIR].path());
	context->onAppCmd = application_callback;
	context->onInputEvent = input_eventcallback;
	context->userData = this;

	_readyToLaunch = 0;
	_soundEngine = BSoundEngine::instance();
	_eventManager = new AndroidEventManager(this);
	_gameEngine = BGameEngine::createInstance(this);
	_graphicEngine = new AndroidGraphicEngine(this);
	_networkWrapper = BNetworkWrapper::instance();

	_gameEngine->setEventManager(_eventManager);
	_gameEngine->setProfileManager(_profileManager);
	_gameEngine->setNetworkWrapper(_networkWrapper);

	delete _instance;
	_instance = this;

}

void AndroidApplication::createFilesystem()
{
    DIR *dp;
    struct dirent* entry;
    BString key;
    std::basic_string<char> s;
    s.resize(12, 'a');
    if((dp  = opendir(_storageDirectory.c_str())) == NULL)
        return ;

   // Log::info("Storage Directory = %s", _storageDirectory.c_str());
    while ((entry = readdir(dp)) != NULL)
    {
    	if (entry->d_type == DT_DIR && findEntryInDirMap(entry->d_name, &key))
    	{
    		_dirMap[key].setPath(_storageDirectory);
    	}
    }
    closedir(dp);

    DirMap::iterator it = _dirMap.begin();
    DirMap::iterator ite = _dirMap.end();

    for (; it != ite; ++it)
    {
    	if (!it->second.isValid())
    	{
    		BString path = _storageDirectory + "/" + it->second.dirName();
    		if (mkdir(path.c_str(), 0777))
    			it->second.setPath(_storageDirectory);
    	}
    }

}

void AndroidApplication::run()
{
	int ident;
	int events;
	struct android_poll_source* source;

	app_dummy();
	_gameEngine->setCurrentScript("scripts/interfaces/main.lua");
	if (!isIndependent())
		(*_gameEngine)();
	else
	{
		_run = true;
		while (_run)
		{

			while ((ident = ALooper_pollAll(_applicationEnabled ? 0 : -1, NULL, &events, (void**)&source)) >= 0)
			{


				if (source != NULL)
					source->process(_context, source);

				if (_context->destroyRequested != 0)
				{
					_run = false;
					break;
				}
			}

			//if (_context->window != NULL && _applicationEnabled)
			//	stepApplication();

		}
		Log::debug("Exit");
		_gameEngine->stop();
	}
	ANativeActivity_finish(_context->activity);
	::exit(0);
}

bool AndroidApplication::processEvents()
{
	if (_independent)
		return true;
	int ident;
	int events;
	struct android_poll_source* source;

	{
		(ident = ALooper_pollAll(_applicationEnabled ? 0 : -1, NULL, &events, (void**)&source));
		if (source != NULL)
			source->process(_context, source);
		Log::debug("Proccess");
	}
	while (!_applicationEnabled)

	return _context->destroyRequested != 0;
}

void AndroidApplication::onDestroy()
{
	_applicationEnabled = false;
	Log::debug("onDestroy");
}

void AndroidApplication::onFocus(bool focus)
{
	_applicationEnabled = focus;
	if (_independent && _gameEngine->status() == BGameEngine::PAUSE && _applicationEnabled)
			_gameEngine->resume();
}

void AndroidApplication::stepApplication()
{

}

void AndroidApplication::onConfigurationChanged()
{

}

void AndroidApplication::onCreateWindow()
{
	Log::debug("On create window");
	//if (_gameEngine)
	//	_gameEngine->initGraphicEngine();
	_graphicEngine->setSurfaceCreated(true);
	launchGame(GRAPHICS_OK);
}

void AndroidApplication::onDestroyWindow()
{
	Log::debug("onDestroyWindow");
	_graphicEngine->setSurfaceCreated(false);
	_graphicEngine->stop();
}

void AndroidApplication::onLowMemory()
{

}

void AndroidApplication::onPause()
{
	_readyToLaunch = 0;
	_applicationEnabled = false;
	Log::debug("onPause");
	if (_independent && _gameEngine->status() == BGameEngine::START)
		_gameEngine->pause();
}

void AndroidApplication::onResume()
{
	Log::debug("onResume");
	launchGame(APK_OK);
}

void AndroidApplication::launchGame(int mask)
{
	_readyToLaunch |= mask;
	if ((_readyToLaunch & APK_OK) && (_readyToLaunch & GRAPHICS_OK))
	{
		if (_independent && _gameEngine->status() == BGameEngine::READY)
		{
				_gameEngine->setGraphicEngine(_graphicEngine);
				_gameEngine->startWith("scripts/interfaces/main.lua");
		}
		else if (_independent && _gameEngine->status() == BGameEngine::PAUSE && _applicationEnabled)
			_gameEngine->resume();
	}
}

void AndroidApplication::onStart()
{
	_soundEngine->start();
	//_gameEngine->setGraphicEngine(_graphicEngine);
	Log::debug("onStart");
	launchGame(APK_OK);
}

Context* AndroidApplication::context() const
{
	return _context;
}

void AndroidApplication::onStop()
{
	Log::debug("onStop");
	_graphicEngine->stop();
}

void AndroidApplication::onSaveInstanceState()
{

}

void AndroidApplication::exit()
{
	_run = false;
}

AssetManager* AndroidApplication::getAssetManager() const
{
	return _context->activity->assetManager;
}

AndroidApplication::~AndroidApplication()
{
	_instance = NULL;
}

AndroidEventManager* AndroidApplication::eventManager() const
{
	return (AndroidEventManager*)_eventManager;
}

void AndroidApplication::callJNI()
{
	B_ASSERT(_context->activity->vm!=NULL);
	JavaVM* java = _context->activity->vm;
	JNIEnv *jni = NULL;

	java->AttachCurrentThread(&jni, NULL);


	B_ASSERT(jni!=NULL);
	jclass activityClass = jni->GetObjectClass(_context->activity->clazz);
	B_ASSERT(activityClass!=NULL);

	jmethodID methodID = (jni)->GetMethodID(activityClass, "getStorageDirectory", "()Ljava/lang/String;");
	B_ASSERT(methodID!=NULL);
	jstring string = reinterpret_cast<jstring>(jni->CallObjectMethod(_context->activity->clazz, methodID));

	jboolean isCopy = false;
	const char* dir = (jni)->GetStringUTFChars(string, &isCopy);
	_storageDirectory = dir;
	(jni)->ReleaseStringUTFChars(string, dir);
	Log::debug("DIRECTORY = %s", _storageDirectory.c_str());

	_context->activity->vm->DetachCurrentThread();
}

static void application_callback(Context* context, int32_t command)
{

	AndroidApplication& activity = *(AndroidApplication*) context->userData;
	context->activityState = command;
	switch (command) {
	case APP_CMD_CONFIG_CHANGED:
		activity.onConfigurationChanged();
		break;
	case APP_CMD_INIT_WINDOW:
		if (context->window != NULL)
			activity.onCreateWindow();
		break;
	case APP_CMD_DESTROY:
		activity.onDestroy();
		break;
	case APP_CMD_GAINED_FOCUS:
		activity.onFocus(true);
		break;
	case APP_CMD_LOST_FOCUS:
		activity.onFocus(false);
		break;
	case APP_CMD_LOW_MEMORY:
	   activity.onLowMemory();
	   break;
   case APP_CMD_PAUSE:
	   context->activityState = APP_CMD_PAUSE;
	   activity.onPause();
	   break;
   case APP_CMD_RESUME:
	   activity.onResume();
	   break;
   case APP_CMD_SAVE_STATE:
	   activity.onSaveInstanceState();
	   break;
   case APP_CMD_START:
	   activity.onStart();
	   break;
   case APP_CMD_STOP:
	   activity.onStop();
	   break;
   case APP_CMD_TERM_WINDOW:
	   activity.onDestroyWindow();
	   break;
	}
}

static int32_t input_eventcallback(struct android_app* app, AInputEvent* event)
{
	int32_t type = AInputEvent_getType(event);
	AndroidApplication* a = (AndroidApplication*)app->userData;
	switch (type)
	{
	case AINPUT_EVENT_TYPE_MOTION:
		switch (AInputEvent_getSource(event))
		{
			case AINPUT_SOURCE_TOUCHSCREEN:
			return a->eventManager()->processTouchEvent(event);
			break;
		}
		break;
	case AINPUT_EVENT_TYPE_KEY:
		Log::debug("EVENT INPUT OK");
		switch(AKeyEvent_getKeyCode(event))
		{
		case AKEYCODE_BACK:

			return 1;
			break;
		}
		return 0;


		break;
	}
}
