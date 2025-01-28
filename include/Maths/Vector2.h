#pragma once

#include <iostream>

struct Vector2
{
    float x;
    float y;

    Vector2();
    Vector2(float, float);
    Vector2(Vector2&);

    const Vector2 &operator+=(Vector2);
    const Vector2 &operator-=(Vector2);
    const Vector2 &operator*=(float);
    Vector2 operator+(Vector2);
    Vector2 operator-(Vector2);
    Vector2 operator*(float);
    const Vector2 operator=(Vector2);

    const bool operator==(const Vector2&);
    const bool operator!=(const Vector2&);

    friend std::ostream &operator<<(std::ostream &, const Vector2);

    const float Magnitude() const;
};