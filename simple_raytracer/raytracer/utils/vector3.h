#pragma once
#include <cmath>

#include "math.h"

class Vector3
{
public:
    float x, y, z;

    Vector3() : x(0), y(0), z(0) {}
    Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}

    Vector3 operator+(const Vector3& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }

    Vector3 operator-(const Vector3& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }

    Vector3 operator*(const Vector3& other) const {
        return {x * other.x, y * other.y, z * other.z};
    }

    Vector3 operator/(const Vector3& other) const {
        return {x / other.x, y / other.y, z / other.z};
    }

    Vector3 operator*(const float scalar) const {
        return {x * scalar, y * scalar, z * scalar};
    }

    Vector3 operator/(const float scalar) const {
        return {x / scalar, y / scalar, z / scalar};
    }

    Vector3 operator-(const float scalar) const {
        return {x - scalar, y - scalar, z - scalar};
    }
    
    Vector3 operator+(const float scalar) const {
        return {x + scalar, y + scalar, z + scalar};
    }

    float dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vector3 cross(const Vector3& other) const {
        return {y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x};
    }

    float length() const {
        return math::qsqrt(x * x + y * y + z * z);
    }

    static Vector3 reflect(const Vector3& v, const Vector3& n) {
        return v - n * 2 * v.dot(n);
    }
    
    Vector3 normalize() const {
        const float magnitude = length();
        if (magnitude != 0.f) {
            return{ x / magnitude, y / magnitude, z / magnitude };
        }
        
        return { };
    }

    static Vector3 random_unit_vector()
    {
        while (true)
        {
            auto rng_v = random_vector(-1, 1);
            if (rng_v.length() < 1) {
                return rng_v.normalize();
            }
        }
    }

    static Vector3 random_vector(const float min, const float max)
    {
        return {
            math::random_float(min,max),
            math::random_float(min,max),
            math::random_float(min,max)
        };
    }
};
