#include <iostream>
#include "raytracer/raytracer.h"

enum
{
    WINDOW_WIDTH = 480,
    WINDOW_HEIGHT = 320
};

int main()
{
    const Raytracer tracer(WINDOW_WIDTH, WINDOW_HEIGHT);

    tracer.render_image();
    
    return EXIT_SUCCESS;
}
