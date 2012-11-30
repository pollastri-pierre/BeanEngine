/*
 * BResourceManager.cpp
 *
 *  Created on: 22 mars 2012
 *      Author: NC
 */

#include "BResourceManager.h"
#include <sound_engine/BSoundBGM.h>
#include <sound_engine/BSoundEffect.h>

BResourceManager::BResourceManager() {
	// TODO Auto-generated constructor stub
	setNewConfFile(true);
}

BResourceManager* BResourceManager::_instance = NULL;

BResourceManager* BResourceManager::instance()
{
	return (_instance != NULL) ? (_instance) : (_instance = new BResourceManager());
}

void BResourceManager::destroy()
{
	delete _instance;
}

BFont* BResourceManager::loadFont(const BString& name, const BString& path, unsigned int size)
{
	if (this->_fonts.find(name) != _fonts.end())
	{
		//Log::debug("GET FONT");
		return _fonts[name];
	}

	if (size == 0)
		return NULL;

	BFont* font;

	font = BFont::create(name, path);
	font->load(size);
	_fonts[name] = font;
	//Log::debug("LOAD FONT <%s>", name.c_str());
	return font;
}

BImage* BResourceManager::loadImage(const BString& name, const BString& path)
{
	if (this->_images.find(name) != _images.end())
	{
		return _images[name];
	}

	if (path == "")
		return NULL;

	BImage* image;

	image = BImage::create(name, path);
	_images[name] = image;
	image->load(path);
	return image;
}

BSound*	BResourceManager::loadSound(const BRRawData& data)
{
	if (_sounds.find(data.path) != _sounds.end())
	{
		return _sounds[data.path];
	}


	BSound* sound = NULL;

	if (data.other.soundType == BRRawData::SOUND_BGM)
	{
		sound = new BSoundBGM(data.name, data.path);
		sound->load(data.path);
	}
	else
	{
		sound = new BSoundEffect(data.path, data.path);
		sound->load(data.path);
	}

	_sounds[data.path] = sound;

	return sound;
}



BSound* BResourceManager::getSound(const BString& name)
{
	const int nbr = 2;
	const char* table[] = {"bgms", "effects"};

	BString n;
	BResourceTree& tree = BConfigLoader::instance()->conf();

	for (int i = 0; i < nbr; i++)
	{
		if (tree.find(table[i]) != tree.end() &&
			boost::get<BResourceMap>(tree.find(table[i])->second).find(name) != boost::get<BResourceMap>(tree.find(table[i])->second).end())
		{
			n = boost::get<BRRawData>(boost::get<BResourceMap>(tree.find(table[i])->second)[name]).path;
		}
		else continue;

		//Log::debug("%s Sound path is %s", name.c_str(), n.c_str());

		if (this->_sounds.find(n) != _sounds.end())
		{
			return _sounds[n];
		}
		Log::debug("CA PLANTE LA BGM");
	}
	return NULL;
}

bool BResourceManager::initIterators()
{
	if (_type >= END || _loaded >= _total)
		return false;
	//Log::debug("LOAD RESOURCES");
	BResourceTree& tree = BConfigLoader::instance()->conf();
	if (_type == IMAGE && tree.find("images") != tree.end())
	{
		_current = boost::get<BResourceMap>(tree["images"]).begin();
		_end = boost::get<BResourceMap>(tree["images"]).end();
	}
	else if (_type == SOUND_BGM && tree.find("bgms") != tree.end())
	{
		_current = boost::get<BResourceMap>(tree["bgms"]).begin();
		_end = boost::get<BResourceMap>(tree["bgms"]).end();
	}
	else if (_type == SOUND_EFFECT && tree.find("effects") != tree.end())
	{
		_current = boost::get<BResourceMap>(tree["effects"]).begin();
		_end = boost::get<BResourceMap>(tree["effects"]).end();
	}
	else if (_type == FONT && tree.find("fonts") != tree.end())
	{
		_current = boost::get<BResourceMap>(tree["fonts"]).begin();
		_end = boost::get<BResourceMap>(tree["fonts"]).end();
	}

	return true;
}

void BResourceManager::unloadResources()
{
	{
		BImageMap::iterator it = _images.begin();
		BImageMap::iterator ite = _images.end();

		for (; it != ite; ++it)
		{
			it->second->unload();
		}

	}
}

