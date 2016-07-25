// Copyright 2015, VIXL authors
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//   * Neither the name of ARM Limited nor the names of its contributors may be
//     used to endorse or promote products derived from this software without
//     specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "test-utils.h"

#include <sys/mman.h>

#include "globals-vixl.h"
#include "aarch64/cpu-aarch64.h"

namespace vixl {

// BSD uses `MAP_ANON` instead of the Linux `MAP_ANONYMOUS`. The `MAP_ANONYMOUS`
// alias should generally be available, but is not always, so define it manually
// if necessary.
#if !defined(MAP_ANONYMOUS) && defined(MAP_ANON)
#define MAP_ANONYMOUS MAP_ANON
#endif

ExecutableMemory::ExecutableMemory(size_t size)
  : size_(size),
    buffer_(mmap(NULL, size, PROT_READ | PROT_WRITE | PROT_EXEC,
                 MAP_SHARED | MAP_ANONYMOUS, -1, 0)) {
  VIXL_ASSERT(reinterpret_cast<intptr_t>(buffer_) != -1);
}

ExecutableMemory::~ExecutableMemory() {
  munmap(buffer_, size_);
}

void ExecutableMemory::Write(const byte* code_start, size_t code_size) const {
  VIXL_CHECK(code_size <= size_);
  memcpy(buffer_, code_start, size_);
}

void ExecutableMemory::Execute(int offset) const {
  void (*test_function)(void);

  VIXL_ASSERT((offset >= 0) && (static_cast<size_t>(offset) < size_));
  VIXL_STATIC_ASSERT(sizeof(buffer_) == sizeof(test_function));
  VIXL_STATIC_ASSERT(sizeof(uintptr_t) == sizeof(test_function));
  uintptr_t entry_point = reinterpret_cast<uintptr_t>(buffer_);
  entry_point += offset;
  memcpy(&test_function, &entry_point, sizeof(test_function));

#if defined(__aarch64__)
  aarch64::CPU::EnsureIAndDCacheCoherency(buffer_, size_);
#elif defined(__arm__)
  // TODO: Do not use __builtin___clear_cache and instead implement
  // `CPU::EnsureIAndDCacheCoherency` for aarch32.
  __builtin___clear_cache(buffer_, reinterpret_cast<char*>(buffer_) + size_);
#endif
  test_function();
}

}  // namespace vixl
