//
//  BView.cpp
//  BeanEngine
//
//  Created by Pierre POLLASTRI on 27/10/12.
//  Copyright (c) 2012 Pierre POLLASTRI. All rights reserved.
//

#include "BView.h"
#include <bobjects/BRenderer.h>
#include <game_engine/BGameEngine.h>
#define CLASS BView

std::queue<int> BView::FREEREF_LIST;
int BView::LAST_REF = -1;

B_DEFINE_CLASS("View", BGraphicObject)

B_NO_FUNCTION

B_METHODS_BEGIN
    B_METHOD(registerCallback)
    B_METHOD(unregisterCallback)
B_METHODS_END

static const char* CALLBACK_KEY[] = {
        "onTouch",
        "onDraw"
};

BView::BView()
{
    _state = BGameEngine::instance()->L();
    setUserInteractionEnabled(true);
    unref();
}

bool BView::processSingleEvent(const BTouchEvent *event)
{
    if (_refs[TOUCH_CALLBACK] != -1) {
        int size = (int)unpackTable(TOUCH_CALLBACK);
        if (size > 0) {
            lua_pushinteger(_state, event->type());
            lua_pushnumber(_state, event->x());
            lua_pushnumber(_state, event->y());
            lua_pushinteger(_state, event->index());
            size += 4;
            lua_call(_state, size - 1, 1);
            if (lua_isboolean(_state, -1))
                return lua_toboolean(_state, -1);
            }
    }
    return false;
}

size_t BView::unpackTable(BView::Key key) const
{
    size_t size = 0;
    if (_refs[key] == -1)
        return 0;
    lua_getglobal(_state, REGISTER_NAME);
    lua_rawgeti(_state, -1, _refs[key]);
    int table = lua_gettop(_state);
    if (!lua_istable(_state, table))
        return 0;
    lua_pushnil(_state);
    for (; lua_next(_state, table) != 0; size++) {
        lua_pushvalue(_state, -2);
        lua_remove(_state, -3);
    }
    return size;
}

static int getCallbackKeyIndex(const BString& key)
{
    size_t size = sizeof(CALLBACK_KEY) / sizeof(const char*);
    for (int i = 0; i < size; i++)
    {
        if (strcmp(CALLBACK_KEY[i], key.c_str()) == 0) {
            return i;
        }
    }
    return -1;
}

void BView::registerCallback(const BString &key, int ref)
{
    int index = getCallbackKeyIndex(key);
    if (index != -1 && _refs[index] != -1)
        luaL_unref(_state, LUA_REGISTRYINDEX, _refs[index]);
    if (index != -1)
        _refs[index] = ref;
    else
        luaL_unref(_state, LUA_REGISTRYINDEX, ref);
}

void BView::unregisterCallback(const BString &key)
{
    int index = getCallbackKeyIndex(key);
    if (index != -1)
    {
        lua_unref_table(_state, _refs[index]);
        _refs[index] = -1;
    }
}

void BView::onDraw(BCanvas *const canvas, const BCanvasContext &context)
{
    float tX = context.rect.x() + _rotationCenter.x() * canvas->scalerX();
	float tY = context.rect.y() + _rotationCenter.y() * canvas->scalerY();
	BColor c = backgroundColor();
	c.setAlpha(c.alpha() * context.opacity);
	canvas->pushMatrix();
	canvas->translate(tX, tY);
	canvas->rotate(_rotation);
	canvas->translate(-tX, -tY);
	canvas->setRect(context.rect);
    BRenderer renderer(canvas);
    if (_refs[DRAW_CALLBACK] != -1) {
        int size = (int)unpackTable(DRAW_CALLBACK);
        if (size > 0) {
            BClass<BRenderer>::newInstance(_state, &renderer);
            lua_pushnumber(_state, context.rect.x());
            lua_pushnumber(_state, context.rect.y());
            lua_pushnumber(_state, context.rect.width());
            lua_pushnumber(_state, context.rect.height());
            lua_pushnumber(_state, context.opacity);
            size += 6;
            //luaL_callk(_state, size - 1, 0, 0);
            lua_call(_state, size - 1, 0);
        }
    }
    canvas->popMatrix();
}

int BView::lua_ref_table(lua_State* L)
{
    int ref = -1;
    int table = lua_gettop(L);
    
    if (LAST_REF == -1)
    {
        lua_newtable(L);
        lua_setglobal(L, REGISTER_NAME);
        LAST_REF = 1;
        FREEREF_LIST = std::queue<int>();
    }
    
    if (FREEREF_LIST.size() > 0) {
        ref = FREEREF_LIST.front();
        FREEREF_LIST.pop();
    }
    
    lua_getglobal(L, REGISTER_NAME);
    if (!lua_istable(L, -1)) {
        lua_pop(L, 1);
        LAST_REF = -1;
        return lua_ref_table(L);
    }
    ref = (ref > 0) ? ref : LAST_REF++;
    lua_pushinteger(L, ref);
    lua_pushvalue(L, table);
    lua_rawset(L, -3);
    return ref;
}

void BView::lua_unref_table(lua_State* L, int idx)
{
    /*lua_getglobal(L, REGISTER_NAME);
    if (lua_istable(L, -1) && idx > 0)
    {
        FREEREF_LIST.push(idx);
        lua_pushnil(L);
        lua_rawseti(L, -1, idx);
    }*/
}

void BView::resetCallbackRegistry()
{
    LAST_REF = -1;
    while (FREEREF_LIST.size() > 0)
        FREEREF_LIST.pop();
}

void BView::unref()
{
    size_t size = sizeof(_refs) / sizeof(int);
    for (int i = 0; i < size; i++) {
        if (_refs[i] != -1)
            lua_unref_table(_state, _refs[i]);
            //luaL_unref(_state, LUA_REGISTRYINDEX, _refs[i]);
        _refs[i] = -1;
    }
}

BView::~BView()
{
    unref();
}

B_IMPLEMENTATION(registerCallback)
    if (lua_isstring(state, 2) && lua_istable(state, 3))
    {
        BString key = lua_tostring(state, 2);
        int ref = lua_ref_table(state);//luaL_ref(state, LUA_REGISTRYINDEX);
         Log::debug("REF FOUND %i", ref);
        self->registerCallback(key, ref);
    }
    else
    {
        Log::debug("NO REF FOUND");
    }
    return 0;
B_END

B_IMPLEMENTATION(unregisterCallback)
    if (lua_isstring(state, 2))
    {
        BString key = lua_tostring(state, 2);
        self->unregisterCallback(key);
    }
    return 0;
B_END