void BResourceManager::unloadResources(BResourceTree& tree)
{
	const char* keys[] = {"images", "bgms", "effects", "fonts", NULL};
	const int	table[]  = {IMAGE, SOUND_BGM, SOUND_BGM, FONT};

	for (int i = 0; keys[i]; i++)
	{
		BResourceMap::iterator _it = boost::get<BResourceMap>((tree)[keys[i]]).begin();
		BResourceMap::iterator _ite = boost::get<BResourceMap>((tree)[keys[i]]).end();

		for (; _it != _ite; ++_it)
		{

			switch (table[i])
			{
			case IMAGE:
				_images[boost::get<BRRawData>(_it->second).path]->unload();
				delete _images[boost::get<BRRawData>(_it->second).path];
				_images.erase(boost::get<BRRawData>(_it->second).path);
				Log::debug("Unload image <%s>", boost::get<BRRawData>(_it->second).path.c_str());
				break;
			case SOUND_BGM:
				_sounds[boost::get<BRRawData>(_it->second).path]->unload();
				delete _sounds[boost::get<BRRawData>(_it->second).path];
				_sounds.erase(boost::get<BRRawData>(_it->second).path);
				Log::debug("Unload sound <%s>", boost::get<BRRawData>(_it->second).path.c_str());
				break;
			case FONT:
				_fonts[boost::get<BRRawData>(_it->second).name]->unload();
				delete _fonts[boost::get<BRRawData>(_it->second).name];
				_fonts.erase(boost::get<BRRawData>(_it->second).name);
				Log::debug("Unload font <%s>", boost::get<BRRawData>(_it->second).name.c_str());
				break;
			}

		}

	}

}

bool BResourceManager::loadResources(int nbr)
{
	if (_newConfFile == false && _loaded == _total)
		return false;
	//Log::debug("DEBUT LOAD NEW_CONF = %i LOADED = %i/%i", _newConfFile, _loaded, _total);
	if (_index == 0 )
	{
		_total = boost::get<int>(BConfigLoader::instance()->conf()["resources_count"]);
        unloadResources(BConfigLoader::instance()->confToDelete());
		initIterators();
		_index = 1;
	}
	for (; _current != _end && nbr > 0; nbr--, ++_current)
	{
		switch (_type)
		{
		case IMAGE:
		{
			BRRawData data = boost::get<BRRawData>(_current->second);
			loadImage(data.path, data.path);
		}
		break;
		case FONT:
		{
			BRRawData data = boost::get<BRRawData>(_current->second);
			loadFont(data.name, data.path, data.other.fontsize);
		}
		break;
		case SOUND_BGM:
		{
			BRRawData data = boost::get<BRRawData>(_current->second);
			loadSound(data);
		}
		break;
		case SOUND_EFFECT:
		{
			BRRawData data = boost::get<BRRawData>(_current->second);
			loadSound(data);
		}
		break;
		}
		_loaded++;
	}

	//Log::debug("After for Loaded = %i, Total = %i", _loaded, _total);

	if (_loaded < _total && nbr > 0)
	{
		_type += 1;
		if (initIterators())
			loadResources(nbr);
		//return 0;
	}
	_newConfFile = _loaded < _total;

	//Log::debug("Loaded = %i, Total = %i", _loaded, _total);

	return _loaded < _total;
}

BImage* BResourceManager::getImage(const BString& name)
{
	BString n;
    if (!BConfigLoader::instance()->isValid())
        return NULL;
	BResourceTree& tree = BConfigLoader::instance()->conf();
	if (tree.find("images") != tree.end() &&
		boost::get<BResourceMap>(tree.find("images")->second).find(name) != boost::get<BResourceMap>(tree.find("images")->second).end())
	{
		n = boost::get<BRRawData>(boost::get<BResourceMap>(tree.find("images")->second)[name]).path;
	}
	else return NULL;

	//Log::debug("%s Image path is %s", name.c_str(), n.c_str());

	if (this->_images.find(n) != _images.end())
	{
		return _images[n];
	}

	return NULL;
}

void BResourceManager::reload()
{
	{
		BFontMap::iterator it = _fonts.begin();
		BFontMap::iterator ite = _fonts.end();

		for (; it != ite; ++it)
		{
			it->second->load(it->second->size());
		}

	}

	{
		BImageMap::iterator it = _images.begin();
		BImageMap::iterator ite = _images.end();

		for (; it != ite; ++it)
		{
			it->second->load(it->first);
		}
	}
}

void BResourceManager::setNewConfFile(bool value)
{
	_newConfFile = value;
	if (value)
	{
		_index = 0;
		_total = 0;
		_loaded = 0;
		_type = IMAGE;
	}
}

void BResourceManager::unloadFont(const BString& name)
{
	if (this->_fonts.find(name) != _fonts.end())
	{
		delete _fonts[name];
		_fonts.erase(name);
	}
}

void BResourceManager::unloadImage(const BString& name)
{
	if (this->_images.find(name) != _images.end())
	{
		delete _images[name];
		_images.erase(name);
	}
}

int BResourceManager::resourcesCount() const
{
    return _total;
}

int BResourceManager::loadedResourcesCount() const
{
    return _loaded;
}

BResourceManager::~BResourceManager() {
	// TODO Auto-generated destructor stub
}
