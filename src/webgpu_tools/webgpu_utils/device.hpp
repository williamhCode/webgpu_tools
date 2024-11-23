#pragma once

#include "webgpu/webgpu_cpp.h"
#include "./shader.hpp"
#include "./buffer.hpp"
#include "./texture.hpp"
#include "./pipeline.hpp"
#include "./bind_group.hpp"

namespace wgpu::utils {

// all device util wrappers
struct DeviceWrapper {
  wgpu::Device _device;

  void SetDevice(const wgpu::Device &device) {
    _device = device;
  }

  // Shader
  wgpu::ShaderModule LoadShaderModule(const std::filesystem::path &path) const {
    return wgpu::utils::LoadShaderModule(_device, path);
  }

  // Buffers
  wgpu::Buffer CreateBuffer(wgpu::BufferUsage usage, uint64_t size, const void *data = nullptr) const {
    return wgpu::utils::CreateBuffer(_device, usage, size, data);
  }

  wgpu::Buffer CreateVertexBuffer(uint64_t size, const void *data = nullptr) const {
    return wgpu::utils::CreateVertexBuffer(_device, size, data);
  }

  wgpu::Buffer CreateIndexBuffer(uint64_t size, const void *data = nullptr) const {
    return wgpu::utils::CreateIndexBuffer(_device, size, data);
  }

  wgpu::Buffer CreateUniformBuffer(uint64_t size, const void *data = nullptr) const {
    return wgpu::utils::CreateUniformBuffer(_device, size, data);
  }

  wgpu::Buffer CreateStorageBuffer(uint64_t size, const void *data = nullptr) const {
    return wgpu::utils::CreateStorageBuffer(_device, size, data);
  }

  // Textures
  void WriteTexture(const wgpu::Texture &texture, glm::uvec2 size, const void *data) const {
    wgpu::utils::WriteTexture(_device, texture, size, data);
  }

  void WriteTexture(const wgpu::Texture &texture, glm::uvec3 size, const void *data) const {
    wgpu::utils::WriteTexture(_device, texture, size, data);
  }

  wgpu::Texture CreateTexture(wgpu::TextureUsage usage, const wgpu::utils::TextureDescriptor2D& desc, const void* data = nullptr) const {
    return wgpu::utils::CreateTexture(_device, usage, desc, data);
  }

  wgpu::Texture CreateBindingTexture(const wgpu::utils::TextureDescriptor2D& desc, const void* data = nullptr) const {
    return wgpu::utils::CreateBindingTexture(_device, desc, data);
  }

  wgpu::Texture CreateRenderTexture(const wgpu::utils::TextureDescriptor2D& desc, const void* data = nullptr) const {
    return wgpu::utils::CreateRenderTexture(_device, desc, data);
  }

  // Pipeline
  wgpu::RenderPipeline MakeRenderPipeline(const wgpu::utils::RenderPipelineDescriptor &descriptor) const {
    return wgpu::utils::MakeRenderPipeline(_device, descriptor);
  }

  // Bind Group
  wgpu::BindGroup MakeBindGroup(
    const wgpu::BindGroupLayout& layout,
    std::initializer_list<wgpu::utils::BindingInitializationHelper> entriesInitializer
  ) const {
    return wgpu::utils::MakeBindGroup(_device, layout, entriesInitializer);
  }

  wgpu::BindGroupLayout MakeBindGroupLayout(
    std::initializer_list<wgpu::utils::BindingLayoutEntryInitializationHelper> entriesInitializer
  ) const {
    return wgpu::utils::MakeBindGroupLayout(_device, entriesInitializer);
  }
};

}
