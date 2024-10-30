#pragma once

#include "webgpu/webgpu_cpp.h"

struct SDL_Window;

wgpu::Surface SDL_GetWGPUSurface(const wgpu::Instance& instance, SDL_Window* window);
