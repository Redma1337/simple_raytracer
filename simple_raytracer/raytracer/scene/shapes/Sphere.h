#pragma once
#include "../shape.h"

class Sphere : public Shape
{
public:
    float radius;
    Vector3 color;

    Sphere(const Vector3& pos, const float radius, const Vector3& color)
        : Shape(pos), radius(radius), color(color)
    {}

    // Check if a ray intersects with the sphere
    bool intersect(const Ray& ray, float& t) override;
    Vector3 get_color() override;
};
