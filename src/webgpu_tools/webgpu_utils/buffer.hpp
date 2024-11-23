#pragma once

#include "webgpu/webgpu_cpp.h"

namespace wgpu::utils {

wgpu::Buffer CreateBuffer(const wgpu::Device &device, wgpu::BufferUsage usage, uint64_t size, const void *data = nullptr);
wgpu::Buffer CreateVertexBuffer(const wgpu::Device &device, uint64_t size, const void *data = nullptr);
wgpu::Buffer CreateIndexBuffer(const wgpu::Device &device, uint64_t size, const void *data = nullptr);
wgpu::Buffer CreateUniformBuffer(const wgpu::Device &device, uint64_t size, const void *data = nullptr);
wgpu::Buffer CreateStorageBuffer(const wgpu::Device &device, uint64_t size, const void *data = nullptr);

}
