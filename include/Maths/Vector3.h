#pragma once

#include <iostream>

struct Vector3
{
    float x;
    float y;
    float z;

    Vector3();
    Vector3(float, float, float);
    Vector3(Vector3&);

    const Vector3 &operator+=(Vector3);
    const Vector3 &operator-=(Vector3);
    const Vector3 &operator*=(float);
    Vector3 operator+(Vector3);
    Vector3 operator-(Vector3);
    Vector3 operator*(float);
    const Vector3 operator=(Vector3);

    const bool operator==(const Vector3&);
    const bool operator!=(const Vector3&);

    friend std::ostream &operator<<(std::ostream &, const Vector3);

    const float Magnitude() const;
};