#pragma once
#include <GLFW/glfw3.h>

#include "scene/scene.h"
#include "utils/math.h"

class Raytracer
{
    GLFWwindow* window_{}; //init as nullptr, keep this raw ptr since glfw manages it
    Scene scene_{};

    Vector3 camera_pos_;
    float fov_;
    int image_width_;
    int image_height_;

    Ray get_ray_to_pixel(int x, int y) const;
public:
    Raytracer(int image_width, int image_height, Vector3 camera_pos = { 0, 0, 0 }, float fov = math::pi / 2);

    bool setup_glfw();

    std::shared_ptr<Sphere> add_sphere(const Vector3& pos, float radius, const Vector3& color);
    void add_light(const Vector3& pos);
    void render_image();
};
