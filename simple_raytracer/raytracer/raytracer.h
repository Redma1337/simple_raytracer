#pragma once
#include <GLFW/glfw3.h>
#include <iostream>

class Raytracer
{
    GLFWwindow* window_{}; //init as nullptr
    
    int image_width_;
    int image_height_;
public:
    Raytracer(int image_width, int image_height);

    bool setup_glfw();
    void render_image() const;
};
