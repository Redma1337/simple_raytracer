#pragma once
#include <future>
#include <GLFW/glfw3.h>

#include "scene/scene.h"
#include "utils/math.h"

class Raytracer
{
    GLFWwindow* window_{}; //init as nullptr, keep this raw ptr since glfw manages it
    Scene scene_{};

    Vector3 camera_pos_;
    int max_depth_;
    int image_width_, image_height_;
    int available_threads_, chunk_size_;

    std::vector<std::vector<Vector3>> image_pixels_;
    std::vector<std::future<void>> futures_;
    Vector3 top_left_pixel_loc_;
    Vector3 pixel_delta_u_;
    Vector3 pixel_delta_v_;

    Ray get_ray_to_pixel(int x, int y) const;
    void render_image_chunk(int x_start_pos, int x_end_pos);
    void stop_rendering_thread() const;
    void spawn_render_threads();
    void render_frame() const;

    void update_light_circular(Vector3 center, float time, float radius, float speed);
public:
    Raytracer(int image_width, int image_height, int max_depth, Vector3 camera_pos = { 0, 0, 0 });

    bool setup_glfw();

    std::shared_ptr<Sphere> add_sphere(const Vector3& pos, float radius, const Vector3& color, bool is_solid = true);
    void start_rendering();
};
