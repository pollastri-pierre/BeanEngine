/*
 * BConfigLoader.cpp
 *
 *  Created on: 19 avr. 2012
 *      Author: NC
 */

#include "BConfigLoader.h"
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <exception>
#include <iostream>
#include <string>
#include <biomodule/BReader.h>
#include <application/Log.h>
#include <application/BApplication.h>

typedef boost::property_tree::ptree ptree;

struct ParserFunction {

	const char* key;
	void (*f)(ptree& pt, BResourceTree&, BStyleTree&);

};

static void parse_node(ptree& pt, BResourceTree& brt, BStyleTree& bst);
static void parse_image(ptree& pt, BResourceTree& brt, BStyleTree& bst);
static void parse_style(ptree& pt, BResourceTree& brt, BStyleTree& bst);
static void parse_font(ptree& pt, BResourceTree& brt, BStyleTree& bst);
static void parse_bgm(ptree& pt, BResourceTree& brt, BStyleTree& bst);
static void parse_effect(ptree& pt, BResourceTree& brt, BStyleTree& bst);

static ParserFunction _pfunc [] = {
		{"images", parse_node},
		{"resources", parse_node},
		{"styles", parse_node},
		{"style", parse_style},
		{"image", parse_image},
		{"fonts", parse_node},
		{"font", parse_font},
		{"sounds", parse_node},
		{"bgm", parse_bgm},
		{"effect", parse_effect},
		{NULL, NULL}
};

static void parse_node(ptree& pt, BResourceTree& brt, BStyleTree& bst)
{
	BOOST_FOREACH(boost::property_tree::ptree::value_type const& v, pt)
	{
		for (int i = 0; _pfunc[i].f != NULL; i++)
		{
			if (strcasecmp(v.first.c_str(), _pfunc[i].key) == 0)
			{
				boost::property_tree::ptree node = v.second;
				_pfunc[i].f(node, brt, bst);
			}
		}
	}
}

static void parse_font(ptree& pt, BResourceTree& brt, BStyleTree& bst)
{
	BRRawData data;
	try
	{
		data.name = pt.get<std::string>("<xmlattr>.name");
	}
	catch (...)
	{
		Log::warn("Warning: No name in font markup. Font is ignored.");
		return ;
	}

	std::string s;

	try
	{
		s = pt.get<std::string>("<xmlattr>.directory");
	}
	catch (...)
	{
		Log::warn("Warning: No directory in font [%s] markup. Font is ignored.", data.name.c_str());
		return ;
	}

	if (s[s.length() - 1] != '/')
		s += "/";

	try
	{
		data.path = s + pt.get<std::string>("<xmlattr>.filename");
	}
	catch (...)
	{
		Log::warn("Warning: No filename in font [%s] markup. Font is ignored.", data.name.c_str());
		return ;
	}


	//Log::debug("XML FONT <%s> <%s>", data.name.c_str(), data.path.c_str());
	//boost::get<BResourceMap>(brt["images"])[data.name] = data;

	try
	{
		data.other.fontsize = pt.get<int>("<xmlattr>.fontsize");
	}
	catch (...)
	{
		const int def = 60;
		data.other.fontsize = def;
		Log::warn("Warning: No directory in font [%s] markup. Setting fontsize to %i", data.name.c_str(), def);
	}


	boost::get<BResourceMap>(brt["fonts"])[data.name] = data;
	(brt)["resources_count"] = boost::get<int>((brt)["resources_count"] ) + 1;
	//Log::debug("OK FONT <%s> <%s> <%i>", data.name.c_str(), data.path.c_str(), data.other);
}

static void parse_bgm(ptree& pt, BResourceTree& brt, BStyleTree& bst)
{
	BRRawData data;
	data.name = pt.get<std::string>("<xmlattr>.name");
	std::string s = pt.get<std::string>("<xmlattr>.directory");

	if (s[s.length() - 1] != '/')
		s += "/";

	data.path = s + pt.get<std::string>("<xmlattr>.filename");
	//Log::debug("XML BGM <%s> <%s>", data.name.c_str(), data.path.c_str());
	//boost::get<BResourceMap>(brt["images"])[data.name] = data;
	data.other.soundType = BRRawData::SOUND_BGM;
	boost::get<BResourceMap>(brt["bgms"])[data.name] = data;
	(brt)["resources_count"] = boost::get<int>((brt)["resources_count"] ) + 1;
	//Log::debug("OK BGM <%s> <%s>", data.name.c_str(), data.path.c_str());
}

