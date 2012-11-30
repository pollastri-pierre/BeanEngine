//
//  SFMLCanvas.h
//  BeanEngine
//
//  Created by Pierre POLLASTRI on 24/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef BeanEngine_SFMLCanvas_h
#define BeanEngine_SFMLCanvas_h

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "BCanvas.h"

class SFMLCanvas : public BCanvas {
    sf::RenderWindow&   _window;
    float               _scalerX, _scalerY;
    BRect               _rect;
    BColor              _color;
    float               _rotation;
    bool                _fullscreen;
    
public:
    SFMLCanvas(sf::RenderWindow& window, float scalerX, float scalerY, bool fullscreen);
    virtual void clear();
	virtual void setColor(const BColor& color);
	virtual void setRect(const BRect& rect);
	virtual void fillRect();
	virtual bool stretched() const;
	virtual float scalerX() const;
	virtual float scalerY() const;
	virtual void pushMatrix();
	virtual void popMatrix();
	virtual void translate(float x, float y);
    virtual void translate(float value, float x, float y, float z);
	virtual void rotate(float angle);
    virtual void rotate(float angle, float x, float y, float z);
	virtual void drawChar(BChar c, BFont* font);
	virtual void drawImage(BImage* image, BRect subRect);
	virtual bool sizeofChar(BChar c, BFont* font, int size, lua_Number* width, lua_Number* height, lua_Number* left, lua_Number* top);
	virtual void clipRect(const BRect& rect);
	virtual void unclip();
    virtual ~SFMLCanvas() {};
};

#endif
