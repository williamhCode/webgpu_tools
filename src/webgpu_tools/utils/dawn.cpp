#include "./dawn.hpp"

namespace dawn::utils {

wgpu::PipelineLayout MakePipelineLayout(const wgpu::Device& device,
                                        std::vector<wgpu::BindGroupLayout> bgls) {
    wgpu::PipelineLayoutDescriptor descriptor;
    descriptor.bindGroupLayoutCount = uint32_t(bgls.size());
    descriptor.bindGroupLayouts = bgls.data();
    return device.CreatePipelineLayout(&descriptor);
}

wgpu::BindGroupLayout MakeBindGroupLayout(
    const wgpu::Device& device,
    std::initializer_list<BindingLayoutEntryInitializationHelper> entriesInitializer) {
    std::vector<wgpu::BindGroupLayoutEntry> entries;
    for (const BindingLayoutEntryInitializationHelper& entry : entriesInitializer) {
        entries.push_back(entry);
    }

    wgpu::BindGroupLayoutDescriptor descriptor;
    descriptor.entryCount = entries.size();
    descriptor.entries = entries.data();
    return device.CreateBindGroupLayout(&descriptor);
}

BindingLayoutEntryInitializationHelper::BindingLayoutEntryInitializationHelper(
    uint32_t entryBinding,
    wgpu::ShaderStage entryVisibility,
    wgpu::BufferBindingType bufferType,
    bool bufferHasDynamicOffset,
    uint64_t bufferMinBindingSize) {
    binding = entryBinding;
    visibility = entryVisibility;
    buffer.type = bufferType;
    buffer.hasDynamicOffset = bufferHasDynamicOffset;
    buffer.minBindingSize = bufferMinBindingSize;
}

BindingLayoutEntryInitializationHelper::BindingLayoutEntryInitializationHelper(
    uint32_t entryBinding,
    wgpu::ShaderStage entryVisibility,
    wgpu::SamplerBindingType samplerType) {
    binding = entryBinding;
    visibility = entryVisibility;
    sampler.type = samplerType;
}

BindingLayoutEntryInitializationHelper::BindingLayoutEntryInitializationHelper(
    uint32_t entryBinding,
    wgpu::ShaderStage entryVisibility,
    wgpu::TextureSampleType textureSampleType,
    wgpu::TextureViewDimension textureViewDimension,
    bool textureMultisampled) {
    binding = entryBinding;
    visibility = entryVisibility;
    texture.sampleType = textureSampleType;
    texture.viewDimension = textureViewDimension;
    texture.multisampled = textureMultisampled;
}

BindingLayoutEntryInitializationHelper::BindingLayoutEntryInitializationHelper(
    uint32_t entryBinding,
    wgpu::ShaderStage entryVisibility,
    wgpu::StorageTextureAccess storageTextureAccess,
    wgpu::TextureFormat format,
    wgpu::TextureViewDimension textureViewDimension) {
    binding = entryBinding;
    visibility = entryVisibility;
    storageTexture.access = storageTextureAccess;
    storageTexture.format = format;
    storageTexture.viewDimension = textureViewDimension;
}

// ExternalTextureBindingLayout never contains data, so just make one that can be reused instead
// of declaring a new one every time it's needed.
// wgpu::ExternalTextureBindingLayout kExternalTextureBindingLayout = {};

// BindingLayoutEntryInitializationHelper::BindingLayoutEntryInitializationHelper(
//     uint32_t entryBinding,
//     wgpu::ShaderStage entryVisibility,
//     wgpu::ExternalTextureBindingLayout* bindingLayout) {
//     binding = entryBinding;
//     visibility = entryVisibility;
//     nextInChain = bindingLayout;
// }

BindingLayoutEntryInitializationHelper::BindingLayoutEntryInitializationHelper(
    const wgpu::BindGroupLayoutEntry& entry)
    : wgpu::BindGroupLayoutEntry(entry) {}

BindingInitializationHelper::BindingInitializationHelper(uint32_t binding,
                                                         const wgpu::Sampler& sampler)
    : binding(binding), sampler(sampler) {}

BindingInitializationHelper::BindingInitializationHelper(uint32_t binding,
                                                         const wgpu::TextureView& textureView)
    : binding(binding), textureView(textureView) {}

// BindingInitializationHelper::BindingInitializationHelper(
//     uint32_t binding,
//     const wgpu::ExternalTexture& externalTexture)
//     : binding(binding) {
//     externalTextureBindingEntry.externalTexture = externalTexture;
// }

BindingInitializationHelper::BindingInitializationHelper(uint32_t binding,
                                                         const wgpu::Buffer& buffer,
                                                         uint64_t offset,
                                                         uint64_t size)
    : binding(binding), buffer(buffer), offset(offset), size(size) {}

BindingInitializationHelper::BindingInitializationHelper(const BindingInitializationHelper&) =
    default;

BindingInitializationHelper::~BindingInitializationHelper() = default;

wgpu::BindGroupEntry BindingInitializationHelper::GetAsBinding() const {
    wgpu::BindGroupEntry result;

    result.binding = binding;
    result.sampler = sampler;
    result.textureView = textureView;
    result.buffer = buffer;
    result.offset = offset;
    result.size = size;
    // if (externalTextureBindingEntry.externalTexture != nullptr) {
    //     result.nextInChain = &externalTextureBindingEntry;
    // }

    return result;
}

wgpu::BindGroup MakeBindGroup(
    const wgpu::Device& device,
    const wgpu::BindGroupLayout& layout,
    std::initializer_list<BindingInitializationHelper> entriesInitializer) {
    std::vector<wgpu::BindGroupEntry> entries;
    for (const BindingInitializationHelper& helper : entriesInitializer) {
        entries.push_back(helper.GetAsBinding());
    }

    wgpu::BindGroupDescriptor descriptor;
    descriptor.layout = layout;
    descriptor.entryCount = static_cast<uint32_t>(entries.size());
    descriptor.entries = entries.data();

    return device.CreateBindGroup(&descriptor);
}

uint32_t GetTexelBlockSizeInBytes(wgpu::TextureFormat textureFormat) {
  switch (textureFormat) {
    case wgpu::TextureFormat::R8Unorm:
    case wgpu::TextureFormat::R8Snorm:
    case wgpu::TextureFormat::R8Uint:
    case wgpu::TextureFormat::R8Sint:
    case wgpu::TextureFormat::Stencil8:
      return 1u;

    case wgpu::TextureFormat::R16Uint:
    case wgpu::TextureFormat::R16Sint:
    case wgpu::TextureFormat::R16Float:
    case wgpu::TextureFormat::RG8Unorm:
    case wgpu::TextureFormat::RG8Snorm:
    case wgpu::TextureFormat::RG8Uint:
    case wgpu::TextureFormat::RG8Sint:
      return 2u;

    case wgpu::TextureFormat::R32Float:
    case wgpu::TextureFormat::R32Uint:
    case wgpu::TextureFormat::R32Sint:
    case wgpu::TextureFormat::RG16Uint:
    case wgpu::TextureFormat::RG16Sint:
    case wgpu::TextureFormat::RG16Float:
    case wgpu::TextureFormat::RGBA8Unorm:
    case wgpu::TextureFormat::RGBA8UnormSrgb:
    case wgpu::TextureFormat::RGBA8Snorm:
    case wgpu::TextureFormat::RGBA8Uint:
    case wgpu::TextureFormat::RGBA8Sint:
    case wgpu::TextureFormat::BGRA8Unorm:
    case wgpu::TextureFormat::BGRA8UnormSrgb:
    case wgpu::TextureFormat::RGB10A2Uint:
    case wgpu::TextureFormat::RGB10A2Unorm:
    case wgpu::TextureFormat::RG11B10Ufloat:
    case wgpu::TextureFormat::RGB9E5Ufloat:
      return 4u;

    case wgpu::TextureFormat::RG32Float:
    case wgpu::TextureFormat::RG32Uint:
    case wgpu::TextureFormat::RG32Sint:
    case wgpu::TextureFormat::RGBA16Uint:
    case wgpu::TextureFormat::RGBA16Sint:
    case wgpu::TextureFormat::RGBA16Float:
      return 8u;

    case wgpu::TextureFormat::RGBA32Float:
    case wgpu::TextureFormat::RGBA32Uint:
    case wgpu::TextureFormat::RGBA32Sint:
      return 16u;

    case wgpu::TextureFormat::Depth16Unorm:
      return 2u;

    case wgpu::TextureFormat::Depth24Plus:
    case wgpu::TextureFormat::Depth32Float:
      return 4u;

    case wgpu::TextureFormat::BC1RGBAUnorm:
    case wgpu::TextureFormat::BC1RGBAUnormSrgb:
    case wgpu::TextureFormat::BC4RUnorm:
    case wgpu::TextureFormat::BC4RSnorm:
      return 8u;

    case wgpu::TextureFormat::BC2RGBAUnorm:
    case wgpu::TextureFormat::BC2RGBAUnormSrgb:
    case wgpu::TextureFormat::BC3RGBAUnorm:
    case wgpu::TextureFormat::BC3RGBAUnormSrgb:
    case wgpu::TextureFormat::BC5RGUnorm:
    case wgpu::TextureFormat::BC5RGSnorm:
    case wgpu::TextureFormat::BC6HRGBUfloat:
    case wgpu::TextureFormat::BC6HRGBFloat:
    case wgpu::TextureFormat::BC7RGBAUnorm:
    case wgpu::TextureFormat::BC7RGBAUnormSrgb:
      return 16u;

    case wgpu::TextureFormat::ETC2RGB8Unorm:
    case wgpu::TextureFormat::ETC2RGB8UnormSrgb:
    case wgpu::TextureFormat::ETC2RGB8A1Unorm:
    case wgpu::TextureFormat::ETC2RGB8A1UnormSrgb:
    case wgpu::TextureFormat::EACR11Unorm:
    case wgpu::TextureFormat::EACR11Snorm:
      return 8u;

    case wgpu::TextureFormat::ETC2RGBA8Unorm:
    case wgpu::TextureFormat::ETC2RGBA8UnormSrgb:
    case wgpu::TextureFormat::EACRG11Unorm:
    case wgpu::TextureFormat::EACRG11Snorm:
      return 16u;

    case wgpu::TextureFormat::ASTC4x4Unorm:
    case wgpu::TextureFormat::ASTC4x4UnormSrgb:
    case wgpu::TextureFormat::ASTC5x4Unorm:
    case wgpu::TextureFormat::ASTC5x4UnormSrgb:
    case wgpu::TextureFormat::ASTC5x5Unorm:
    case wgpu::TextureFormat::ASTC5x5UnormSrgb:
    case wgpu::TextureFormat::ASTC6x5Unorm:
    case wgpu::TextureFormat::ASTC6x5UnormSrgb:
    case wgpu::TextureFormat::ASTC6x6Unorm:
    case wgpu::TextureFormat::ASTC6x6UnormSrgb:
    case wgpu::TextureFormat::ASTC8x5Unorm:
    case wgpu::TextureFormat::ASTC8x5UnormSrgb:
    case wgpu::TextureFormat::ASTC8x6Unorm:
    case wgpu::TextureFormat::ASTC8x6UnormSrgb:
    case wgpu::TextureFormat::ASTC8x8Unorm:
    case wgpu::TextureFormat::ASTC8x8UnormSrgb:
    case wgpu::TextureFormat::ASTC10x5Unorm:
    case wgpu::TextureFormat::ASTC10x5UnormSrgb:
    case wgpu::TextureFormat::ASTC10x6Unorm:
    case wgpu::TextureFormat::ASTC10x6UnormSrgb:
    case wgpu::TextureFormat::ASTC10x8Unorm:
    case wgpu::TextureFormat::ASTC10x8UnormSrgb:
    case wgpu::TextureFormat::ASTC10x10Unorm:
    case wgpu::TextureFormat::ASTC10x10UnormSrgb:
    case wgpu::TextureFormat::ASTC12x10Unorm:
    case wgpu::TextureFormat::ASTC12x10UnormSrgb:
    case wgpu::TextureFormat::ASTC12x12Unorm:
    case wgpu::TextureFormat::ASTC12x12UnormSrgb:
      return 16u;

    case wgpu::TextureFormat::Depth24PlusStencil8:
    case wgpu::TextureFormat::Depth32FloatStencil8:
      break;

#ifndef __EMSCRIPTEN__
    // Unorm and Snorm 16 formats.
    case wgpu::TextureFormat::R16Unorm:
    case wgpu::TextureFormat::R16Snorm:
      return 2u;
    case wgpu::TextureFormat::RG16Unorm:
    case wgpu::TextureFormat::RG16Snorm:
      return 4u;
    case wgpu::TextureFormat::RGBA16Unorm:
    case wgpu::TextureFormat::RGBA16Snorm:
      return 8u;

    // Block size of a multi-planar format depends on aspect.
    case wgpu::TextureFormat::R8BG8Biplanar420Unorm:
    case wgpu::TextureFormat::R8BG8Biplanar422Unorm:
    case wgpu::TextureFormat::R8BG8Biplanar444Unorm:
    case wgpu::TextureFormat::R10X6BG10X6Biplanar420Unorm:
    case wgpu::TextureFormat::R10X6BG10X6Biplanar422Unorm:
    case wgpu::TextureFormat::R10X6BG10X6Biplanar444Unorm:
    case wgpu::TextureFormat::R8BG8A8Triplanar420Unorm:
    case wgpu::TextureFormat::External:
#endif // __EMSCRIPTEN__

    case wgpu::TextureFormat::Undefined:
      break;
  }

  std::unreachable();
}

}
