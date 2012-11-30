/*
 * BString.cpp
 *
 *  Created on: 27 janv. 2012
 *      Author: Pierre Pollastri
 */

#include "BString.h"
#include <sstream>
#ifndef __IOS__
#   include <utf8.h>
#endif

BString::BString() : _unicode(NULL) {
	// TODO Auto-generated constructor stub

}

BString::BString(const char* str) : _unicode(NULL)
{
	assign(str);
}

BString::BString(const std::string& str) : _unicode(NULL)
{
	assign(str.c_str());
}

const BChar* BString::toUnicode()
{
#ifndef __IOS__
	if (_unicode)
		return _unicode;
	_unicodeSize = utf8_to_wchar(c_str(), size(), NULL, 0, 0);
	_unicode = new BChar[_unicodeSize];
	utf8_to_wchar(c_str(), size(), _unicode, _unicodeSize, 0);
	return _unicode;
#else
    return NULL;
#endif
}

size_t BString::unicodeSize()
{
	if (!_unicode)
		toUnicode();
	return _unicodeSize;
}

BString BString::toLowerCase() const
{
	BString out = *this;
	int d = 'A' - 'a';
	for (int i = 0; i < size(); i++)
	{
		if (out[i] >= 'A' && out[i] <= 'Z')
			out[i] = out[i] - d;
	}
	return out;
}

void BString::resetUnicode()
{
	delete [] _unicode;
	_unicode = NULL;
}

BString& BString::operator <<(const char* str)
{
	std::ostringstream oss;
	oss << *this << str;
	this->assign(oss.str());
	return *this;
}

BString& BString::operator <<(const std::string& str)
{
	std::ostringstream oss;
	oss << *this << str;
	this->assign(oss.str());
	return *this;
}

BString& BString::operator <<(const BString& str)
{
	std::ostringstream oss;
	oss << *this << str;
	this->assign(oss.str());
	return *this;
}

BString& BString::operator <<(int nbr)
{
	std::ostringstream oss;
	oss << *this << nbr;
	this->assign(oss.str());
	return *this;
}

BString& BString::operator <<(float nbr)
{
	std::ostringstream oss;
	oss << *this << nbr;
	this->assign(oss.str());
	return *this;
}

BString& BString::operator <<(double nbr)
{
	std::ostringstream oss;
	oss << *this << nbr;
	this->assign(oss.str());
	return *this;
}

BString& BString::operator <<(bool value)
{
	std::ostringstream oss;
	if (value)
		oss << *this << "true";
	else
		oss << *this << "false";
	this->assign(oss.str());
	return *this;
}


BString& BString::operator=(const std::string& str)
{
	assign(str.c_str());
    return *this;
}

BString& BString::operator=(const char* str)
{
	assign(str);
    return *this;
}

BString::~BString() {
	// TODO Auto-generated destructor stub
}
