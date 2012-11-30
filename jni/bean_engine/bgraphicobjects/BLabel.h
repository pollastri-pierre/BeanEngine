/*
 * BLabel.h
 *
 *  Created on: 5 févr. 2012
 *      Author: NC
 */

#ifndef BLABEL_H_
#define BLABEL_H_

#include "BRectangle.h"
#include <graphic_engine/BFont.h>

class BLabel : public BRectangle {

public:
	enum Alignment
	{
		CENTER,
		LEFT,
		RIGHT,
	};

protected:

	BString _text;
	lua_Number _fontSize;
	BFont* 	_font;
	BString _fontName;
	BColor _textColor;
	int _spacing;

	Alignment _vAlign;
	Alignment _hAlign;

public:

	BLabel();
	virtual void setText(BString text);
	virtual void setFontSize(lua_Number _fontSize);
	virtual lua_Number fontSize() const;
	virtual void setAlignment(Alignment vAlign, Alignment hAlign);
	virtual void setAlignment(const char* vAlign, const char* hAlign);
	virtual void setTextColor(const BColor& color);
	virtual BColor textColor() const;
	static const char* alignmentToString(Alignment aligment);
	static Alignment stringToAlignment(BString str);
	virtual const char* vAlignmentToString() const;
	virtual const char* hAlignmentToString() const;
	virtual Alignment vAlignment() const;
	virtual Alignment hAlignment() const;
	virtual void setSpacing(int spacing);
	virtual int spacing() const;
	virtual BString text() const;
	virtual lua_Number getTextSize(BCanvas * const canvas);
	virtual void onDraw(BCanvas * const canvas, const BCanvasContext& context);
	virtual void setStyle(BStyle& style);
	virtual void setFont(const BString& font);
	~BLabel();


	B_DECLARATION(setText)
	B_DECLARATION(text)
	B_DECLARATION(setFontSize)
	B_DECLARATION(fontSize)
	B_DECLARATION(setFontColor)
	B_DECLARATION(fontColor)
	B_DECLARATION(setAlignment)
	B_DECLARATION(alignment)
	B_DECLARATION(setLetterSpacing)
	B_DECLARATION(letterSpacing)
	B_DECLARATION(setFont)

};

#endif /* BLABEL_H_ */
