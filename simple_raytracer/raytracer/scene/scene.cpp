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

Vector3 Scene::compute_color(const Ray& ray) const
{
    if (objects_.empty())
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

        const Vector3 hit_direction_normal = (hit_point - nearest_shape->position).normalize();
        const Vector3 light_direction_normal = (light_source_ - hit_point).normalize();

        // Lambertian reflection https://lavalle.pl/vr/node197.html max(0,nl)
        const float nl = hit_direction_normal.dot(light_direction_normal);
        const float lambert_intensity = std::max(0.0f, nl);

        const Vector3 lambert_color = nearest_shape->get_color() * lambert_intensity;
        const Vector3 ambient_color = nearest_shape->get_color() * LAMBERT_AMBIENT_INTENSITY; // for better effect
        
        return lambert_color + ambient_color;
    }

    return {};
}

void Scene::add_object(const std::shared_ptr<Shape>& shape)
{
    //position of each shape is translated relative to camera pos (view origin)
    shape->position = point_to_screen(shape->position);
    objects_.push_back(shape);
}

void Scene::add_light_source(const Vector3& light_source)
{
    light_source_ = light_source;
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
