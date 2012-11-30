/*
 * BProfileData.h
 *
 *  Created on: 28 juin 2012
 *      Author: NC
 */

#ifndef BPROFILEDATA_H_
#define BPROFILEDATA_H_

#include "BObject.h"
#include "BClass.hpp"
#include <profile_manager/BProfileNode.h>

class BNode : public BObject
{
private:
	BProfileNode* 	_node;
	bool			_isRoot;

public:
	BNode();
	BNode(BProfileNode* node, bool root = false);
	virtual	std::string toString() const;
	virtual ~BNode();

	B_LUA_CLASS
	B_DECLARATION(setName)
	B_DECLARATION(push)
	B_DECLARATION(pop)
	B_DECLARATION(popAll)
	B_DECLARATION(set)
	B_DECLARATION(children)
	B_DECLARATION(child)
	B_DECLARATION(type)
	B_DECLARATION(toString)
	B_DECLARATION(toNumber)
	B_DECLARATION(toBoolean)
	B_DECLARATION(name)
	B_DECLARATION(parent)
};

#endif /* BPROFILEDATA_H_ */
