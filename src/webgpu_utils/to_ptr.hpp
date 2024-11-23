#pragma once

template <typename T>
const T* ToPtr(T&& value) {
  return &value;
}
