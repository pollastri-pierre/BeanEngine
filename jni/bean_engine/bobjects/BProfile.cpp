/*
 * BProfile.cpp
 *
 *  Created on: 28 juin 2012
 *      Author: NC
 */

#include "BProfile.h"
#include <game_engine/BGameEngine.h>
#include <boost/algorithm/string.hpp>

#define CLASS BProfile

B_DEFINE_CLASS("Profile", BObject)

B_NO_FUNCTION

B_METHODS_BEGIN
	B_METHOD(save)
	B_METHOD(create)
	B_METHOD(delete)
	B_METHOD(data)
B_METHODS_END

BProfile::BProfile()
{
	// TODO Auto-generated constructor stub

}

BProfile::~BProfile()
{
	// TODO Auto-generated destructor stub
	BProfileManager* mgr = BGameEngine::instance()->profileManager();
	if (mgr == NULL)
		return;
	if (mgr->open(_name) == NULL)
		delete _root;
}

void BProfile::save()
{
	BProfileManager* mgr = BGameEngine::instance()->profileManager();
	if (mgr == NULL)
		return;
	mgr->save(_name);
}

void BProfile::create()
{
	BProfileManager* mgr = BGameEngine::instance()->profileManager();
	if (mgr == NULL)
		return;
	mgr->createProfile(_name, _root);
}

void BProfile::deleteProfile()
{
	BProfileManager* mgr = BGameEngine::instance()->profileManager();
	if (mgr == NULL)
		return;
	mgr->deleteProfile(_name);
}

BProfileNode* BProfile::data(BString path)
{
	std::vector<std::string> children;
	std::string str(path.c_str());
	BProfileNode* node = _root;
	boost::split(children, str, boost::is_any_of("/"), boost::token_compress_on);
	std::vector<std::string>::iterator it = children.begin();
	std::vector<std::string>::iterator ite = children.end();

	for (; it != ite && node; it++)
		node = node->child(*it);

	return node;
}

BProfileNode* BProfile::data()
{
	return _root;
}

void BProfile::createDefaultRoot()
{
	_root = new BProfileNode("root");
	_root->push("__profile.name", _name);
}

B_CONSTRUCTOR
(void)self;
	if (lua_gettop(state) == 0)
	{
		luaL_error(state, "Constructor Profile(name) need a name as #1 parameter.");
		return;
	}
	_name = luaL_checkstring(state, 1);
	BProfileManager* mgr = BGameEngine::instance()->profileManager();
	if (mgr == NULL)
		return;
	_root = mgr->open(_name);

	if (_root == NULL)
		createDefaultRoot();

B_END

B_IMPLEMENTATION(save)
	self->save();
	return 0;
B_END

B_IMPLEMENTATION(create)
	self->create();
	return 0;
B_END

B_IMPLEMENTATION(delete)
	self->deleteProfile();
	return 0;
B_END

B_IMPLEMENTATION(data)
	int argc = lua_gettop(state);
	BProfileNode* node;
	if (argc == 1)
		node = self->data();
	else
		node = self->data(luaL_checkstring(state, 2));
	if (node == NULL)
	{
		lua_pushnil(state);
		return 1;
	}
	BNode* obj = new BNode(node, true);
	return BClass<BNode>::newInstance(state, obj);
B_END
