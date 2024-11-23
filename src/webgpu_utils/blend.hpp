#pragma once

#include "webgpu/webgpu_cpp.h"

namespace wgpu::utils {

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

} // namespace wgpu::utils
