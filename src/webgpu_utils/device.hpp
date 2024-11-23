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
  wgpu::Device device;

  // Shader
  wgpu::ShaderModule LoadShaderModule(const std::filesystem::path &path) const {
    return wgpu::utils::LoadShaderModule(device, path);
  }

  // Buffers
  wgpu::Buffer CreateBuffer(wgpu::BufferUsage usage, uint64_t size, const void *data = nullptr) const {
    return wgpu::utils::CreateBuffer(device, usage, size, data);
  }

  wgpu::Buffer CreateVertexBuffer(uint64_t size, const void *data = nullptr) const {
    return wgpu::utils::CreateVertexBuffer(device, size, data);
  }

  wgpu::Buffer CreateIndexBuffer(uint64_t size, const void *data = nullptr) const {
    return wgpu::utils::CreateIndexBuffer(device, size, data);
  }

  wgpu::Buffer CreateUniformBuffer(uint64_t size, const void *data = nullptr) const {
    return wgpu::utils::CreateUniformBuffer(device, size, data);
  }

  wgpu::Buffer CreateStorageBuffer(uint64_t size, const void *data = nullptr) const {
    return wgpu::utils::CreateStorageBuffer(device, size, data);
  }

  // Textures
  void WriteTexture(const wgpu::Texture &texture, glm::uvec2 size, const void *data) const {
    wgpu::utils::WriteTexture(device, texture, size, data);
  }

  void WriteTexture(const wgpu::Texture &texture, glm::uvec3 size, const void *data) const {
    wgpu::utils::WriteTexture(device, texture, size, data);
  }

  wgpu::Texture CreateTexture(wgpu::TextureUsage usage, const wgpu::utils::TextureDescriptor2D& desc, const void* data = nullptr) const {
    return wgpu::utils::CreateTexture(device, usage, desc, data);
  }

  wgpu::Texture CreateBindingTexture(const wgpu::utils::TextureDescriptor2D& desc, const void* data = nullptr) const {
    return wgpu::utils::CreateBindingTexture(device, desc, data);
  }

  wgpu::Texture CreateRenderTexture(const wgpu::utils::TextureDescriptor2D& desc, const void* data = nullptr) const {
    return wgpu::utils::CreateRenderTexture(device, desc, data);
  }

  // Pipeline
  wgpu::RenderPipeline MakeRenderPipeline(const wgpu::utils::RenderPipelineDescriptor &descriptor) const {
    return wgpu::utils::MakeRenderPipeline(device, descriptor);
  }

  // Bind Group
  wgpu::BindGroup MakeBindGroup(
    const wgpu::BindGroupLayout& layout,
    std::initializer_list<wgpu::utils::BindingInitializationHelper> entriesInitializer
  ) const {
    return wgpu::utils::MakeBindGroup(device, layout, entriesInitializer);
  }

  wgpu::BindGroupLayout MakeBindGroupLayout(
    std::initializer_list<wgpu::utils::BindingLayoutEntryInitializationHelper> entriesInitializer
  ) const {
    return wgpu::utils::MakeBindGroupLayout(device, entriesInitializer);
  }
};

}
