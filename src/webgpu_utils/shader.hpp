#pragma once

#include "webgpu/webgpu_cpp.h"
#include <filesystem>
#include <string>

namespace wgpu::utils {

wgpu::ShaderModule LoadShaderModule(const wgpu::Device &device, const std::filesystem::path &path);
wgpu::ShaderModule LoadShaderModuleSource(const Device& device, const std::string& source);

}

