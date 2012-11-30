//
//  SFMLApplication.h
//  BeanEngine
//
//  Created by Pierre POLLASTRI on 23/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef BeanEngine_SFMLApplication_h
#define BeanEngine_SFMLApplication_h

#include "BApplication.h"
#include <graphic_engine/SFMLGraphicEngine.h>
#include <sound_engine/BSoundEngine.h>
#include <event_manager/BEventManager.h>
#include <event_manager/SFMLEventManager.h>
#include <game_engine/BGameEngine.h>

#include <SFML/Graphics.hpp>
#include "ResourcePath.hpp"
#include <profile_manager/BProfileManager.h>
#include <network_engine/BNetworkWrapper.h>

#include <map>

class SFMLApplication : public BApplication {
    
public:
    struct Context {
        bool        debugMode;
        int         screenWidth;
        int         screenHeight;
        BString     path;
        BString     scriptsDir;
        BString     script;
        BString     appName;
        BString     filesystem;
    };
    
private:
    
    typedef std::list<std::pair<BString, BString > > BScriptList;
    
    // Engines
    BGameEngine*            _gameEngine;
    BGraphicEngine*         _graphicEngine;
    BSoundEngine*           _soundEngine;
    SFMLEventManager*       _eventManager;
    BProfileManager*        _profileManager;
    
    //SFML
    sf::RenderWindow*       _window;
    
    // App
    Context*                _context;
    
    //
    bool                    _run;
    bool                    _open;
    
    BScriptList             _scripts;
    BString                 _firstScript;
    
    int         _framerate;

    
public:
    SFMLApplication(Context* context);
    virtual void run();
	virtual void onFocus(bool focus);
	virtual void onDestroy();
	virtual void onStart();
	virtual void onPause();
	virtual void onResume();
	virtual void onStop();
	virtual void onDestroyWindow();
	virtual void onCreateWindow();
	virtual void onLowMemory();
	virtual bool processEvents();
	virtual void stepApplication();
    virtual void exit();
    virtual const Context* context() const;
    virtual void notifyCurrentScript(const BString& path);
private:
    void createFilesystem();
};

#endif
