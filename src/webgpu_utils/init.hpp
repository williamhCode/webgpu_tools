#pragma once

#include "webgpu/webgpu_cpp.h"

namespace wgpu::utils {

wgpu::Adapter RequestAdapter(const wgpu::Instance& instance, const wgpu::RequestAdapterOptions& options);

wgpu::Device RequestDevice(const wgpu::Instance& instance, const wgpu::Adapter& adapter, const wgpu::DeviceDescriptor& descriptor);

void PrintLimits(const wgpu::Limits& limits);

void PrintSurfaceCapabilities(const SurfaceCapabilities& config);

}

