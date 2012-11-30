//
//  SFMLImage.cpp
//  BeanEngine
//
//  Created by Pierre POLLASTRI on 24/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "SFMLImage.h"
#include <application/SFMLApplication.h>

SFMLImage::SFMLImage(const BString& name, const BString& path) : BImage(name, path)
{
    
}

void SFMLImage::load(const BString& path)
{
    BImage::load(path);
    SFMLApplication* app = (SFMLApplication*)BApplication::instance();
    if (!_image.loadFromFile(app->context()->path + path))
    {
        Log::error("Cannot load image %s", path.c_str());
        return ;
    }
    _size.setWidth(_image.getSize().x);
    _size.setHeight(_image.getSize().y);
    glGenTextures(1, &_textureId); 
    glBindTexture(GL_TEXTURE_2D, _textureId); 
    /*glTexParameteri(GL_TEXTURE_2D, 
                    GL_TEXTURE_MAG_FILTER, 
                    GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, 
                    GL_TEXTURE_MIN_FILTER, 
                    GL_LINEAR_MIPMAP_LINEAR);*/
    //GLfloat maxA;
    //glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxA);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,maxA);//Anisotropic Filtering Attempt
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _size.width(), _size.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, _image.getPixelsPtr());
    _image = sf::Image();
}

GLuint SFMLImage::textureId() const
{
    return _textureId;
}

sf::Image& SFMLImage::image()
{
    return _image;
}

void SFMLImage::unload()
{
    _image = sf::Image();
    glDeleteTextures(1, &_textureId);
}

const BSize& SFMLImage::size() const
{
    return _size;
}