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

#include "macro-assembler-aarch64.h"

namespace vixl {
namespace aarch64 {

inline EmissionCheckScope::EmissionCheckScope(MacroAssembler* masm,
                                              size_t size,
                                              AssertPolicy assert_policy) {
  Open(masm, size, assert_policy);
}


inline EmissionCheckScope::~EmissionCheckScope() { Close(); }


inline EmissionCheckScope::EmissionCheckScope(MacroAssembler* masm,
                                              size_t size,
                                              AssertPolicy assert_policy,
                                              PoolPolicy pool_policy) {
  Open(masm, size, assert_policy, pool_policy);
}


inline void EmissionCheckScope::Open(MacroAssembler* masm,
                                     size_t size,
                                     AssertPolicy assert_policy,
                                     PoolPolicy pool_policy) {
  masm_ = masm;
  pool_policy_ = pool_policy;
  if (masm_ == NULL) {
    // Nothing to do.
    // We may reach this point in a context of conditional code generation. See
    // `MacroAssembler::MoveImmediateHelper()` for an example.
    return;
  }
  if (pool_policy_ == kCheckPools) {
    // Do not use the more generic `EnsureEmitFor()` to avoid duplicating the
    // work to check that enough space is available in the buffer. It is done
    // below when opening `CodeBufferCheckScope`.
    masm_->EnsureEmitPoolsFor(size);
    masm_->BlockPools();
  }
  // The buffer should be checked *after* we emit the pools.
  CodeBufferCheckScope::Open(masm_, size, kCheck, assert_policy);
}


inline void EmissionCheckScope::Close() {
  if (masm_ == NULL) {
    // Nothing to do.
    return;
  }
  if (pool_policy_ == kCheckPools) {
    masm_->ReleasePools();
  }
}


inline void UseScratchRegisterScope::Open(MacroAssembler* masm) {
  VIXL_ASSERT(!initialised_);
  VIXL_ASSERT(masm != NULL);
  available_ = masm->GetScratchRegisterList();
  availablefp_ = masm->GetScratchFPRegisterList();
  old_available_ = available_->GetList();
  old_availablefp_ = availablefp_->GetList();
  VIXL_ASSERT(available_->GetType() == CPURegister::kRegister);
  VIXL_ASSERT(availablefp_->GetType() == CPURegister::kVRegister);
#ifdef VIXL_DEBUG
  initialised_ = true;
#endif
}


inline void UseScratchRegisterScope::Close() {
  if (available_) {
    available_->SetList(old_available_);
    available_ = NULL;
  }
  if (availablefp_) {
    availablefp_->SetList(old_availablefp_);
    availablefp_ = NULL;
  }
#ifdef VIXL_DEBUG
  initialised_ = false;
#endif
}


inline UseScratchRegisterScope::UseScratchRegisterScope(MacroAssembler* masm) {
#ifdef VIXL_DEBUG
  initialised_ = false;
#else
  USE(initialised_);
#endif
  Open(masm);
}

// This allows deferred (and optional) initialisation of the scope.
inline UseScratchRegisterScope::UseScratchRegisterScope()
    : available_(NULL),
      availablefp_(NULL),
      old_available_(0),
      old_availablefp_(0) {
#ifdef VIXL_DEBUG
  initialised_ = false;
#endif
}

inline UseScratchRegisterScope::~UseScratchRegisterScope() { Close(); }


}  // namespace aarch64
}  // namespace vixl
