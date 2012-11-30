/*
 * AndroidGraphicEngine.h
 *
 *  Created on: 31 janv. 2012
 *      Author: NC
 */

#ifdef __ANDROID__

#ifndef ANDROIDGRAPHICENGINE_H_
#define ANDROIDGRAPHICENGINE_H_

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <android_native_app_glue.h>
#include "BGraphicEngine.h"
#include <resource_manager/BImage.h>
#include <BApplication.h>

class AndroidApplication;

class AndroidGraphicEngine : public BGraphicEngine
{

private:
	EGLDisplay _display;
	EGLSurface _surface;
	EGLContext _context;
	int _width, _height;
	bool _initialized;
	AndroidApplication* _application;
	bool _surfaceCreated;

public:
	AndroidGraphicEngine(BApplication* application);
	virtual void setSurfaceCreated(bool surface);
	virtual void draw(BScene* scene, float* scaler, BRect& rect, bool fulscreen = false);
	virtual void init();
	virtual void stop();
	virtual bool isInitialized() const;
	virtual bool isSurfaceCreated() const;
	~AndroidGraphicEngine();
	virtual void adjustToRatio(BScene* scene, BRect* rect, float* scalers, bool fullscreen = false);
	virtual long screenWidth() const;
	virtual long screenHeight() const;
};

#endif /* ANDROIDGRAPHICENGINE_H_ */
#endif
