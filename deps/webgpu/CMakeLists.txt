cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

project(webgpu VERSION 1.0.0)

include(FetchContent)

if (EMSCRIPTEN)
  add_library(webgpu INTERFACE)

  set(EMDAWNWEBGPU_DIR "${PROJECT_SOURCE_DIR}/emdawnwebgpu_pkg")

  target_compile_options(webgpu INTERFACE
    "--use-port=${EMDAWNWEBGPU_DIR}/emdawnwebgpu.port.py"
  )

  target_link_options(webgpu INTERFACE
    "--use-port=${EMDAWNWEBGPU_DIR}/emdawnwebgpu.port.py"
  )

  target_include_directories(webgpu INTERFACE
    "${EMDAWNWEBGPU_DIR}/webgpu/include"
    "${EMDAWNWEBGPU_DIR}/webgpu_cpp/include"
  )

else (EMSCRIPTEN)
  if (CMAKE_BUILD_TYPE MATCHES Debug)
    FetchContent_Declare(
      dawn
      URL https://github.com/williamhCode/dawn-single-lib/releases/download/chromium%2F7262/dawn-osx-universal-relwithdebinfo+assert.tar.gz
    )
  else()
    FetchContent_Declare(
      dawn
      URL https://github.com/williamhCode/dawn-single-lib/releases/download/chromium%2F7262/dawn-osx-universal-release.tar.gz
    )
  endif()
  FetchContent_MakeAvailable(dawn)

  find_package(Dawn CONFIG REQUIRED PATHS ${dawn_SOURCE_DIR})

  add_library(webgpu INTERFACE)
  target_link_libraries(webgpu INTERFACE dawn::webgpu_dawn)
endif (EMSCRIPTEN)
