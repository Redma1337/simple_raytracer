#pragma once
#include <memory>
#include <vector>

#include "ray.h"
#include "shapes/Sphere.h"

constexpr float LAMBERT_AMBIENT_INTENSITY = 0.1f;

class Scene
{
    float view_matrix_[4][4] = {};

    std::vector<std::shared_ptr<Shape>> objects_{};
public:
    Scene();
    
    Vector3 compute_color(const Ray& ray, int recursion_depth) const;
    void add_object(const std::shared_ptr<Shape>& shape);

    void setup_view_matrix(const Vector3& camera_pos);
    Vector3 point_to_screen(const Vector3& point) const;
};
