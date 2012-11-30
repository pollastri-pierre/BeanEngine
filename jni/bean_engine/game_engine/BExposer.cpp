/*
 * BExposer.cpp
 *
 *  Created on: 27 janv. 2012
 *      Author: NC
 */

#include "BExposer.h"
#include "BGameEngine.h"
#include <network_engine/BNetworkWrapper.h>
#include <bobjects/BRect.h>
#include <bobjects/BSize.h>
#include <bobjects/BColor.h>
#include <bobjects/BNode.h>
#include <bobjects/BProfile.h>
#include <bobjects/BRenderer.h>

#include <bgraphicobjects/BGraphicObject.h>
#include <bgraphicobjects/BScene.h>
#include <bgraphicobjects/BRectangle.h>
#include <bgraphicobjects/BLabel.h>
#include <bgraphicobjects/BPicture.h>
#include <bgraphicobjects/BSprite.h>
#include <bgraphicobjects/BAnimatedSprite.h>
#include <bgraphicobjects/BView.h>

#include <sound_engine/BSoundEngine.h>
#include <sound_engine/BSoundEffect.h>
#include <event_manager/BTouchEvent.h>

#include <math.h>
#ifdef __ANDROID__
#define LOG(x, y) __android_log_print(ANDROID_LOG_DEBUG, "BeanEngine", x, y)
#include <android/log.h>
#else
#include <iostream>
#define LOG(x, y) std::cout << "<lua_print>" << y << std::endl
#endif

const luaL_Reg BExposer::_globalFunctions[] = {
		{"isRunning", BGameEngine::isRunning},
		{"display", BGameEngine::display},
		{"getCurrentFramerate", BGameEngine::getCurrentFramerate},
		{"setFramerate", BGameEngine::setFramerate},
		{"framerate", BGameEngine::framerate},
		{"nextFile", BGameEngine::nextFile},
		{"setCurrentScene", BGameEngine::setCurrentScene},
		{"pushString", BGameEngine::pushString},
		{"pushBoolean", BGameEngine::pushBoolean},
		{"pushNumber", BGameEngine::pushNumber},
		{"popString", BGameEngine::popString},
		{"popBoolean", BGameEngine::popBoolean},
		{"popNumber", BGameEngine::popNumber},
		//{"random", BExposer::random},
		{"setFullscreen", BGameEngine::setFullscreen},
		{"getTouchCount", BGameEngine::getTouchCount},
		{"popTouch", BGameEngine::popTouch},
		{"parseConfig", BGameEngine::loadConf},
		{"loadResources", BGameEngine::loadResources},
        {"getLoadedResourcesCount", BGameEngine::getLoadedResourcesCount},
        {"getResourcesCount", BGameEngine::getResourcesCount},
        {"getScreenSize", BGameEngine::getScreenSize},
		{"playBGM", BExposer::playBGM},
		{"pauseBGM", BExposer::pauseBGM},
		{"stopBGM", BExposer::stopBGM},
		{"setBGMVolume", BExposer::setBGMVolume},
		{"setBGMLoops", BExposer::setBGMLoops},
		{"getBGMName", BExposer::getBGMName},
		{"getBGMVolume", BExposer::getBGMVolume},
		{"getBGMLoops", BExposer::getBGMLoops},
		{"isBGMPlaying", BExposer::isBGMPlaying},
		{"playEffect", BExposer::playEffect},
		{"getProfileNames", BGameEngine::getProfileNames},
		{"getProfileCount", BGameEngine::getProfileCount},
		{"startServer", BNetworkWrapper::startServer},
		{"startClient", BNetworkWrapper::startClient},
		{"stopNetwork", BNetworkWrapper::stopNetwork},
		{"setPlayerName", BNetworkWrapper::setPlayerName},
		{"getAvailableGames", BNetworkWrapper::getAvailableGames},
		{"enableNetworkService", BNetworkWrapper::enableNetworkService},
		{"disableNetworkService", BNetworkWrapper::disableNetworkService},
		{"askForGames", BNetworkWrapper::askForGames},
		{"joinGame", BNetworkWrapper::joinGame},
		{"gameJoined", BNetworkWrapper::gameJoined},
		{"waitingToJoinGame", BNetworkWrapper::waitingToJoinGame},
		{"myId", BNetworkWrapper::myId},
		{"getPlayersList", BNetworkWrapper::getPlayersList},
		{"playersListChanged", BNetworkWrapper::playersListChanged},
		{"isClient", BNetworkWrapper::isClient},
		{"isServer", BNetworkWrapper::isServer},
		{"popTcpMessage", BNetworkWrapper::popTcpMessage},
		{"sendTcpMessage", BNetworkWrapper::sendTcpMessage},
		{"sendUdpMessage", BNetworkWrapper::sendUdpMessage},
		{"popUdpMessage", BNetworkWrapper::popUdpMessage},
		{"clearTcpMessage", BNetworkWrapper::clearTcpMessage},
		{"clearUdpMessage", BNetworkWrapper::clearUdpMessage},
		{"pushValueInTcpMessage", BNetworkWrapper::pushValueInTcpMessage},
		{"pushValueInUdpMessage", BNetworkWrapper::pushValueInUdpMessage},
		{"TcpMessage", BNetworkWrapper::pushValueInTcpMessage},
		{"UdpMessage", BNetworkWrapper::pushValueInUdpMessage},
		{"isGameReady", BNetworkWrapper::isGameReady},
		{"startSynchronizePlayers", BNetworkWrapper::startSynchronizePlayers},
		{"sendReady", BNetworkWrapper::sendReady},
		{"lockGame", BNetworkWrapper::lockGame},
		{"isConnected", BNetworkWrapper::isConnected},
		{"keepConnectedWhenClientLeave", BNetworkWrapper::keepConnectedWhenClientLeave},
		{"getTcpMessageCount", BNetworkWrapper::getTcpMessageCount},
		{"getUdpMessageCount", BNetworkWrapper::getUdpMessageCount},
		{"debug", BExposer::print},
#ifdef __ANDROID__
#ifndef DEFINE_lua
		{"print", BExposer::print},
#endif
#endif
    
		{NULL, NULL}
};

