#pragma once

#include "webgpu/webgpu_cpp.h"
#include "dawn/utils/WGPUHelpers.h"
#include <filesystem>
#include <memory>
#include <vector>

namespace wgpu::utils {

// clang-format off
wgpu::Adapter RequestAdapter(const wgpu::Instance &instance, const wgpu::RequestAdapterOptions *options);

wgpu::Device RequestDevice(const wgpu::Adapter &instance, const wgpu::DeviceDescriptor *descriptor);

void SetUncapturedErrorCallback(const wgpu::Device &device);

wgpu::ShaderModule LoadShaderModule(const wgpu::Device &device, const std::filesystem::path &path);

void PrintLimits(wgpu::Limits const &limits);

wgpu::Buffer CreateBuffer(const wgpu::Device &device, wgpu::BufferUsage usage, size_t size, const void *data = nullptr);
wgpu::Buffer CreateVertexBuffer(const wgpu::Device &device, size_t size, const void *data = nullptr);
wgpu::Buffer CreateIndexBuffer(const wgpu::Device &device, size_t size, const void *data = nullptr);
wgpu::Buffer CreateUniformBuffer(const wgpu::Device &device, size_t size, const void *data = nullptr);
wgpu::Buffer CreateStorageBuffer(const wgpu::Device &device, size_t size, const void *data = nullptr);

wgpu::Texture CreateTexture(const wgpu::Device& device, wgpu::TextureUsage usage, wgpu::Extent3D size, wgpu::TextureFormat format, const void* data);
wgpu::Texture CreateBindingTexture(const wgpu::Device &device, wgpu::Extent3D size, wgpu::TextureFormat format, const void *data = nullptr);
wgpu::Texture CreateRenderTexture(const wgpu::Device &device, wgpu::Extent3D size, wgpu::TextureFormat format, const void *data = nullptr);

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

struct VertexBufferLayout : public wgpu::VertexBufferLayout {
  VertexBufferLayout(
    uint64_t arrayStride,
    std::vector<wgpu::VertexAttribute> attributes,
    VertexStepMode stepMode = VertexStepMode::Vertex
  );
  ~VertexBufferLayout() = default;

  VertexBufferLayout(const VertexBufferLayout& other);
  VertexBufferLayout& operator=(const VertexBufferLayout& other);

  std::vector<wgpu::VertexAttribute> cAttributes;
};

struct RenderPipelineMaker{
  wgpu::PipelineLayout layout;
  ShaderModule module;
  std::vector<wgpu::utils::VertexBufferLayout> buffers;
  wgpu::PrimitiveState primitive;
  wgpu::DepthStencilState depthStencil;
  wgpu::MultisampleState multisample;
  ShaderModule fsModule;
  std::vector<wgpu::ColorTargetState> targets;

  wgpu::RenderPipeline Make(const wgpu::Device &device);
};
// clang-format on

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

// useful dawn utils
using dawn::utils::MakeBindGroup;
using dawn::utils::MakeBindGroupLayout;
using dawn::utils::MakePipelineLayout;

} // namespace wgpu::utils

template <typename T>
const T* ToPtr(const T&& value) {
  return &value;
}

template <typename T, size_t N>
constexpr const T* ToPtr(const T (&&a)[N]) {
  return a;
}
