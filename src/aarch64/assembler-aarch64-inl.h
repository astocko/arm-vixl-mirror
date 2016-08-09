// Copyright 2016, VIXL authors
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


#include "assembler-aarch64.h"

namespace vixl {
namespace aarch64 {

inline CodeBufferCheckScope::CodeBufferCheckScope(Assembler* assm,
                                                  size_t size,
                                                  CheckPolicy check_policy,
                                                  AssertPolicy assert_policy)
#ifdef VIXL_DEBUG
    : initialised_(false)
#endif
{
  Open(assm, size, check_policy, assert_policy);
}


inline CodeBufferCheckScope::CodeBufferCheckScope()
#ifdef VIXL_DEBUG
    : initialised_(false)
#endif
{
  // Nothing to do.
}


inline void CodeBufferCheckScope::Open(Assembler* assm,
                                       size_t size,
                                       CheckPolicy check_policy,
                                       AssertPolicy assert_policy) {
  VIXL_ASSERT(!initialised_);
  VIXL_ASSERT(assm != NULL);
  if (check_policy == kCheck) assm->EnsureSpaceFor(size);
#ifdef VIXL_DEBUG
  assm_ = assm;
  size_ = size;
  assert_policy_ = assert_policy;
  assm_->bind(&start_);
  assm_->AcquireBuffer();
  initialised_ = true;
#else
  USE(assert_policy);
#endif
}


inline CodeBufferCheckScope::~CodeBufferCheckScope() { Close(); }


inline void CodeBufferCheckScope::Close() {
#ifdef VIXL_DEBUG
  if (!initialised_) {
    return;
  }
  assm_->ReleaseBuffer();
  switch (assert_policy_) {
    case kNoAssert:
      break;
    case kExactSize:
      VIXL_ASSERT(assm_->GetSizeOfCodeGeneratedSince(&start_) == size_);
      break;
    case kMaximumSize:
      VIXL_ASSERT(assm_->GetSizeOfCodeGeneratedSince(&start_) <= size_);
      break;
    default:
      VIXL_UNREACHABLE();
  }
  initialised_ = false;
#endif
}

}  // namespace aarch64
}  // namespace vixl
