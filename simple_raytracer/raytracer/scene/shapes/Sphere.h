#pragma once
#include <limits>

#include "../shape.h"

class Sphere : public Shape
{
    double lower_root_boundary_{0};
    double upper_root_boundary_{std::numeric_limits<double>::infinity()};
public:
    float radius;
    Vector3 color;

    Sphere(const Vector3& pos, const float radius, const Vector3& color)
        : Shape(pos), radius(radius), color(color)
    {}

    // Check if a ray intersects with the sphere
    bool intersect(const Ray& ray, float& root) override;
    Vector3 get_color() override;
    Vector3 normal_at(const Vector3& point) override;
};
