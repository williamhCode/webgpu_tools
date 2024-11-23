#pragma once

#include "webgpu/webgpu_cpp.h"

namespace wgpu::utils {

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

wgpu::RenderPipeline MakeRenderPipeline(
  const wgpu::Device& device, const utils::RenderPipelineDescriptor& descriptor
);

wgpu::PipelineLayout
MakePipelineLayout(const wgpu::Device& device, const std::vector<wgpu::BindGroupLayout>& bgls);

} // namespace wgpu::utils
