/*
 * BGameEngine.c++
 *
 *  Created on: 24 janv. 2012
 *      Author: Pierre Pollastri
 *      mail: pollastri.p@gmail.com
 */

#include "BGameEngine.h"
#include <application/Log.h>
#include "BExposer.h"
#include <biomodule/BReader.h>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <sched.h>
#include "BAutoupdateObject.h"
#include <resource_manager/BConfigLoader.h>
#include <bgraphicobjects/BView.h>

using namespace boost;

BGameEngine* BGameEngine::_instance = NULL;

BLuaArgs BGameEngine::_globalArguments;
BLuaArgs BGameEngine::_nextGlobalArguments;

static useconds_t operator*(struct timeval& tv)
{
	return (tv.tv_sec) * 1000000 + tv.tv_usec;
}

static struct timeval operator+(struct timeval& tv1, struct timeval& tv2)
{
	struct timeval tv;
	tv.tv_sec = tv1.tv_sec + tv2.tv_sec + (tv1.tv_usec + tv2.tv_usec) / 1000000;
	tv.tv_usec = (tv1.tv_usec + tv2.tv_usec) % 1000000;
	return tv;
}

static struct timeval operator+(struct timeval& tv1, useconds_t t)
{
	struct timeval tv;
	tv.tv_sec = tv1.tv_sec + (tv1.tv_usec + t) / 1000000;
	tv.tv_usec = (tv1.tv_usec + t) % 1000000;
	return tv;
}

static bool operator<(struct timeval& tv1, struct timeval& tv2)
{
	return tv1.tv_sec < tv2.tv_sec || (tv1.tv_sec == tv2.tv_sec && tv1.tv_usec < tv2.tv_usec);
}

static bool operator>(struct timeval& tv1, struct timeval& tv2)
{
	return tv1.tv_sec > tv2.tv_sec || (tv1.tv_sec == tv2.tv_sec && tv1.tv_usec > tv2.tv_usec);
}

static useconds_t operator-(struct timeval& tv1, struct timeval& tv2)
{
	return (tv1.tv_sec - tv2.tv_sec) * 1000000 + tv1.tv_usec - tv2.tv_usec;
}

BGameEngine::BGameEngine(BApplication* app) : _application(app), _luaState(NULL), _status(READY), _mutex(NULL)
{
	setFramerate(60);
	_actualFramerate = -1;
	_currentFrame = 0;
	_defaultSize = BSize(960, 640);
	_currentScene = NULL;
	_graphicEngine = NULL;
	_initWindow = 0;
	_fullscreen = false;
	_eventManager = NULL;
	_networkWrapper = NULL;
}

void BGameEngine::setEventManager(BEventManager* eventManager)
{
	_eventManager = eventManager;
}

void BGameEngine::setProfileManager(BProfileManager* profileManager)
{
	_profileManager = profileManager;
}

BProfileManager* BGameEngine::profileManager() const
{
	return _profileManager;
}

BGameEngine::BGameEngine(BGameEngine&) {}

BGameEngine& BGameEngine::operator=(BGameEngine& ge) {return ge;}

void BGameEngine::pause()
{
	_status = PAUSE;
	if (_mutex)
		_mutex->lock();
}

void BGameEngine::stop()
{
	_status = STOP;
}

void BGameEngine::resume()
{
	_status = START;
	if (_mutex)
		_mutex->unlock();
}

BGameEngine::Status BGameEngine::status() const
{
	return _status;
}


std::map<BString, bool> ___included;
regex ___requireExpression("require +\"([a-zA-Z0-9_.]+)\" \"+([a-zA-Z0-9_./]+)\"");

#include <resource_manager/BResourceManager.h>

static inline BString loadScript(BString& scriptPath)
{
	BString data;

	BReader* reader = BReader::create();
	if (!reader->open(scriptPath))
	{
		Log::warn("Warning: File [%s] not found", scriptPath.c_str());
		return "";
	}
	Log::debug("Load script [%s]", scriptPath.c_str());
	reader->readAll();
	data = reader->data();
	___included[scriptPath] = true;

	bool finish = false;
	while (!finish)
	{
		boost::sregex_iterator it(data.begin(), data.end(), ___requireExpression);
		boost::sregex_iterator end;

		finish = it == end;
		if (!finish)
		{
			BString path;
			path << (*it)[2] << "/" << (*it)[1];
			if (___included.find(path) != ___included.end())
			{
				//erase_all(data, it->str());
				replace_first(data, it->str(), "");
			}
			else
			{
				BString s = loadScript(path) << "\n";
				replace_first(data, it->str(), s);
				//replace_all(data, it->str(), "");
			}
		}
	}
	delete reader;
	return data;
}

