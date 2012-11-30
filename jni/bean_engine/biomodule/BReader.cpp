/*
 * BReader.cpp
 *
 *  Created on: 5 f�vr. 2012
 *      Author: NC
 */

#include "BReader.h"
#include "BAndroidReader.h"
#include "SFMLReader.h"

BReader* BReader::create(BufferMode mode)
{
#ifdef __ANDROID__
	return new BAndroidReader(mode);
#else
    return new SFMLReader(mode);
#endif
    
}
