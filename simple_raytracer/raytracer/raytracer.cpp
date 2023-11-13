#include "raytracer.h"

Raytracer::Raytracer(const int image_width, const int image_height)
    : image_width_{ image_width }, image_height_{ image_height }
{
    if (!setup_glfw())
    {
        std::cout << "Failed to setup glfw context." << std::endl;
    }
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

void Raytracer::render_image() const
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

        for (int x = 0; x <= 50; x++) {
            for (int y = 0; y <= 50; y++) {
                glColor3f(1, 0, 0);
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

