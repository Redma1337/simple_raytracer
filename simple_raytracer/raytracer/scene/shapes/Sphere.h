#pragma once
#include <limits>

#include "../shape.h"

class Sphere : public Shape
{
    double lower_root_boundary_{0.001};
    double upper_root_boundary_{std::numeric_limits<double>::infinity()};
    bool is_solid_;
public:
    float radius;
    Vector3 color;

    Sphere(const Vector3& pos, const float radius, const Vector3& color, bool is_solid)
        : Shape(pos), radius(radius), color(color), is_solid_(is_solid)
    {}

    // Check if a ray intersects with the sphere
    bool intersect(const Ray& ray, float& root) override;
    Ray disperse(const Ray& ray, const Vector3& hit_point, const Vector3& face_normal) override;
    Vector3 get_color() override;
    Vector3 normal_at(const Vector3& point) override;
};
