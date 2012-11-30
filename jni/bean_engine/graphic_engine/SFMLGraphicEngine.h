//
//  SFMLGraphicEngine.h
//  BeanEngine
//
//  Created by Pierre POLLASTRI on 24/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef BeanEngine_SFMLGraphicEngine_h
#define BeanEngine_SFMLGraphicEngine_h

#include "BGraphicEngine.h"
#include <SFML/Graphics.hpp>

class SFMLGraphicEngine : public BGraphicEngine {
    
    sf::RenderWindow*   _window;
    sf::View*           _view;
    int                 _width, _height;
    
public:
    SFMLGraphicEngine(sf::RenderWindow* window);
    virtual void setSurfaceCreated(bool surface);
	virtual void init();
	virtual void draw(BScene* scene, float* scaler, BRect& rect, bool fulscreen = false);
	virtual void stop();
	virtual void adjustToRatio(BScene* scene, BRect* rect, float* scalers, bool fullscreen = false);
	virtual bool isInitialized() const;
	virtual bool isSurfaceCreated() const;
    virtual long screenWidth() const;
    virtual long screenHeight() const;
    void onResize(int width, int height);
    
};

#endif
