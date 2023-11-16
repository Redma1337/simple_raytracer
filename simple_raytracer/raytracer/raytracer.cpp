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

    available_threads_ = std::thread::hardware_concurrency();
    chunk_size_ = image_width_ / available_threads_;
    image_pixels_.resize(image_width_, std::vector<Vector3>(image_height_));

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

void Raytracer::update_light(float time)
{
    // Update the light source position in a circular motion
    float radius = 10.0f;  // Adjust the radius as needed
    float angular_speed = 0.5f;  // Adjust the angular speed as needed

    float angle = angular_speed * time;
    float light_source_x = 0 + radius * std::cos(angle);
    float light_source_y = 1;
    float light_source_z = -5 + radius * std::sin(angle) ;


    scene_.add_light_source(Vector3(light_source_x, light_source_y, light_source_z));
}

void Raytracer::start_rendering()
{
    auto start_time = std::chrono::high_resolution_clock::now();
    
    while (!glfwWindowShouldClose(window_))
    {
        if (window_ == nullptr)
        {
            glfwTerminate();
            return;
        }
        
        spawn_render_threads();

        render_frame();

        // sample fps
        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

        update_light(static_cast<float>(elapsed_time) / 1000.f);
    }

    stop_rendering_thread();
    glfwTerminate();
}

void Raytracer::render_frame() const
{
    glBegin(GL_POINTS);

    for (int x = 0; x < image_width_; ++x)
    {
        for (int y = 0; y < image_height_; ++y)
        {
            const auto& pixel_color = image_pixels_[x][y];
            glColor3f(pixel_color.x, pixel_color.y, pixel_color.z);
            glVertex2i(x, y);
        }
    }

    glfwSwapBuffers(window_);
    glfwPollEvents();

    glEnd();
}

Ray Raytracer::get_ray_to_pixel(int x, int y) const
{
    const float half_fov = fov_ / 2.0f;
    const float aspect_ratio = image_width_ / image_height_;
    const float tan_half_fov = std::tan(half_fov);

    const float u = (2.0f * x - image_width_) / image_width_;
    const float v = (image_height_ - 2.0f * y) / image_height_;

    const float ray_x = tan_half_fov * u * aspect_ratio;
    const float ray_y = tan_half_fov * v;

    auto norm_dir_vec = Vector3{ ray_x, ray_y, -1.0f }.normalize();

    return { camera_pos_, norm_dir_vec };
}

void Raytracer::render_image_chunk(const int x_start_pos, const int x_end_pos)
{
    for (int x = x_start_pos; x < x_end_pos; ++x)
    {
        for (int y = 0; y < image_height_; ++y)
        {
            auto pixel_ray = get_ray_to_pixel(x, y);
            image_pixels_[x][y] = scene_.compute_color(pixel_ray);
        }
    }
}


void Raytracer::stop_rendering_thread() const
{
    for (auto& future : futures_)
    {
        future.wait();
    }
}

void Raytracer::spawn_render_threads()
{
    futures_.clear();
    
    for (int i = 0; i < available_threads_; ++i)
    {
        int chunk_start_x = i * chunk_size_;
        int chunk_end_x = (i == available_threads_ - 1) ? image_width_ : (i + 1) * chunk_size_;

        futures_.emplace_back(
            std::async(
                std::launch::async,
                &Raytracer::render_image_chunk,
                this,
                chunk_start_x,
                chunk_end_x
            )
        );
    }

    // Wait for all threads to finish rendering
    for (auto& future : futures_)
    {
        future.wait();
    }
}


