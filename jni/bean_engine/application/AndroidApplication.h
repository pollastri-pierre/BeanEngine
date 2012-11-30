#pragma once

#include <android/window.h>
#include <android/native_activity.h>
#include <android/asset_manager.h>
#include <jni.h>

typedef struct android_app Context;

#include <graphic_engine/AndroidGraphicEngine.h>
#include <BApplication.h>
#include <event_manager/BEventManager.h>

#include <sound_engine/AndroidSoundEngine.h>
#include <event_manager/AndroidEventManager.h>

#include <profile_manager/BProfileManager.h>
#include <network_engine/BNetworkWrapper.h>

#define GRAPHICS_OK (0x01)
#define APK_OK		(0x02)
#define OK_GO		(0x04)

typedef AAssetManager AssetManager;

class BGameEngine;

class AndroidApplication : public BApplication {

private:

	static AndroidApplication* _instance;

	Context* _context;
	bool _run;
	bool _applicationEnabled;

	int _readyToLaunch;

	BString _storageDirectory;

	// Modules
	BGameEngine* _gameEngine;
	BGraphicEngine* _graphicEngine;
	BEventManager* _eventManager;
	BSoundEngine* _soundEngine;
	BProfileManager* _profileManager;
	BNetworkWrapper* _networkWrapper;

public:
	AndroidApplication(Context* context);
	void run();
	void stepApplication();
	void onFocus(bool focus);
	void onDestroy();
	void onStart();
	void onPause();
	void onResume();
	void onStop();
	void onDestroyWindow();
	void onCreateWindow();
	void onConfigurationChanged();
	void onLowMemory();
	void onSaveInstanceState();
	bool processEvents();
	void launchGame(int mask);
	void exit();
	AndroidEventManager* eventManager() const;
	Context* context() const;
	AssetManager* getAssetManager() const;
	~AndroidApplication();

private:
	void callJNI();
	void createFilesystem();
};


