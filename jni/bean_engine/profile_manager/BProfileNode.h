/*
 * BProfileNode.h
 *
 *  Created on: 28 juin 2012
 *      Author: NC
 */

#ifndef BPROFILENODE_H_
#define BPROFILENODE_H_

#include <application/BString.h>
#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <lua.h>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <application/Log.h>

class BProfileNode
{
public:
	typedef std::list<BProfileNode*> NodeList;
	enum NodeType
	{
		Number = 7,
		Boolean = 1,
		String = 16,
		List = 42
	};

private:
	union __data_t
	{
		lua_Number 					number;
		bool						boolean;
		BString*					string;
		NodeList*					list;
	};

	friend class boost::serialization::access;

	BString			_name;
	NodeType		_type;
	BProfileNode* 	_parent;
	__data_t 		_data;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & _name;
		NodeType type = _type;
		ar & _type;
		switch (_type)
		{
		case Number:
			ar & _data.number;
			break;
		case Boolean:
			ar & _data.boolean;
			break;
		case String:
			if (_type != type)
				_data.string = new BString();
			ar & (*_data.string);
			break;
		case List:
			if (_type != type)
				_data.list = new NodeList();
			ar & *_data.list;

			NodeList::iterator it = _data.list->begin();
			NodeList::iterator ite = _data.list->end();

			for (; it != ite; ++it)
			{
				(*it)->_parent = this;
			}

			break;
		}
	}

public:
	BProfileNode();
	BProfileNode(const BString& name);
	void 			push(BString key, lua_Number number);
	void 			push(BString key, bool boolean);
	void 			push(BString key, const BString& string);
	void 			push(BString key, BProfileNode* child);
	void 			push(BProfileNode* child);
	void 			set(lua_Number number);
	void 			set(bool boolean);
	void 			set(const BString& string);
	void 			set(BProfileNode* node);
	void 			pop(BString key);
	void 			popAll(BString key);
	void 			popAll();
	bool 			toType(NodeType type, void* data);
	void			setName(const BString& name);
	BString 		name() const;
	BProfileNode* 	parent() const;
	NodeType		type() const;
	NodeList		children(const BString& key) const;
	NodeList		children() const;
	BProfileNode*	child(const BString& key) const;
	BString			toString(int level);
	const BString&	getString() const;
	lua_Number		toNumber() const;
	bool 			toBoolean() const;
	virtual 		~BProfileNode();

private:
	void changeType(NodeType type);
	void deleteData();
	void emptyList(NodeList& list);
};

#endif /* BPROFILENODE_H_ */
