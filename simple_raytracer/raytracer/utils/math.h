#pragma once

namespace math
{
    static float pi = 3.14159265358979f;

    //quake inverse square root algorithm
    inline float qsqrt(float x)
    {
        union
        {
            int i;
            float f;
        } u;

        u.f = x;
        u.i = (1 << 29) + (u.i >> 1) - (1 << 22);

        u.f = u.f + x / u.f;
        u.f = 0.25f * u.f + x / u.f;

        return u.f;
    }
}
