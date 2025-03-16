cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

include(FetchContent)

if (EMSCRIPTEN)
  FetchContent_Declare(
    dawn

    DOWNLOAD_COMMAND
      cd ${PROJECT_SOURCE_DIR}/dawn-src &&
      git init &&
      git fetch --depth=1 https://dawn.googlesource.com/dawn chromium/6930 &&
      git reset --hard FETCH_HEAD

    SOURCE_DIR ${PROJECT_SOURCE_DIR}/dawn-src
  )

  set(DAWN_FETCH_DEPENDENCIES ON)
  set(DAWN_EMSCRIPTEN_TOOLCHAIN "/Users/williamhou/Documents/Coding/Github/emscripten")

  FetchContent_MakeAvailable(dawn)

  add_library(webgpu INTERFACE)
  target_link_libraries(webgpu INTERFACE
    webgpu_cpp
  )

else (EMSCRIPTEN)
  if (CMAKE_BUILD_TYPE MATCHES Debug)
    FetchContent_Declare(
      dawn
      URL https://github.com/williamhCode/dawn-single-lib/releases/download/chromium%2F6800/dawn-osx-universal-relwithdebinfo+assert.tar.gz
    )
  else()
    FetchContent_Declare(
      dawn
      URL https://github.com/williamhCode/dawn-single-lib/releases/download/chromium%2F6800/dawn-osx-universal-release.tar.gz
    )
  endif()
  FetchContent_MakeAvailable(dawn)

  find_package(Dawn CONFIG REQUIRED PATHS ${dawn_SOURCE_DIR})

  add_library(webgpu INTERFACE)
  target_link_libraries(webgpu INTERFACE dawn::webgpu_dawn)
endif (EMSCRIPTEN)
