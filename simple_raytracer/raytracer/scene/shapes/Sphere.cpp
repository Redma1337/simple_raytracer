﻿#include "Sphere.h"

#include "../../utils/math.h"

bool Sphere::intersect(const Ray& ray, float& root)
{
    //https://viclw17.github.io/2018/07/16/raytracing-ray-sphere-intersection
    const Vector3 oc = ray.origin - position;
    const float a = ray.direction.dot(ray.direction);
    const float b = 2.0f * oc.dot(ray.direction);
    const float c = oc.dot(oc) - radius * radius;
    const float discriminant = b * b - 4 * a * c;
    
    if(discriminant < 0){
        return false;
    } 

    // since we dont need max precision we dont need to check both roots here
    root = (-b - math::qsqrt(discriminant)) / (2.0 * a);
    return true;
}

Vector3 Sphere::get_color()
{
    return color;
}

Vector3 Sphere::normal_at(const Vector3& point)
{
    return (point - position) / radius;
}
