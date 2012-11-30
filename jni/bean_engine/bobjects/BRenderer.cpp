//
//  BRenderer.cpp
//  BeanEngine
//
//  Created by Pierre POLLASTRI on 01/11/12.
//  Copyright (c) 2012 Pierre POLLASTRI. All rights reserved.
//

#include "BRenderer.h"
#include <resource_manager/BResourceManager.h>

#define CLASS BRenderer

B_DEFINE_CLASS("Renderer", BObject)

B_NO_FUNCTION

static int lua_no_func(lua_State* state);

B_METHODS_BEGIN
    B_METHOD(setColor)
    B_METHOD(setRect)
    B_METHOD(fillRect)
    B_METHOD(rotate)
    B_METHOD(translate)
    B_METHOD(scalerX)
    B_METHOD(scalerY)
    B_METHOD(pushMatrix)
    B_METHOD(popMatrix)
    B_METHOD(drawImage)
    B_METHOD(drawChar)
    {"__gc", lua_no_func},
B_METHODS_END

BRenderer::BRenderer(BCanvas* canvas) : canvas(canvas)
{
    
}

std::string BRenderer::toString() const {
    return "BRenderer";
}

bool BRenderer::release()
{
    Log::debug("RELEASE");
    return false;
}

B_IMPLEMENTATION(setColor)
    BColor color;
    int argc = lua_gettop(state) - 1;
    if (argc == 3)
        color = BColor(luaL_checkinteger(state, 2), luaL_checkinteger(state, 3),
                       luaL_checkinteger(state, 4));
    else if (argc == 4)
        color = BColor(luaL_checkinteger(state, 2), luaL_checkinteger(state, 3),
                       luaL_checkinteger(state, 4), luaL_checkinteger(state, 5));
    else
    {
        luaL_error(state, "setColor: invalid number of parameters, expects 4 got %i", argc);
        return 0;
    }
    self->canvas->setColor(color);
    return 0;
B_END

B_IMPLEMENTATION(setRect)
    BRect rect;
    int argc = lua_gettop(state) - 1;
    if (argc == 4)
        rect = BRect(luaL_checknumber(state, 2), luaL_checknumber(state, 3),
                     luaL_checknumber(state, 4), luaL_checknumber(state, 5));
    else
    {
       luaL_error(state, "setRect: invalid number of parameters, expects 4 got %i", argc);
        return 0;
    }
    self->canvas->setRect(rect);
    return 0;
B_END

B_IMPLEMENTATION(fillRect)
    self->canvas->fillRect();
    return 0;
B_END

B_IMPLEMENTATION(popMatrix)
    self->canvas->popMatrix();
    return 0;
B_END

B_IMPLEMENTATION(pushMatrix)
    self->canvas->pushMatrix();
    return 0;
B_END

B_IMPLEMENTATION(rotate)
    int argc = lua_gettop(state) - 1;
    if (argc == 1)
        self->canvas->rotate(luaL_checknumber(state, 2));
    else if (argc == 3)
        self->canvas->rotate(luaL_checknumber(state, 2), luaL_checknumber(state, 3),
                             luaL_checknumber(state, 4), luaL_checknumber(state, 5));
    else
        luaL_error(state, "no such method rotate with %i args", argc);
    return 0;
B_END

B_IMPLEMENTATION(translate)
    int argc = lua_gettop(state) - 1;
    if (argc == 2)
        self->canvas->translate(luaL_checknumber(state, 2), luaL_checknumber(state, 3));
    else if (argc == 4)
        self->canvas->rotate(luaL_checknumber(state, 2), luaL_checknumber(state, 3),
                             luaL_checknumber(state, 4), luaL_checknumber(state, 5));
    else
        luaL_error(state, "no such method translate with %i args", argc);
    return 0;
B_END

B_IMPLEMENTATION(scalerX)
    lua_pushnumber(state, self->canvas->scalerX());
    return 1;
B_END

B_IMPLEMENTATION(scalerY)
    lua_pushnumber(state, self->canvas->scalerY());
    return 1;
B_END

B_IMPLEMENTATION(drawImage)
    int argc = lua_gettop(state) - 1;
    BImage* image = NULL;
    BRect subrect;
    if (argc >= 1 && lua_isstring(state, 2))
        image = BResourceManager::instance()->getImage(luaL_checkstring(state, 2));
    if (argc == 5)
        subrect = BRect(luaL_checknumber(state, 3), luaL_checknumber(state, 4),
                        luaL_checknumber(state, 5), luaL_checknumber(state, 6));
    if (argc == 1 && image)
        subrect = BRect(0, 0, image->size().width(), image->size().height());
    if (image)
        self->canvas->drawImage(image, subrect);
    return 0;
B_END

B_IMPLEMENTATION(drawChar)
    BString c = luaL_checkstring(state, 2);
    BFont* font = BResourceManager::instance()->loadFont(luaL_checkstring(state, 3));
    if (font && c.length() > 0)
        self->canvas->drawChar(c.toUnicode()[0], font);
    return 0;
B_END


static int lua_no_func(lua_State* state)
{
    return 0;
}