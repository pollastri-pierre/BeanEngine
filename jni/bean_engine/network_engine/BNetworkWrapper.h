/*
 * BNetworkWrapper.h
 *
 *  Created on: 7 juil. 2012
 *      Author: Pierre Pollastri
 */

#ifndef BNETWORKWRAPPER_H_
#define BNETWORKWRAPPER_H_

#include <application/BString.h>
#ifndef __IOS__
#include <game_engine/BExposer.h>
#else

#endif
#include "BNetworkEngine.h"

class BNetworkWrapper
{

private:
    static BNetworkWrapper*     _instance;
    unsigned int				_services;
    BString                     _playerName;
    BPackage					_tcpMessage;
    BPackage					_udpMessage;
    
public:
	static BNetworkWrapper* instance();
	void startServer(const BString& game, unsigned int maxPlayer, const BString& password);
	void startClient();
	void setPlayerName(const BString& name);
    void start();
	void stop();
	void setServices();
	void operator()(void);

	virtual ~BNetworkWrapper();


	// Lua Func
	static int startServer(lua_State* state);
	static int startClient(lua_State* state);
	static int stopNetwork(lua_State* state);
	static int setPlayerName(lua_State* state);
	static int getAvailableGames(lua_State* state);
	static int askForGames(lua_State* state);
	static int enableNetworkService(lua_State* state);
	static int disableNetworkService(lua_State* state);
	static int joinGame(lua_State* state);
	static int waitingToJoinGame(lua_State* state);
	static int isConnected(lua_State* state);
	static int keepConnectedWhenClientLeave(lua_State* state);
	static int gameJoined(lua_State* state);
	static int isServer(lua_State* state);
	static int isClient(lua_State* state);
	static int getPlayersList(lua_State* state);
	static int myId(lua_State* state);
	static int popUdpMessage(lua_State* state);
	static int getUdpMessageCount(lua_State* state);
	static int getTcpMessageCount(lua_State* state);
	static int sendUdpMessage(lua_State* state);
	static int sendTcpMessage(lua_State* state);
	static int popTcpMessage(lua_State* state);
	static int lockGame(lua_State* state);
	static int playersListChanged(lua_State* state);
	static int pushValueInTcpMessage(lua_State* state);
	static int pushValueInUdpMessage(lua_State* state);
	static int clearUdpMessage(lua_State* state);
	static int clearTcpMessage(lua_State* state);
    static int startSynchronizePlayers(lua_State* state);
    static int sendReady(lua_State* state);
    static int isGameReady(lua_State* state);

private:
	BNetworkWrapper();
	static void startThread();
};

#endif /* BNETWORKWRAPPER_H_ */
