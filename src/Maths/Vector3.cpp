#include "Maths/Vector3.h"
#include "Maths/Maths.h"

Vector3::Vector3()
{
    x = 0;
    y = 0;
    z = 0;
}

Vector3::Vector3(float _x, float _y, float _z)
{
    x = _x;
    y = _y;
    z = _z;
}

Vector3::Vector3(Vector3 &other)
{
    x = other.x;
    y = other.y;
    z = other.z;
}

const Vector3 &Vector3::operator+=(Vector3 other)
{
    x += other.x;
    y += other.y;
    z += other.z;

    return *this;
}

const Vector3 &Vector3::operator-=(Vector3 other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;

    return *this;
}

const Vector3 &Vector3::operator*=(float multiplier)
{
    x *= multiplier;
    y *= multiplier;
    z *= multiplier;

    return *this;
}

Vector3 Vector3::operator+(Vector3 other)
{
    Vector3 temp(*this);

    temp.x += other.x;
    temp.y += other.y;
    temp.z += other.z;

    return temp;
}

Vector3 Vector3::operator-(Vector3 other)
{
    Vector3 temp(*this);

    temp.x -= other.x;
    temp.y -= other.y;
    temp.z -= other.z;
    
    return temp;
}

Vector3 Vector3::operator*(float multiplier)
{
    Vector3 temp(*this);

    temp.x *= multiplier;
    temp.y *= multiplier;
    temp.z *= multiplier;
    
    return temp;
}

const Vector3 Vector3::operator=(Vector3 other)
{
    x = other.x;
    y = other.y;
    z = other.z;

    return *this;
}

const bool Vector3::operator==(const Vector3& other)
{
    return (x == other.x) && (y == other.y) && (z == other.z);
}

const bool Vector3::operator!=(const Vector3& other)
{
    return !(*this == other);
}

std::ostream &operator<<(std::ostream &os, const Vector3 vector3)
{
    os << "Vector3(" << vector3.x << ", " << vector3.y << ", " << vector3.z << ")";

    return os;
}

const float Vector3::Magnitude() const
{
    return Maths::Absolute(z) + Maths::Absolute(y) + Maths::Absolute(z);
}