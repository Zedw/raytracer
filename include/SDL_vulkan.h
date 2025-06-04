#ifndef SDL_VULKAN_H_
#define SDL_VULKAN_H_
#include <SDL2/SDL_video.h>
#ifdef __cplusplus
extern "C" {
#endif
static inline void SDL_Vulkan_GetDrawableSize(SDL_Window *window, int *w, int *h)
{
    SDL_GL_GetDrawableSize(window, w, h);
}
#ifdef __cplusplus
}
#endif
#endif // SDL_VULKAN_H_
