#pragma once

#include <iostream>

struct Vector4
{
    float x;
    float y;
    float z;
    float w;

    Vector4();
    Vector4(float, float, float, float);
    Vector4(Vector4&);

    const Vector4 &operator+=(Vector4);
    const Vector4 &operator-=(Vector4);
    const Vector4 &operator*=(float);
    Vector4 operator+(Vector4);
    Vector4 operator-(Vector4);
    Vector4 operator*(float);
    const Vector4 operator=(Vector4);

    const bool operator==(const Vector4&);
    const bool operator!=(const Vector4&);

    friend std::ostream &operator<<(std::ostream &, const Vector4);

    const float Magnitude() const;
};