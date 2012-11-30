//
//  SFMLApplication.cpp
//  BeanEngine
//
//  Created by Pierre POLLASTRI on 23/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "SFMLApplication.h"
#include <SFML/Window.hpp>
#include <boost/range.hpp>

#include <dirent.h>

//#include <boost/algorithm/string.hpp>
#include <string>
//#include <boost/algorithm/find.hpp>
#include <boost/algorithm/string/find_iterator.hpp>

static void normalizePath(BString& path)
{
    if ((path)[path.length()] != '/')
        path += "/";
}

SFMLApplication::SFMLApplication(Context* context)
: _context(context)
{
    _gameEngine = NULL;
    _graphicEngine = NULL;
    _profileManager = NULL;
    _eventManager = NULL;
    _framerate = 60;
    _window = NULL;
    normalizePath(context->path);
    normalizePath(context->filesystem);
    createFilesystem();
}

void SFMLApplication::run()
{
    setIndependentMode(false);
    _run = true;
    _open = true;
    _firstScript = _context->script;
    do
    {
        if (_run)
        {
            onStart();
            onCreateWindow();
            _window->setVerticalSyncEnabled(false);
            _window->setFramerateLimit(0);
            _run = false;
            _gameEngine->setFramerate(_framerate);
            (*_gameEngine)();
            _window->setFramerateLimit(25);
        }
        else
        {
            _window->clear();
            processEvents();
            _window->display();
        }

    } while (_open && _context->debugMode);
    
    onDestroyWindow();
    onStop();
    onDestroy();
}

void SFMLApplication::onStop()
{
    
}

void SFMLApplication::onStart()
{
    delete _profileManager;
    _eventManager = new SFMLEventManager();
    _soundEngine = BSoundEngine::instance();
    BNetworkWrapper::instance();
    _profileManager = BProfileManager::createProfileManager(_dirMap[B_PROFILES_DIR].path());
    _dirMap[B_SCRIPTS_DIR].setPath(_context->scriptsDir);
    _dirMap[B_RESOURCES_DIR].setPath(_context->path);
    _gameEngine = BGameEngine::createInstance(this);
    _gameEngine->setProfileManager(_profileManager);
    _gameEngine->setCurrentScript(_context->script);
    _gameEngine->setEventManager(_eventManager);
}

void SFMLApplication::onResume()
{
    
}

void SFMLApplication::onCreateWindow()
{
    delete _graphicEngine;
    if (_window == NULL)
    {
        sf::ContextSettings settings;
        settings.depthBits = 24;
        settings.stencilBits = 8;
        settings.antialiasingLevel = 8;
        settings.majorVersion = 8;
        settings.minorVersion = 0;
        _window =  new sf::RenderWindow(sf::VideoMode(_context->screenWidth, _context->screenHeight), _context->appName.c_str(), sf::Style::Fullscreen, settings);
    }
    _graphicEngine = new SFMLGraphicEngine(_window);
    _gameEngine->setGraphicEngine(_graphicEngine);
}

void SFMLApplication::onDestroyWindow()
{
    
}

void SFMLApplication::onFocus(bool focus)
{
    
}

void SFMLApplication::onPause()
{
    
}

void SFMLApplication::onLowMemory()
{
    
}

bool SFMLApplication::processEvents()
{
    bool value = true;
    sf::Event event;
    if (_gameEngine)
    {
        _framerate = _gameEngine->framerate();
    }
    while (_window->pollEvent(event))
    {
        // Close window : exit
        if (event.type == sf::Event::Closed)
        {
            _window->close();
            value = false;
            _open = false;
        }
        if  (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
        {
            _run = true;
            _context->script = _scripts.back().second;
            value = false;
        }
        // Escape pressed : exit
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        {
            _window->close();
            value = false;
            _open = false;
        }
        
        if (event.type == sf::Event::Resized)
        {
            dynamic_cast<SFMLGraphicEngine*>(_graphicEngine)->onResize(event.size.width, event.size.height);
        }
        _eventManager->processTouchEvents(event);
    }
    return value;
}

void SFMLApplication::stepApplication()
{
    
}

void SFMLApplication::onDestroy()
{
    
}

const SFMLApplication::Context* SFMLApplication::context() const
{
    return _context;
}

void SFMLApplication::createFilesystem()
{
    DIR *dp;
    struct dirent* entry;
    BString key;
    std::basic_string<char> s;
    s.resize(12, 'a');
    if((dp  = opendir(_context->filesystem.c_str())) == NULL)
        return ;
    
    // Log::info("Storage Directory = %s", _storageDirectory.c_str());
    while ((entry = readdir(dp)) != NULL)
    {
    	if (entry->d_type == DT_DIR && findEntryInDirMap(entry->d_name, &key))
    	{
    		_dirMap[key].setPath(_context->filesystem.c_str());
    	}
    }
    closedir(dp);
    
    DirMap::iterator it = _dirMap.begin();
    DirMap::iterator ite = _dirMap.end();
    
    for (; it != ite; ++it)
    {
    	if (!it->second.isValid())
    	{
    		BString path = _context->filesystem + "/" + it->second.dirName();
    		if (mkdir(path.c_str(), 0777))
    			it->second.setPath(_context->filesystem);
    	}
    }
    
}

void SFMLApplication::notifyCurrentScript(const BString &path)
{
    BString shortName;
    int found = 0;
    int index = 0;
    for (index = path.length(); index >= 0; index--)
    {
       if (path[index] == '/')
       {
           found++;
           if (found == 2)
               break;
       }
    }
    
    if (found == 2)
        shortName << path.substr(++index);
    else
        shortName << path;
    _scripts.push_back(std::pair<BString, BString>(shortName, path));
    Log::debug("_scripts[%s] = %s", shortName.c_str(), path.c_str());
}

void SFMLApplication::exit()
{
    ::exit(0);
}