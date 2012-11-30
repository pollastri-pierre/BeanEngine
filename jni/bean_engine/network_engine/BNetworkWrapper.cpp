/*
 * BNetworkWrapper.cpp
 *
 *  Created on: 7 juil. 2012
 *      Author: Pierre Pollastri
 */

#include "BNetworkWrapper.h"
#include "BNetworkGame.h"
#include "BClientHandler.h"
#include "BServerHandler.h"

BNetworkWrapper* BNetworkWrapper::_instance = NULL;

BNetworkWrapper::BNetworkWrapper() : _playerName("BeanPlayer")
{
    _services = 0;
}

BNetworkWrapper* BNetworkWrapper::instance()
{
	if (_instance == NULL)
		_instance = new BNetworkWrapper();
	return _instance;
}

void BNetworkWrapper::startServer(const BString& game, unsigned int max, const BString& pwd)
{
	BNetworkGame g(game, max, pwd);
    BNetworkEngine::instance()->startAsServer(g, _services);
}

int BNetworkWrapper::askForGames(lua_State* state)
{
	if (_instance == NULL)
	{
		Log::error("NetworkEngine not created");
		return 0;
	}
	BNetworkEngine::instance()->clientHandler()->askForGames();
	return 0;
}

void BNetworkWrapper::startClient()
{
    BNetworkEngine::instance()->startAsClient(_services);
}

void BNetworkWrapper::setPlayerName(const BString& name)
{
	_playerName = name;
	BNetworkEngine::instance()->setPlayer(name);
}

void BNetworkWrapper::stop()
{
    BNetworkEngine::instance()->stop();
}

int BNetworkWrapper::startClient(lua_State* state)
{
	if (_instance == NULL)
		return 0;
	_instance->startClient();
	return 0;
}

int BNetworkWrapper::startServer(lua_State* state)
{
	if (_instance == NULL)
		return 0;
	int argc = lua_gettop(state);
	if (argc == 1)
		_instance->startServer(luaL_checkstring(state, 1), 4, "");
	else if  (argc == 2)
		_instance->startServer(luaL_checkstring(state, 1), luaL_checkinteger(state, 2), "");
	else if (argc == 3)
		_instance->startServer(luaL_checkstring(state, 1), luaL_checkinteger(state, 2), luaL_checkstring(state, 3));
	else
		_instance->startServer(BString("Game_") + _instance->_playerName, 4, "");
	return 0;
}

int BNetworkWrapper::stopNetwork(lua_State* state)
{
	if (_instance == NULL)
		return 0;
	_instance->stop();
	return 0;
}

int BNetworkWrapper::lockGame(lua_State* state)
{
	if (BNetworkEngine::instance()->serverHandler())
		BNetworkEngine::instance()->serverHandler()->startGame();
	return 0;
}

int BNetworkWrapper::setPlayerName(lua_State* state)
{
	if (_instance == NULL)
		return 0;
	_instance->setPlayerName(luaL_checkstring(state, 1));
	return 0;
}

#define PUSH_STRING(x, y) lua_pushstring(state, x); lua_pushstring(state, y); lua_rawset(state, -3);
#define PUSH_NUMBER(x, y) lua_pushstring(state, x); lua_pushnumber(state, y); lua_rawset(state, -3);
#define PUSH_BOOL(x, y) lua_pushstring(state, x); lua_pushboolean(state, y); lua_rawset(state, -3);

int BNetworkWrapper::getAvailableGames(lua_State* state)
{
	BClientHandler* client = BNetworkEngine::instance()->clientHandler();
	if (_instance == NULL || client == NULL)
		return 0;
	const BNetworkGameMap& games = client->games();


	BNetworkGameMap::const_iterator it = games.begin();
	BNetworkGameMap::const_iterator ite = games.end();

	lua_newtable(state);
	for (int index = 1; it != ite ; ++it, index++)
	{
		lua_pushnumber(state, index);   /* Push the table index */
		lua_newtable(state);
		PUSH_STRING("name", it->second.getName().c_str());
		PUSH_STRING("hostIp", it->first.c_str());
		PUSH_STRING("author", it->second.getAuthor().c_str());
		PUSH_NUMBER("maxPlayer", it->second.getMaximumPlayerAllowed());
		PUSH_NUMBER("playerCount", it->second.getNbrPlayers());
		PUSH_BOOL("needPwd", it->second.needPassword());
		lua_rawset(state, -3);
	}

	return 1;
}

int BNetworkWrapper::isServer(lua_State* state)
{
	BClientHandler* client = BNetworkEngine::instance()->clientHandler();
	if (_instance == NULL || client == NULL)
	{
		lua_pushboolean(state, true);
		return 1;
	}
	lua_pushboolean(state, false);
	return 1;
}

int BNetworkWrapper::isClient(lua_State* state)
{
	BClientHandler* client = BNetworkEngine::instance()->clientHandler();
	if (_instance == NULL || client == NULL)
	{
		lua_pushboolean(state, false);
		return 1;
	}
	lua_pushboolean(state, true);
	return 1;
}

