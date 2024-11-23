#include "./render_pass.hpp"

namespace wgpu::utils {

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
    depthStencilAttachment = &cDepthStencilAttachmentInfo;
  } else {
    depthStencilAttachment = nullptr;
  }

  return *this;
}

}
