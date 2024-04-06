#include "webgpu.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace wgpu::utils {

using namespace wgpu;

Adapter RequestAdapter(const Instance& instance, RequestAdapterOptions const* options) {
  struct UserData {
    WGPUAdapter adapter = nullptr;
    bool requestEnded = false;
  };
  UserData userData;

  auto onAdapterRequestEnded = [](
                                 WGPURequestAdapterStatus status, WGPUAdapter adapter,
                                 char const* message, void* pUserData
                               ) {
    UserData& userData = *static_cast<UserData*>(pUserData);
    if (status == WGPURequestAdapterStatus_Success) {
      userData.adapter = adapter;
    } else {
      std::cout << "Could not get WebGPU adapter: " << message << "\n";
    }
    userData.requestEnded = true;
  };

  instance.RequestAdapter(options, onAdapterRequestEnded, &userData);

  // assert(userData.requestEnded);

  return userData.adapter;
}

Device RequestDevice(const Adapter& instance, DeviceDescriptor const* descriptor) {
  struct UserData {
    WGPUDevice device = nullptr;
    bool requestEnded = false;
  };
  UserData userData;

  auto onDeviceRequestEnded = [](
                                WGPURequestDeviceStatus status, WGPUDevice device,
                                char const* message, void* pUserData
                              ) {
    UserData& userData = *static_cast<UserData*>(pUserData);
    if (status == WGPURequestDeviceStatus_Success) {
      userData.device = device;
    } else {
      std::cout << "Could not get WebGPU adapter: " << message << "\n";
    }
    userData.requestEnded = true;
  };

  instance.RequestDevice(descriptor, onDeviceRequestEnded, &userData);

  // assert(userData.requestEnded);

  return userData.device;
}

void SetUncapturedErrorCallback(const Device& device) {
  auto onUncapturedError = [](WGPUErrorType type, char const* message, void* userdata) {
    std::cout << "Device error: type " << type;
    if (message) std::cout << " (message: " << message << ")";
    std::cout << "\n";
  };

  device.SetUncapturedErrorCallback(onUncapturedError, nullptr);
}

ShaderModule LoadShaderModule(const Device& device, const std::filesystem::path& path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open shader file" + path.string());
  }
  std::stringstream buffer;
  buffer << file.rdbuf();

  return dawn::utils::CreateShaderModule(device, buffer.str());
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

wgpu::Buffer CreateBuffer(const wgpu::Device& device, wgpu::BufferUsage usage, size_t size, const void* data) {
  BufferDescriptor bufferDesc{
    .usage = BufferUsage::CopyDst | usage,
    .size = size,
  };
  Buffer buffer = device.CreateBuffer(&bufferDesc);
  if (data) device.GetQueue().WriteBuffer(buffer, 0, data, size);
  return buffer;
}

wgpu::Buffer CreateVertexBuffer(const wgpu::Device& device, size_t size, const void* data) {
  return CreateBuffer(device, BufferUsage::Vertex, size, data);
}

wgpu::Buffer CreateIndexBuffer(const wgpu::Device& device, size_t size, const void* data) {
  return CreateBuffer(device, BufferUsage::Index, size, data);
}

wgpu::Buffer CreateUniformBuffer(const wgpu::Device& device, size_t size, const void* data) {
  return CreateBuffer(device, BufferUsage::Uniform, size, data);
}

wgpu::Buffer CreateStorageBuffer(const wgpu::Device& device, size_t size, const void* data) {
  return CreateBuffer(device, BufferUsage::Storage, size, data);
}

wgpu::Texture CreateTexture(
  const wgpu::Device& device,
  wgpu::TextureUsage usage,
  wgpu::Extent3D size,
  wgpu::TextureFormat format,
  const void* data
) {
  TextureDescriptor textureDesc{
    .usage = TextureUsage::CopyDst | usage,
    .size = size,
    .format = format,
  };
  Texture texture = device.CreateTexture(&textureDesc);
  if (data) {
    auto texelBlockSize = dawn::utils::GetTexelBlockSizeInBytes(format);
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
  return texture;
}

wgpu::Texture CreateBindingTexture(
  const wgpu::Device& device,
  wgpu::Extent3D size,
  wgpu::TextureFormat format,
  const void* data
) {
  return CreateTexture(device, TextureUsage::TextureBinding, size, format, data);
}

wgpu::Texture CreateRenderTexture(
  const wgpu::Device& device,
  wgpu::Extent3D size,
  wgpu::TextureFormat format,
  const void* data
) {
  return CreateTexture(device, TextureUsage::RenderAttachment | TextureUsage::TextureBinding, size, format, data);
}

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

VertexBufferLayout::VertexBufferLayout(
  uint64_t arrayStride,
  std::vector<wgpu::VertexAttribute> attributes,
  VertexStepMode stepMode
) : cAttributes(std::move(attributes)) {
  this->arrayStride = arrayStride;
  this->stepMode = stepMode;
  attributeCount = cAttributes.size();
  this->attributes = cAttributes.data();
}

VertexBufferLayout::VertexBufferLayout(const VertexBufferLayout& other) {
  *this = other;
}

VertexBufferLayout& VertexBufferLayout::operator=(const VertexBufferLayout& other
) {
  cAttributes = other.cAttributes;
  arrayStride = other.arrayStride;
  stepMode = other.stepMode;
  attributeCount = other.attributeCount;
  attributes = cAttributes.data();
  return *this;
}

} // namespace wgpu::utils
