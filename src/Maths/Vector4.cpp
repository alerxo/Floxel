#include "Maths/Vector4.h"
#include "Maths/Maths.h"

Vector4::Vector4()
{
    x = 0;
    y = 0;
    z = 0;
    w = 0;
}

Vector4::Vector4(float _x, float _y, float _z, float _w)
{
    x = _x;
    y = _y;
    z = _z;
    w = _w;
}

Vector4::Vector4(Vector4 &other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;
}

const Vector4 &Vector4::operator+=(Vector4 other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;

    return *this;
}

const Vector4 &Vector4::operator-=(Vector4 other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;

    return *this;
}

const Vector4 &Vector4::operator*=(float multiplier)
{
    x *= multiplier;
    y *= multiplier;
    z *= multiplier;
    w *= multiplier;

    return *this;
}

Vector4 Vector4::operator+(Vector4 other)
{
    Vector4 temp(*this);

    temp.x += other.x;
    temp.y += other.y;
    temp.z += other.z;
    temp.w += other.w;

    return temp;
}

Vector4 Vector4::operator-(Vector4 other)
{
    Vector4 temp(*this);

    temp.x -= other.x;
    temp.y -= other.y;
    temp.z -= other.z;
    temp.w -= other.w;
    
    return temp;
}

Vector4 Vector4::operator*(float multiplier)
{
    Vector4 temp(*this);

    temp.x *= multiplier;
    temp.y *= multiplier;
    temp.z *= multiplier;
    temp.w *= multiplier;
    
    return temp;
}

const Vector4 Vector4::operator=(Vector4 other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;

    return *this;
}

const bool Vector4::operator==(const Vector4& other)
{
    return (x == other.x) && (y == other.y) && (z == other.z) && (w == other.w);
}

const bool Vector4::operator!=(const Vector4& other)
{
    return !(*this == other);
}

std::ostream &operator<<(std::ostream &os, const Vector4 vector4)
{
    os << "Vector4(" << vector4.x << ", " << vector4.y << ", " << vector4.z << ", " << vector4.w << ")";

    return os;
}

const float Vector4::Magnitude() const
{
    return Maths::Absolute(w) + Maths::Absolute(z) + Maths::Absolute(y) + Maths::Absolute(z);
}