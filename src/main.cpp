#include <SDL2/SDL.h>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include "renderer.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

int main(int argc, char* argv[]) {
    (void)argc; (void)argv; // unused
    int width = 400;
    int height = 300;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL init failed\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Raytracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Failed to create window\n";
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    std::vector<Uint32> pixels(width*height);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

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

        for (int j = 0; j < height; ++j) {
            for (int i = 0; i < width; ++i) {
                double u = (double)i / (width-1);
                double v = (double)(height-1-j) / (height-1);
                Ray r(cam, lower_left_corner + horizontal*u + vertical*v - cam);
                Vec3 color = trace(r, scene);
                int ir = static_cast<int>(255.999 * std::clamp(color.x, 0.0, 1.0));
                int ig = static_cast<int>(255.999 * std::clamp(color.y, 0.0, 1.0));
                int ib = static_cast<int>(255.999 * std::clamp(color.z, 0.0, 1.0));
                pixels[j*width + i] = (0xFF<<24) | (ir<<16) | (ig<<8) | ib;
            }
        }
        SDL_UpdateTexture(texture, nullptr, pixels.data(), width * sizeof(Uint32));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    };

    render(camera, yaw, pitch);

    SDL_Event e;
    bool quit = false;
    const double moveSpeed = 2.0;
    const double rotateSpeed = 1.5;
    Uint32 lastTicks = SDL_GetTicks();
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL2_ProcessEvent(&e);
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
                width = e.window.data1;
                height = e.window.data2;
                pixels.assign(width * height, 0);
                SDL_DestroyTexture(texture);
                texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
            }
        }

        Uint32 current = SDL_GetTicks();
        double dt = (current - lastTicks) / 1000.0;
        lastTicks = current;
        scene.time += dt * 0.5;

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        const Uint8* state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_ESCAPE]) quit = true;

        Vec3 forward(
            std::cos(pitch) * std::sin(yaw),
            std::sin(pitch),
            -std::cos(pitch) * std::cos(yaw));
        Vec3 right = normalize(cross(Vec3(0,1,0), forward));
        Vec3 upVec = cross(forward, right);

        ImGui::Begin("Lighting");
        float lightColor[3] = {(float)scene.lightColor.x, (float)scene.lightColor.y, (float)scene.lightColor.z};
        if (ImGui::ColorEdit3("Color", lightColor)) {
            scene.lightColor = Vec3(lightColor[0], lightColor[1], lightColor[2]);
        }
        float dir[3] = {(float)scene.lightDir.x, (float)scene.lightDir.y, (float)scene.lightDir.z};
        if (ImGui::SliderFloat3("Direction", dir, -1.0f, 1.0f)) {
            scene.lightDir = normalize(Vec3(dir[0], dir[1], dir[2]));
        }
        ImGui::End();

        if (state[SDL_SCANCODE_W]) { camera = camera + forward * moveSpeed * dt; }
        if (state[SDL_SCANCODE_S]) { camera = camera - forward * moveSpeed * dt; }
        if (state[SDL_SCANCODE_A]) { camera = camera - right * moveSpeed * dt; }
        if (state[SDL_SCANCODE_D]) { camera = camera + right * moveSpeed * dt; }
        if (state[SDL_SCANCODE_Q]) { camera = camera + upVec * moveSpeed * dt; }
        if (state[SDL_SCANCODE_E]) { camera = camera - upVec * moveSpeed * dt; }

        if (state[SDL_SCANCODE_UP])    { pitch += rotateSpeed * dt; }
        if (state[SDL_SCANCODE_DOWN])  { pitch -= rotateSpeed * dt; }
        if (state[SDL_SCANCODE_LEFT])  { yaw   += rotateSpeed * dt; }
        if (state[SDL_SCANCODE_RIGHT]) { yaw   -= rotateSpeed * dt; }

        const double maxPitch = M_PI/2 - 0.01;
        if (pitch > maxPitch) pitch = maxPitch;
        if (pitch < -maxPitch) pitch = -maxPitch;

        ImGui::Render();
        render(camera, yaw, pitch);

        SDL_Delay(1);
    }

    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
