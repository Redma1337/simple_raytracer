#include "Sphere.h"

#include "../../utils/math.h"

bool Sphere::intersect(const Ray& ray, float& root)
{
    //https://viclw17.github.io/2018/07/16/raytracing-ray-sphere-intersection
    const Vector3 oc = ray.origin - position;
    const float a = ray.direction.dot(ray.direction);
    const float half_b = oc.dot(ray.direction);
    const float c = oc.dot(oc) - radius * radius;
    const float discriminant = half_b * half_b - a * c;
    
    if(discriminant < 0){
        return false;
    }
    const auto sqrt_discriminant = math::qsqrt(discriminant);

    //check both results of the quadratic equation to ensure they are in valid range
    root = (-half_b - sqrt_discriminant) / a;
    if (root <= lower_root_boundary_ || upper_root_boundary_ <= root) {
        root = (-half_b + sqrt_discriminant) / a;
        if (root <= lower_root_boundary_ || upper_root_boundary_ <= root)
            return false;
    }
    
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
