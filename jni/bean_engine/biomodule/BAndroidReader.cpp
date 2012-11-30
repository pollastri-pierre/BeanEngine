/*
 * BAndroidReader.cpp
 *
 *  Created on: 5 fvr. 2012
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
		_data = (char*) realloc(_data, (sizebóEıô³{ê§óå°9ª¯·ê·°„†ÚÒZ{m…Î¥å”Ö†´85•ûµ”‹„6şŠB‚½vßåâQ†rx÷q6bT£–•@²äP.ï$íÃpã.3Ò,Aæ»Ü’wÍ£¯8~ÓÂTnÚ¨ì#Û
3¹o ²+÷M¹çÅi	é:âòŠúêKT"èÜÈó9:×Pø}Ã8¯W¯’nK¶$¹ÿQ;½”s,ï•yzFêËRaüî[WXi©í+mµåÀØW,‘<ÕlŞÉ»±:Å*îÜ´¯ÛüJ£º6åfÖ–¥”ÂÂ¾ñëd£×Râ÷¼+l ¿ş'çA8ŞÂ“ƒk®=¸"pÑö¾K:$éÛ=6¹Åæ‡Ôúd¨ô®0ÈhB›ùòHË+->»¤Å9µv ¡>«Ñ}a=cô@£	é­HÏö¯\Fz«H’Œ‘¼–Ë¹$È!¼!#éhãoeƒ²,ı'èÅnú #£¿!ìÿÂñèĞï‰½)y1³ƒ
Ã@„ßÄç	¹?—û.‹ú7êŸ¨ ş–-W:f{EŞ¶tô‚›'p”Fgwç˜–äÛ¦+ÏRŠH~¢ëÄ~ô>EõÔÿ édâ¦•«s'8?Jšù“®Ó%1Ìı ¸Jº«+#µëêPrVÒF“rÉÜK&u$$ó[Ò’5$[1mw ÄÊ­š6æó?+Ãù_ÿµN 
Ç)$3)íHiCJ{Ü›¹á1=¤ü_`ã}O	›_RK¨Ÿ‰c(”ÂB~iù
hèR£Gå÷l7#4æi3îGò—LÚ@ğÛ%CXøs|!÷d+W‘§üÈsK—òÙÃ#KKt…ù™y]5à¨é:}nBZD)rÎôpU+–ª¦‘,-Ÿ«æù$%ïWÊJyŸT(0µæ›’Ö–tycnK+(¬F<ä¯é”›‘Ü÷µ—ÁŠ¶}“U«/’gvÙBòAÓî±ŞXJÀÑ„£!¥¸”Õ<êº¾Ôõ1VàÚƒ1bØÄXƒS„ÊÑçs×¥v ¬ÇôÏu´äŞ@ Â°VXGô¹Aa}hàTZ„xn{ÒÚ5U3Ğ0#yD[`¬hAzëÒsH“òïÉ[¹|Çs¸CjF	/5ËÏ2D~]îZx’›±ŞĞ°¡±"ÏäqŒ¸YàæE