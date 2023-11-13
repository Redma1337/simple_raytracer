#pragma once
#include "../utils/vector3.h"
#include <cmath>
#include "ray.h"

class Shape
{
public:
    virtual ~Shape() = default;
    virtual bool intersect(const Ray& ray, float& t) = 0;
    virtual Vector3 normalize(const Vector3& hit_point) = 0;
};
