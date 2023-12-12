#include "raytracer.h"
#include <iomanip>
#include <iostream>

Raytracer::Raytracer(const int image_width, const int image_height, const int max_depth, const Vector3 camera_pos)
    : camera_pos_{camera_pos}, max_depth_{max_depth}, image_width_{image_width}, image_height_{image_height}
{
    if (!setup_glfw())
    {
        std::cout << "Failed to setup glfw context." << std::endl;
    }

    available_threads_ = std::thread::hardware_concurrency();
    chunk_size_ = image_width_ / available_threads_;
    image_pixels_.resize(image_width_, std::vector<Vector3>(image_height_));

    const auto viewport_height = 2.0;
    const auto viewport_width = viewport_height * (static_cast<double>(image_width_)/image_height_);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    const auto viewport_u = Vector3(viewport_width, 0, 0);
    const auto viewport_v = Vector3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    pixel_delta_u_ = viewport_u / image_width_;
    pixel_delta_v_ = viewport_v / image_height_;

    // Calculate the location of the upper left pixel.
    const auto viewport_upper_left = camera_pos_ - viewport_u/2 - viewport_v/2;
    top_left_pixel_loc_ = viewport_upper_left + (pixel_delta_u_ + pixel_delta_v_) * 0.5f;


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
std::shared_ptr<Sphere> Raytracer::add_sphere(const Vector3& pos, const float radius, const Vector3& color, const bool is_solid)
{
    const auto new_sphere = std::make_shared<Sphere>(pos, radius, color, is_solid);
    scene_.add_object(new_sphere);
    return new_sphere;
}

void Raytracer::start_rendering()
{
    while (!glfwWindowShouldClose(window_))
    {
        if (window_ == nullptr)
        {
            glfwTerminate();
            return;
        }
        
        spawn_render_threads();

        render_frame();
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

Ray Raytracer::get_ray_to_pixel(const int x, const int y) const
{
    const auto pixel_center = top_left_pixel_loc_ + pixel_delta_u_ * x + pixel_delta_v_ * y;
    const auto ray_direction = pixel_center - camera_pos_ - Vector3(0, 0, 1); // this is only needed if we move the camera
    return { camera_pos_, ray_direction };
}

void Raytracer::render_image_chunk(const int x_start_pos, const int x_end_pos)
{
    for (int x = x_start_pos; x < x_end_pos; ++x)
    {
        for (int y = 0; y < image_height_; ++y)
        {
            auto pixel_ray = get_ray_to_pixel(x, y);
            image_pixels_[x][y] = scene_.compute_color(pixel_ray, max_depth_);
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


