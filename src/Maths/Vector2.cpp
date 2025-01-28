#include "Maths/Vector2.h"
#include "Maths/Maths.h"

Vector2::Vector2()
{
    x = 0;
    y = 0;
}

Vector2::Vector2(float _x, float _y)
{
    x = _x;
    y = _y;
}

Vector2::Vector2(Vector2 &other)
{
    x = other.x;
    y = other.y;
}

const Vector2 &Vector2::operator+=(Vector2 other)
{
    x += other.x;
    y += other.y;

    return *this;
}

const Vector2 &Vector2::operator-=(Vector2 other)
{
    x -= other.x;
    y -= other.y;

    return *this;
}

const Vector2 &Vector2::operator*=(float multiplier)
{
    x *= multiplier;
    y *= multiplier;

    return *this;
}

Vector2 Vector2::operator+(Vector2 other)
{
    Vector2 temp(*this);

    temp.x += other.x;
    temp.y += other.y;

    return temp;
}

Vector2 Vector2::operator-(Vector2 other)
{
    Vector2 temp(*this);

    temp.x -= other.x;
    temp.y -= other.y;
    
    return temp;
}

Vector2 Vector2::operator*(float multiplier)
{
    Vector2 temp(*this);

    temp.x *= multiplier;
    temp.y *= multiplier;
    
    return temp;
}

const Vector2 Vector2::operator=(Vector2 other)
{
    x = other.x;
    y = other.y;

    return *this;
}

const bool Vector2::operator==(const Vector2& other)
{
    return (x == other.x) && (y == other.y);
}

const bool Vector2::operator!=(const Vector2& other)
{
    return !(*this == other);
}

std::ostream &operator<<(std::ostream &os, const Vector2 vector2)
{
    os << "Vector2(" << vector2.x << ", " << vector2.y << ")";

    return os;
}

const float Vector2::Magnitude() const
{
    return Maths::Absolute(x) + Maths::Absolute(y);
}