void BExposer::exposeClasses(lua_State* state)
{
	BClass<BObject>::expose(state);
	BClass<BPoint>::expose(state);
	BClass<BRect>::expose(state);
	BClass<BSize>::expose(state);
	BClass<BColor>::expose(state);
	BClass<BGraphicObject>::expose(state);
	BClass<BScene>::expose(state);
	BClass<BRectangle>::expose(state);
	BClass<BLabel>::expose(state);
	BClass<BPicture>::expose(state);
	BClass<BSprite>::expose(state);
	BClass<BSprite>::expose(state);
	BClass<BAnimatedSprite>::expose(state);
	BClass<BNode>::expose(state);
	BClass<BProfile>::expose(state);
    BClass<BView>::expose(state);
    BClass<BRenderer>::expose(state);
}

void BExposer::exposeGlobalFunction(lua_State* state)
{
	//luaL_register(state, NULL, _globalFunctions);
	for (int index = 0; _globalFunctions[index].name != NULL; index++)
	{
		lua_pushcfunction(state, _globalFunctions[index].func);
		lua_setglobal(state, _globalFunctions[index].name);
	}

	BString globals;
	globals << "TOUCH_DOWN = " << BTouchEvent::DOWN << "\n";
	globals << "TOUCH_UP = " << BTouchEvent::UP << "\n";
	globals << "TOUCH_MOVE = " << BTouchEvent::MOVE << "\n";
	luaL_dostring(state, globals.c_str());
}

int BExposer::random(lua_State* state)
{
	lua_Integer min = luaL_checkinteger(state, 1);
	lua_Integer max = luaL_checkinteger(state, 2);

	lua_pushinteger(state, rand() % (max - min) + min);

	return 1;
}

int BExposer::print(lua_State* state)
{
#ifdef __LUA_DEBUG__
	int args = lua_gettop(state);
	int index = 1;

	while (index <= args)
	{
		if (lua_isnumber(state, index))
			LOG("Lua print - %g", luaL_checknumber(state, index));
		else if (lua_isstring(state, index))
			LOG("Lua print - %s", luaL_checkstring(state, index));
		else
		{
			LOG("Lua print - %s", BClass<BObject>::self(state, index)->toString().c_str());
		}
		index++;
	}
#endif
	return 0;
}

int BExposer::playBGM(lua_State* state)
{
	BSoundEngine* engine = BSoundEngine::instance();

	int arg = lua_gettop(state);

	if (arg == 1)
	{
		engine->playBGM(luaL_checkint(state, 1));
	}
	else if (arg == 4)
	{
		const char* name = luaL_checkstring(state, 1);
		int level = luaL_checknumber(state, 2);
		int volume = luaL_checknumber(state, 3);
		bool isLooping = lua_toboolean(state, 4);
		BSound* s = BResourceManager::instance()->getSound(name);
		if (s == NULL)
		{
			luaL_error(state, "Error: playBGM BGM %s not found.", name);
			return 0;
		}
		engine->playBGM(name, s->path(), level, volume, isLooping);
	}
	else
	{
		luaL_error(state, "Error: playBGM invalid parameters count.");
	}
	return 0;
}

