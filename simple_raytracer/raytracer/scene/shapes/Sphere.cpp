#include "Sphere.h"

bool Sphere::intersect(const Ray& ray, float& t) override
{
    const Vector3 oc = ray.origin - center;
    const float a = ray.direction.x * ray.direction.x + ray.direction.y * ray.direction.y + ray.direction.z * ray.direction.z;
    const float b = 2.0 * (oc.x * ray.direction.x + oc.y * ray.direction.y + oc.z * ray.direction.z);
    const float c = oc.x * oc.x + oc.y * oc.y + oc.z * oc.z - radius * radius;
    const float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return false; // No intersection
    }

    // Find the nearest root that is in the acceptable range
    const float sqrt_discriminant = sqrt(discriminant);
    const float root1 = (-b - sqrt_discriminant) / (2.0 * a);
    const float root2 = (-b + sqrt_discriminant) / (2.0 * a);

    if (root1 > 0.001) {
        t = root1;
        return true;
    } else if (root2 > 0.001) {
        t = root2;
        return true;
    } else {
        return false;
    }
}

Vector3 Sphere::normalize(const Vector3& hit_point)
{
    Vector3 normal = (hit_point - center) * (1.0 / radius);
    normal = normal * (1.0 / radius);
    return normal;
}