static void parse_effect(ptree& pt, BResourceTree& brt, BStyleTree& bst)
{
	BRRawData data;
	data.name = pt.get<std::string>("<xmlattr>.name");
	std::string s = pt.get<std::string>("<xmlattr>.directory");

	if (s[s.length() - 1] != '/')
		s += "/";

	data.path = s + pt.get<std::string>("<xmlattr>.filename");
	//Log::debug("XML EFFECT <%s> <%s>", data.name.c_str(), data.path.c_str());
	//boost::get<BResourceMap>(brt["images"])[data.name] = data
	data.other.soundType = BRRawData::SOUND_EFFECT;
	boost::get<BResourceMap>(brt["effects"])[data.name] = data;
	(brt)["resources_count"] = boost::get<int>((brt)["resources_count"] ) + 1;
	//Log::debug("OK SoundEffect <%s> <%s>", data.name.c_str(), data.path.c_str());
}

static void parse_image(ptree& pt, BResourceTree& brt, BStyleTree& bst)
{
	BRRawData data;
	data.name = pt.get<std::string>("<xmlattr>.name");
	std::string s = pt.get<std::string>("<xmlattr>.directory");

	if (s[s.length() - 1] != '/')
		s += "/";

	data.path = s + pt.get<std::string>("<xmlattr>.filename");
	//Log::debug("XML IMAGE <%s> <%s>", data.name.c_str(), data.path.c_str());
	//boost::get<BResourceMap>(brt["images"])[data.name] = data;
	boost::get<BResourceMap>(brt["images"])[data.name] = data;
	(brt)["resources_count"] = boost::get<int>((brt)["resources_count"] ) + 1;
	//Log::debug("OK IMAGE <%s> <%s>", data.name.c_str(), data.path.c_str());
}

static void parse_style(ptree& pt, BResourceTree& brt, BStyleTree& bst)
{
	BStyle style;
	try
	{
	 const ptree& attributes = pt.get_child("<xmlattr>");
	 if(!attributes.empty())
	 	{
	 		BOOST_FOREACH(const ptree::value_type& attr, attributes)
	 		{
	 			const std::string& attrName = attr.first;
	 			const std::string& attrVal = attr.second.data();

	 			std::string name = attrName;
				const int length = name.length();
				for(int i = 0; i < length; ++i)
					name[i] = std::tolower(name[i]);
	 			style[name] = attrVal;
	 			//Log::debug("Style Name = <%s> Value = <%s>", name.c_str(), attrVal.c_str());

	 		}
	 		if (style.find("name") == style.end())
	 		{
	 			Log::warn("Warning: Style with no name");
	 		}
	 		else
	 		{
	 			std::string name = style["name"];
	 			//const int length = name.length();
	 			//for(int i = 0; i < length; ++i)
	 			//	name[i] = std::tolower(name[i]);

	 			bst[name] = style;
	 		}
	 	}
	}
	catch (std::exception& e)
	{
		Log::debug("EXception %s", e.what());
		return ;
	}

}

BResourceTree& BConfigLoader::conf()
{
	return *_tree;
}

BConfigLoader* BConfigLoader::_instance = NULL;

BConfigLoader::BConfigLoader() {
	// TODO Auto-generated constructor stub
	_isValid = false;
    _tree = NULL;
	_toDelete = NULL;
}

BConfigLoader* BConfigLoader::instance()
{
	if (!_instance)
		_instance = new BConfigLoader();
	return _instance;
}

BStyleTree& BConfigLoader::styles()
{
	return *_styles;
}

