/*
 * BConfigLoader.h
 *
 *  Created on: 19 avr. 2012
 *      Author: NC
 */

#ifndef BCONFIGLOADER_H_
#define BCONFIGLOADER_H_

#include <vector>
#include <map>
#include <boost/variant.hpp>
#include <application/BString.h>
#include <resource_manager/BResourcesTypes.h>
//#include <resource_manager/BResourceManager.h>

typedef boost::variant<BRRawData, int> BResourceData;
typedef std::map<BString, BResourceData> BResourceMap;
typedef	boost::variant<int, BResourceMap, bool> BResourceNode;
typedef std::map<BString, BResourceNode > BResourceTree;

typedef std::map<BString, std::string > 	BStyle;
typedef std::map<BString, BStyle > 		BStyleTree;

class BConfigLoader {

private:
	static BConfigLoader* 	_instance;
	BString 				_currentFile;
	BResourceTree* 			_tree;
	BResourceTree*			_toDelete;
	BStyleTree* 			_styles;
    bool                    _isValid;

public:
	static BConfigLoader* instance();
	bool parse(BString file);
	BResourceTree& conf();
	BResourceTree& confToDelete();
	BStyleTree& styles();
    bool isValid() const;
	virtual ~BConfigLoader();

private:
	BConfigLoader();
	void parseBuffer(const char* buffer);
	void createNewTree(BResourceTree* tree);
};

#endif /* BCONFIGLOADER_H_ */
