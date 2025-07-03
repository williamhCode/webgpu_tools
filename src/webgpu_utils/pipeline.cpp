#include "./pipeline.hpp"
#include "./to_ptr.hpp"

namespace wgpu::utils {

// VertexBufferLayout::VertexBufferLayout(
//   uint64_t arrayStride,
//   std::vector<wgpu::VertexAttribute> attributes,
//   VertexStepMode stepMode
// ) : cAttributes(std::move(attributes)) {
//   this->arrayStride = arrayStride;
//   this->stepMode = stepMode;
//   attributeCount = cAttributes.size();
//   this->attributes = cAttributes.data();
// }

// VertexBufferLayout::VertexBufferLayout(const VertexBufferLayout& other) {
//   *this = other;
// }

// VertexBufferLayout& VertexBufferLayout::operator=(const VertexBufferLayout& other
// ) {
//   cAttributes = other.cAttributes;
//   arrayStride = other.arrayStride;
//   stepMode = other.stepMode;
//   attributeCount = other.attributeCount;
//   attributes = cAttributes.data();
//   return *this;
// }

RenderPipeline MakeRenderPipeline(
  const wgpu::Device& device, const utils::RenderPipelineDescriptor& desc
) {
  // convert vertex buffer layout
  std::vector<wgpu::VertexBufferLayout> buffers;
  std::vector<std::vector<wgpu::VertexAttribute>> attributess;

  for (const auto& buffer : desc.buffers) {
    auto& attributes = attributess.emplace_back();

    for (size_t j = 0; j < buffer.attributes.size(); j++) {
      const auto& attr = buffer.attributes[j];
      attributes.push_back({
        .format = attr.format,
        .offset = attr.offset,
        .shaderLocation = (uint32_t)j,
      });
    }

    buffers.push_back({
      .stepMode = buffer.stepMode,
      .arrayStride = buffer.arrayStride,
      .attributeCount = attributes.size(),
      .attributes = attributes.data(),
    });
  }

  return device.CreateRenderPipeline(ToPtr(wgpu::RenderPipelineDescriptor{
    .layout = MakePipelineLayout(device, desc.bgls),
    .vertex{
      .module = desc.vs,
      .entryPoint = "vs_main",
      .bufferCount = buffers.size(),
      .buffers = buffers.data(),
    },
    .primitive = desc.primitive,
    .depthStencil = desc.depthStencil.format == TextureFormat::Undefined
      ? nullptr : &desc.depthStencil,
    .multisample = desc.multisample,
    .fragment = desc.fs
      ? ToPtr(FragmentState{
        .module = desc.fs,
        .entryPoint = "fs_main",
        .targetCount = desc.targets.size(),
        .targets = desc.targets.data(),
      }) : nullptr,
  }));
}

wgpu::PipelineLayout MakePipelineLayout(
  const wgpu::Device& device, const std::vector<wgpu::BindGroupLayout>& bgls
) {
  return device.CreatePipelineLayout(ToPtr(wgpu::PipelineLayoutDescriptor{
    .bindGroupLayoutCount = uint32_t(bgls.size()),
    .bindGroupLayouts = bgls.data(),
  }));
}

} // namespace wgpu::utils
