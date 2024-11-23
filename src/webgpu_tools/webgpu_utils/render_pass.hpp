#pragma once

#include "webgpu/webgpu_cpp.h"
#include <vector>

namespace wgpu::utils {

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

}