int BNetworkWrapper::getPlayersList(lua_State* state)
{
	if (_instance == NULL)
		return 0;
	BInGamePlayerMap players = BNetworkEngine::instance()->players();

	BInGamePlayerMap::const_iterator it = players.begin();
	BInGamePlayerMap::const_iterator ite = players.end();

	lua_newtable(state);
	for (int index = 1; it != ite ; ++it, index++)
	{
		lua_pushnumber(state, index);   /* Push the table index */
		lua_newtable(state);
		PUSH_STRING("name", it->second.getName().c_str());
		PUSH_BOOL("ready", it->second.isReady());
		PUSH_NUMBER("id", it->second.playerNumber());
		lua_rawset(state, -3);
	}
	return 1;
}

int BNetworkWrapper::myId(lua_State* state)
{
	if (_instance == NULL)
		return 0;
	lua_pushinteger(state, BNetworkEngine::instance()->me().playerNumber());
	return 1;
}

int BNetworkWrapper::popUdpMessage(lua_State* state)
{
	if (_instance == NULL)
		return 0;
	BNetworkEngine* e = BNetworkEngine::instance();
	if (e->udpMessagesCount() == 0)
	{
		lua_pushnil(state);
		return 1;
	}
	BPackage package = e->popUdpMessage();

	BNetworkData& d = package.data();
	BNetworkData::iterator it = d.begin();
	BNetworkData::iterator ite = d.end();
	lua_newtable(state);
	for (int index = 1; it != ite ; ++it, index++)
	{
		switch (it->second.type())
		{
		case BNetworkVariant::BOOLEAN:
			PUSH_BOOL(it->first.c_str(), it->second.toBool());
		break;
		case BNetworkVariant::STRING:
			PUSH_STRING(it->first.c_str(), it->second.toString().c_str());
			break;
		default:
			PUSH_NUMBER(it->first.c_str(), it->second.toNumber());
			break;
		}
	}

	return 1;
}

int BNetworkWrapper::getTcpMessageCount(lua_State* state)
{
	lua_pushnumber(state, BNetworkEngine::instance()->tcpMessagesCount());
	return 1;
}

int BNetworkWrapper::getUdpMessageCount(lua_State* state)
{
	lua_pushnumber(state, BNetworkEngine::instance()->udpMessagesCount());
	return 1;
}

int BNetworkWrapper::sendUdpMessage(lua_State* state)
{
	if (_instance == NULL)
		return 0;
	BNetworkEngine::instance()->sendUdpMessage(_instance->_udpMessage);
	_instance->_udpMessage = BPackage();
	return 0;
}

int BNetworkWrapper::sendTcpMessage(lua_State* state)
{
	if (_instance == NULL)
		return 0;
	BNetworkEngine::instance()->sendTcpMessage(_instance->_tcpMessage);
	_instance->_tcpMessage = BPackage();
	return 0;
}

int BNetworkWrapper::pushValueInTcpMessage(lua_State* state)
{
	if (_instance == NULL)
		return 0;
	int argc = lua_gettop(state);

	int index = 1;
	while (argc >= 2)
	{
		if (lua_isnumber(state, index + 1))
		{
			Log::debug("NUMBER");
			_instance->_tcpMessage.data()[luaL_checkstring(state, index++)] = lua_tonumber(state, index++);
		}
		else if (lua_isstring(state, index + 1))
		{
			Log::debug("STRING");
			_instance->_tcpMessage.data()[luaL_checkstring(state, index++)] = BString(luaL_checkstring(state, index++));
		}
		else //if (lua_isboolean(state, index + 1))
		{
			Log::debug("BOOL");
			_instance->_tcpMessage.data()[luaL_checkstring(state, index++)] = (bool)lua_toboolean(state, index++);
		}

		argc -= 2;
	}
	return 0;
}

int BNetworkWrapper::pushValueInUdpMessage(lua_State* state)
{
	if (_instance == NULL)
		return 0;
	int argc = lua_gettop(state);

	int index = 1;
	while (argc >= 2)
	{
		if (lua_isstring(state, index + 1))
			_instance->_udpMessage.data()[luaL_checkstring(state, index++)] = BString(luaL_checkstring(state, index++));
		else if (lua_isboolean(state, index + 1))
			_instance->_udpMessage.data()[luaL_checkstring(state, index++)] = (bool)lua_toboolean(state, index++);
		else if (lua_isnumber(state, index + 1))
			_instance->_udpMessage.data()[luaL_checkstring(state, index++)] = lua_tonumber(state, index++);
		argc -= 2;
	}
	return 0;
}

int BNetworkWrapper::clearUdpMessage(lua_State* state)
{
	if (_instance == NULL)
		return 0;
	_instance->_udpMessage = BPackage();
	return 0;
}

int BNetworkWrapper::clearTcpMessage(lua_State* state)
{
	if (_instance == NULL)
		return 0;
	_instance->_tcpMessage = BPackage();
	return 0;
}

