#pragma once

#include "slang.h"
#include "slang-com-ptr.h"
#include <expected>
#include <string>
#include <vector>

struct SlangContext {
	Slang::ComPtr<slang::IGlobalSession> globalSession;

  std::string includeDir;
  std::vector<const char*> searchPaths;

  std::expected<void, std::string> Init(std::string includeDir);

  // compiles and saves the module object file to the includeDir
  std::string CompileModuleObject(
    const std::string& moduleName,
    const std::vector<slang::PreprocessorMacroDesc>& macros = {}
  );

  // compiles and returns the source code of the module
  std::string GetModuleSource(
    const std::string& moduleName,
    const std::vector<slang::PreprocessorMacroDesc>& macros = {},
    SlangCompileTarget target = SLANG_WGSL
  );
};
