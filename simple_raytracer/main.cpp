#include <iostream>
#include "raytracer/raytracer.h"

enum
{
    WINDOW_WIDTH = 800,
    WINDOW_HEIGHT = 450
};

int main()
{
    Raytracer tracer(WINDOW_WIDTH, WINDOW_HEIGHT, 50);

    tracer.add_sphere({ 0, 100.5f, -1 }, 100, { 0.5f, 0.2f, 0.0f });
    tracer.add_sphere({ 0, 0, -1 }, 0.5f, { 0.2f, 0.5f, 0.2f });
    tracer.add_sphere({ -1, -0.3f, -1 }, 0.5f, { 0.8f, 0.8f, 0.8f }, false);
    tracer.add_sphere({ 1, 0, -1 }, 0.5f, { 0.8f, 0.6f, 0.2f }, false);

    tracer.start_rendering();
    
    return EXIT_SUCCESS;
}
