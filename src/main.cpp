#include <SDL2/SDL.h>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "renderer.h"

int main(int argc, char* argv[]) {
    (void)argc; (void)argv; // unused
    const int width = 400;
    const int height = 300;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL init failed\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Raytracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
    if (!window) {
        std::cerr << "Failed to create window\n";
        SDL_Quit();
        return 1;
    }

    SDL_Surface* surface = SDL_GetWindowSurface(window);
    if (!surface) {
        std::cerr << "Failed to get surface\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Scene scene;
    scene.sphere = Sphere(Vec3(0,0,-3), 0.5, Material(Vec3(1,0,0), 0.7));
    scene.plane = Plane(Vec3(0,-0.5,-3), Vec3(0,1,0), Material(Vec3(0.8,0.8,0.8), 0.2));
    scene.lightDir = normalize(Vec3(-0.5, 1.0, -0.5));
    scene.lightColor = Vec3(1.0, 1.0, 0.9);

    Vec3 camera(0,0,0);
    double yaw = 0.0;
    double pitch = 0.0;

    auto render = [&](const Vec3& cam, double yawAngle, double pitchAngle){
        double viewport_height = 2.0;
        double viewport_width = viewport_height * (double)width / (double)height;
        Vec3 forward(
            std::cos(pitchAngle) * std::sin(yawAngle),
            std::sin(pitchAngle),
            -std::cos(pitchAngle) * std::cos(yawAngle));
        Vec3 right = normalize(cross(Vec3(0,1,0), forward));
        Vec3 up = cross(forward, right);
        Vec3 horizontal = right * viewport_width;
        Vec3 vertical = up * viewport_height;
        Vec3 lower_left_corner = cam + forward - horizontal/2 - vertical/2;

        Uint32* pixels = static_cast<Uint32*>(surface->pixels);
        for (int j = 0; j < height; ++j) {
            for (int i = 0; i < width; ++i) {
                double u = (double)i / (width-1);
                double v = (double)(height-1-j) / (height-1);
                Ray r(cam, lower_left_corner + horizontal*u + vertical*v - cam);
                Vec3 color = trace(r, scene);
                int ir = static_cast<int>(255.999 * std::clamp(color.x, 0.0, 1.0));
                int ig = static_cast<int>(255.999 * std::clamp(color.y, 0.0, 1.0));
                int ib = static_cast<int>(255.999 * std::clamp(color.z, 0.0, 1.0));
                pixels[j*surface->w + i] = SDL_MapRGB(surface->format, ir, ig, ib);
            }
        }
        SDL_UpdateWindowSurface(window);
    };

    render(camera, yaw, pitch);

    SDL_Event e;
    bool quit = false;
    const double moveSpeed = 2.0;
    const double rotateSpeed = 1.5;
    Uint32 lastTicks = SDL_GetTicks();
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        Uint32 current = SDL_GetTicks();
        double dt = (current - lastTicks) / 1000.0;
        lastTicks = current;

        const Uint8* state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_ESCAPE]) quit = true;

        Vec3 forward(
            std::cos(pitch) * std::sin(yaw),
            std::sin(pitch),
            -std::cos(pitch) * std::cos(yaw));
        Vec3 right = normalize(cross(Vec3(0,1,0), forward));
        Vec3 upVec = cross(forward, right);

        bool moved = false;

        if (state[SDL_SCANCODE_W]) { camera = camera + forward * moveSpeed * dt; moved = true; }
        if (state[SDL_SCANCODE_S]) { camera = camera - forward * moveSpeed * dt; moved = true; }
        if (state[SDL_SCANCODE_A]) { camera = camera - right * moveSpeed * dt; moved = true; }
        if (state[SDL_SCANCODE_D]) { camera = camera + right * moveSpeed * dt; moved = true; }
        if (state[SDL_SCANCODE_Q]) { camera = camera + upVec * moveSpeed * dt; moved = true; }
        if (state[SDL_SCANCODE_E]) { camera = camera - upVec * moveSpeed * dt; moved = true; }

        if (state[SDL_SCANCODE_UP])    { pitch += rotateSpeed * dt; moved = true; }
        if (state[SDL_SCANCODE_DOWN])  { pitch -= rotateSpeed * dt; moved = true; }
        if (state[SDL_SCANCODE_LEFT])  { yaw   += rotateSpeed * dt; moved = true; }
        if (state[SDL_SCANCODE_RIGHT]) { yaw   -= rotateSpeed * dt; moved = true; }

        const double maxPitch = M_PI/2 - 0.01;
        if (pitch > maxPitch) pitch = maxPitch;
        if (pitch < -maxPitch) pitch = -maxPitch;

        if (moved) {
            render(camera, yaw, pitch);
        }

        SDL_Delay(1);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
