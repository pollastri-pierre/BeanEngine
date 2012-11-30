/*
 * BProfileManager.h
 *
 *  Created on: 28 juin 2012
 *      Author: Pierre Pollastri
 */

#ifndef BPROFILEMANAGER_H_
#define BPROFILEMANAGER_H_

#include <application/BString.h>
#include <application/BApplication.h>
#include "BProfileNode.h"
#include <vector>
#include <map>
#include <fstream>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

typedef std::vector<BString> BStringVector;
//int wctomb ( char * pmb, wchar_t character );
class BProfileManager
{

	struct ProfileData
	{
		BString			_filename;
		BProfileNode* 	_root;
	};

	typedef std::map<BString, BProfileManager::ProfileData> BProfileMap;

	BProfileMap	_profiles;
	BString		_directory;

public:
	static BProfileManager* createProfileManager(const BString& directory);
	void					createProfile(const BString& profile, BProfileNode* root);
	BProfileNode* 			open(const BString& profile);
	void 					save(const BString& profile);
	void 					deleteProfile(const BString& profile);
	int 					getProfileCount();
	BStringVector			getProfileNames();
	virtual 				~BProfileManager();

private:
	BProfileManager(const BString& directory);
};

#endif /* BPROFILEMANAGER_H_ */
