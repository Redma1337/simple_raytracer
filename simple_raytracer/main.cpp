#include <iostream>
#include "raytracer/raytracer.h"

enum
{
    WINDOW_WIDTH = 1024,
    WINDOW_HEIGHT = 768
};

int main()
{
    Raytracer tracer(WINDOW_WIDTH, WINDOW_HEIGHT);

    tracer.add_sphere({ -4, -4, -10 }, 3, { 1, 0, 0 });
    tracer.add_sphere({ 0, 1, -8 }, 3, { 0, 1, 0 });
    tracer.add_light({ -2, 1, -4 });

    tracer.render_image();
    
    return EXIT_SUCCESS;
}
