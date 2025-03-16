cmake_minimum_required(VERSION 3.5...3.28)

include(FetchContent)

# Detect platform and architecture
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(SLANG_OS "macos")
  if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
    set(SLANG_ARCH "aarch64")
  else()
    set(SLANG_ARCH "x86_64")
  endif()
  set(SLANG_LIB "libslang.dylib")

# not tested yet
# elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
#     set(SLANG_OS "windows")
#     if(CMAKE_SYSTEM_PROCESSOR MATCHES "(AMD64|x86_64)")
#         set(SLANG_ARCH "x86_64")
#     else()
#         set(SLANG_ARCH "aarch64")
#     endif()
#     set(SLANG_LIB "slang.dll")
# elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
#     set(SLANG_OS "linux")
#     if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
#         set(SLANG_ARCH "aarch64")
#     else()
#         set(SLANG_ARCH "x86_64")
#     endif()
#     set(SLANG_LIB "libslang.so")

else()
  message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()

set(SLANG_VERSION "2025.6.1")
set(SLANG_GITHUB "https://github.com/shader-slang/slang")
set(SLANG_BASENAME "slang-${SLANG_VERSION}-${SLANG_OS}-${SLANG_ARCH}")
set(SLANG_ARCHIVE "${SLANG_BASENAME}.tar.gz")
set(SLANG_URL "${SLANG_GITHUB}/releases/download/v${SLANG_VERSION}/${SLANG_ARCHIVE}")

FetchContent_Declare(slang URL ${SLANG_URL})
FetchContent_MakeAvailable(slang)
set(SLANG_ROOT ${slang_SOURCE_DIR})

# Define Slang as an imported library
add_library(slang SHARED IMPORTED GLOBAL)
set_target_properties(slang PROPERTIES
  IMPORTED_LOCATION "${SLANG_ROOT}/lib/${SLANG_LIB}"
  INTERFACE_INCLUDE_DIRECTORIES "${SLANG_ROOT}/include"
)


