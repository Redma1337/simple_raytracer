﻿#include "Sphere.h"

#include "../../utils/math.h"

bool Sphere::intersect(const Ray& ray, float& root)
{
    const Vector3 oc = ray.origin - position;
    const float a = ray.direction.dot(ray.direction);
    const float half_b = oc.dot(ray.direction);
    const float c = oc.dot(oc) - radius * radius;
    const float discriminant = half_b * half_b - a * c;

    //kein Ergebnis
    if(discriminant < 0){
        return false;
    }

    //1 oder 2  Ergebnisse
    const auto sqrt_discriminant = math::qsqrt(discriminant);

    //beide Ergebnisse im Interval von 0.001 und unendlich prüfen
    root = (-half_b - sqrt_discriminant) / a;
    if (root <= lower_root_boundary_ || upper_root_boundary_ <= root) {
        root = (-half_b + sqrt_discriminant) / a;
        if (root <= lower_root_boundary_ || upper_root_boundary_ <= root)
            return false;
    }
    
    return true;
}

Ray Sphere::disperse(const Ray& ray, const Vector3& hit_point, const Vector3& face_normal)
{
    if (is_solid_)
    {
        const Vector3 lambertion_direction = face_normal + Vector3::random_unit_vector();
        return { hit_point, lambertion_direction };
    }

    const Vector3 reflected_direction = Vector3::reflect(ray.direction.normalize(), face_normal);
    return { hit_point, reflected_direction };
}

Vector3 Sphere::get_color()
{
    return color;
}

Vector3 Sphere::normal_at(const Vector3& point)
{
    return (point - position) / radius;
}
