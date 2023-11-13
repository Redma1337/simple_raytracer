#include "scene.h"

Vector3 Scene::compute_color(const Ray& ray, Shape& shape) const
{
    float t;
    if (shape.intersect(ray, t)) {
        // Hit point on the sphere
        const Vector3 hit_point = ray.origin + ray.direction * t;

        // Normal vector at the hit point
        Vector3 normal = shape.normalize(hit_point);

        // Lambertian shading (simple diffuse reflection)
        const float intensity = std::max(0.0f, normal.x * ray.direction.x + normal.y * ray.direction.y + normal.z * ray.direction.z);
        return Vector3{ 1, 1, 1 } * intensity;
    }

    // Background color (black in this case)
    return { 0, 0, 0 };
}
