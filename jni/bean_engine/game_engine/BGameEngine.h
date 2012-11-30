#pragma once

#include <application/BApplication.h>
#include <string>
#include <boost/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <bobjects/BClass.hpp>
#include <list>
#include <unistd.h>
#include <sys/time.h>
#include <boost/variant.hpp>
#include <application/BString.h>

#include <bgraphicobjects/BScene.h>
#include <graphic_engine/BGraphicEngine.h>
#include <event_manager/BEventManager.h>
#include <profile_manager/BProfileManager.h>
#include <network_engine/BNetworkWrapper.h>

typedef std::map<BString, boost::variant<lua_Integer, lua_Number, BString, bool> > BLuaArgs;

class BGameEngine {

public:
	enum Status
	{
		STOP,
		START,
		PAUSE,
		READY
	};


private:
	BApplication* _application;
	Status _status;

	// Moteurs
	int 				_initWindow;
	BGraphicEngine* 	_graphicEngine;
	BEventManager* 		_eventManager;
	BNetworkWrapper*	_networkWrapper;

	// Gestion du threading
	boost::thread* 				_thread;
	boost::recursive_mutex* 	_mutex;

	// Gestion de LUA
	BString _currentScriptPath;
	BString _nextScriptPath;
	lua_State*	_luaState;

	// Gestion du GC
	std::list<BObject*> ___gc;

	// Donnees de jeux
	bool _running;
	bool _fullscreen;
	BScene* _currentScene;
	BSize _defaultSize;
	static BLuaArgs _globalArguments;
	static BLuaArgs _nextGlobalArguments;
	BTouchEvents _events;

	// Données d'ecran/map
	float _screenScalers[2];
	BRect _screenBoundingRect;

	// Gestion du framerate;
	int _framerate;
	useconds_t _delta;
	struct timeval _beforeTime;
	struct timeval _afterTime;
	struct timeval _nextTime;
	struct timeval _beginRecord;
	int _timeToWait;
	int _actualFramerate;
	int _currentFrame;

	// Profile manager
	BProfileManager* _profileManager;

	static BGameEngine* _instance;


public:

	void start();
	void pause();
	void stop();
	void resume();
	void startWith(std::string path);
	void setCurrentScript(std::string path);
	void operator()(void);
	Status status() const;
	void initGraphicEngine();
	bool isRunning();
	bool isFullScreen() const;
    int framerate() const;
    lua_State* L() const;
	static BGameEngine* instance();
	static void release(BObject* object);
	static void collect();
	BSize defaultSceneSize() const;
	~BGameEngine();

	void setFramerate(int fps);
	void setGraphicEngine(BGraphicEngine* engine);
	void setEventManager(BEventManager* eventManager);
	void setNetworkWrapper(BNetworkWrapper* networkWrapper);
	void setProfileManager(BProfileManager* profileManager);
	BProfileManager* profileManager() const;

	void begin();
	int update(lua_State* state);

	static int isRunning(lua_State* state);

	static int display(lua_State* state);
	static int loopBegin(lua_State* state);

	static int setFramerate(lua_State* state);
	static int getCurrentFramerate(lua_State* state);
	static int setCurrentScene(lua_State* state);
	static int nextFile(lua_State* state);
	static int pushNumber(lua_State* state);
	static int pushString(lua_State* state);
	static int pushBoolean(lua_State* state);
	static int popNumber(lua_State* state);
	static int popBoolean(lua_State* state);
	static int popString(lua_State* state);
	static int setFullscreen(lua_State* state);
	static int getTouchCount(lua_State* state);
	static int popTouch(lua_State* state);
	static int loadConf(lua_State* state);
	static int loadResources(lua_State* state);
	static int getProfileNames(lua_State* state);
	static int getProfileCount(lua_State* state);
	static int framerate(lua_State* state);
    static int getResourcesCount(lua_State* state);
    static int getLoadedResourcesCount(lua_State* state);
    static int getScreenSize(lua_State* state);
	static BGameEngine* createInstance(BApplication* application);

private:
	BGameEngine(BApplication* application);
	BGameEngine(BGameEngine&);
	BGameEngine& operator=(BGameEngine&);
	void convertMapToScreen();
	static void startThread();
	int setLuaPath( lua_State* state, const char* path);
};
