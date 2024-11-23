#include "./texture.hpp"

namespace wgpu::utils {

using namespace wgpu;

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
  auto texelBlockSize = GetTexelBlockSizeInBytes(texture.GetFormat());
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

