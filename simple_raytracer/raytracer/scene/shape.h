#pragma once
#include "../utils/vector3.h"
#include <cmath>
#include "ray.h"

class Shape
{
public:
    Vector3 position;
    
    explicit Shape(const Vector3& position)
        : position(position)
    {}
    
    virtual ~Shape() = default;
    virtual bool intersect(const Ray& ray, float& t) = 0;
    virtual Vector3 get_color() = 0;
};
