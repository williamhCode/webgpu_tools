#pragma once

#include "webgpu/webgpu_cpp.h"
#include <filesystem>

namespace wgpu::utils {

wgpu::ShaderModule LoadShaderModule(const wgpu::Device &device, const std::filesystem::path &path);

}

