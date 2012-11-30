/*
 * BGraphicObject.cpp
 *
 *  Created on: 30 janv. 2012
 *      Author: NC
 */

#include "BGraphicObject.h"

#undef  BOOST_LCAST_NO_WCHAR_T
#undef	BOOST_NO_STD_WSTRING

using namespace boost;

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#define CLASS BGraphicObject

B_DEFINE_CLASS("GraphicObject", BRect)

B_NO_FUNCTION

B_METHODS_BEGIN

	B_METHOD(setSize)
	B_METHOD(backgroundColor)
	B_METHOD(setBackgroundColor)
	B_METHOD(setVisible)
	B_METHOD(isVisible)
	B_METHOD(setRotation)
	B_METHOD(setUserInteractionEnabled)
	B_METHOD(isUserInteractionEnabled)
	B_METHOD(intersects)
	B_METHOD(setOpacity)
	B_METHOD(opacity)
	B_METHOD(moveUp)
	B_METHOD(moveDown)
	B_METHOD(moveToTop)
	B_METHOD(moveToBottom)
	B_METHOD(setCenter)
	B_METHOD(rotation)
	B_METHOD(setStyle)

B_METHODS_END

BGraphicObject::BGraphicObject() {
	_rect = NULL;
	_visible = true;
	_userInteractionEnabled = false;
	_parent = NULL;
	_backgroundColor = BColor(0, 0, 0);
	_rotation = 0;
	_opacity = 1.0;
}

bool BGraphicObject::keyExists(const BStyle& style, const BString& key)
{
	return style.find(key) != style.end();
}

bool BGraphicObject::stringToBool(std::string& str)
{
	return strcasecmp(str.c_str(), "true") == 0;
}

lua_Number BGraphicObject::stringToNumber(std::string& str)
{
	return boost::lexical_cast<double>(str);
}

std::vector<std::string> BGraphicObject::strToVector(const std::string& str, const std::string& sep)
{
	std::vector< std::string > SplitVec;
	split( SplitVec, str, is_any_of(sep), token_compress_on );
	return SplitVec;
}

BColor BGraphicObject::stringToColor(std::string& str)
{
	std::vector<std::string> c = strToVector(str, " ");
	const int lenght = c.size();
	lua_Number red, green, blue, alpha = 255;
	if (lenght >= 1)
		red = stringToNumber(c[0]);
	if (lenght >= 2)
		green = stringToNumber(c[1]);
	if (lenght >= 3)
		blue = stringToNumber(c[2]);
	if (lenght >= 4)
		alpha = stringToNumber(c[3]);
	return BColor(red, green, blue, alpha);
}

BPoint BGraphicObject::stringToPoint(std::string& str)
{
	std::vector<std::string> c = strToVector(str, " ");
	const int lenght = c.size();
	lua_Number x, y = 0;
	if (lenght >= 1)
		x = stringToNumber(c[0]);
	if (lenght >= 2)
		y = stringToNumber(c[1]);
	return BPoint(x, y);
}

BSize BGraphicObject::stringToSize(std::string& str)
{
	std::vector<std::string> c = strToVector(str, " ");
	const int lenght = c.size();
	lua_Number width, height = 0;
	if (lenght >= 1)
		width = stringToNumber(c[0]);
	if (lenght >= 2)
		height = stringToNumber(c[1]);
	return BSize(width, height);
}

void BGraphicObject::setStyle(BStyle &style)
{
	if (keyExists(style, "visible"))
		_visible = stringToBool(style["visible"]);
	if (keyExists(style, "userinteractionenabled"))
		_userInteractionEnabled = stringToBool(style["userinteractionenabled"]);
	if (keyExists(style, "backgroundcolor"))
		_backgroundColor = stringToColor(style["backgroundcolor"]);
	if (keyExists(style, "width"))
		setWidth(stringToNumber(style["width"]));
	if (keyExists(style, "height"))
		setHeight(stringToNumber(style["height"]));
	if (keyExists(style, "size"))
		setSize(stringToSize(style["size"]));
	if (keyExists(style, "center"))
		_rotationCenter = stringToPoint(style["center"]);
	if (keyExists(style, "rotation"))
		_rotation = (stringToNumber(style["rotation"]));
	if (keyExists(style, "opacity"))
		_opacity = (stringToNumber(style["opacity"]));
	if (keyExists(style, "x"))
		setX((stringToNumber(style["x"])));
	if (keyExists(style, "y"))
		setY((stringToNumber(style["y"])));
	if (keyExists(style, "position"))
		setOrigin(((stringToPoint(style["position"]))));
}

void BGraphicObject::setStyle(const BString & styleName)
{
    if (!BConfigLoader::instance()->isValid())
        return ;
	BStyleTree& styles = BConfigLoader::instance()->styles();
    
	if (styles.find(styleName) != styles.end())
	{
		setStyle(styles[styleName]);
	}
	else
	{
		Log::warn("Warning: Cannot find style [%s]", styleName.c_str());
	}
}

