/*
 * BString.h
 *
 *  Created on: 27 janv. 2012
 *      Author: NC
 */

#ifndef BSTRING_H_
#define BSTRING_H_

#include <wchar.h>
#include <string>

typedef std::basic_string<wchar_t> wstring;
typedef wchar_t BChar;

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/string.hpp>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

class BString : public std::string {

private:
	BChar* _unicode;
	size_t _unicodeSize;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<std::string>(*this);
	}

public:
	BString();
	BString(const char* str);
	BString(const std::string& str);
	BString& operator<<(const std::string& str);
	BString& operator<<(const char* str);
	BString& operator<<(const BString& str);
	BString& operator<<(int nbr);
	BString& operator<<(double nbr);
	BString& operator<<(float nbr);
	BString& operator<<(bool value);
	BString& operator=(const std::string& str);
	BString& operator=(const char* str);
	const BChar* toUnicode();
	BString toLowerCase() const;
	size_t	unicodeSize();
	void resetUnicode();
	virtual ~BString();
};

#endif /* BSTRING_H_ */
