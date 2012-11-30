/*
 * BAndroidReader.cpp
 *
 *  Created on: 5 f�vr. 2012
 *      Author: NC
 */

#ifdef __ANDROID__

#include "BAndroidReader.h"
#include <AndroidApplication.h>
#include <iostream>

BAndroidReader::BAndroidReader(BReader::BufferMode mode)
{
	// TODO Auto-generated constructor stub
	_data = NULL;
	_asset = NULL;
	_size = 0;
	_fd = -1;
	_mode = mode;
}

char* BAndroidReader::data() const
{
	return _data;
}

bool BAndroidReader::open(const BString& path, const char* absolute_path)
{
	const AndroidApplication* activity = (AndroidApplication*)BApplication::instance();
	if (!activity)
		return false;
	AAssetManager* manager = activity->getAssetManager();
	_asset = AAssetManager_open(manager, path.c_str(), AASSET_MODE_RANDOM);
	return _asset != NULL;
}

void BAndroidReader::readAll()
{
	if (!_asset)
		return ;
	const int bufferSize = 4096;
	char buffer[bufferSize];
	off_t size = 0;
	_size = 0;
	int readBytes;
	while ((readBytes = AAsset_read(_asset, buffer, bufferSize)) > 0)
	{
		_data = (char*) realloc(_data, (sizeb�E���{���9���귰����Z{m�Υ�ֆ�85�������6��B��v���Q�rx�q6bT���@���P.�$��p�.3�,A�ܐ�w����8~��Tnڨ�#�
3�o��+�M���i	�:����KT"����9:�P�}�8�W��nK�$���Q;��s,�yzF��Ra��[WXi��+m����W,�<�l�ɻ�:�*�ܴ���J��6�f֖����¾��d��R���+l ��'�A8��k�=�"p���K�:$��=6�����d���0�hB���H�+->���9�v��>���}a=c�@�	�H���\Fz�H�������$�!�!#�h��oe��,��'��n��#��!������)y1��
�@����	�?��.���7꟨���-W:f{E޶t�'p�Fgw瘖�ۦ+�R�H�~���~�>E��� �d⦕�s'8?J������%1�� �J��+#���PrVҝF�r��K&u$$�[Ғ5$[1mw �ʭ�6��?+��_��N�
�)$3)�HiCJ{ܛ��1=��_`�}O	�_RK�����c(��B~i�
h�R�G��l7#4�i3�G�L�@���%CX�s|!�d+W����sK����#KKt���y]5��:}nBZD)r��pU+�����,-����$%�W�Jy�T(0���֖tycnK+(�F<�锛��������}�U�/�gv�B�A���XJ�ф�!����<꺾��1V����1b��X�S����s��v ����u���@�°VXG��Aa}h�TZ�xn{��5U3�0#yD[`��hAz��sH�����[�|�s�CjF	/5��2D~]�Zx�����а��"��q��Y��E