#include <utility>

#include "./render_pass.hpp"

namespace wgpu::utils {

RenderPassDescriptor::RenderPassDescriptor(
  std::vector<wgpu::RenderPassColorAttachment> colorAttachments,
  std::optional<wgpu::RenderPassDepthStencilAttachment> depthStencilAttachment
)
    : cColorAttachments(std::move(colorAttachments)) {
  colorAttachmentCount = cColorAttachments.size();
  this->colorAttachments = cColorAttachments.data();
  if (depthStencilAttachment.has_value()) {
    cDepthStencilAttachmentInfo = depthStencilAttachment.value();
    this->depthStencilAttachment = &cDepthStencilAttachmentInfo;
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
    depthStencilAttachment = &cDepthStencilAttachmentInfo;
  } else {
    depthStencilAttachment = nullptr;
  }

  return *this;
}

}
