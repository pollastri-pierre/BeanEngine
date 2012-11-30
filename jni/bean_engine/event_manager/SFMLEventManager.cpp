//
//  SFMLEventManager.cpp
//  BeanEngine
//
//  Created by Pierre POLLASTRI on 20/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "SFMLEventManager.h"
#include <application/Log.h>

SFMLEventManager::SFMLEventManager()
{
    _down = false;
}

BTouchEvents SFMLEventManager::getEvents()
{
    BTouchEvents events = _events;
    _events.clear();
    return events;
}

void SFMLEventManager::processTouchEvents(sf::Event &event)
{
    int index = 0;
    lua_Number x, y = 0;
    BTouchEvent::Type type;
    if (event.type == sf::Event::MouseButtonPressed && _down == false && event.mouseButton.button == sf::Mouse::Left)
    {
        _down = true;
        type = BTouchEvent::DOWN;
        x = event.mouseButton.x;
        y = event.mouseButton.y;
        goto event_ok;
    }
    else if (event.type == sf::Event::MouseButtonReleased && _down == true && event.mouseButton.button == sf::Mouse::Left)
    {
        _down = false;
        type = BTouchEvent::UP;
        x = event.mouseButton.x;
        y = event.mouseButton.y;
        goto event_ok;
    }
    else if (event.type == sf::Event::MouseMoved && _down == true)
    {
        type = BTouchEvent::MOVE;
        x = event.mouseMove.x;
        y = event.mouseMove.y;
        goto event_ok;
    }
    
    return;
event_ok:
    _events.add(x, y, index, type);
    return ;
}