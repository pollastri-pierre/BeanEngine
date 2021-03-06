/*
 * AndroidGraphicEngine.cpp
 *
 *  Created on: 31 janv. 2012
 *      Author: NC
 */

#ifdef __ANDROID__

#include "AndroidGraphicEngine.h"
#include "AndroidCanvas.h"

#include <BApplication.h>
#include <android_native_app_glue.h>
#include <AndroidApplication.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <resource_manager/BResourceManager.h>

#include "BFreetypeFont.h"

BFont* _FONT = NULL;

AndroidGraphicEngine::AndroidGraphicEngine(BApplication* application) {
	// TODO Auto-generated constructor stub
	_display = EGL_NO_DISPLAY;
	_context = EGL_NO_CONTEXT;
	_surface = EGL_NO_SURFACE;
	_application = (AndroidApplication*)application;
	_initialized = false;
}

void AndroidGraphicEngine::init()
{

	if (_initialized || !_surfaceCreated)
		return ;
	_initialized = true;
	_application = (AndroidApplication*)BApplication::instance();
	EGLint format, configs, error;
	EGLConfig config;
	int i = 0;
	const EGLint attributes[] = { EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
			EGL_BLUE_SIZE, 5, EGL_GREEN_SIZE, 6, EGL_RED_SIZE, 5,
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_NONE };
	_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (_display == EGL_NO_DISPLAY)
		goto ERROR;
	if (!eglInitialize(_display, NULL, NULL))
		goto ERROR;

	if (!eglChooseConfig(_display, attributes, &config, 1, &configs)
			|| (configs <= 0))
		goto ERROR;


	if (!eglGetConfigAttrib(_display, config, EGL_NATIVE_VISUAL_ID, &format))
		goto ERROR;

	ANativeWindow_setBuffersGeometry(_application->context()->window, 0, 0, format);

	_surface = eglCreateWindowSurface(_display, config, _application->context()->window, NULL);

	if (_surface == EGL_NO_SURFACE)
		goto ERROR;

	_context = eglCreateContext(_display, config, NULL, NULL);

	if (_context == EGL_NO_CONTEXT)
		goto ERROR;
	if (!eglMakeCurrent(_display, _surface, _surface, _context)
			|| !eglQuerySurface(_display, _surface, EGL_WIDTH, &_width)
			|| !eglQuerySurface(_display, _surface, EGL_HEIGHT, &_height)
			|| (_width <= 0) || (_height <= 0))
		goto ERROR;

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glEnable(GL_CULL_FACE);
	eglSwapInterval(_display, EGL_MAX_SWAP_INTERVAL);
	//glMatrixMode(GL_PROJECTION);

	glDisable(GL_ALPHA_TEST);
	glShadeModel(GL_FLAT);
	glDisable(GL_DITHER);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_NORMALIZE);
	glDisable(GL_MULTISAMPLE);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

	glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);

	glViewport(0, 0, _width, _height);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnable(GL_BLEND);
	glEnable(GL_SCISSOR_TEST);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	BResourceManager::instance()->reload();
	return ;
	ERROR: Log::error("Error while starting GraphicsService");
	_application->exit();
}

void AndroidGraphicEngine::setSurfaceCreated(bool surface)
{
	_surfaceCreated = surface;
}

bool AndroidGraphicEngine::isInitialized() const
{
	return _initialized;
}

bool AndroidGraphicEngine::isSurfaceCreated() const
{
	return _surfaceCreated;
}

