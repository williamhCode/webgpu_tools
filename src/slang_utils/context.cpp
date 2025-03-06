#include "./context.hpp"
#include "./result.hpp"
#include <print>
#include <utility>
#include <filesystem>

using namespace slang;

std::expected<void, std::string>
SlangContext::Init(std::string _includeDir) {
  SlangGlobalSessionDesc desc = {};
  TRY_SLANG(createGlobalSession(&desc, globalSession.writeRef()));

  includeDir = std::move(_includeDir);
  searchPaths.push_back(includeDir.c_str());

  return {};
}

std::string SlangContext::CompileModuleObject(
  const std::string& moduleName,
  const std::vector<slang::PreprocessorMacroDesc>& macros
) {
  TargetDesc targetDesc{.format = SLANG_OBJECT_CODE};

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

  Slang::ComPtr<IBlob> diagnostics;
  Slang::ComPtr<IModule> module;
  module = session->loadModule(moduleName.c_str(), diagnostics.writeRef());
  if (diagnostics) {
    return {(const char*)diagnostics->getBufferPointer()};
  }
  namespace fs = std::filesystem;
  auto path = fs::path(module->getFilePath()).replace_extension(".slang-module").string();
  module->writeToFile(includeDir.c_str());

  return {};
}

std::string SlangContext::GetModuleSource(
  const std::string& moduleName,
  const std::vector<slang::PreprocessorMacroDesc>& macros,
  SlangCompileTarget target
) {
  TargetDesc targetDesc{.format = SLANG_WGSL};

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

  Slang::ComPtr<IBlob> diagnostics;
  Slang::ComPtr<IModule> moduleText;
  moduleText = session->loadModule("text", diagnostics.writeRef());
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
