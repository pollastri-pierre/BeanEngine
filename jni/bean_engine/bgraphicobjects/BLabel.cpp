/*
 * BLabel.cpp
 *
 *  Created on: 5 févr. 2012
 *      Author: NC
 */

#include "BLabel.h"
#include <graphic_engine/AndroidGraphicEngine.h>
#include <resource_manager/BResourceManager.h>

#define CLASS BLabel

B_DEFINE_CLASS("Label", BRectangle)

B_NO_FUNCTION

B_METHODS_BEGIN

B_METHOD(setText)
B_METHOD(text)
B_METHOD(setFontSize)
B_METHOD(fontSize)
B_METHOD(setLetterSpacing)
B_METHOD(letterSpacing)
B_METHOD(setAlignment)
B_METHOD(alignment)
B_METHOD(setFontColor)
B_METHOD(fontColor)
B_METHOD(setFont)

B_METHODS_END

struct Pair
{
	const char* str;
	BLabel::Alignment align;
};

static Pair ___alignements[] = {
		{"center", BLabel::CENTER},
		{"left", BLabel::LEFT},
		{"right", BLabel::RIGHT},
		{"top", BLabel::LEFT},
		{"bottom", BLabel::RIGHT},
		{NULL, BLabel::CENTER}
};

BLabel::BLabel()
{
	_font = NULL;
	_fontSize = 40;
	_spacing = 0;
	_vAlign = CENTER;
	_hAlign = CENTER;
	_fontName = "Default";
	_backgroundColor = BColor(255, 255, 255, 0);
}

void BLabel::setText(BString text)
{
	_text = text;
	_text.resetUnicode();
	_text.toUnicode();
}

BString BLabel::text() const
{
	return _text;
}

void BLabel::setFontSize(lua_Number fontSize)
{
	_fontSize = fontSize;
}

lua_Number BLabel::fontSize() const
{
	return _fontSize;
}

void BLabel::setStyle(BStyle& style)
{
	BRectangle::setStyle(style);
	if (keyExists(style, "text"))
		setText(style["text"]);
	if (keyExists(style, "fontsize"))
		setFontSize((stringToNumber(style["fontsize"])));
	if (keyExists(style, "font"))
		setFont(style["font"]);
	if (keyExists(style, "letterspacing"))
		_spacing = (stringToNumber(style["letterspacing"]));
	if (keyExists(style, "textcolor"))
		_textColor = (stringToColor(style["textcolor"]));
	if (keyExists(style, "fontcolor"))
		_textColor = (stringToColor(style["fontcolor"]));
	if (keyExists(style, "alignment"))
	{
		std::vector<std::string> align = strToVector(style["alignment"], " ");
		if (align.size() >= 2)
			setAlignment(align[1].c_str(), align[0].c_str());
	}
}

lua_Number BLabel::getTextSize(BCanvas * const canvas)
{
	if (!_font)
		return 0;
	lua_Number textSize = 0;
	int size = _fontSize * canvas->scalerY();
	int rSpacing = _spacing * canvas->scalerX();

	lua_Number width, height, left, top = 0;

	for (int i = 0; i < _text.unicodeSize(); i++)
	{
		if (_text[i] == ' ')
		{
			canvas->sizeofChar('a', _font, size, &width, &height, &left, &top);
			width = width / 2.0;
		}
		else
			canvas->sizeofChar(_text.toUnicode()[i], _font, size, &width, &height, &left, &top);

		if (i == 0)
			left = 0;
		else
			width += left;

		if ((i + 1) == _text.unicodeSize())
			rSpacing = 0.0;
		textSize += width + rSpacing;
	}
	return textSize;
}

