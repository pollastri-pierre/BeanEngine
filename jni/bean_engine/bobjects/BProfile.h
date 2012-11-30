/*
 * BProfile.h
 *
 *  Created on: 28 juin 2012
 *      Author: NC
 */

#ifndef BPROFILE_H_
#define BPROFILE_H_

#include "BObject.h"
#include "BClass.hpp"
#include <game_engine/BGameEngine.h>
#include "BNode.h"

class BProfile : public BObject
{
	BString _name;
	BProfileNode* _root;

public:
	BProfile();
	void save();
	void create();
	void deleteProfile();
	BProfileNode* data(BString path);
	BProfileNode* data();
	virtual ~BProfile();

	B_LUA_CLASS
	B_DECLARATION(save);
	B_DECLARATION(create);
	B_DECLARATION(delete);
	B_DECLARATION(data);

private:
	void createDefaultRoot();
};

#endif /* BPROFILE_H_ */