void BGameEngine::operator ()()
{
	_status = START;
    
#if defined (__SFML__)
    _luaState = luaL_newstate();
#else
    _luaState = lua_open();
#endif
	luaL_openlibs(_luaState);
    luaopen_base(_luaState);
    luaopen_table(_luaState);
    luaopen_string(_luaState);
    luaopen_io(_luaState);
    luaopen_math(_luaState);

    BExposer::exposeGlobalFunction(_luaState);
    BExposer::exposeClasses(_luaState);

    setLuaPath(_luaState, BString(BString(_application->getDirectory("scripts")) + "/scripts/?.lua").c_str());
    setLuaPath(_luaState, BString(BString(_application->getDirectory("scripts")) + "?.lua").c_str());

    while (!_currentScriptPath.empty() && _status != STOP)
    {
    	___included.clear();
        BApplication::instance()->notifyCurrentScript(_currentScriptPath);
#	if defined (_B_USE_NO_INSTALL_)
    	BString script = loadScript(_currentScriptPath);
    	if (script.size())
    	{
#	endif
    		_running = true;
    		if (_graphicEngine)
    			_graphicEngine->init();
    		gettimeofday(&_instance->_beforeTime, NULL);
    		_nextTime = _beforeTime + _delta;
            if (_eventManager)
                _eventManager->getEvents();
            BView::resetCallbackRegistry();
#	if defined(_B_USE_NO_INSTALL_)
    		int status = luaL_dostring(_luaState, script.c_str());
#else
    		BString finalPath = _application->getDirectory("scripts") +  _currentScriptPath;
    		int status = luaL_dofile(_luaState, finalPath.c_str());
#	endif
    		if (status != 0)
    		{
    			Log::debug("Lua script terminated LUA_RETURN = [%s]", lua_tostring(_luaState, -1));
    			lua_pop(_luaState, 1);
    		}
#	if defined (_B_USE_NO_INSTALL_)
    	}
#	endif
    	BObject::___gc_collect();
    	_currentScriptPath.clear();

    	if (!_nextScriptPath.empty())
    	{
    		_currentScriptPath = _nextScriptPath;
    		_nextScriptPath.clear();
    	}
    }

    Log::debug("BGameEngine exits!");
    if (_instance == this)
    {
    	_instance = NULL;
    	delete this;
    }
   
#   ifdef __ANDROID__
    _application->exit();
#   endif
}

void BGameEngine::begin()
{
	convertMapToScreen();
	//return 0;
}

int BGameEngine::update(lua_State* state)
{
	/*while (_instance->_graphicEngine && _instance->_initWindow > 0)
	{
		Log::debug("INIT GRAPHIC ENGINE");
		_instance->_graphicEngine->init();
		_instance->_initWindow--;
	}*/

	// Rendu graphique

	struct timeval now;

	if (_currentScene && _eventManager)
	{
		_events.release();
		_events = _eventManager->getEvents();
		BEventContext context = {_screenScalers, _screenBoundingRect};
		_currentScene->processEvents(_events, context);
	}

	//lua_gc(state, LUA_GCCOLLECT, 0);

	//collect();

	gettimeofday(&now, NULL);
	//if (/*now < _nextTime || */_currentFrame % 5 == 0)
	//{
	if (_currentScene && _graphicEngine)
		_graphicEngine->draw(_currentScene, _screenScalers, _screenBoundingRect, _fullscreen);

	BAutoupdateObject::updateObjects();

		gettimeofday(&_instance->_afterTime, NULL);
			_timeToWait = _delta - (_afterTime - _beforeTime);
			if (_timeToWait > 0)
				usleep(_timeToWait);
	//}

	gettimeofday(&_instance->_beforeTime, NULL);

	if (_beforeTime > _nextTime)
		_nextTime = _nextTime + _delta;

	if (_beforeTime - _beginRecord >= 1000000 || _instance->_actualFramerate == -1)
	{
		memcpy(&_instance->_beginRecord, &_instance->_beforeTime, sizeof (_instance->_beforeTime));
		_instance->_actualFramerate = _instance->_currentFrame;
		_instance->_currentFrame = 0;
	}
	_instance->_currentFrame++;
	return 0;
}

void BGameEngine::startThread()
{
	_instance->operator ()();
}

