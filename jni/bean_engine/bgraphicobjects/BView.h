//
//  BView.h
//  BeanEngine
//
//  Created by Pierre POLLASTRI on 27/10/12.
//  Copyright (c) 2012 Pierre POLLASTRI. All rights reserved.
//

#ifndef __BeanEngine__BView__
#define __BeanEngine__BView__

#include "BGraphicObject.h"
#include <queue>
#define REGISTER_NAME "BeanEngineCallbacksRegistry"

class BView : public BGraphicObject {
    
private:
    
    enum Key {
        TOUCH_CALLBACK = 0,
        DRAW_CALLBACK = 1
    };

    
    int         _refs[2];
    lua_State*  _state;

    static std::queue<int> FREEREF_LIST;
    static int LAST_REF;
    
public:
    BView();
	virtual bool processSingleEvent(const BTouchEvent* event);
    virtual void registerCallback(const BString& key, int ref);
    virtual void unregisterCallback(const BString& key);
    virtual void unref();
    virtual void onDraw(BCanvas * const canvas, const BCanvasContext& context);
    virtual size_t unpackTable(Key key) const;
    virtual ~BView();
    B_DECLARATION(registerCallback)
    B_DECLARATION(unregisterCallback)
    
private:
    static int lua_ref_table(lua_State* L);
    static void lua_unref_table(lua_State* L, int idx);

public:
    static void resetCallbackRegistry();
};

#endif /* defined(__BeanEngine__BView__) */
