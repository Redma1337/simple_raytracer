#pragma once
#include "../shape.h"

class Sphere : Shape
{
public:
    Vector3 center;
    float radius;

    Sphere(const Vector3& center, const float radius)
        : center(center), radius(radius) {}

    // Check if a ray intersects with the sphere
    bool intersect(const Ray& ray, float& t) override;
    Vector3 normalize(const Vector3& hit_point) override;
};
