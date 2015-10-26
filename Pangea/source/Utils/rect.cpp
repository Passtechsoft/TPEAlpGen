#include "rect.h"

Rect::Rect() :
m_posX  (0),
m_posY   (0),
m_width (0),
m_height(0)
{

}


Rect::Rect(float rectm_posX, float rectm_posY, float rectm_width, float rectm_height) :
m_posX  (rectm_posX),
m_posY   (rectm_posY),
m_width (rectm_width),
m_height(rectm_height)
{

}


Rect::Rect(const sf::Vector2f& position, const sf::Vector2f& size) :
m_posX(position.x),
m_posY(position.y),
m_width(size.x),
m_height(size.y)
{

}


Rect::Rect(const Rect& rectangle) :
m_posX  (rectangle.m_posX),
m_posY   (rectangle.m_posY),
m_width (rectangle.m_width),
m_height(rectangle.m_height)
{
}


bool Rect::contains(float x, float y) const
{
    // Rectangles with negative dimensions are allowed, so we must handle them correctly

    // Compute the real min and max of the rectangle on both axes
    float minX = std::min(m_posX, m_posX + m_width);
    float maxX = std::max(m_posX, m_posX + m_width);
    float minY = std::min(m_posY, m_posY + m_height);
    float maxY = std::max(m_posY, m_posY + m_height);

    return (x >= minX) && (x < maxX) && (y >= minY) && (y < maxY);
}


bool Rect::contains(const sf::Vector2f& point) const
{
    return contains(point.x, point.y);
}


bool Rect::intersects(const Rect& rectangle) const
{
    Rect intersection;
    return intersects(rectangle, intersection);
}

PublicRect Rect::getRect()
{
	return PublicRect(m_posX, m_posY, m_width, m_height);
}

bool Rect::intersects(const Rect& rectangle, Rect& intersection) const
{
    // Rectangles with negative dimensions are allowed, so we must handle them correctly

    // Compute the min and max of the first rectangle on both axes
    float r1MinX = std::min(m_posX, m_posX + m_width);
    float r1MaxX = std::max(m_posX, m_posX + m_width);
    float r1MinY = std::min(m_posY, m_posY + m_height);
    float r1MaxY = std::max(m_posY, m_posY + m_height);

    // Compute the min and max of the second rectangle on both axes
    float r2MinX = std::min(rectangle.m_posX, rectangle.m_posX + rectangle.m_width);
    float r2MaxX = std::max(rectangle.m_posX, rectangle.m_posX + rectangle.m_width);
    float r2MinY = std::min(rectangle.m_posY, rectangle.m_posY + rectangle.m_height);
    float r2MaxY = std::max(rectangle.m_posY, rectangle.m_posY + rectangle.m_height);

    // Compute the intersection boundaries
    float interm_posX   = std::max(r1MinX, r2MinX);
    float interm_posY    = std::max(r1MinY, r2MinY);
    float interRight  = std::min(r1MaxX, r2MaxX);
    float interBottom = std::min(r1MaxY, r2MaxY);

    // If the intersection is valid (positive non zero area), then there is an intersection
    if ((interm_posX < interRight) && (interm_posY < interBottom))
    {
        intersection = Rect(interm_posX, interm_posY, interRight - interm_posX, interBottom - interm_posY);
        return true;
    }
    else
    {
        intersection = Rect(0, 0, 0, 0);
        return false;
    }
}

///\note Check 1
void Rect::setZone(float newPosX, float newPosY, float newWidth, float newHeight)
{
    m_posX=newPosX;
    m_posY=newPosY;
    if(newWidth)
        m_width=newWidth;
    if(newHeight)
        m_height=newHeight;
}

void Rect::setPosition(float newPosX, float newPosY)
{
    setZone(newPosX, newPosY);
}

void Rect::setWidthHeight(float newWidth, float newHeight)
{
	setZone(m_posX, m_posY, newWidth, newHeight);
}

inline bool operator ==(Rect& left, Rect& right)
{
    return (left.getRect().x == right.getRect().x) && (left.getRect().y == right.getRect().y) &&
           (left.getRect().width == right.getRect().width) && (left.getRect().height == right.getRect().height);
}


inline bool operator !=(const Rect& m_posX, const Rect& right)
{
    return !(m_posX == right);
}
