// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// https://developers.google.com/protocol-buffers/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef GOOGLE_PROTOBUF_RAW_PTR_H__
#define GOOGLE_PROTOBUF_RAW_PTR_H__

#include <algorithm>

#include "absl/base/optimization.h"

// Must be included last.
#include "google/protobuf/port_def.inc"

namespace google {
namespace protobuf {
namespace internal {

PROTOBUF_EXPORT ABSL_CACHELINE_ALIGNED extern const char
    kZeroBuffer[std::max(ABSL_CACHELINE_SIZE, 64)];

// This class is trivially copyable/trivially destructible and constexpr
// constructible. The class allows for storing a raw pointer to a non-trivial
// object in a constexpr context.
template <typename T>
class RawPtr {
 public:
  constexpr RawPtr() : RawPtr(kZeroBuffer) {
    static_assert(sizeof(T) <= sizeof(kZeroBuffer), "");
    static_assert(alignof(T) <= ABSL_CACHELINE_SIZE, "");
  }
  explicit constexpr RawPtr(const void* p) : p_(const_cast<void*>(p)) {}

  bool IsDefault() const { return p_ == kZeroBuffer; }
  void DeleteIfNotDefault() {
    if (!IsDefault()) delete Get();
  }
  void ClearIfNotDefault() {
    if (!IsDefault()) Get()->Clear();
  }

  void Set(const void* p) { p_ = const_cast<void*>(p); }
  T* Get() const { return reinterpret_cast<T*>(p_); }
  T* operator->() const { return Get(); }
  T& operator*() const { return *Get(); }

 private:
  void* p_;
};

constexpr void* DefaultRawPtr() {
  return const_cast<void*>(static_cast<const void*>(kZeroBuffer));
}

}  // namespace internal
}  // namespace protobuf
}  // namespace google

#include "google/protobuf/port_undef.inc"

#endif  // GOOGLE_PROTOBUF_RAW_PTR_H__
