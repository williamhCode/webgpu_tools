#pragma once

#define TRY_SLANG(statement)                                                           \
  {                                                                                    \
    SlangResult res = (statement);                                                     \
    if (SLANG_FAILED(res)) {                                                           \
      return std::unexpected("Slang Error, status = " + std::to_string(res));          \
    }                                                                                  \
  }