void BGameEngine::start(void)
{
	_status = START;

	int retcode;
	int policy;

	_mutex = new boost::recursive_mutex();
	_thread = new boost::thread(startThread);
	pthread_t threadID = (pthread_t) _thread->native_handle();

	struct sched_param param;

	if ((retcode = pthread_getschedparam(threadID, &policy, &param)) != 0)
	{
		errno = retcode;
		//exit(EXIT_FAILURE);
	}

	policy = SCHED_FIFO;
	param.sched_priority = 4;

	if ((retcode = pthread_setschedparam(threadID, policy, &param)) != 0)
	{
		errno = retcode;
	}

}

void BGameEngine::startWith(std::string path)
{
	setCurrentScript(path);
	start();
}

void BGameEngine::setCurrentScript(std::string path)
{
	_currentScriptPath = path;
}

BGameEngine* BGameEngine::instance()
{
	return _instance;
}

void BGameEngine::release(BObject* object)
{
	if (instance())
		instance()->___gc.push_back(object);
}

BGameEngine::~BGameEngine()
{
	if (_luaState)
		lua_close(_luaState);
}

void BGameEngine::setFramerate(int fps)
{
	if (fps <= 0)
		fps = 60;
	_framerate = fps;
	_delta = 1000000 / _framerate;
}

int BGameEngine::isRunning(lua_State* state)
{
	lua_pushboolean(state, _instance->isRunning());
	_instance->begin();
	return 1;
}

bool BGameEngine::isRunning()
{
	if (_application->isIndependent() && _status == PAUSE)
	{
		_mutex->lock();
		_mutex->unlock();
	}
    else if (!_application->isIndependent())
		_running = _running && _application->processEvents();
	if (_graphicEngine && _graphicEngine->isSurfaceCreated() && !_graphicEngine->isInitialized())
		_graphicEngine->init();
	return _running && _status != STOP;
}

void BGameEngine::setNetworkWrapper(BNetworkWrapper* networkWrapper)
{
	_networkWrapper = networkWrapper;
}

void BGameEngine::initGraphicEngine()
{
	_initWindow++;
}

BSize BGameEngine::defaultSceneSize() const
{
	return _defaultSize;
}

void BGameEngine::setGraphicEngine(BGraphicEngine* engine)
{
	_graphicEngine = engine;
}

int BGameEngine::setFullscreen(lua_State* state)
{
	bool value = lua_toboolean(state, 1);
	_instance->_fullscreen =  value;
	return 0;
}

int BGameEngine::display(lua_State* state)
{
	return _instance->update(state);
}

int BGameEngine::loopBegin(lua_State* state)
{
	return 0;//_instance->begin(state);
}

void BGameEngine::collect()
{
	std::list<BObject*>::iterator it = _instance->___gc.begin();
	std::list<BObject*>::iterator ite = _instance->___gc.end();
	for (; it != ite; ++it)
		(*it)->release();
	_instance->___gc.clear();
}

int BGameEngine::setFramerate(lua_State* state)
{
	int framerate = luaL_checkinteger(state, 1);
	_instance->setFramerate(framerate);
	return 0;
}

int BGameEngine::getCurrentFramerate(lua_State* state)
{
	lua_pushinteger(state, _instance->_actualFramerate);
	return 1;
}

int BGameEngine::setCurrentScene(lua_State* state)
{
	BScene* scene = BClass<BScene>::self(state);
	_instance->_currentScene = scene;
	return 0;
}

int BGameEngine::nextFile(lua_State* state)
{
	const char* file = luaL_checklstring(state, 1, NULL);
	const char* path = luaL_checklstring(state, 2, NULL);

	BString p;
	p << path;
	if (path[strlen(path) - 1] != '/')
		p << "/";
	p << file;
	_instance->_nextScriptPath = p;
	_instance->_running = false;

	return 0;
}

int BGameEngine::pushBoolean(lua_State* state)
{
	BString key = luaL_checklstring(state, 1, NULL);
	bool value = (bool)lua_toboolean(state, 2);
	_nextGlobalArguments[key] = value;
	return 0;
}

int BGameEngine::pushString(lua_State* state)
{
	BString key = luaL_checklstring(state, 1, NULL);
	const char* str = luaL_checkstring(state, 2);
	if (str)
	{
		_nextGlobalArguments[key] = BString(str);
	}
	return 0;
}

int BGameEngine::pushNumber(lua_State* state)
{
	BString key = luaL_checklstring(state, 1, NULL);
	bool value = (bool)luaL_checkinteger(state, 2);
	_nextGlobalArguments[key] = value;
	return 0;
}

int BGameEngine::popBoolean(lua_State* state)
{
	BString key = luaL_checklstring(state, 1, NULL);
	if (_globalArguments.find(key) != _globalArguments.end())
		lua_pushboolean(state, boost::get<bool>(_globalArguments[key]));
	else
		lua_pushnil(state);
	return 1;
}

