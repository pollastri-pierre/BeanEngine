/*
 * BAutoupdateObject.h
 *
 *  Created on: 29 mars 2012
 *      Author: NC
 */

#ifndef BAUTOUPDATEOBJECT_H_
#define BAUTOUPDATEOBJECT_H_

#include <list>

class BAutoupdateObject {

private:
	typedef std::list<BAutoupdateObject*> ObjectList;
	int _curFrame;
	int _frameToUpdate;
	bool _unregister;
	static ObjectList _objects;

public:
	BAutoupdateObject();
	void unregisterAtTheEnd(bool value);
	void registerObject();
	void unregisterObject();
	int frameToUpdate() const;
	static void updateObjects();
	virtual bool update();
	virtual void setFrameToUpdate(int frame);
	virtual bool onUpdate() = 0;
	virtual void onDelete() = 0;
	~BAutoupdateObject();
};

#endif /* BAUTOUPDATEOBJECT_H_ */
