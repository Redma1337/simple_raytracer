#include "raytracer.h"
#include <iomanip>
#include <iostream>

Raytracer::Raytracer(const int image_width, const int image_height, const Vector3 camera_pos, const float fov)
    : camera_pos_{camera_pos}, fov_{fov}, image_width_{image_width}, image_height_{image_height}
{
    if (!setup_glfw())
    {
        std::cout << "Failed to setup glfw context." << std::endl;
    }

    scene_.setup_view_matrix(camera_pos_);
}

bool Raytracer::setup_glfw()
{
    //internal glfw setup
    if (!glfwInit())
        return false;

    //init our window ptr
    window_ = glfwCreateWindow(
        image_width_,
        image_height_,
        "Raytrace Render",
        nullptr,
        nullptr
    );

    //something went wrong while trying to set window ptr
    if (!window_)
    {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window_);

    glViewport(0, 0, image_width_, image_height_);
    glMatrixMode(GL_PROJECTION);
    glOrtho(0, image_width_, 0, image_height_, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    return true;
}

//wrap the functionality of scene
std::shared_ptr<Sphere> Raytracer::add_sphere(const Vector3& pos, const float radius, const Vector3& color)
{
    const auto new_sphere = std::make_shared<Sphere>(pos, radius, color);
    scene_.add_object(new_sphere);
    return new_sphere;
}

void Raytracer::add_light(const Vector3& pos)
{
    scene_.add_light_source(pos);
}

Ray Raytracer::get_ray_to_pixel(int x, int y) const
{
    const float u = (2.0 * x - image_width_) / image_width_;
    const float v = (image_height_ - 2.0 * y) / image_height_;
    const float aspect_ratio = image_width_ / image_height_;
    const float ray_x = std::tan(fov_ / 2) * u * aspect_ratio;
    const float ray_y = std::tan(fov_ / 2) * v;

    auto norm_dir_vec = Vector3{ ray_x, ray_y, -1 }.normalize();
    return { camera_pos_, norm_dir_vec };
}

void Raytracer::render_image()
{
    if (window_ == nullptr)
    {
        glfwTerminate();
        return;
    }



    while (!glfwWindowShouldClose(window_))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glPointSize(1.0f);
        glBegin(GL_POINTS);

        for (int x = 0; x <= image_width_; x++) {
            for (int y = 0; y <= image_height_; y++) {
                auto pixel_ray = get_ray_to_pixel(x, y);
                const auto pixel_color = scene_.compute_color(pixel_ray);
                
                glColor3f(pixel_color.x, pixel_color.y, pixel_color.z);
                glVertex2i(x, y);
            }
        }

        glEnd();

        glfwSwapBuffers(window_);

        glfwPollEvents();
    }

    //this cleans up for us
    glfwTerminate();
}


