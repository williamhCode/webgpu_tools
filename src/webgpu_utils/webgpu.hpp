#pragma once

#include "webgpu/webgpu_cpp.h"
#include "dawn/utils/WGPUHelpers.h"
#include <filesystem>
#include <vector>

namespace wgpu::utils {

namespace fs = std::filesystem;

// clang-format off
wgpu::Adapter RequestAdapter(const wgpu::Instance &instance, const wgpu::RequestAdapterOptions *options);

wgpu::Device RequestDevice(const wgpu::Adapter &instance, const wgpu::DeviceDescriptor *options);

void SetUncapturedErrorCallback(const wgpu::Device &device);

wgpu::ShaderModule LoadShaderModule(const wgpu::Device &device, const fs::path &path);

void PrintLimits(wgpu::Limits const &limits);

wgpu::Buffer CreateBuffer(const wgpu::Device &device, wgpu::BufferUsage usage, size_t size, const void *data = nullptr);
wgpu::Buffer CreateVertexBuffer(const wgpu::Device &device, size_t size, const void *data = nullptr);
wgpu::Buffer CreateIndexBuffer(const wgpu::Device &device, size_t size, const void *data = nullptr);
wgpu::Buffer CreateUniformBuffer(const wgpu::Device &device, size_t size, const void *data = nullptr);
wgpu::Buffer CreateStorageBuffer(const wgpu::Device &device, size_t size, const void *data = nullptr);

wgpu::Texture CreateTexture(const wgpu::Device &device, wgpu::Extent3D size, wgpu::TextureFormat format, const void *data = nullptr);
wgpu::Texture CreateRenderTexture(const wgpu::Device &device, wgpu::Extent3D size, wgpu::TextureFormat format);

struct RenderPassDescriptor : public wgpu::RenderPassDescriptor{
  RenderPassDescriptor(
    std::vector<wgpu::RenderPassColorAttachment> colorAttachments = {},
    wgpu::RenderPassDepthStencilAttachment depthStencilAttachment = wgpu::RenderPassDepthStencilAttachment{}
  );
  ~RenderPassDescriptor() = default;

  RenderPassDescriptor(const RenderPassDescriptor& otherRenderPass);
  const RenderPassDescriptor& operator=(const RenderPassDescriptor& otherRenderPass);

  std::vector<wgpu::RenderPassColorAttachment> cColorAttachments;
  wgpu::RenderPassDepthStencilAttachment cDepthStencilAttachmentInfo = {};
};

struct VertexBufferLayout : public wgpu::VertexBufferLayout {
  VertexBufferLayout(
    uint64_t arrayStride,
    std::vector<wgpu::VertexAttribute> attributes,
    VertexStepMode stepMode = VertexStepMode::Vertex
  );
  ~VertexBufferLayout() = default;

  VertexBufferLayout(const VertexBufferLayout& other);
  const VertexBufferLayout& operator=(const VertexBufferLayout& other);

  std::vector<wgpu::VertexAttribute> cAttributes;
};
// clang-format on

namespace BlendState {
const wgpu::BlendState AlphaBlending = {
  .color{
    .operation = wgpu::BlendOperation::Add,
    .srcFactor = wgpu::BlendFactor::SrcAlpha,
    .dstFactor = wgpu::BlendFactor::OneMinusSrcAlpha,
  },
  .alpha{
    .operation = wgpu::BlendOperation::Add,
    .srcFactor = wgpu::BlendFactor::One,
    .dstFactor = wgpu::BlendFactor::OneMinusSrcAlpha,
  },
};
}

// useful dawn utils
using dawn::utils::MakeBindGroup;
using dawn::utils::MakeBindGroupLayout;
using dawn::utils::MakePipelineLayout;

} // namespace utils

template <typename T>
const T *ToPtr(const T &&value) {
  return &value;
}

template <typename T, size_t N>
constexpr const T *ToPtr(const T (&&a)[N]) {
  return a;
}

