#include <SDL2/SDL.h>
#include <iostream>
#include <algorithm>
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

    Vec3 camera(0,0,0);

    auto render = [&](const Vec3& cam){
        double viewport_height = 2.0;
        double viewport_width = viewport_height * (double)width / (double)height;
        Vec3 horizontal(viewport_width, 0, 0);
        Vec3 vertical(0, viewport_height, 0);
        Vec3 lower_left_corner = cam - horizontal/2 - vertical/2 - Vec3(0,0,1);

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

    render(camera);
    SDL_Event e;
    bool quit = false;
    const double step = 0.1;
    bool redraw = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_LEFT:  camera.x -= step; redraw = true; break;
                case SDLK_RIGHT: camera.x += step; redraw = true; break;
                case SDLK_UP:    camera.z -= step; redraw = true; break;
                case SDLK_DOWN:  camera.z += step; redraw = true; break;
                case SDLK_q:     camera.y += step; redraw = true; break;
                case SDLK_e:     camera.y -= step; redraw = true; break;
                case SDLK_ESCAPE: quit = true; break;
                default: break;
                }
            }
        }
        if (redraw) {
            render(camera);
            redraw = false;
        }
        SDL_Delay(16);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
