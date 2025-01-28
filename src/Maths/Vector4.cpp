#include "Maths/Vector4.h"
#include "Maths/Maths.h"

Vector4::Vector4()
{
    w = 0;
    x = 0;
    y = 0;
    z = 0;
}

Vector4::Vector4(float _w, float _x, float _y, float _z)
{
    w = _w;
    x = _x;
    y = _y;
    z = _z;
}

Vector4::Vector4(Vector4 &other)
{
    w = other.w;
    x = other.x;
    y = other.y;
    z = other.z;
}

const Vector4 &Vector4::operator+=(Vector4 other)
{
    w += other.w;
    x += other.x;
    y += other.y;
    z += other.z;

    return *this;
}

const Vector4 &Vector4::operator-=(Vector4 other)
{
    w -= other.w;
    x -= other.x;
    y -= other.y;
    z -= other.z;

    return *this;
}

const Vector4 &Vector4::operator*=(float multiplier)
{
    w *= multiplier;
    x *= multiplier;
    y *= multiplier;
    z *= multiplier;

    return *this;
}

Vector4 Vector4::operator+(Vector4 other)
{
    Vector4 temp(*this);

    temp.w += other.w;
    temp.x += other.x;
    temp.y += other.y;
    temp.z += other.z;

    return temp;
}

Vector4 Vector4::operator-(Vector4 other)
{
    Vector4 temp(*this);

    temp.w -= other.w;
    temp.x -= other.x;
    temp.y -= other.y;
    temp.z -= other.z;
    
    return temp;
}

Vector4 Vector4::operator*(float multiplier)
{
    Vector4 temp(*this);

    temp.w *= multiplier;
    temp.x *= multiplier;
    temp.y *= multiplier;
    temp.z *= multiplier;
    
    return temp;
}

const Vector4 Vector4::operator=(Vector4 other)
{
    w = other.w;
    x = other.x;
    y = other.y;
    z = other.z;

    return *this;
}

const bool Vector4::operator==(const Vector4& other)
{
    return (w == other.w) && (x == other.x) && (y == other.y) && (z == other.z);
}

const bool Vector4::operator!=(const Vector4& other)
{
    return !(*this == other);
}

std::ostream &operator<<(std::ostream &os, const Vector4 vector4)
{
    os << "Vector4(" << vector4.w << ", " << vector4.x << ", " << vector4.y << ", " << vector4.z << ")";

    return os;
}

const float Vector4::Magnitude() const
{
    return Maths::Absolute(w) + Maths::Absolute(z) + Maths::Absolute(y) + Maths::Absolute(z);
}