int BExposer::pauseBGM(lua_State* state)
{
	BSoundEngine* engine = BSoundEngine::instance();

	int arg = lua_gettop(state);

	if (arg == 1)
	{
		engine->pauseBGM(luaL_checkint(state, 1));
	}
	else
	{
		luaL_error(state, "Error: playBGM invalid parameters count. Need 1");
	}
	return 0;
}

int BExposer::stopBGM(lua_State* state)
{
	BSoundEngine* engine = BSoundEngine::instance();

	int arg = lua_gettop(state);

	if (arg == 1)
	{
		engine->stopBGM(luaL_checkint(state, 1));
	}
	else
	{
		luaL_error(state, "Error: stopBGM invalid parameters count. Need 1");
	}
	return 0;
}

int BExposer::setBGMVolume(lua_State* state)
{
	BSoundEngine* engine = BSoundEngine::instance();

	int arg = lua_gettop(state);

	if (arg == 2)
	{
		engine->setBGMVolume(luaL_checkint(state, 1), luaL_checkint(state, 2));
	}
	else
	{
		luaL_error(state, "Error: setBGMVolume invalid parameters count. Need 2");
	}
	return 0;
}

int BExposer::setBGMLoops(lua_State* state)
{
	BSoundEngine* engine = BSoundEngine::instance();

	int arg = lua_gettop(state);

	if (arg == 2)
	{
		engine->setBGMVolume(luaL_checkint(state, 1), lua_toboolean(state, 2));
	}
	else
	{
		luaL_error(state, "Error: setBGMLoops invalid parameters count. Need 2");
	}
	return 0;
}

int BExposer::getBGMName(lua_State* state)
{
	BSoundEngine* engine = BSoundEngine::instance();

	int arg = lua_gettop(state);

	if (arg == 1)
	{
		lua_pushstring(state, engine->getBGMName(luaL_checkint(state, 1)).c_str());
		return 1;
	}
	else
	{
		luaL_error(state, "Error: getBGMName invalid parameters count. Need 1");
	}
	return 0;
}

int BExposer::getBGMLoops(lua_State* state)
{
	BSoundEngine* engine = BSoundEngine::instance();

	int arg = lua_gettop(state);

	if (arg == 1)
	{
		lua_pushboolean(state, engine->getBGMLoops(luaL_checkint(state, 1)));
		return 1;
	}
	else
	{
		luaL_error(state, "Error: getBGMName invalid parameters count. Need 1");
	}
	return 0;
}

int BExposer::getBGMVolume(lua_State* state)
{
	BSoundEngine* engine = BSoundEngine::instance();

	int arg = lua_gettop(state);

	if (arg == 1)
	{
		lua_pushnumber(state, engine->getBGMVolume(luaL_checkint(state, 1)));
		return 1;
	}
	else
	{
		luaL_error(state, "Error: getBGMName invalid parameters count. Need 1");
	}
	return 0;
}

int BExposer::isBGMPlaying(lua_State* state)
{
	BSoundEngine* engine = BSoundEngine::instance();

	int arg = lua_gettop(state);

	if (arg == 1)
	{
		lua_pushboolean(state, engine->isBGMPlaying(luaL_checkint(state, 1)));
		return 1;
	}
	else
	{
		luaL_error(state, "Error: isBGMPlaying invalid parameters count. Need 1");
	}
	return 0;
}

int BExposer::playEffect(lua_State* state)
{
	BSoundEngine* engine = BSoundEngine::instance();

	int arg = lua_gettop(state);

	if (arg == 2)
	{
		const char* name = luaL_checkstring(state, 1);
		int volume = luaL_checknumber(state, 2);
		BSound* s = BResourceManager::instance()->getSound(name);
		if (s == NULL)
		{
			luaL_error(state, "Error: playEffect Effect [%s] not found.", name);
			return 0;
		}
		engine->playSoundEffect(dynamic_cast<BSoundEffect*>(s), volume);
	}
	else
	{
		luaL_error(state, "Error: playBGM invalid parameters count.");
	}
	return 0;
}