int BGameEngine::popString(lua_State* state)
{
	BString key = luaL_checklstring(state, 1, NULL);
	if (_globalArguments.find(key) != _globalArguments.end())
		lua_pushstring(state, boost::get<BString>(_globalArguments[key]).c_str());
	else
		lua_pushnil(state);
	return 1;
}

int BGameEngine::getProfileNames(lua_State* state)
{
	lua_createtable(state, 0, 0);
	int tableIndex = lua_gettop(state);
	BStringVector vec = _instance->_profileManager->getProfileNames();
	for (int i = 0; i < vec.size(); i++)
	{
		lua_pushstring(state, vec[i].c_str());
		lua_rawseti(state, tableIndex, i + 1);
	}
	return 1;
}

int BGameEngine::getProfileCount(lua_State* state)
{
	lua_pushinteger(state, _instance->profileManager()->getProfileCount());
	return 1;
}

int BGameEngine::popNumber(lua_State* state)
{
	BString key = luaL_checklstring(state, 1, NULL);
	if (_globalArguments.find(key) != _globalArguments.end())
		lua_pushinteger(state, boost::get<lua_Number>(_globalArguments[key]));
	else
		lua_pushnil(state);
	return 1;
}

int BGameEngine::getTouchCount(lua_State* state)
{
	lua_pushnumber(state, _instance->_events.size());
	return 1;
}

int BGameEngine::popTouch(lua_State* state)
{
	if (_instance->_events.size() == 0)
	{
		lua_pushnil(state);
		return 1;
	}
	BTouchEvent* e = _instance->_events[0];
	lua_pushnumber(state, e->x());
	lua_pushnumber(state, e->y());
	lua_pushnumber(state, e->type());
	lua_pushnumber(state, e->index());
	_instance->_events.remove(0);
	return 4;
}

int BGameEngine::framerate() const
{
    return _framerate;
}

int BGameEngine::framerate(lua_State* state)
{
	lua_pushinteger(state, BGameEngine::instance()->framerate());
	return 1;
}

int BGameEngine::loadConf(lua_State* state)
{
	const char* file = luaL_checklstring(state, 1, NULL);
	const char* path = luaL_checklstring(state, 2, NULL);

	BString p;
	p << path;
	if (path[strlen(path) - 1] != '/')
		p << "/";
	p << file;
	if (BConfigLoader::instance()->parse(p))
		BResourceManager::instance()->setNewConfFile(true);
    else
       BResourceManager::instance()->setNewConfFile(false);
	return 0;
}

int BGameEngine::loadResources(lua_State* state)
{
	int nbr = (int)luaL_checkinteger(state, 1);
	bool r = BResourceManager::instance()->loadResources(nbr);
	lua_pushboolean(state, r);
	return 1;
}

void BGameEngine::convertMapToScreen()
{
	if (!_graphicEngine || !_currentScene)
		return ;
	_graphicEngine->adjustToRatio(_currentScene, &_screenBoundingRect, _screenScalers, _fullscreen);
}

int BGameEngine::setLuaPath( lua_State* L, const char* path )
{
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "path"); // get field "path" from table at top of stack (-1)
    std::string cur_path = lua_tostring(L, -1 ); // grab path string from top of stack
    cur_path.append(";"); // do your path magic here
    cur_path.append(path);
    lua_pop(L, 1); // get rid of the string on the stack we just pushed on line 5
    lua_pushstring(L, cur_path.c_str() ); // push the new one
    lua_setfield(L, -2, "path" );
    lua_pop( L, 1 ); // get rid of package table from top of stack
    return 0;
}

int BGameEngine::getResourcesCount(lua_State *state)
{
    lua_pushinteger(state, BResourceManager::instance()->resourcesCount());
    return 1;
}

int BGameEngine::getScreenSize(lua_State* state)
{
    long width = 0;
    long height = 0;
    if (_instance->_graphicEngine)
    {
        width = _instance->_graphicEngine->screenWidth();
        height = _instance->_graphicEngine->screenHeight();
    }
    lua_pushinteger(state, width);
    lua_pushinteger(state, height);
    return 2;
}

int BGameEngine::getLoadedResourcesCount(lua_State *state)
{
    lua_pushinteger(state, BResourceManager::instance()->loadedResourcesCount());
    return 1;
}

BGameEngine* BGameEngine::createInstance(BApplication* application)
{
	delete _instance;
	_instance = new BGameEngine(application);
	return _instance;
}

lua_State* BGameEngine::L() const
{
    return _luaState;
}
