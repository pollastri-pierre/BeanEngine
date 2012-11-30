/*
 * BSound.cpp
 *
 *  Created on: 11 juin 2012
 *      Author: NC
 */

#include "BSound.h"

BSound::BSound(const BString& name, const BString& path) {

	_name = name;
	_path = path;

}

BString BSound::path() const { return _path; }

BString BSound::name() const { return  _name; }