std::string BGraphicObject::toString() const
{
	BString string;
	string << "GraphicObject { visible = " << _visible << ", " << _position.toString() << ", " << _size.toString() << "}";
	return std::string(string.c_str());
}

BRect BGraphicObject::rect()
{
	return *this;
}

void BGraphicObject::setUserInteractionEnabled(bool value)
{
	_userInteractionEnabled = value;
}

void BGraphicObject::setBackgroundColor(const BColor& color)
{
	_backgroundColor = color;
}

void BGraphicObject::addChild(BGraphicObject* child)
{
	_children.push_back(child);
	child->retain();
}

const BGraphicObject::BChildrenMap& BGraphicObject::children() const
{
	return _children;
}

bool BGraphicObject::isUserInteractionEnabled() const
{
	return _userInteractionEnabled;
}

const BColor& BGraphicObject::backgroundColor() const
{
	return _backgroundColor;
}

bool BGraphicObject::isVisible() const
{
	return _visible;
}

void BGraphicObject::setVisible(bool value)
{
	_visible = value;
}

void BGraphicObject::removeChild(BGraphicObject* object)
{
	object->_parent = NULL;
	_children.remove(object);
}

void BGraphicObject::setRotation(lua_Number angle)
{
	_rotation = angle;
}

lua_Number BGraphicObject::rotation() const
{
	return _rotation;
}

bool BGraphicObject::intersect(BGraphicObject* object) const
{
	return 	x() <= ((object)->x() + (object)->width()) && (object)->x() <= (x() + width()) &&
			y() <= ((object)->y() + (object)->height()) && (object)->y() <= (y() + height());
}

void BGraphicObject::dealloc()
{
	if (_parent)
		_parent->removeChild(this);
	BChildrenMap::iterator it = _children.begin();
	BChildrenMap::iterator ite = _children.end();
	for (; it != ite; ++it)
	{
		(*it)->_parent = NULL;
		(*it)->release();
	}
	_children.clear();
}

bool BGraphicObject::release()
{
	if (_parent != NULL)
		return false;
    dealloc();
    return BObject::release();
}

void BGraphicObject::setOpacity(lua_Number opacity)
{
	if (opacity > 1.0)
		_opacity = 1.0;
	else if (opacity < 0.0)
		_opacity = 0.0;
	else
		_opacity = opacity;
}

lua_Number BGraphicObject::opacity() const
{
	return _opacity;
}

BGraphicObject::~BGraphicObject()
{

}

B_CONSTRUCTOR

(void)self;
	BGraphicObject* parent = BClass<BGraphicObject>::self(state, 1);

	if (parent)
	{
		this->_parent = parent;
		parent->addChild(this);
	}

	setWidth(1.0);
	setHeight(1.0);
	_rotationCenter.setX(0.5);
	_rotationCenter.setY(0.5);

B_END

B_IMPLEMENTATION(setUserInteractionEnabled)

	self->setUserInteractionEnabled((bool)lua_toboolean(state, 2));
	return 0;

B_END

B_IMPLEMENTATION(setVisible)
	self->setVisible((bool)lua_toboolean(state, 2));
	return 0;

B_END

B_IMPLEMENTATION(isVisible)

	lua_pushboolean(state, self->isVisible());
	return 1;

B_END

B_IMPLEMENTATION(isUserInteractionEnabled)

	lua_pushboolean(state, self->isUserInteractionEnabled());
	return 1;

B_END

B_IMPLEMENTATION(backgroundColor)

	return BClass<BColor>::newInstance(state, new BColor(self->backgroundColor()));

B_END

B_IMPLEMENTATION(setBackgroundColor)

	BColor c;
	c.fromLua(state, 1);
	self->setBackgroundColor(c);
	return 0;

B_END

B_IMPLEMENTATION(setRotation)

	self->setRotation(luaL_checknumber(state, 2));
	return 0;

B_END

B_IMPLEMENTATION(rotation)

	lua_pushnumber(state, self->rotation());
	return 1;

B_END

B_IMPLEMENTATION(intersects)

	BGraphicObject* o = BClass<BGraphicObject>::self(state, 2);
	if (!o)
	{
		lua_pushnil(state);
		return 1;
	}
	lua_pushboolean(state, self->intersect(o));
	return 1;

B_END

B_IMPLEMENTATION(removeChild)

	BGraphicObject* o = BClass<BGraphicObject>::self(state, 2);
	if (!o)
		return 0;
	self->removeChild(o);
	return 0;

B_END

B_IMPLEMENTATION(setOpacity)

	self->setOpacity(luaL_checknumber(state, 2));
	return 0;

B_END

