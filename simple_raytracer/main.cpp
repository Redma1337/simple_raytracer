#include <iostream>
#include "raytracer/raytracer.h"

enum
{
    WINDOW_WIDTH = 800,
    WINDOW_HEIGHT = 600
};

int main()
{
    Raytracer tracer(WINDOW_WIDTH, WINDOW_HEIGHT);

    tracer.add_sphere({ 0, 100.5f, -1 }, 100, { 1, 1, 1 });
    tracer.add_sphere({ 0, 0, -1 }, 0.5f, { 1, 0, 0 });
    tracer.add_sphere({ -0.8, -0.4f, -1 }, 0.4f, { 0, 1, 0 });
    tracer.add_light({ -2, 1, -4 });

    tracer.start_rendering();
    
    return EXIT_SUCCESS;
}
