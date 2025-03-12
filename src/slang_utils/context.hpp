#pragma once

#include "slang.h"
#include "slang-com-ptr.h"
#include <expected>
#include <filesystem>
#include <string>
#include <vector>

struct SlangContext {
	Slang::ComPtr<slang::IGlobalSession> globalSession;

  std::filesystem::path includeDir;
  std::vector<const char*> searchPaths;

  SlangContext(std::filesystem::path includeDir);

  // compiles a module and saves a .slang-module file to the includeDir
  void CompileModuleObject(
    const std::string& moduleName,
    const std::vector<slang::PreprocessorMacroDesc>& macros = {}
  );

  // compiles a module and returns the target code
  std::string GetModuleSource(
    const std::string& moduleName,
    const std::vector<slang::PreprocessorMacroDesc>& macros = {},
    SlangCompileTarget target = SLANG_WGSL
  );
};
