/*
 * BResourceManager.h
 *
 *  Created on: 22 mars 2012
 *      Author: NC
 */

#ifndef BRESOURCEMANAGER_H_
#define BRESOURCEMANAGER_H_

#include <application/BString.h>
#include <graphic_engine/BFont.h>
#include <resource_manager/BImage.h>
#include <resource_manager/BSound.h>
#include <map>
#include "BConfigLoader.h"

typedef std::map<BString, BFont*> BFontMap;
typedef std::map<BString, BImage*> BImageMap;
typedef std::map<BString, BSound*> BSoundMap;

class BResourceManager {

private:
	static BResourceManager* _instance;

	BFontMap 	_fonts;
	BImageMap 	_images;
	BSoundMap	_sounds;

	// Variable de chargement des resources

	typedef int ResourceType;

	const static ResourceType IMAGE = 0;
	const static ResourceType SOUND_BGM = 1;
	const static ResourceType SOUND_EFFECT = 2;
	const static ResourceType FONT = 3;
	const static ResourceType END = 4;

	ResourceType _type;
	int _total;
	int _loaded;
	int _index;
	bool _newConfFile;

	BResourceMap::iterator _current;
	BResourceMap::iterator _end;


public:
	static BResourceManager* instance();
	void destroy();
	BFont* loadFont(const BString& name, const BString& path = "", unsigned int size = 0);
	void unloadFont(const BString& name);
	BImage* loadImage(const BString& name, const BString& path = "");
	BSound* loadSound(const BRRawData& data);
	BSound* getSound(const BString& name);
	BImage* getImage(const BString& name);
	void unloadImage(const BString& name);
	void reload();
	void setNewConfFile(bool value);
	bool loadResources(int nbr);
    int resourcesCount() const;
    int loadedResourcesCount() const;
private:
	bool initIterators();
	void unloadResources();
	void unloadResources(BResourceTree& tree);
	BResourceManager();
	~BResourceManager();

};

#endif /* BRESOURCEMANAGER_H_ */
