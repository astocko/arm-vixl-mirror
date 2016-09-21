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


#ifndef VIXL_CODE_GENERATION_SCOPES_H_
#define VIXL_CODE_GENERATION_SCOPES_H_


#include "assembler-base-vixl.h"
#include "macro-assembler-interface.h"


namespace vixl {

// This scope will:
// - Allow code emission from the specified `Assembler`.
// - Optionally reserve space in the `CodeBuffer` (if it is managed by VIXL).
// - Optionally, on destruction, check the size of the generated code.
//   (The size can be either exact or a maximum size.)
class CodeBufferCheckScope {
 public:
  // Tell whether or not the scope needs to ensure the associated CodeBuffer
  // has enough space for the requested size.
  enum CheckPolicy { kNoCheck, kCheck };

  // Tell whether or not the scope should assert the amount of code emitted
  // within the scope is consistent with the requested amount.
  enum AssertPolicy {
    kNoAssert,    // No assert required.
    kExactSize,   // The code emitted must be exactly size bytes.
    kMaximumSize  // The code emitted must be at most size bytes.
  };

  // This constructor implicitly calls `Open` to initialise the scope
  // (`assembler` must not be `NULL`), so it is ready to use immediately after
  // it has been constructed.
  CodeBufferCheckScope(AssemblerBase* assembler,
                       size_t size,
                       CheckPolicy check_policy = kCheck,
                       AssertPolicy assert_policy = kMaximumSize)
      : assembler_(NULL)
#ifdef VIXL_DEBUG
        ,
        initialised_(false)
#endif
  {
    Open(assembler, size, check_policy, assert_policy);
  }

  // This constructor does not implicitly initialise the scope. Instead, the
  // user is required to explicitly call the `Open` function before using the
  // scope.
  CodeBufferCheckScope()
      : assembler_(NULL)
#ifdef VIXL_DEBUG
        ,
        initialised_(false)
#endif
  {
    // Nothing to do.
  }

  virtual ~CodeBufferCheckScope() { Close(); }

  // This function performs the actual initialisation work.
  void Open(AssemblerBase* assembler,
            size_t size,
            CheckPolicy check_policy = kCheck,
            AssertPolicy assert_policy = kMaximumSize) {
    VIXL_ASSERT(!initialised_);
    VIXL_ASSERT(assembler != NULL);
    assembler_ = assembler;
    if (check_policy == kCheck) assembler_->GetBuffer()->EnsureSpaceFor(size);
#ifdef VIXL_DEBUG
    limit_ = assembler_->GetSizeOfCodeGenerated() + size;
    assert_policy_ = assert_policy;
    previous_allow_assembler_ = assembler_->AllowAssembler();
    assembler_->SetAllowAssembler(true);
    initialised_ = true;
#else
    USE(assert_policy);
#endif
  }

  // This function performs the cleaning-up work. It must succeed even if the
  // scope has not been opened. It is safe to call multiple times.
  void Close() {
#ifdef VIXL_DEBUG
    if (!initialised_) {
      return;
    }
    assembler_->SetAllowAssembler(previous_allow_assembler_);
    switch (assert_policy_) {
      case kNoAssert:
        break;
      case kExactSize:
        VIXL_ASSERT(assembler_->GetSizeOfCodeGenerated() == limit_);
        break;
      case kMaximumSize:
        VIXL_ASSERT(assembler_->GetSizeOfCodeGenerated() <= limit_);
        break;
      default:
        VIXL_UNREACHABLE();
    }
    initialised_ = false;
#endif
  }

 protected:
  AssemblerBase* assembler_;
  AssertPolicy assert_policy_;
  size_t limit_;
  bool previous_allow_assembler_;
  bool initialised_;
};


// This scope will:
// - Do the same as `CodeBufferCheckSCope`, but:
//   - Always reserve space in the `CodeBuffer`.
//   - Always check the size of the generated code on destruction.
// - Dump pools if the specified size would push them out of range.
// - Block pools in the scope.
// This scope allows the `Assembler` and `MacroAssembler` to be freely and
// safely mixed for its duration.
class EmissionCheckScope : public CodeBufferCheckScope {
 public:
  EmissionCheckScope(MacroAssemblerInterface* masm,
                     size_t size,
                     AssertPolicy assert_policy = kMaximumSize) {
    Open(masm, size, assert_policy);
  }
  virtual ~EmissionCheckScope() { Close(); }

