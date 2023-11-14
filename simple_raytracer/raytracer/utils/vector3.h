#pragma once
#include <cmath>

class Vector3
{
public:
    float x, y, z;

    Vector3() : x(0), y(0), z(0) {}
    Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}

    Vector3 operator+(const Vector3& other) const {
        return Vector3{x + other.x, y + other.y, z + other.z};
    }

    Vector3 operator-(const Vector3& other) const {
        return Vector3{x - other.x, y - other.y, z - other.z};
    }

    Vector3 operator*(const float scalar) const {
        return Vector3{x * scalar, y * scalar, z * scalar};
    }

    Vector3 operator*(const Vector3& other) const {
        return Vector3{x * other.x, y * other.y, z * other.z};
    }

    float dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    //reflection of vector on surface = I − 2 * dot(I, N) * N
    Vector3 reflect(const Vector3& norm_other) const {
        return *this - norm_other * (2.0f * this->dot(norm_other));
    }

    Vector3 normalize() const {
        const float magnitude = std::sqrt(x * x + y * y + z * z);

        if (magnitude != 0.f) {
            return{ x / magnitude, y / magnitude, z / magnitude };
        }
        
        return { };
    }
};
