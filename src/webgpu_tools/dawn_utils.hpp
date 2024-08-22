#pragma once

#include "webgpu/webgpu_cpp.h"

namespace dawn::utils {

wgpu::PipelineLayout MakePipelineLayout(const wgpu::Device& device, std::vector<wgpu::BindGroupLayout> bgls);

// extern wgpu::ExternalTextureBindingLayout kExternalTextureBindingLayout;

struct BindingLayoutEntryInitializationHelper : wgpu::BindGroupLayoutEntry {
    BindingLayoutEntryInitializationHelper(uint32_t entryBinding,
                                           wgpu::ShaderStage entryVisibility,
                                           wgpu::BufferBindingType bufferType,
                                           bool bufferHasDynamicOffset = false,
                                           uint64_t bufferMinBindingSize = 0);
    BindingLayoutEntryInitializationHelper(uint32_t entryBinding,
                                           wgpu::ShaderStage entryVisibility,
                                           wgpu::SamplerBindingType samplerType);
    BindingLayoutEntryInitializationHelper(
        uint32_t entryBinding,
        wgpu::ShaderStage entryVisibility,
        wgpu::TextureSampleType textureSampleType,
        wgpu::TextureViewDimension viewDimension = wgpu::TextureViewDimension::e2D,
        bool textureMultisampled = false);
    BindingLayoutEntryInitializationHelper(
        uint32_t entryBinding,
        wgpu::ShaderStage entryVisibility,
        wgpu::StorageTextureAccess storageTextureAccess,
        wgpu::TextureFormat format,
        wgpu::TextureViewDimension viewDimension = wgpu::TextureViewDimension::e2D);
    // BindingLayoutEntryInitializationHelper(uint32_t entryBinding,
    //                                        wgpu::ShaderStage entryVisibility,
    //                                        wgpu::ExternalTextureBindingLayout* bindingLayout);

    // NOLINTNEXTLINE(runtime/explicit)
    BindingLayoutEntryInitializationHelper(const wgpu::BindGroupLayoutEntry& entry);
};

wgpu::BindGroupLayout MakeBindGroupLayout(
    const wgpu::Device& device,
    std::initializer_list<BindingLayoutEntryInitializationHelper> entriesInitializer);

// Helpers to make creating bind groups look nicer:
//
//   dawn::utils::MakeBindGroup(device, layout, {
//       {0, mySampler},
//       {1, myBuffer, offset, size},
//       {3, myTextureView}
//   });

// Structure with one constructor per-type of bindings, so that the initializer_list accepts
// bindings with the right type and no extra information.
struct BindingInitializationHelper {
    BindingInitializationHelper(uint32_t binding, const wgpu::Sampler& sampler);
    BindingInitializationHelper(uint32_t binding, const wgpu::TextureView& textureView);
    // BindingInitializationHelper(uint32_t binding, const wgpu::ExternalTexture& externalTexture);
    BindingInitializationHelper(uint32_t binding,
                                const wgpu::Buffer& buffer,
                                uint64_t offset = 0,
                                uint64_t size = wgpu::kWholeSize);
    BindingInitializationHelper(const BindingInitializationHelper&);
    ~BindingInitializationHelper();

    wgpu::BindGroupEntry GetAsBinding() const;

    uint32_t binding;
    wgpu::Sampler sampler;
    wgpu::TextureView textureView;
    wgpu::Buffer buffer;
    // wgpu::ExternalTextureBindingEntry externalTextureBindingEntry;
    uint64_t offset = 0;
    uint64_t size = 0;
};

wgpu::BindGroup MakeBindGroup(
    const wgpu::Device& device,
    const wgpu::BindGroupLayout& layout,
    std::initializer_list<BindingInitializationHelper> entriesInitializer);

}
