#pragma once
#include <algorithm>
#include "ray.h"
#include "shapes/Sphere.h"

class Scene
{
public:
    Vector3 compute_color(const Ray& ray, Shape& shape) const;
};
