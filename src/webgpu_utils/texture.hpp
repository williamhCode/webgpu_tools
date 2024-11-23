#pragma once

#include "webgpu/webgpu_cpp.h"
#include "glm/ext/vector_uint2.hpp"
#include "glm/ext/vector_uint3.hpp"

namespace wgpu::utils {

void WriteTexture(const wgpu::Device &device, const wgpu::Texture &texture, glm::uvec2 size, const void *data);
void WriteTexture(const wgpu::Device &device, const wgpu::Texture &texture, glm::uvec3 size, const void *data);

// NOTE: add more options when needed
struct TextureDescriptor2D {
  glm::uvec2 size;
  wgpu::TextureFormat format;
  uint32_t sampleCount = 1;
};
wgpu::Texture CreateTexture(const wgpu::Device& device, wgpu::TextureUsage usage, const wgpu::utils::TextureDescriptor2D& desc, const void* data = nullptr);
wgpu::Texture CreateBindingTexture(const wgpu::Device& device, const wgpu::utils::TextureDescriptor2D& desc, const void* data = nullptr);
wgpu::Texture CreateRenderTexture(const wgpu::Device& device, const wgpu::utils::TextureDescriptor2D& desc, const void* data = nullptr);

uint32_t GetTexelBlockSizeInBytes(wgpu::TextureFormat textureFormat);

}