  enum PoolPolicy { kIgnorePools, kCheckPools };

 protected:
  // This constructor should only be used from code that is *currently
  // generating* the pools, to avoid an infinite loop.
  EmissionCheckScope(MacroAssemblerInterface* masm,
                     size_t size,
                     AssertPolicy assert_policy,
                     PoolPolicy pool_policy) {
    Open(masm, size, assert_policy, pool_policy);
  }

  void Open(MacroAssemblerInterface* masm,
            size_t size,
            AssertPolicy assert_policy = kMaximumSize,
            PoolPolicy pool_policy = kCheckPools) {
    pool_policy_ = pool_policy;
    if (masm == NULL) {
      // Nothing to do.
      // We may reach this point in a context of conditional code generation.
      // See `aarch64::MacroAssembler::MoveImmediateHelper()` for an example.
      return;
    }
    if (pool_policy_ == kCheckPools) {
      // Do not use the more generic `EnsureEmitFor()` to avoid duplicating the
      // work to check that enough space is available in the buffer. It is done
      // below when opening `CodeBufferCheckScope`.
      masm->EnsureEmitPoolsFor(size);
      masm->BlockPools();
    }
    // The buffer should be checked *after* we emit the pools.
    CodeBufferCheckScope::Open(masm->GetAssemblerBase(),
                               size,
                               kCheck,
                               assert_policy);
  }

  void Close() {
    if (GetMacroAssembler() == NULL) {
      // Nothing to do.
      return;
    }
    if (pool_policy_ == kCheckPools) {
      GetMacroAssembler()->ReleasePools();
    }
  }

  MacroAssemblerInterface* GetMacroAssembler() const {
    return dynamic_cast<MacroAssemblerInterface*>(assembler_);
  }

  PoolPolicy pool_policy_;
};

// Use this scope when you need a one-to-one mapping between methods and
// instructions. This scope will:
// - Do the same as `EmissionCheckScope`.
// - Block access to the MacroAssemblerInterface (using run-time assertions).
class ExactAssemblyScope : public EmissionCheckScope {
 public:
  ExactAssemblyScope(MacroAssemblerInterface* masm,
                     size_t size,
                     AssertPolicy assert_policy = kExactSize)
      : EmissionCheckScope(masm, size, assert_policy) {
    VIXL_ASSERT(assert_policy != kNoAssert);
#ifdef VIXL_DEBUG
    previous_allow_macro_assembler_ = masm->AllowMacroAssembler();
    masm->SetAllowMacroAssembler(false);
#else
    USE(previous_allow_macro_assembler_);
#endif
  }

  virtual ~ExactAssemblyScope() {
#ifdef VIXL_DEBUG
    MacroAssemblerInterface* masm =
        dynamic_cast<MacroAssemblerInterface*>(assembler_);
    masm->SetAllowMacroAssembler(previous_allow_macro_assembler_);
#endif
  }

 protected:
  // This protected constructor allows overriding the pool policy. It is
  // available to allow this scope to be used in code that handles generation
  // of pools.
  ExactAssemblyScope(MacroAssemblerInterface* masm,
                     size_t size,
                     AssertPolicy assert_policy,
                     PoolPolicy pool_policy)
      : EmissionCheckScope(masm, size, assert_policy, pool_policy) {
    VIXL_ASSERT(assert_policy != kNoAssert);
#ifdef VIXL_DEBUG
    previous_allow_macro_assembler_ = masm->AllowMacroAssembler();
    masm->SetAllowMacroAssembler(false);
#endif
  }

 private:
  bool previous_allow_macro_assembler_;
};


}  // namespace vixl

#endif  // VIXL_CODE_GENERATION_SCOPES_H_