void AndroidGraphicEngine::stop()
{
	_initialized = false;
	 if (_display != EGL_�r��\ �s���O.R�c�HO2���d!+��[�ֲ��l`#��������`'�������� �8��r������]�@ A��y�c��g<�/y�k����=��'>󅯄��!	Eh��p�'�Dd��hD'1�El��x�'	IDb��~���='�r2�����"5i�ANr��<�%�)@A
Q�"��)AIJQ�2���@E*Q�*4�-iEk�Жv���Dg�Еnt�=�Eo�З~�g �`�P��Ԡ&��h�0���x&0�ILf�Y�Z�q�;<�)�y�G>���.��O҈�4�)͘�z6��#�*w	��8�DJRQ�┠��N��f(��F2���`&����2��,`!�X�����`%��������`'���!���}�� O/����=�|����9�Y�q�\����5�s����6� ��<�y�c�󂗼�-�x�>��� �c��D&
Q�Ftb�_��&�8���p�'�O���$!)�HN.R��<�%�)@A
Q�"e���X�r��������,a)�u�>������"���[<$�m�E�oT�b��4�%��@F2��,d%��ANJR�Ҕ�,�(O*R��T�*iD5�S��Ԣ6uhG]�S��t�#��L�2���b4c����|�<���a.�X�:ӄ�4�9-hI+Zӆ�����C_�џd��P�1�Ld���T&0�S���y�W��u��FGz1�l����m��RNr�+\�	&�9�.�HVr�;L��tf0�Y,`!�X�����`%���լa���Nv��=�e�9�A��ld����r���8'8�m�p�{��|���CX�!�@D"��֟�,�8�.r��\�*׸�nr�@����'<�oy�{>�O|����O~�_B_HB�0D'6q�K<��$$#9)HIZґ�d$9�B���$>	�EB���Ԥ!3�(J1�S��4���pF0��Lc:3�IV����&y�G~
P�B�%(I)JS������BujP�ZԦu�G}А�4�)�hNZҊִ�-��Dg�ҍ���'��C_�џd��,f3��,b1�X�6����8'��Mnq�@��W��_��oB��C�Hl��vv��N����a/9�a�p�S��g9�y.p�k\�w�G A��y�c����5ox�{>���|�'���_B{(���D!*шN���$")�Df���ld'9�En�|� )Da�P�b��@E*Q�*T�թA]�Q�4��iJ3�ӂ���5mhK;:҉�t�+��Nzҋ��9��hP����V�}�_��7����7���/	��C�-�M���D!*шN�	�1���g�X�
�r����.�y�C��|����'>�|�	y��r~d���Q�f����2��,�=��B����c=��&6���lc;;��>�s��½�Xְ�u�g��f���mlg;��n����� �8|�֧}Q>�qNp�S�f���׳��lb3[��6���C�d���^����G8�1�s����4g8˹em��ܹ��Fp^���e�p�k\��#����%�x�O~�`�������H����E�J5Zҟd�9�q���)(FqщΌ`XMۄ%�Dd��hD'1�Ob���d$')�L����� '��M�p�'��C\⑀�$"�ICZ�S���"��Lӄ�4�9-�Eo�З�t�+��Nz2��a(��F2�ьa,��&2��d���\�����A��9��r��<�9/��g$ 5iHKV��>��AA
Q�"��iB���0�)Le�X�
V����6r���(w��[��y��D!*1�El����WY�b���լa-�X�6���la'��������'9�i�p�s����e�p�k\�7��m$��<�!�x�����S5�w�_(���y�{>�O|�_�Ə�J�����/~����E��D$щA���$ 1IHO2R����6iC[zӎ�t�#}�K?�3���a.��rV���X�vS�r���D�R��Ԡ&��K=�Ӏ�4�)-iFsZЊ�t�+��Nz^8?�gt\��1�\�+�@ A��y�c��g<�/y�G>�/|����O�G�џd��P�1�Q�fc�x&0�)Leә�Lf��E,f	KY�Z���B������g�Y�J]�Q�C�G9�qNp�S��g9�yj\�Q��ԡ.��O�����G!6��C\��!!�HL�R�d$')IEjҐ�t�'�D1�,��:(x萠��̟e<G9��:�3�U�ҫ���<:޿��1��5׳1��p'�?:Pp���\9q���[m�N�ܽ[P�2
��UE��'�O��A�U���N;���'>�|�?��o���_B(��p�'�Dd���� &��~qVĠ���a.�����,a)�X�
V��X�ֲ��l`#�����sޅ���M��