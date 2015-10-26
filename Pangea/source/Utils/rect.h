
/// Info: This class is very similary to sf::Rect of the
/// SFML - Simple and Fast Multimedia Library wrote by Laurent Gomilla (laurent@sfml-dev.org)
/// Have a small thought to him!
#ifndef	RECT_H_INCLUDED
#define RECT_H_INCLUDED

#include "general.h"

class PublicRect
{
public:
	PublicRect(){};
	PublicRect(float x_, float y_) : x(x_), y(y_) {}
	PublicRect(float x_, float y_, float width_, float height_) : x(x_), y(y_), width(width_), height(height_) {}
	float x=0, y=0, width=0, height=0;
};

class Rect
{
public:

    Rect();

    Rect(float rectLeft, float rectTop, float rectWidth, float rectHeight);

    Rect(const sf::Vector2f& position, const sf::Vector2f& size);

    explicit Rect(const Rect& rectangle);

    bool contains(float x, float y) const;

    bool contains(const sf::Vector2f& point) const;

    bool intersects(const Rect& rectangle) const;

    bool intersects(const Rect& rectangle, Rect& intersection) const;

    PublicRect getRect();

    void setZone(float newPosX, float newPosY, float newWidth=0, float newHeight=0);

    void setWidthHeight(float newWidth, float newHeight);

    void setPosition(float newPosX, float newPosY);

protected:
    float m_posX;
    float m_posY;
    float m_width;
    float m_height;
};

bool operator ==(const Rect& left, const Rect& right);

bool operator !=(const Rect& left, const Rect& right);

#endif // RECT_H_INCLUDED
