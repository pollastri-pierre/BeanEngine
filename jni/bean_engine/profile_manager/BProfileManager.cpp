/*
 * BProfileManager.cpp
 *
 *  Created on: 28 juin 2012
 *      Author: NC
 */

#include "BProfileManager.h"
#include <application/Log.h>
#include <unistd.h>
#include <cstdlib>
#include <dirent.h>

BProfileManager::BProfileManager(const BString& directory) : _directory(directory)
{
    DIR *dp;
    struct dirent* entry;
    BString key;

    if((dp  = opendir(directory.c_str())) == NULL)
        return ;
    BProfileNode* root = NULL;
    BProfileNode* name = NULL;
    BString n;

    while ((entry = readdir(dp)) != NULL)
    {
    	if (entry->d_type == DT_REG)
    	{
    		root = new BProfileNode();
    		BString path = directory + "/" + BString(entry->d_name);
    		std::ifstream ifs(path.c_str());
    		if (ifs.is_open())
    		{
    			boost::archive::text_iarchive ar(ifs);
    			ar >> *root;
    			name = root->child("__profile.name");
    			if (name)
    			{
    		    	if (name->toType(BProfileNode::String, &n))
    		    	{
    		    		_profiles[n]._root = root;
    		    		_profiles[n]._filename = entry->d_name;
    		    	}
    		    	else
    		    	{
    		    		delete root;
    		    	}
    			}
    			else
    			{
    				delete root;
    			}
    		}
    		else
    			delete root;
    		ifs.close();
    	}
    }
    closedir(dp);
}

BProfileManager* BProfileManager::createProfileManager(const BString& directory)
{
	return new BProfileManager(directory);
}

void BProfileManager::createProfile(const BString& profile, BProfileNode* root)
{
	if (_profiles.find(profile) != _profiles.end())
		return;
	Log::debug("DIR = %s", _directory.c_str());
	_profiles[profile]._root = root;
	std::ofstream out;
	BString filename;
	int t = 0;

	BString path;

	do
	{
		filename.clear();
		filename << "save_" << t << ".bprofile";
		path = _directory + "/" + filename;
		t++;

	} while (::access(path.c_str(), F_OK) == 0);
	_profiles[profile]._filename = filename;
	out.open(path.c_str());
	boost::archive::text_oarchive ar(out);
	ar << *root;
	out.close();
}

/*int wctomb( char * pmb, wchar_t character )
{
	return ::wctomb(pmb, character);
}*/

BProfileNode* BProfileManager::open(const BString& profile)
{
	if (_profiles.find(profile) != _profiles.end())
		return _profiles[profile]._root;
	return NULL;
}

void BProfileManager::save(const BString& profile)
{
	if (_profiles.find(profile) == _profiles.end())
		return ;
	BString path = _directory + "/" + _profiles[profile]._filename;
	std::ofstream out;
	out.open(path.c_str(), std::ios::trunc | std::ios::out);
	boost::archive::text_oarchive ar(out);
	ar << *_profiles[profile]._root;
	out.close();
}

void BProfileManager::deleteProfile(const BString& profile)
{
	if (_profiles.find(profile) == _profiles.end())
		return;
	BString path = _directory + "/" + _profiles[profile]._filename;
	if (::remove(path.c_str()) == -1)
		Log::error("Cannot remove profile file <%s>", profile.c_str());
}

int BProfileManager::getProfileCount()
{
	return _profiles.size();
}

BStringVector BProfileManager::getProfileNames()
{
	BStringVector names;
	BProfileMap::iterator it = _profiles.begin();
	BProfileMap::iterator ite = _profiles.end();
	for (; it != ite; ++it)
		names.push_back(it->first);
	return names;
}

BProfileManager::~BProfileManager()
{
	// TODO Auto-generated destructor stub
}

