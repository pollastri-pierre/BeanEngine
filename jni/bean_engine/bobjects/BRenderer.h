//
//  BRenderer.h
//  BeanEngine
//
//  Created by Pierre POLLASTRI on 01/11/12.
//  Copyright (c) 2012 Pierre POLLASTRI. All rights reserved.
//

#ifndef __BeanEngine__BRenderer__
#define __BeanEngine__BRenderer__

#include <iostream>
#include "BObject.h"
#include <graphic_engine/BCanvas.h>

class BRenderer : public BObject
{
private:
    BCanvas * const canvas;
    
public:
    BRenderer() : canvas(NULL) {};
    BRenderer(BCanvas * const canvas);
    virtual bool release();
    virtual	std::string toString() const;
    
    B_DECLARATION(setColor)
    B_DECLARATION(setRect)
    B_DECLARATION(fillRect)
    B_DECLARATION(popMatrix)
    B_DECLARATION(pushMatrix)
    B_DECLARATION(rotate)
    B_DECLARATION(translate)
    B_DECLARATION(scalerX)
    B_DECLARATION(scalerY)
    B_DECLARATION(drawImage)
    B_DECLARATION(drawChar)
    
};

#endif /* defined(__BeanEngine__BRenderer__) */
