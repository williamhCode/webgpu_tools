#include "./shader.hpp"
#include <fstream>
#include <sstream>

namespace wgpu::utils {

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

}
