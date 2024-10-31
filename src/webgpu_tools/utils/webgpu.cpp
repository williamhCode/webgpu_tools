#include "./webgpu.hpp"
#include "glm/fwd.hpp"
#include "webgpu/webgpu_cpp.h"
#include "magic_enum.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <print>

#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#endif

namespace wgpu::utils {

using namespace wgpu;

Adapter RequestAdapter(Instance& instance, const RequestAdapterOptions& options) {
  wgpu::Adapter adapter;
  instance.WaitAny(
    instance.RequestAdapter(
      &options, wgpu::CallbackMode::WaitAnyOnly,
      [](
        wgpu::RequestAdapterStatus status, wgpu::Adapter result,
        wgpu::StringView message, wgpu::Adapter* userdata
      ) {
        if (status != wgpu::RequestAdapterStatus::Success) {
          std::cout << "Could not get WebGPU adapter: " << message.data << "\n";
          return;
        }
        *userdata = std::move(result);
      },
      &adapter
    ),
    UINT64_MAX
  );

  return adapter;

// #if defined(__EMSCRIPTEN__)
//   auto onAdapterRequestEnded = [](
//                                  WGPURequestAdapterStatus status, WGPUAdapter adapter,
//                                  char const* message, void* pUserData
//                                ) {
//     UserData& userData = *static_cast<UserData*>(pUserData);
//     if (status == WGPURequestAdapterStatus_Success) {
//       userData.adapter = adapter;
//     } else {
//       std::cout << "Could not get WebGPU adapter: " << message << "\n";
//     }
//     userData.requestEnded = true;
//   };

//   instance.RequestAdapter(options, onAdapterRequestEnded, &userData);

//   while (!userData.requestEnded) {
//     emscripten_sleep(100);
//   }
//   return userData.adapter;
// #endif
}

Device RequestDevice(const Adapter& adapter, const DeviceDescriptor& descriptor) {
  auto instance = adapter.GetInstance();
  wgpu::Device device;
  instance.WaitAny(
    adapter.RequestDevice(
      &descriptor, wgpu::CallbackMode::WaitAnyOnly,
      [](
        wgpu::RequestDeviceStatus status, wgpu::Device result, wgpu::StringView message,
        wgpu::Device* userdata
      ) {
        if (status != wgpu::RequestDeviceStatus::Success) {
          std::cout << "Could not get WebGPU device: " << message.data << "\n";
          return;
        }
        *userdata = std::move(result);
      },
      &device
    ),
    UINT64_MAX
  );

  return device;

// #ifdef __EMSCRIPTEN__
//   auto onDeviceRequestEnded = [](
//                                 WGPURequestDeviceStatus status, WGPUDevice device,
//                                 char const* message, void* pUserData
//                               ) {
//     UserData& userData = *static_cast<UserData*>(pUserData);
//     if (status == WGPURequestDeviceStatus_Success) {
//       userData.device = device;
//     } else {
//       std::cout << "Could not get WebGPU adapter: " << message << "\n";
//     }
//     userData.requestEnded = true;
//   };

//   instance.RequestDevice(descriptor, onDeviceRequestEnded, &userData);

//   while (!userData.requestEnded) {
//     emscripten_sleep(100);
//   }

//   return userData.device;
// #endif
}

ShaderModule LoadShaderModule(const Device& device, const std::filesystem::path& path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open shader file" + path.string());
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  auto source = buffer.str();

  wgpu::ShaderModuleWGSLDescriptor wgslDesc;
  wgslDesc.code = source.c_str();
  wgpu::ShaderModuleDescriptor descriptor;
  descriptor.nextInChain = &wgslDesc;
  return device.CreateShaderModule(&descriptor);
}

// clang-format off
void PrintLimits(const wgpu::Limits &limits) {
  std::cout << " - maxTextureDimension1D: " << limits.maxTextureDimension1D << "\n";
  std::cout << " - maxTextureDimension2D: " << limits.maxTextureDimension2D << "\n";
  std::cout << " - maxTextureDimension3D: " << limits.maxTextureDimension3D << "\n";
  std::cout << " - maxTextureArrayLayers: " << limits.maxTextureArrayLayers << "\n";
  std::cout << " - maxBindGroups: " << limits.maxBindGroups << "\n";
  std::cout << " - maxBindGroupsPlusVertexBuffers: " << limits.maxBindGroupsPlusVertexBuffers << "\n";
  std::cout << " - maxBindingsPerBindGroup: " << limits.maxBindingsPerBindGroup << "\n";
  std::cout << " - maxDynamicUniformBuffersPerPipelineLayout: " << limits.maxDynamicUniformBuffersPerPipelineLayout << "\n";
  std::cout << " - maxDynamicStorageBuffersPerPipelineLayout: " << limits.maxDynamicStorageBuffersPerPipelineLayout << "\n";
  std::cout << " - maxSampledTexturesPerShaderStage: " << limits.maxSampledTexturesPerShaderStage << "\n";
  std::cout << " - maxSamplersPerShaderStage: " << limits.maxSamplersPerShaderStage << "\n";
  std::cout << " - maxStorageBuffersPerShaderStage: " << limits.maxStorageBuffersPerShaderStage << "\n";
  std::cout << " - maxStorageTexturesPerShaderStage: " << limits.maxStorageTexturesPerShaderStage << "\n";
  std::cout << " - maxUniformBuffersPerShaderStage: " << limits.maxUniformBuffersPerShaderStage << "\n";
  std::cout << " - maxUniformBufferBindingSize: " << limits.maxUniformBufferBindingSize << "\n";
  std::cout << " - maxStorageBufferBindingSize: " << limits.maxStorageBufferBindingSize << "\n";
  std::cout << " - minUniformBufferOffsetAlignment: " << limits.minUniformBufferOffsetAlignment << "\n";
  std::cout << " - minStorageBufferOffsetAlignment: " << limits.minStorageBufferOffsetAlignment << "\n";
  std::cout << " - maxVertexBuffers: " << limits.maxVertexBuffers << "\n";
  std::cout << " - maxBufferSize: " << limits.maxBufferSize << "\n";
  std::cout << " - maxVertexAttributes: " << limits.maxVertexAttributes << "\n";
  std::cout << " - maxVertexBufferArrayStride: " << limits.maxVertexBufferArrayStride << "\n";
  std::cout << " - maxInterStageShaderComponents: " << limits.maxInterStageShaderComponents << "\n";
  std::cout << " - maxInterStageShaderVariables: " << limits.maxInterStageShaderVariables << "\n";
  std::cout << " - maxColorAttachments: " << limits.maxColorAttachments << "\n";
  std::cout << " - maxColorAttachmentBytesPerSample: " << limits.maxColorAttachmentBytesPerSample << "\n";
  std::cout << " - maxComputeWorkgroupStorageSize: " << limits.maxComputeWorkgroupStorageSize << "\n";
  std::cout << " - maxComputeInvocationsPerWorkgroup: " << limits.maxComputeInvocationsPerWorkgroup << "\n";
  std::cout << " - maxComputeWorkgroupSizeX: " << limits.maxComputeWorkgroupSizeX << "\n";
  std::cout << " - maxComputeWorkgroupSizeY: " << limits.maxComputeWorkgroupSizeY << "\n";
  std::cout << " - maxComputeWorkgroupSizeZ: " << limits.maxComputeWorkgroupSizeZ << "\n";
  std::cout << " - maxComputeWorkgroupsPerDimension: " << limits.maxComputeWorkgroupsPerDimension << "\n";
}
// clang-format on

void PrintSurfaceCapabilities(const SurfaceCapabilities& config) {
  for (size_t i = 0; i < config.formatCount; i++) {
    std::println("Format: {}", magic_enum::enum_name(config.formats[i]));
  }
  for (size_t i = 0; i < config.presentModeCount; i++) {
    std::println("Present Mode: {}", magic_enum::enum_name(config.presentModes[i]));
  }
  for (size_t i = 0; i < config.alphaModeCount; i++) {
    std::println("Alpha Mode: {}", magic_enum::enum_name(config.alphaModes[i]));
  }
}

wgpu::Buffer CreateBuffer(const wgpu::Device& device, wgpu::BufferUsage usage, uint64_t size, const void* data) {
  BufferDescriptor bufferDesc{
    .usage = BufferUsage::CopyDst | usage,
    .size = size,
  };
  Buffer buffer = device.CreateBuffer(&bufferDesc);
  if (data) device.GetQueue().WriteBuffer(buffer, 0, data, size);
  return buffer;
}

wgpu::Buffer CreateVertexBuffer(const wgpu::Device& device, uint64_t size, const void* data) {
  return CreateBuffer(device, BufferUsage::Vertex, size, data);
}

wgpu::Buffer CreateIndexBuffer(const wgpu::Device& device, uint64_t size, const void* data) {
  return CreateBuffer(device, BufferUsage::Index, size, data);
}

wgpu::Buffer CreateUniformBuffer(const wgpu::Device& device, uint64_t size, const void* data) {
  return CreateBuffer(device, BufferUsage::Uniform, size, data);
}

wgpu::Buffer CreateStorageBuffer(const wgpu::Device& device, uint64_t size, const void* data) {
  return CreateBuffer(device, BufferUsage::Storage, size, data);
}

void WriteTexture(
  const wgpu::Device& device,
  const wgpu::Texture& texture,
  glm::uvec2 size,
  const void* data
) {
  WriteTexture(device, texture, {size.x, size.y, 1}, data);
}

void WriteTexture(
  const wgpu::Device& device,
  const wgpu::Texture& texture,
  glm::uvec3 _size,
  const void* data
) {
  wgpu::Extent3D size{_size.x, _size.y, _size.z};
  auto texelBlockSize = dawn::utils::GetTexelBlockSizeInBytes(texture.GetFormat());
  ImageCopyTexture destination{
    .texture = texture,
  };
  TextureDataLayout dataLayout{
    .bytesPerRow = size.width * texelBlockSize,
    .rowsPerImage = size.height,
  };
  device.GetQueue().WriteTexture(
    &destination, data, size.width * size.height * texelBlockSize, &dataLayout, &size
  );
}

wgpu::Texture CreateTexture(
  const wgpu::Device& device,
  wgpu::TextureUsage usage,
  const wgpu::utils::TextureDescriptor2D& desc,
  const void* data
) {
  wgpu::TextureDescriptor textureDesc{
    .usage = TextureUsage::CopyDst | usage,
    .size = Extent3D(desc.size.x, desc.size.y),
    .format = desc.format,
    .sampleCount = desc.sampleCount,
  };
  Texture texture = device.CreateTexture(&textureDesc);
  if (data) {
    WriteTexture(device, texture, desc.size, data);
  }
  return texture;
}

wgpu::Texture CreateBindingTexture(
  const wgpu::Device& device,
  const wgpu::utils::TextureDescriptor2D& descriptor,
  const void* data
) {
  return CreateTexture(device, TextureUsage::TextureBinding, descriptor, data);
}

wgpu::Texture CreateRenderTexture(
  const wgpu::Device& device,
  const wgpu::utils::TextureDescriptor2D& descriptor,
  const void* data
) {
  return CreateTexture(
    device, TextureUsage::RenderAttachment | TextureUsage::TextureBinding, descriptor,
    data
  );
}

// wgpu::Texture CreateTexture(
//   const wgpu::Device& device,
//   wgpu::TextureUsage usage,
//   glm::uvec2 _size,
//   wgpu::TextureFormat format,
//   const void* data
// ) {
//   wgpu::Extent3D size{_size.x, _size.y};
//   wgpu::TextureDescriptor textureDesc{
//     .usage = TextureUsage::CopyDst | usage,
//     .size = size,
//     .format = format,
//   };
//   Texture texture = device.CreateTexture(&textureDesc);
//   if (data) {
//     WriteTexture(device, texture, _size, data);
//   }
//   return texture;
// }

// wgpu::Texture CreateBindingTexture(
//   const wgpu::Device& device,
//   glm::uvec2 size,
//   wgpu::TextureFormat format,
//   const void* data
// ) {
//   return CreateTexture(device, TextureUsage::TextureBinding, size, format, data);
// }

// wgpu::Texture CreateRenderTexture(
//   const wgpu::Device& device,
//   glm::uvec2 size,
//   wgpu::TextureFormat format,
//   const void* data
// ) {
//   return CreateTexture(device, TextureUsage::RenderAttachment | TextureUsage::TextureBinding, size, format, data);
// }

RenderPassDescriptor::RenderPassDescriptor(
  std::vector<wgpu::RenderPassColorAttachment> colorAttachments,
  wgpu::RenderPassDepthStencilAttachment depthStencilAttachment
) : cColorAttachments(std::move(colorAttachments)) {
  colorAttachmentCount = cColorAttachments.size();
  this->colorAttachments = cColorAttachments.data();

  if (depthStencilAttachment.view.Get() != nullptr) {
    cDepthStencilAttachmentInfo = depthStencilAttachment;
    this->depthStencilAttachment = &cDepthStencilAttachmentInfo;
  } else {
    this->depthStencilAttachment = nullptr;
  }
}

RenderPassDescriptor::RenderPassDescriptor(const RenderPassDescriptor& other) {
  *this = other;
}

RenderPassDescriptor& RenderPassDescriptor::operator=(const RenderPassDescriptor& otherRenderPass) {
  cDepthStencilAttachmentInfo = otherRenderPass.cDepthStencilAttachmentInfo;
  cColorAttachments = otherRenderPass.cColorAttachments;
  colorAttachmentCount = otherRenderPass.colorAttachmentCount;

  colorAttachments = cColorAttachments.data();

  if (otherRenderPass.depthStencilAttachment != nullptr) {
    // Assign desc.depthStencilAttachment to this->depthStencilAttachmentInfo;
    depthStencilAttachment = &cDepthStencilAttachmentInfo;
  } else {
    depthStencilAttachment = nullptr;
  }

  return *this;
}

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

RenderPipeline MakeRenderPipeline(const wgpu::Device &device, const utils::RenderPipelineDescriptor &desc) {
  // convert vertex buffer layout
  std::vector<wgpu::VertexBufferLayout> buffers;
  std::vector<std::vector<wgpu::VertexAttribute>> attributess;

  for (const auto & buffer : desc.buffers) {
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
      .arrayStride = buffer.arrayStride,
      .stepMode = buffer.stepMode,
      .attributeCount = attributes.size(),
      .attributes = attributes.data(),
    });
  }

  return device.CreateRenderPipeline(ToPtr(wgpu::RenderPipelineDescriptor{
    .layout = utils::MakePipelineLayout(device, desc.bgls),
    .vertex{
      .module = desc.vs,
      .entryPoint = "vs_main",
      .bufferCount = buffers.size(),
      .buffers = buffers.data(),
    },
    .primitive = desc.primitive,
    .depthStencil = desc.depthStencil.format == TextureFormat::Undefined ? nullptr : &desc.depthStencil,
    .multisample = desc.multisample,
    .fragment = desc.fs ? ToPtr(FragmentState{
      .module = desc.fs,
      .entryPoint = "fs_main",
      .targetCount = desc.targets.size(),
      .targets = desc.targets.data(),
    }) : nullptr,
  }));
}

} // namespace wgpu::utils