void BGraphicObject::setSize(const BSize& size)
{
	BSize s = this->size();
	BRect::setSize(size);
	if (s.width() > 0.0)
		_rotationCenter.setX(_rotationCenter.x() / s.width() * width());
	else
		_rotationCenter.setX(width() / 2.0);
	if (s.height() > 0.0)
		_rotationCenter.setY(_rotationCenter.y() / s.height() * height());
	else
		_rotationCenter.setY(height() / 2.0);
}

B_IMPLEMENTATION(setSize)


	int ret = B_FUNC_NAME(BRect, setSize)(state);

	return ret;
B_END

B_IMPLEMENTATION(setCenter)

	self->_rotationCenter.setX(luaL_checknumber(state, 2));
	self->_rotationCenter.setY(luaL_checknumber(state, 3));
	return 0;

B_END

B_IMPLEMENTATION(opacity)

	lua_pushnumber(state, self->opacity());
	return 1;

B_END

B_IMPLEMENTATION(setStyle)

	self->setStyle(luaL_checkstring(state, 2));
	return 0;

B_END

void BGraphicObject::draw(BCanvas * const canvas, const BCanvasContext& context)
{
	if (!isVisible() || context.opacity == 0)
		return ;
	canvas->clipRect(context.rect);
	canvas->pushMatrix();
	onDraw(canvas, context);
	if (_children.size() == 0)
	{
		canvas->unclip();
		canvas->popMatrix();
		return ;
	}
	BChildrenMap::iterator it = _children.begin();
	BChildrenMap::iterator ite = _children.end();

	for (; it != ite; ++it)
	{
		BGraphicObject* object = *it;

		BRect childRect;

		if (canvas->stretched())
			childRect = object->rect();
		else
		{
			childRect.setX(context.rect.x() + object->x() * canvas->scalerX());
			childRect.setY(context.rect.y() + object->y() * canvas->scalerY());
			childRect.setWidth(object->width() * canvas->scalerX());
			childRect.setHeight(object->height() * canvas->scalerY());
		}

		BCanvasContext c = {childRect, context.opacity * object->opacity()};

		object->draw(canvas, c);
	}
	canvas->unclip();
	canvas->popMatrix();
}

void BGraphicObject::moveUp()
{
	BChildrenMap::iterator it = _parent->_children.begin();
	BChildrenMap::iterator ite = _parent->_children.end();
	int index = 0;
	for (; it != ite && *it != this;++it, ++index);
	if (it == ite)
		return ;
	_parent->_children.splice((it++), _parent->_children, it);
}

void BGraphicObject::moveDown()
{
	BChildrenMap::iterator it = _parent->_children.begin();
	BChildrenMap::iterator ite = _parent->_children.end();
	int index = 0;
	for (; it != ite && *it != this;++it, ++index);
	if (it == ite || index == 0)
		return ;
	_parent->_children.remove(this);
	index--;
	it = _parent->_children.begin();
	for (;index > 0; index--)
		++it++;
	_parent->_children.insert(it, this);
}

void BGraphicObject::moveToTop()
{
	_parent->_children.remove(this);
	_parent->_children.insert(_parent->_children.end(), this);
}

void BGraphicObject::moveToBottom()
{
	_parent->_children.remove(this);
	_parent->_children.insert(_parent->_children.begin(), this);
}

void BGraphicObject::onDraw(BCanvas * const canvas, const BCanvasContext& context)
{

}

void BGraphicObject::processEvents(BTouchEvents& events, BEventContext& context)
{
	if (!_userInteractionEnabled)
		return ;

	BChildrenMap::reverse_iterator it = _children.rbegin();
	BChildrenMap::reverse_iterator ite = _children.rend();

    if (_children.size() > 0) {
        BEventContext c = {context.scalers, BRect()};
        c.rect.setX(context.rect.x() + x() * context.scalerX());
        c.rect.setY(context.rect.y() + y() * context.scalerY());
        c.rect.setWidth(this->width() * context.scalerX());
        c.rect.setHeight(this->height() * context.scalerY());
        for (;it != ite /*&& events.size()*/; ++it)
        {
            (*it)->processEvents(events, c);
        }
    }
	if (events.size() == 0)
		return ;

	for (int index = 0; index < events.size(); index++)
	{
		events[index]->toMapCoordinates(context);
		if (processSingleEvent(events[index]))
		{
			events.remove(index);
			index--;
		}
	}
}

bool BGraphicObject::processSingleEvent(const BTouchEvent* event)
{
	return false;
}

B_IMPLEMENTATION(moveUp)
	self->moveUp();
	return 0;
B_END

B_IMPLEMENTATION(moveDown)
	self->moveDown();
	return 0;
B_END

B_IMPLEMENTATION(moveToTop)
	self->moveToTop();
	return 0;
B_END

B_IMPLEMENTATION(moveToBottom)
	self->moveToBottom();
	return 0;
B_END
