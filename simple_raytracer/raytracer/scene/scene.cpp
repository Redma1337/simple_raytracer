#include "scene.h"

Scene::Scene()
{
    /**
     * The translation matrix looks like:
     * | 1 0 0 0 |
     * | 0 1 0 0 |
     * | 0 0 1 0 |
     * | 0 0 0 1 |
     */
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            view_matrix_[i][j] = i == j ? 1.0f : 0.0f;
        }
    }
}

Vector3 Scene::compute_color(const Ray& ray, int recursion_depth) const
{
    if (objects_.empty() || recursion_depth <= 0)
        return {};

    // find the nearest shape, so the order in the vector doesnt define which is the most foreground
    float nearest_hit_distance = std::numeric_limits<float>::infinity();
    std::shared_ptr<Shape> nearest_shape = nullptr;

    for (const std::shared_ptr<Shape>& shape : objects_)
    {
        float local_hit_distance;

        if (shape->intersect(ray, local_hit_distance) && local_hit_distance < nearest_hit_distance)
        {
            nearest_hit_distance = local_hit_distance;
            nearest_shape = shape;
        }
    }
    
    if (nearest_shape)
    {
        const Vector3 hit_point = ray.origin + ray.direction * nearest_hit_distance;
        const Vector3 outward_normal = nearest_shape->normal_at(hit_point);
        const bool front_face = ray.direction.dot(outward_normal) < 0;
        const Vector3 face_adjusted_normal = front_face ? outward_normal : outward_normal * -1;
        

        const Ray scattered = nearest_shape->disperse(ray, hit_point, face_adjusted_normal);
        return nearest_shape->get_color() * compute_color(scattered, recursion_depth-1);
    }

    // Set up a gradient background
    const float t = 0.5f * (ray.direction.normalize().y + 1.0f); // Vertical position normalized to [0, 1]
    const Vector3 background_color = Vector3(1.0f, 1.0f, 1.0f) * (1.0f - t) + Vector3(0.5f, 0.7f, 1.0f) * t;
    
    return background_color;
}

void Scene::add_object(const std::shared_ptr<Shape>& shape)
{
    //position of each shape is translated relative to camera pos (view origin)
    shape->position = point_to_screen(shape->position);
    objects_.push_back(shape);
}

void Scene::setup_view_matrix(const Vector3& camera_pos)
{
    /**
     * Relative matrix looks like
     * | 1 0 0 x |
     * | 0 1 0 y |
     * | 0 0 1 z |
     * | 0 0 0 1 |
     */
    view_matrix_[0][3] = camera_pos.x;
    view_matrix_[1][3] = camera_pos.y;
    view_matrix_[2][3] = camera_pos.z;
}

Vector3 Scene::point_to_screen(const Vector3& point) const
{
    /**
     * https://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/
     * 
     * | a b c d |   | x |   | a*x + b*y + c*z + d |
     * | e f g h | * | y | = | e*x + f*y + g*z + h |
     * | i j k l |   | z |   | i*x + j*y + k*z + l |
     * | m n o p |  <- we can ignore w because we dont have perspective changes
     */
    float x = view_matrix_[0][0] * point.x + view_matrix_[0][1] * point.y + view_matrix_[0][2] * point.z + view_matrix_[0][3];
    float y = view_matrix_[1][0] * point.x + view_matrix_[1][1] * point.y + view_matrix_[1][2] * point.z + view_matrix_[1][3];
    float z = view_matrix_[2][0] * point.x + view_matrix_[2][1] * point.y + view_matrix_[2][2] * point.z + view_matrix_[2][3];
    float w = view_matrix_[3][0] * point.x + view_matrix_[3][1] * point.y + view_matrix_[3][2] * point.z + view_matrix_[3][3];

    return {x, y, z};
}
