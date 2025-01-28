#include "Maths/Maths.h"

const float Maths::Absolute(const float value)
{
    return value >= 0 ? value : -value;
}