/*
 * BProfileNode.cpp
 *
 *  Created on: 28 juin 2012
 *      Author: Pierre Pollastri
 */

#include "BProfileNode.h"
#include <application/Log.h>
#include <sstream>

BProfileNode::BProfileNode() : 	_type(BProfileNode::Number),
								_parent(NULL)
{

}

BProfileNode::BProfileNode(const BString& name) : 	_name(name),
													_type(BProfileNode::Number),
													_parent(NULL)
{
	_data.number = 0;

	std::ostringstream ost;
		boost::archive::text_oarchive oa(ost, boost::archive::no_header);
		oa << *this;
}

void BProfileNode::deleteData()
{
	switch (_type)
	{
	case String:
		delete _data.string;
		break;
	case List:
		emptyList(*_data.list);
		break;
	}
}

void BProfileNode::emptyList(NodeList& list)
{
	while (list.size() > 0)
		delete list.front();
}

void BProfileNode::changeType(BProfileNode::NodeType type)
{
	if (type == _type)
		return ;
	deleteData();
	_type = type;
	switch (type)
	{
	case Number:
		_data.number = 0;
		break;
	case Boolean:
		_data.boolean = false;
		break;
	case String:
		_data.string = new BString();
		break;
	case List:
		_data.list = new NodeList();
		break;
	}
}

void BProfileNode::push(BString key, lua_Number number)
{
	changeType(BProfileNode::List);
	BProfileNode* node = new BProfileNode(key);
	node->_parent = this;
	node->set(number);
	_data.list->push_back(node);
}

void BProfileNode::push(BString key, bool boolean)
{
	changeType(BProfileNode::List);
	BProfileNode* node = new BProfileNode(key);
	node->_parent = this;
	node->set(boolean);
	_data.list->push_back(node);
}

void BProfileNode::push(BString key, const BString& string)
{
	changeType(BProfileNode::List);
	BProfileNode* node = new BProfileNode(key);
	node->_parent = this;
	node->set(string);
	_data.list->push_back(node);
}

void BProfileNode::push(BProfileNode* child)
{
	changeType(BProfileNode::List);
	if (child->_parent)
		child->_parent->_data.list->remove(child);
	child->_parent = this;
	_data.list->push_back(child);
}

void BProfileNode::push(BString key, BProfileNode* child)
{
	changeType(BProfileNode::List);
	if (child->_parent)
		child->_parent->_data.list->remove(child);
	child->_parent = this;
	child->_name = key;
	_data.list->push_back(child);
}

void BProfileNode::set(lua_Number number)
{
	changeType(BProfileNode::Number);
	_data.number = number;
}

void BProfileNode::set(bool boolean)
{
	changeType(BProfileNode::Boolean);
	_data.boolean = boolean;
}

void BProfileNode::set(const BString& string)
{
	changeType(BProfileNode::String);
	(*_data.string) = string;
}

void BProfileNode::setName(const BString& name)
{
	_name = name;
}

void BProfileNode::set(BProfileNode* node)
{
	changeType(node->_type);
	_data = node->_data;
	node->_type = BProfileNode::Number;
	if (_type == BProfileNode::List)
	{
		NodeList::iterator it = _data.list->begin();
		NodeList::iterator ite = _data.list->end();

		for (;it != ite; ++it)
			(*it)->_parent = this;
	}
}

void BProfileNode::pop(BString key)
{
	NodeList::iterator it = _data.list->begin();
	NodeList::iterator ite = _data.list->end();

	for (;it != ite; ++it)
	{
		if (((*it)->_name) == key)
		{
			delete (*it);
			return ;
		}
	}
}

void BProfileNode::popAll(BString key)
{
	NodeList::iterator it = _data.list->begin();
	NodeList::iterator ite = _data.list->end();
	NodeList::iterator tmp;

	for (;it != ite; it = tmp)
	{
		tmp = it;
		++tmp;
		if (((*it)->_name) == key)
		{
			delete (*it);
			return ;
		}
	}
}

BString BProfileNode::toString(int level)
{
	BString string;

	for (int i = 0; i < level; i++)
		string << "-";

	string << "<" << _name << ">";

	switch (_type)
	{
	case Number:
		string << " {Number: " << _data.number << "}";
		break;
	case Boolean:
		string << " {Boolean: " << _data.boolean << "}";
		break;
	case String:
		string << " {String: " << *_data.string << "}";
		break;
	case List:
		NodeList::iterator it = _data.list->begin();
		NodeList::iterator ite = _data.list->end();
		for (; it != ite; ++it)
			string << "\n" << (*it)->toString(level + 1);
		break;
	}
	return string;
}

void BProfileNode::popAll()
{
	emptyList(*_data.list);
}

bool BProfileNode::toType(NodeType type, void* data)
{
	if (type != _type)
		return false;

	if (data == NULL)
		return true;

	switch (_type)
	{
	case Number:
		(*reinterpret_cast<lua_Number*>(data)) = _data.number;
		break;
	case Boolean:
		(*reinterpret_cast<bool*>(data)) = _data.boolean;
		break;
	case String:
		(*reinterpret_cast<BString*>(data)) = *_data.string;
		break;
	case List:
		*reinterpret_cast<NodeList**>(data) = _data.list;
		break;
	}

	return true;
}

const BString& BProfileNode::getString() const
{
	return *_data.string;
}

lua_Number BProfileNode::toNumber() const
{
	return _data.number;
}

bool BProfileNode::toBoolean() const
{
	return _data.boolean;
}

BString BProfileNode::name() const
{
	return _name;
}

BProfileNode* BProfileNode::parent() const
{
	return _parent;
}

BProfileNode::NodeType BProfileNode::type() const
{
	return _type;
}

BProfileNode::NodeList BProfileNode::children(const BString& key) const
{
	if (_type != List)
		return NodeList();
	NodeList list;

	NodeList::iterator it = _data.list->begin();
	NodeList::iterator ite = _data.list->end();

	for (;it != ite; ++it)
	{
		if ((*it)->_name == key)
			list.push_back(*it);
	}

	return list;
}

BProfileNode::NodeList BProfileNode::children() const
{
	if (_type != List)
		return BProfileNode::NodeList();
	NodeList list;

	NodeList::iterator it = _data.list->begin();
	NodeList::iterator ite = _data.list->end();
	for (;it != ite; ++it)
	{
		list.push_back(*it);
	}

	return list;
}

BProfileNode* BProfileNode::child(const BString& key) const
{
	if (_type != List)
		return NULL;
	NodeList::iterator it = _data.list->begin();
	NodeList::iterator ite = _data.list->end();

	for (;it != ite; ++it)
	{
		if ((*it)->name() == key)
			return *it;
	}

	return NULL;
}



BProfileNode::~BProfileNode()
{
	if (_parent && _parent->type() == BProfileNode::List)
		_parent->_data.list->remove(this);
}

