#include "SDL3/SDL.h"
#include "./sdl3webgpu.hpp"
#include "./webgpu_utils.hpp"

#if defined(SDL_PLATFORM_MACOS)
#include <Cocoa/Cocoa.h>
#include <Foundation/Foundation.h>
#include <QuartzCore/CAMetalLayer.h>
#endif

wgpu::Surface SDL_GetWGPUSurface(const wgpu::Instance& instance, SDL_Window* window) {
#if defined(SDL_PLATFORM_EMSCRIPTEN)
  assert(false && "dont use this for emscripten");
#elif defined(SDL_PLATFORM_MACOS)
  {
    NSWindow* nswindow = (__bridge NSWindow*)SDL_GetPointerProperty(
      SDL_GetWindowProperties(window), SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, NULL
    );
    [nswindow.contentView setWantsLayer:YES];
    id metal_layer = [CAMetalLayer layer];
    [nswindow.contentView setLayer:metal_layer];

    wgpu::SurfaceDescriptorFromMetalLayer desc;
    desc.layer = metal_layer;

    return instance.CreateSurface(ToPtr(wgpu::SurfaceDescriptor{
      .nextInChain = &desc,
    }));
  }
// #elif defined(SDL_VIDEO_DRIVER_X11)
//     {
//         Display* x11_display = windowWMInfo.info.x11.display;
//         Window x11_window = windowWMInfo.info.x11.window;
//         return wgpuInstanceCreateSurface(
//             instance,
//             &(WGPUSurfaceDescriptor){
//             .label = NULL,
//                 .nextInChain =
//                 (const WGPUChainedStruct*)&(
//                     WGPUSurfaceDescriptorFromXlibWindow) {
//                 .chain =
//                     (WGPUChainedStruct){
//                         .next = NULL,
//                         .sType = WGPUSType_SurfaceDescriptorFromXlibWindow,
//                 },
//                 .display = x11_display,
//                 .window = x11_window,
//             },
//         });
//     }
// #elif defined(SDL_VIDEO_DRIVER_WAYLAND)
//     {
//         struct wl_display* wayland_display = windowWMInfo.info.wl.display;
//         struct wl_surface* wayland_surface = windowWMInfo.info.wl.display;
//         return wgpuInstanceCreateSurface(
//             instance,
//             &(WGPUSurfaceDescriptor){
//             .label = NULL,
//                 .nextInChain =
//                 (const WGPUChainedStruct*)&(
//                     WGPUSurfaceDescriptorFromWaylandSurface) {
//                 .chain =
//                     (WGPUChainedStruct){
//                         .next = NULL,
//                         .sType =
//                             WGPUSType_SurfaceDescriptorFromWaylandSurface,
//                         },
//                         .display = wayland_display,
//                         .surface = wayland_surface,
//                 },
//         });
//   }
#elif defined(SDL_VIDEO_DRIVER_WINDOWS)
//     {
//         HWND hwnd = windowWMInfo.info.win.window;
//         HINSTANCE hinstance = GetModuleHandle(NULL);
//         return wgpuInstanceCreateSurface(
//             instance,
//             &(WGPUSurfaceDescriptor){
//             .label = NULL,
//                 .nextInChain =
//                 (const WGPUChainedStruct*)&(
//                     WGPUSurfaceDescriptorFromWindowsHWND) {
//                 .chain =
//                     (WGPUChainedStruct){
//                         .next = NULL,
//                         .sType = WGPUSType_SurfaceDescriptorFromWindowsHWND,
//             },
//             .hinstance = hinstance,
//             .hwnd = hwnd,
//         },
//     });
//   }
#error "Unsupported Target"
#endif
}
