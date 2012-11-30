/*
 * BExposer.h
 *
 *  Created on: 27 janv. 2012
 *      Author: NC
 */

#ifndef BEXPOSER_H_
#define BEXPOSER_H_

#include <bobjects/BObject.h>
#include <bobjects/BPoint.h>

#ifdef DEFINE_luajit
extern "C" {
#endif

	#include <lua.h>

#ifdef DEFINE_luajit
}
#endif

class BExposer
{
private:
	static const luaL_Reg _globalFunctions[];

public:
	static void exposeClasses(lua_State* state);
	static void exposeGlobalFunction(lua_State* state);
	virtual ~BExposer();

	static int random(lua_State* state);
	static int print(lua_State* state);
	static int playBGM(lua_State* state);
	static int pauseBGM(lua_State* state);
	static int stopBGM(lua_State* state);
	static int setBGMVolume(lua_State* state);
	static int setBGMLoops(lua_State* state);
	static int getBGMName(lua_State* state);
	static int getBGMLoops(lua_State* state);
	static int getBGMVolume(lua_State* state);
	static int isBGMPlaying(lua_State* state);
	static int playEffect(lua_State* state);
};

#endif /* BEXPOSER_H_ */
