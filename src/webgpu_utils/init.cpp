#include "init.hpp"
#include "dawn/webgpu_cpp_print.h"

#include <sstream>
#include <print>

namespace wgpu::utils {

using namespace wgpu;

static std::string ToString(auto&& obj) {
  return (std::ostringstream() << obj).str();
}

Adapter RequestAdapter(const Instance& instance, const RequestAdapterOptions& options) {
  wgpu::Adapter adapter;
  instance.WaitAny(
    instance.RequestAdapter(
      &options, wgpu::CallbackMode::WaitAnyOnly,
      [](
        wgpu::RequestAdapterStatus status, wgpu::Adapter result,
        wgpu::StringView message, wgpu::Adapter* userdata
      ) {
        if (status != wgpu::RequestAdapterStatus::Success) {
          std::println("Could not get WebGPU adapter: {}", message.data);
          return;
        }
        *userdata = std::move(result);
      },
      &adapter
    ),
    std::numeric_limits<uint64_t>::max()
  );

  return adapter;
}

Device RequestDevice(const Instance& instance, const Adapter& adapter, const DeviceDescriptor& descriptor) {
  wgpu::Device device;
  instance.WaitAny(
    adapter.RequestDevice(
      &descriptor, wgpu::CallbackMode::WaitAnyOnly,
      [](
        wgpu::RequestDeviceStatus status, wgpu::Device result, wgpu::StringView message,
        wgpu::Device* userdata
      ) {
        if (status != wgpu::RequestDeviceStatus::Success) {
          std::println("Could not get WebGPU device: ", message.data);
          return;
        }
        *userdata = std::move(result);
      },
      &device
    ),
    std::numeric_limits<uint64_t>::max()
  );

  return device;
}

// clang-format off
void PrintLimits(const wgpu::Limits &limits) {
  std::println(" - maxTextureDimension1D: {}", limits.maxTextureDimension1D);
  std::println(" - maxTextureDimension2D: {}", limits.maxTextureDimension2D);
  std::println(" - maxTextureDimension3D: {}", limits.maxTextureDimension3D);
  std::println(" - maxTextureArrayLayers: {}", limits.maxTextureArrayLayers);
  std::println(" - maxBindGroups: {}", limits.maxBindGroups);
  std::println(" - maxBindGroupsPlusVertexBuffers: {}", limits.maxBindGroupsPlusVertexBuffers);
  std::println(" - maxBindingsPerBindGroup: {}", limits.maxBindingsPerBindGroup);
  std::println(" - maxDynamicUniformBuffersPerPipelineLayout: {}", limits.maxDynamicUniformBuffersPerPipelineLayout);
  std::println(" - maxDynamicStorageBuffersPerPipelineLayout: {}", limits.maxDynamicStorageBuffersPerPipelineLayout);
  std::println(" - maxSampledTexturesPerShaderStage: {}", limits.maxSampledTexturesPerShaderStage);
  std::println(" - maxSamplersPerShaderStage: {}", limits.maxSamplersPerShaderStage);
  std::println(" - maxStorageBuffersPerShaderStage: {}", limits.maxStorageBuffersPerShaderStage);
  std::println(" - maxStorageTexturesPerShaderStage: {}", limits.maxStorageTexturesPerShaderStage);
  std::println(" - maxUniformBuffersPerShaderStage: {}", limits.maxUniformBuffersPerShaderStage);
  std::println(" - maxUniformBufferBindingSize: {}", limits.maxUniformBufferBindingSize);
  std::println(" - maxStorageBufferBindingSize: {}", limits.maxStorageBufferBindingSize);
  std::println(" - minUniformBufferOffsetAlignment: {}", limits.minUniformBufferOffsetAlignment);
  std::println(" - minStorageBufferOffsetAlignment: {}", limits.minStorageBufferOffsetAlignment);
  std::println(" - maxVertexBuffers: {}", limits.maxVertexBuffers);
  std::println(" - maxBufferSize: {}", limits.maxBufferSize);
  std::println(" - maxVertexAttributes: {}", limits.maxVertexAttributes);
  std::println(" - maxVertexBufferArrayStride: {}", limits.maxVertexBufferArrayStride);
  std::println(" - maxInterStageShaderVariables: {}", limits.maxInterStageShaderVariables);
  std::println(" - maxColorAttachments: {}", limits.maxColorAttachments);
  std::println(" - maxColorAttachmentBytesPerSample: {}", limits.maxColorAttachmentBytesPerSample);
  std::println(" - maxComputeWorkgroupStorageSize: {}", limits.maxComputeWorkgroupStorageSize);
  std::println(" - maxComputeInvocationsPerWorkgroup: {}", limits.maxComputeInvocationsPerWorkgroup);
  std::println(" - maxComputeWorkgroupSizeX: {}", limits.maxComputeWorkgroupSizeX);
  std::println(" - maxComputeWorkgroupSizeY: {}", limits.maxComputeWorkgroupSizeY);
  std::println(" - maxComputeWorkgroupSizeZ: {}", limits.maxComputeWorkgroupSizeZ);
  std::println(" - maxComputeWorkgroupsPerDimension: {}", limits.maxComputeWorkgroupsPerDimension);
}
// clang-format on

void PrintSurfaceCapabilities(const SurfaceCapabilities& config) {
  for (size_t i = 0; i < config.formatCount; i++) {
    std::println("Format: {}", ToString(config.formats[i]));
  }
  for (size_t i = 0; i < config.presentModeCount; i++) {
    std::println("Present Mode: {}", ToString(config.presentModes[i]));
  }
  for (size_t i = 0; i < config.alphaModeCount; i++) {
    std::println("Alpha Mode: {}", ToString(config.alphaModes[i]));
  }
}

}
