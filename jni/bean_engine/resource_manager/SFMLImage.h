//
//  SFMLImage.h
//  BeanEngine
//
//  Created by Pierre POLLASTRI on 24/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef BeanEngine_SFMLImage_h
#define BeanEngine_SFMLImage_h

#include "BImage.h"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

class SFMLImage : public BImage {
    sf::Image   _image;
    GLuint		_textureId;
    
public:
    SFMLImage(const BString& name, const BString& path);
	virtual void load(const BString& path);
	const BSize& size() const;
	virtual void unload();
    sf::Image& image();
    GLuint textureId() const;
};

#endif
