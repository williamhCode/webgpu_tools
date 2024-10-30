#pragma once

#include "webgpu/webgpu_cpp.h"
#include "dawn_utils.hpp"
#include "glm/ext/vector_uint2.hpp"
#include "glm/ext/vector_uint3.hpp"
#include <filesystem>
#include <vector>

namespace wgpu::utils {

wgpu::Adapter RequestAdapter(wgpu::Instance& instance, const wgpu::RequestAdapterOptions& options);

wgpu::Device RequestDevice(const wgpu::Adapter& adapter, const wgpu::DeviceDescriptor& descriptor);

wgpu::ShaderModule LoadShaderModule(const wgpu::Device &device, const std::filesystem::path &path);

void PrintLimits(const wgpu::Limits& limits);
void PrintSurfaceCapabilities(const SurfaceCapabilities& config);

wgpu::Buffer CreateBuffer(const wgpu::Device &device, wgpu::BufferUsage usage, uint64_t size, const void *data = nullptr);
wgpu::Buffer CreateVertexBuffer(const wgpu::Device &device, uint64_t size, const void *data = nullptr);
wgpu::Buffer CreateIndexBuffer(const wgpu::Device &device, uint64_t size, const void *data = nullptr);
wgpu::Buffer CreateUniformBuffer(const wgpu::Device &device, uint64_t size, const void *data = nullptr);
wgpu::Buffer CreateStorageBuffer(const wgpu::Device &device, uint64_t size, const void *data = nullptr);

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

struct RenderPassDescriptor : public wgpu::RenderPassDescriptor{
  RenderPassDescriptor(
    std::vector<wgpu::RenderPassColorAttachment> colorAttachments = {},
    wgpu::RenderPassDepthStencilAttachment depthStencilAttachment = {}
  );
  ~RenderPassDescriptor() = default;

  RenderPassDescriptor(const RenderPassDescriptor& otherRenderPass);
  RenderPassDescriptor& operator=(const RenderPassDescriptor& otherRenderPass);

  std::vector<wgpu::RenderPassColorAttachment> cColorAttachments;
  wgpu::RenderPassDepthStencilAttachment cDepthStencilAttachmentInfo = {};
};

// struct VertexBufferLayout : public wgpu::VertexBufferLayout {
//   VertexBufferLayout(
//     uint64_t arrayStride,
//     std::vector<wgpu::VertexAttribute> attributes,
//     VertexStepMode stepMode = VertexStepMode::Vertex
//   );
//   ~VertexBufferLayout() = default;

//   VertexBufferLayout(const VertexBufferLayout& other);
//   VertexBufferLayout& operator=(const VertexBufferLayout& other);

//   std::vector<wgpu::VertexAttribute> cAttributes;
// };

struct VertexAttribute {
    wgpu::VertexFormat format;
    uint64_t offset;
};

struct VertexBufferLayout {
    uint64_t arrayStride;
    std::vector<wgpu::utils::VertexAttribute> attributes;
    VertexStepMode stepMode = VertexStepMode::Vertex;
};

// custom render pipeline descriptor
// use std::span in c++26
// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p2447r4.html
// can use initializer list here too but idk
struct RenderPipelineDescriptor{
  wgpu::ShaderModule vs;
  wgpu::ShaderModule fs;
  const std::vector<wgpu::BindGroupLayout>& bgls;
  const std::vector<wgpu::utils::VertexBufferLayout>& buffers;
  const std::vector<wgpu::ColorTargetState>& targets;
  wgpu::PrimitiveState primitive;
  wgpu::DepthStencilState depthStencil;
  wgpu::MultisampleState multisample;
};

wgpu::RenderPipeline MakeRenderPipeline(const wgpu::Device &device, const utils::RenderPipelineDescriptor &descriptor);

struct BlendComponent {
  static constexpr wgpu::BlendComponent Replace = {
    .operation = wgpu::BlendOperation::Add,
    .srcFactor = wgpu::BlendFactor::One,
    .dstFactor = wgpu::BlendFactor::Zero,
  };

  static constexpr wgpu::BlendComponent Over = {
    .operation = wgpu::BlendOperation::Add,
    .srcFactor = wgpu::BlendFactor::One,
    .dstFactor = wgpu::BlendFactor::OneMinusSrcAlpha,
  };
};

struct BlendState {
  static constexpr wgpu::BlendState Replace = {
    .color = BlendComponent::Replace,
    .alpha = BlendComponent::Replace,
  };

  static constexpr wgpu::BlendState AlphaBlending = {
    .color{
      .operation = wgpu::BlendOperation::Add,
      .srcFactor = wgpu::BlendFactor::SrcAlpha,
      .dstFactor = wgpu::BlendFactor::OneMinusSrcAlpha,
    },
    .alpha = BlendComponent::Over,
  };

  static constexpr wgpu::BlendState PremultipliedAlphaBlending = {
    .color = BlendComponent::Over,
    .alpha = BlendComponent::Over,
  };
};

// useful dawn utils (copied over cuz emscripeten dont have)
using dawn::utils::MakeBindGroup;
using dawn::utils::MakeBindGroupLayout;
using dawn::utils::MakePipelineLayout;

} // namespace wgpu::utils

template <typename T>
const T* ToPtr(T&& value) {
  return &value;
}

// template <typename T, size_t N>
// constexpr const T* ToPtr(const T (&&a)[N]) {
//   return a;
// }