int BNetworkWrapper::popTcpMessage(lua_State* state)
{
	if (_instance == NULL)
		return 0;

	BNetworkEngine* e = BNetworkEngine::instance();
	if (e->tcpMessagesCount() == 0)
	{
		lua_pushnil(state);
		return 1;
	}
	BPackage package = e->popTcpMessage();

	BNetworkData& d = package.data();
	BNetworkData::iterator it = d.begin();
	BNetworkData::iterator ite = d.end();
	lua_newtable(state);
	for (int index = 1; it != ite ; ++it, index++)
	{
		switch (it->second.type())
		{
		case BNetworkVariant::BOOLEAN:
			PUSH_BOOL(it->first.c_str(), it->second.toBool());
		break;
		case BNetworkVariant::STRING:
			PUSH_STRING(it->first.c_str(), it->second.toString().c_str());
			break;
		default:
			PUSH_NUMBER(it->first.c_str(), it->second.toNumber());
			break;
		}
	}

	return 1;
}

int BNetworkWrapper::playersListChanged(lua_State* state)
{
	if (_instance == NULL)
		return 0;
	lua_pushboolean(state, BNetworkEngine::instance()->playerListChanged());
	return 1;
}

int BNetworkWrapper::enableNetworkService(lua_State* state)
{
	if (_instance == NULL)
		return 0;
	int argc = lua_gettop(state);
	for (int i = 0; i < argc; i++)
	{
		BString s = BString(luaL_checkstring(state, i + 1)).toLowerCase();
		if (s == "broadcast game")
			_instance->_services |= BNetworkEngine::BROADCAST_GAME;
		else if (s == "udp")
			_instance->_services |= BNetworkEngine::UDP;
		else if (s == "tcp")
			_instance->_services |= BNetworkEngine::TCP;
		else if (s == "lan")
			_instance->_services |= (BNetworkEngine::TCP | BNetworkEngine::BROADCAST_GAME | BNetworkEngine::UDP);
	}
	return 0;
}

int BNetworkWrapper::joinGame(lua_State* state)
{
	BClientHandler* client = BNetworkEngine::instance()->clientHandler();
	if (_instance == NULL || client == NULL)
		return 0;

	if (lua_gettop(state) == 2)
	{
		client->connect(luaL_checkstring(state, 1), luaL_checkstring(state, 2));
	}
	else if (lua_gettop(state) == 1)
	{
		client->connect(luaL_checkstring(state, 1), "");
	}
	return 0;
}

int BNetworkWrapper::gameJoined(lua_State* state)
{
	BClientHandler* client = BNetworkEngine::instance()->clientHandler();
	if (_instance == NULL || client == NULL)
	{
		lua_pushboolean(state, true);
		return 1;
	}

	lua_pushboolean(state, client->isConnected());
	return 1;
}

int BNetworkWrapper::waitingToJoinGame(lua_State* state)
{
	BClientHandler* client = BNetworkEngine::instance()->clientHandler();
	if (_instance == NULL || client == NULL)
	{
		lua_pushboolean(state, false);
		return 1;
	}

	lua_pushboolean(state, client->isWaitingForConnection());
	return 1;
}

int BNetworkWrapper::disableNetworkService(lua_State* state)
{
	if (_instance == NULL)
		return 0;
	int argc = lua_gettop(state);
	for (int i = 0; i < argc; i++)
	{
		BString s = BString(luaL_checkstring(state, i + 1)).toLowerCase();
		if (s == "broadcast game")
			_instance->_services &= ~(BNetworkEngine::BROADCAST_GAME);
		else if (s == "udp")
			_instance->_services &= ~(BNetworkEngine::UDP);
		else if (s == "tcp")
			_instance->_services &= ~(BNetworkEngine::TCP);
		else if (s == "lan")
			_instance->_services &= ~(BNetworkEngine::TCP | BNetworkEngine::BROADCAST_GAME | BNetworkEngine::UDP);
	}
	return 0;
}

int BNetworkWrapper::startSynchronizePlayers(lua_State* state)
{
	if (_instance == NULL)
		return 0;
	BNetworkEngine::instance()->startSynchronizePlayers();
	return 0;
}

int BNetworkWrapper::sendReady(lua_State* state)
{
	if (_instance == NULL)
		return 0;
	BNetworkEngine::instance()->sendReady();
	return 0;
}

int BNetworkWrapper::isConnected(lua_State* state)
{
	if (_instance == NULL)
	{
		lua_pushboolean(state, false);
		return 1;
	}
	lua_pushboolean(state, BNetworkEngine::instance()->isConnected());
	return 1;
}

int BNetworkWrapper::keepConnectedWhenClientLeave(lua_State* state)
{
	if (_instance == NULL || !lua_isboolean(state, 1))
		return 0;
	BNetworkEngine::instance()->setKeepConnection(lua_toboolean(state, 1));
	return 0;
}

int BNetworkWrapper::isGameReady(lua_State* state)
{
	if (_instance == NULL)
	{
		lua_pushboolean(state, false);
		return 1;
	}
	lua_pushboolean(state, BNetworkEngine::instance()->isGameReady());
	return 1;
}

BNetworkWrapper::~BNetworkWrapper()
{

}

