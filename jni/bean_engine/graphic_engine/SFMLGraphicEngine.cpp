//
//  SFMLGraphicEngine.cpp
//  BeanEngine
//
//  Created by Pierre POLLASTRI on 24/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "SFMLGraphicEngine.h"
#include "SFMLCanvas.h"
#include <application/SFMLApplication.h>
#include <SFML/OpenGL.hpp>

SFMLGraphicEngine::SFMLGraphicEngine(sf::RenderWindow* window)
{
    _window = window;
    _view = new sf::View(sf::FloatRect(0, 0, 0, 0));
    _width = _window->getSize().x;
    _height = _window->getSize().y;
    
    glEnable(GL_BLEND);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
    
    glEnable( GL_POINT_SMOOTH );
    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_POLYGON_SMOOTH );
    onResize(_width, _height);
}

void SFMLGraphicEngine::setSurfaceCreated(bool surface)
{
    
}

void SFMLGraphicEngine::init()
{
    
}

void SFMLGraphicEngine::onResize(int width, int height)
{
    _width = width;
    _height = height;
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho (0, width, height, 0, 0, 1000000);
    glViewport(0, 0, _width, _height);
}

void SFMLGraphicEngine::draw(BScene *scene, float *scaler, BRect &rect, bool fullscreen)
{   
    BCanvas* canvas = new SFMLCanvas(*_window, scaler[0], scaler[1], fullscreen);

    glDisable(GL_SCISSOR_TEST);
    canvas->clear();
    glEnable(GL_SCISSOR_TEST);
    
    if (!fullscreen)
		glScissor(rect.x(), rect.y(), rect.width(), rect.height());
	else
		glScissor(0, 0, _width, _height);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    BCanvasContext context = {rect, scene->opacity()};
    
	scene->draw(canvas, context);
    
    _window->display();
    delete canvas;
}

void SFMLGraphicEngine::stop()
{
    
}

void SFMLGraphicEngine::adjustToRatio(BScene *scene, BRect *rect, float *scalers, bool fullscreen)
{
    SFMLApplication* app = (SFMLApplication*) BApplication::instance();
    if (_window->getSize().x != _width || _window->getSize().y != _height)
    {
        _window->create(sf::VideoMode(_window->getSize().x, _window->getSize().y), app->context()->appName);
        _width = _window->getSize().x;
        _height = _window->getSize().y;
    }
	if (!fullscreen)
	{
		float fW = (scene->width() * (float)_height) / scene->height();
		float fH = (scene->height() * (float)_width) / scene->width();
        
		float scaler;
        
		if (fW > _width)
		{
			scaler = (float)_width / scene->width();
			rect->set(0, (_height - fH) / 2, _width, (_height - fH) / 2 + fH);
		}
		else
		{
			scaler = (float)_height / scene->height();
			rect->set((_width - fW) / 2, 0, (_width - fW) / 2 + fW, _height);
		}
		scalers[0] = scaler;
		scalers[1] = scaler;
	}
	else
	{
		scalers[0] = (float)_width / scene->width();
		scalers[1] = (float)_height / scene->height();
		rect->setWidth(_width);
		rect->setHeight(_height);
	}

}

long SFMLGraphicEngine::screenHeight() const
{
    return _height;
}

long SFMLGraphicEngine::screenWidth() const
{
    return _width;
}

bool SFMLGraphicEngine::isInitialized() const
{
    return true;
}

bool SFMLGraphicEngine::isSurfaceCreated() const
{
    return true;
}