void BConfigLoader::parseBuffer(const char* buffer)
{
	boost::property_tree::ptree pt;
	std::istringstream is;
	is.str(buffer);
	try
	{
		boost::property_tree::read_xml(is, pt);
	}
	catch (boost::property_tree::xml_parser_error e)
	{
		Log::error("Error: XML EXCEPTION <%s>", e.what());
	}
	BResourceTree* brt = new BResourceTree();
	BStyleTree* bst = new BStyleTree();
	(*brt)["resources_count"] = 0;
	(*brt)["images"] = BResourceMap();
	(*brt)["bgms"] = BResourceMap();
	(*brt)["effects"] = BResourceMap();
	(*brt)["fonts"] = BResourceMap();
	try
	{
		parse_node(pt.get_child("config"), *brt, *bst);
	}
	catch(std::exception e)
	{
		Log::error("Error: Parse Config Error <%s>", e.what());
	}
	//Log::debug("Nombre �����_��5�v��d���^�����9�Q�q����9�Y�q�\����U��������j{4����"6q�K<Ⓚ�$"1IHJ2�����"5iHK:��yBv�r���a(��F2�ьa,��&2�o|�?��o���"4a�_����@D"�\�&y�G~
P�B�E)FqJP�R��e)Gz2��Ld&Y�FS�ќ���iC[�ўt���BW�ѝ����Cy*P�JT�
U�FujP�ZԦu�G}АF4f2S`*Ә�f2��̡/��� 2��\�"����r����&����r�@��O���w�C�'<e.��β�s,b1KX�2�����b5kX�:ֳ��lb3[��6�����b7{��>�s���0G8�1�s����4�_x"�HD&
Q�Ftb�X�&�G�V0�3��&�3���b�Y�B��%,e�Y�
V��լa-������g�{�^�׼�-�x�>��|!���p-iEk�Жv���Dg�Еn� $�M�2��L!��Lc:3��,��W�����)��N9�S��T"���O���$!)�HN
R��Ԥ!-�HO2���d!+��Nr����!/��O
R���(�(N	JR�Ҕ�,�(O*R��T�*ըNjR��ԡ.��O҈�4�)͘��2��,`!�X�����`%�X�ֲ��l�;=��]zћ>����� 2���6C�pF0�Q�fc�x&0�@���&6���lc;;��.v����c?8�!s���8'8�)Ns����<��%.s��\�:7��-���p߆$�	CX��D$��BT���$�����<�1Ox�3��dQ��|Mf�7�-�x�>��dU'��ANF����!6q�K<Ⓚ�$"1IHJ2�����"5iHK:ғ��d�5�Em�P�z�'/��O
R���(�(N	JR�Ҍfc�x&0�ILf
Leә�LfQ�*T��Y�"���,c9+X�*V��w����g�����%!)�HN�R��4��iBS�ќ���iC[�ўt���BW�ѝ����C_�џd��P�1��d-�X�6���la+���v����a/���r���a.�������<�)�x�^۹|%_�ı-.�@[Ⓚ�$�zG8�1�s����4g8�9�s��\�2W��5�s����6w��=	�+���~����/�a�)I(B���#<�H$"��D#:1�I,R��Ԥ!-�HO2���d!+��Nr����!/��O
R���(�(N	JR�Ҕ�}�G0�AfC�pF0��L!��Lc:3��,f3��̣<KX�2�S�JT����h�0�q�g��z��ld���!�r�#�6w��}БNt��X�b�q��Ĉ庐��2�i�`�0�1�e)�X�
V�����<��^�E)FIJQ�2���e>dE*Q�*T�թAm�Q�4��iB3V�k%�X�ֲ���`'�������� �8�	Nr�Ӝ�,�8�.r��\�*7��unq� y�c��g��%�x����|�#������w~���8��/	�D'�I@R���t�'#��M�R��T���M�R�Ft�7}�m̿�#�סM���D "��L����"6q�KB��$$'�HMҒ�,d%9�E���)FqJP�R���DjP��4�!�iBS�����tf0�9�e�Y�B��%,eE��Z�a=��&���mlg;��n�Ү�5���Y$���,c9+X�*V����c=��&6���lc;;��.v���죅q[Ҋִ�-�hO:ҙ.t�=�NOz1�ag��D&1�)0�iL�/��� 2��ds��|��E,f	9�a�p�c�'9�i����<c3��l氚5�e���F6��-le���Nv��=�e�9�E.q�+\����G>�/$�홡"��L�R�d��P�1���c=��%.s��\3�unp�[��w�G A��y�c���7����7��?B/$�M�������5ox�;����$ !�HL���b�%)Ei�P�r����J7���D "��L���� &��M⒒T�&iIGz2��Ld&Y�Fvr��\�&y�G~
P�B�թAMjQ�:ԥ�i@Cј&4��iAKZњ6����@G:љ.�`$���2��L`"�����4�3���b6��AOzћ>�����rV��U�f�'{��INq�3����.�W��s��<泀�,b1KX�&6���lc;;��.v����c?8�!s���׹�Mnq�;���q�<��y�S��`�����'�����q<��U~�{>�OD�/1IG2���� ŨDZ҆�t�#�B S��r6q����Bz�X�&3Y�LjS��la9�Y�񂗄�7aK8���D�8�HOB�׆0�%�@D"�(D%q�K<Ⓚ�$"1IHJ2�����"5iHKV����$��C^�Q���(�)AIJQ�2���@E�R��Ԡ&��K=�Ӏ�4��iB3�ӊ���=��BW�ѝ����C_�џd��p�p�c�$�8�m�p�{�}�G<�	Oy�~�����G�=�מ^���x����|�#������7�Y_Q�J4����"6q�K<Ⓚ�$"1IHJ2����d#;9�I.r����#?(H!
S��T�:5�I-jS��$�ICZ�Q����4e(K9�S�%��K�H'�љ.t����rV���d"3�De�P����7}��:ֳ����/�ϵ%=�H9�S��`2[���2��L`"�����4�3