void BLabel::onDraw(BCanvas * const canvas, const BCanvasContext& context)
{
	BRectangle::onDraw(canvas, context);

	if (!_font)
		_font = BResourceManager::instance()->loadFont(_fontName);
	if (!_font)
		return ;
	lua_Number s = getTextSize(canvas);
	BColor c = _textColor;
	c.setAlpha(c.alpha() * context.opacity);
	canvas->setColor(c);
	lua_Number x, y;
	lua_Number size = _fontSize * canvas->scalerY();

	if (_hAlign == CENTER)
		x = context.rect.x() + (context.rect.width() - s) / 2.0;
	else if (_hAlign == LEFT)
		x = context.rect.x();
	else
		x = context.rect.x() + context.rect.width() - s;

	if (_vAlign == CENTER)
		y = context.rect.y() + (context.rect.height() + size / 2.0) / 2.0;
	else if (_vAlign == LEFT)
		y = context.rect.y() + (3 * size) / 4;
	else
		y = context.rect.y() + context.rect.height();

	lua_Number width, height, left, top = 0;

	int rSpacing = _spacing * canvas->scalerX();

	for (int i = 0; i < _text.unicodeSize(); i++)
	{
		if (_text[i] == ' ')
		{
			canvas->sizeofChar('a', _font, size, &width, &height, &left, &top);
			width = width / 2.0;
		}
		else
			canvas->sizeofChar(_text.toUnicode()[i], _font, size, &width, &height, &left, &top);
		if (i == 0)
			left = 0;
		canvas->setRect(BRect(x + left, y - top, width, height));
		if (_text[i] != ' ')
		canvas->drawChar(_text.toUnicode()[i], _font);
		//Log::debug("%i %s", size, BRect(x, y, width, height).toString().c_str());
		//if (i != 0)
		//	width += left;
		if (i != 0)
			width += left;
		x += width + rSpacing;
	}
/*
	canvas->setRect(BRect(context.rect.x(), y - 1, context.rect.width(), 2));
	canvas->setColor(BColor(255, 0, 0));
	canvas->fillRect();

	canvas->setRect(BRect(context.rect.x(), context.rect.y() + (context.rect.height()) / 2.0, context.rect.width(), 2));
		canvas->setColor(BColor(0, 0, 255));
		canvas->fillRect();
*/
}

void BLabel::setFont(const BString& font)
{
	_fontName = font;
	_font = NULL;
}

void BLabel::setAlignment(BLabel::Alignment vAlign, BLabel::Alignment hAlign)
{
	_vAlign = vAlign;
	_hAlign = hAlign;
}

void BLabel::setAlignment(const char* vAlign, const char* hAlign)
{
	_vAlign = stringToAlignment(vAlign);
	_hAlign = stringToAlignment(hAlign);
}

void BLabel::setTextColor(const BColor& color)
{
	_textColor = color;
}

BColor BLabel::textColor() const
{
	return _textColor;
}

const char* BLabel::alignmentToString(BLabel::Alignment aligment)
{
	int i;
	for (i = 0; ___alignements[i].str && ___alignements[i].align != aligment; i++);
	return ___alignements[i].str;
}

BLabel::Alignment BLabel::stringToAlignment(BString str)
{
	int i;
	for (i = 0; ___alignements[i].str && strcasecmp(___alignements[i].str, str.c_str()); i++);
	return ___alignements[i].align;
}

const char* BLabel::vAlignmentToString() const
{
	return alignmentToString(_vAlign);
}

const char* BLabel::hAlignmentToString() const
{
	return alignmentToString(_hAlign);
}

void BLabel::setSpacing(int spacing)
{
	_spacing = spacing;
}

int BLabel::spacing() const
{
	return _spacing;
}

BLabel::Alignment BLabel::vAlignment() const
{
	return _vAlign;
}

BLabel::Alignment BLabel::hAlignment() const
{
	return _hAlign;
}

BLabel::~BLabel() {
	// TODO Auto-generated destructor stub
}

B_IMPLEMENTATION(setText)

	const char* str = luaL_checkstring(state, 2);
	self->setText(str);
	return 0;

B_END

B_IMPLEMENTATION(text)

	lua_pushstring(state, self->text().c_str());
	return 1;

B_END

B_IMPLEMENTATION(setFontSize)
	lua_Number size = luaL_checknumber(state, 2);
	self->setFontSize(size);
	return 0;
B_END

B_IMPLEMENTATION(fontSize)

	lua_pushnumber(state, self->fontSize());
	return 1;

B_END

B_IMPLEMENTATION(setAlignment)

	const char* h = luaL_checkstring(state, 2);
	const char* v = luaL_checkstring(state, 3);

	self->setAlignment(v, h);

	return 0;

B_END

B_IMPLEMENTATION(setFontColor)

	BColor c;
	c.fromLua(state, 1);
	self->setTextColor(c);
	return 0;

B_END

B_IMPLEMENTATION(alignment)

	lua_pushstring(state, self->hAlignmentToString());
	lua_pushstring(state, self->vAlignmentToString());
	return 2;

B_END

B_IMPLEMENTATION(fontColor)

	return BClass<BColor>::newInstance(state, new BColor(self->textColor()));

B_END

B_IMPLEMENTATION(setLetterSpacing)

	self->setSpacing(luaL_checknumber(state, 2));
	return 0;

B_END

B_IMPLEMENTATION(letterSpacing)

	lua_pushnumber(state, self->spacing());
	return 1;

B_END

B_IMPLEMENTATION(setFont)

	self->setFont(luaL_checkstring(state, 2));
	return 0;

B_END

