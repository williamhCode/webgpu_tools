#include "./context.hpp"
#include <print>
#include <utility>
#include <filesystem>

namespace fs = std::filesystem;
using namespace slang;

SlangContext::SlangContext(fs::path _includeDir) {
  SlangGlobalSessionDesc desc = {};
  SlangResult result = createGlobalSession(&desc, globalSession.writeRef());
  if (SLANG_FAILED(result)) {
    throw std::runtime_error(
      "Failed to create slang global session: status = " + std::to_string(result)
    );
  }

  includeDir = std::move(_includeDir);
  searchPaths.push_back(includeDir.c_str());

  // remove all .slang-module files in includeDir to recompile modules
  for (const auto& entry : fs::directory_iterator(includeDir)) {
    if (entry.path().extension() == ".slang-module") {
      fs::remove(entry.path());
    }
  }
}

void SlangContext::CompileModuleObject(
  const std::string& moduleName,
  const std::vector<slang::PreprocessorMacroDesc>& macros
) {
  SessionDesc sessionDesc{
    .defaultMatrixLayoutMode = SLANG_MATRIX_LAYOUT_COLUMN_MAJOR,

    .searchPaths = searchPaths.data(),
    .searchPathCount = static_cast<SlangInt>(searchPaths.size()),

    .preprocessorMacros = macros.data(),
    .preprocessorMacroCount = static_cast<SlangInt>(macros.size()),
  };

  Slang::ComPtr<ISession> session;
  SlangResult result = globalSession->createSession(sessionDesc, session.writeRef());
  if (SLANG_FAILED(result)) {
    throw std::runtime_error(
      "Failed to create slang session: status = " + std::to_string(result)
    );
  }

  Slang::ComPtr<IBlob> diagnostics;
  Slang::ComPtr<IModule> module;
  module = session->loadModule(moduleName.c_str(), diagnostics.writeRef());
  if (diagnostics) {
    std::println(stderr, "{}", (const char*)diagnostics->getBufferPointer());
  }

  auto path = includeDir / fs::path(module->getFilePath()).replace_extension(".slang-module");
  module->writeToFile(path.c_str());
}

std::string SlangContext::GetModuleSource(
  const std::string& moduleName,
  const std::vector<slang::PreprocessorMacroDesc>& macros,
  SlangCompileTarget target
) {
  TargetDesc targetDesc{.format = target};

  SessionDesc sessionDesc{
    .targets = &targetDesc,
    .targetCount = 1,

    .defaultMatrixLayoutMode = SLANG_MATRIX_LAYOUT_COLUMN_MAJOR,

    .searchPaths = searchPaths.data(),
    .searchPathCount = static_cast<SlangInt>(searchPaths.size()),

    .preprocessorMacros = macros.data(),
    .preprocessorMacroCount = static_cast<SlangInt>(macros.size()),
  };

  Slang::ComPtr<ISession> session;
  globalSession->createSession(sessionDesc, session.writeRef());
  SlangResult result = globalSession->createSession(sessionDesc, session.writeRef());
  if (SLANG_FAILED(result)) {
    throw std::runtime_error(
      "Failed to create slang session: status = " + std::to_string(result)
    );
  }

  Slang::ComPtr<IBlob> diagnostics;
  Slang::ComPtr<IModule> moduleText;
  moduleText = session->loadModule(moduleName.c_str(), diagnostics.writeRef());
  if (diagnostics) {
    std::println(stderr, "{}", (const char*)diagnostics->getBufferPointer());
  }

  Slang::ComPtr<IBlob> codeBlob;
  moduleText->getTargetCode(0, codeBlob.writeRef(), diagnostics.writeRef());
  if (diagnostics) {
    std::println(stderr, "{}", (const char*)diagnostics->getBufferPointer());
  }

  return codeBlob ? (const char*)codeBlob->getBufferPointer() : "";
}
