//
//  SFMLEventManager.h
//  BeanEngine
//
//  Created by Pierre POLLASTRI on 20/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef BeanEngine_SFMLEventManager_h
#define BeanEngine_SFMLEventManager_h

#include <SFML/Window.hpp>
#include "BEventManager.h"
#include "BTouchEvents.h"

class SFMLEventManager : public BEventManager
{
private:
    BTouchEvents    _events;
    bool            _down;
    
public:
    SFMLEventManager();
    virtual BTouchEvents getEvents();
    void processTouchEvents(sf::Event& event);
};

#endif
