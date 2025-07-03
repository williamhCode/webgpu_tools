#include "./buffer.hpp"

namespace wgpu::utils {

wgpu::Buffer CreateBuffer(
  const wgpu::Device& device, wgpu::BufferUsage usage, uint64_t size, const void* data
) {
  wgpu::BufferDescriptor bufferDesc{
    .usage = wgpu::BufferUsage::CopyDst | usage,
    .size = size,
  };
  wgpu::Buffer buffer = device.CreateBuffer(&bufferDesc);
  if (data) device.GetQueue().WriteBuffer(buffer, 0, data, size);
  return buffer;
}

wgpu::Buffer CreateVertexBuffer(
  const wgpu::Device& device, uint64_t size, const void* data
) {
  return CreateBuffer(device, wgpu::BufferUsage::Vertex, size, data);
}

wgpu::Buffer CreateIndexBuffer(
  const wgpu::Device& device, uint64_t size, const void* data
) {
  return CreateBuffer(device, wgpu::BufferUsage::Index, size, data);
}

wgpu::Buffer CreateUniformBuffer(
  const wgpu::Device& device, uint64_t size, const void* data
) {
  return CreateBuffer(device, wgpu::BufferUsage::Uniform, size, data);
}

wgpu::Buffer CreateStorageBuffer(
  const wgpu::Device& device, uint64_t size, const void* data
) {
  return CreateBuffer(device, wgpu::BufferUsage::Storage, size, data);
}

} // namespace wgpu::utils
