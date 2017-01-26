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


// Forward declare internal::Scope.
namespace internal {
template <typename S, typename O, uint32_t M>
class Scope;
}

// User facing scope policies.

class Policy {
 public:
  // Bitfield declaration of all policies supported by scopes.
  enum PolicyType {
    kNone = 0,

    // Guarantee that there is enough space in the code buffer by growing it if
    // necessary.
    kReserveBufferSpace = 0x1,

    // Tell whether or not the scope should assert the amount of code emitted
    // within it is consistent with the requested amount.
    //   - The code emitted must be exactly size bytes:
    kExactSize = 0x2,
    //   - The code emitted must be at most size bytes:
    kMaximumSize = 0x4
    // TODO: The above checks are only performed in a debug build. Consider
    // having extra policies for release builds.
  };

  // Masks defining the layout of policies in a bitfield.
  static const uint32_t kReserveBufferSpaceMask = kReserveBufferSpace;
  static const uint32_t kCheckSizeMask = kExactSize | kMaximumSize;

  // By default, a policy does nothing.
  Policy() : value_(kNone) {}
  // Allow implicitly creating a Policy from one of the underlying values:
  // ~~~
  // Policy p = Policy::kExactSize;
  // Policy p(Policy::kReserveBufferSpace);
  // ~~~
  Policy(PolicyType value) : value_(value) {}  // NOLINT(runtime/explicit)

  // Extract the underlying value of the policy, applying one of the predefined
  // masks.
  PolicyType GetValue(uint32_t mask) {
    VIXL_ASSERT((mask == kReserveBufferSpaceMask) || (mask == kCheckSizeMask));
    return static_cast<PolicyType>(value_ & mask);
  }

  // Create a new Policy by combining two of them. They should not overlap.
  static Policy Combine(const Policy& lhs, const Policy& rhs) {
    VIXL_ASSERT((lhs.value_ & rhs.value_) == 0);
    return Policy(static_cast<PolicyType>(lhs.value_ | rhs.value_));
  }

 private:
  PolicyType value_;

  // Allow the top-level scope to read `value_` directly to check the given
  // policy is correct.
  template <typename S, typename O, uint32_t M>
  friend class internal::Scope;
};

namespace internal {

// Underlying implementation of all of VIXL's scopes.
//
// A scope temporarily acts on the state of an object, which can be an
// assembler, a macro-assembler or a code buffer. The scope semantics are:
//
// * The default constructor creates an "uninitialised" scope. When one does
//   this, "Open" has to be called to initialise it.
//
//    ~~~
//    Scope s;
//    ~~~
//
// * Creating a scope by passing it an object initialises it and makes it
//   effective unless said object is NULL.
//
//    ~~~
//    // The scope becomes effective now.
//    Scope s(&object, 4, Policy::kDoSomething);
//    // This is equivalent to not initialising the scope.
//    Scope s(NULL, 4, Policy::kDoSomething);
//    ~~~
//
//  * If the scope is not initialised, the users may call "Open" to initialise
//    it. It is illegal to initialise a scope if it is already initialised.
//    Additionally, the object must not be NULL, unlike when using the
//    constructor.
//
//    ~~~
//    Scope s;
//    s.Open(&object, 4, Policy::kDoSomething);
//    ~~~
//
//  * The user may call "Close" at any time to reset the state of the
//    object. "Close" may be called multiple times and has to work if the scope
//    is not initialised. You may call "Open" again after a call to "Close".
//
//    ~~~
//    {
//      Scope s(&object, 4, Policy::kDoSomething);
//      s.Close();
//      // Do more things.
//    }
//    ~~~
//
//  * The destructor will always call "Close".

// Adding new scopes utilities using the base `Scope` class.
//
// This class implements the user facing API. Users instantiate scopes by giving
// a Policy, which includes zero or more rules to be enforced by the scope.
//
// Creating a new scope will be done by implementing the `DoOpen` and `DoClose`
// methods. We can assume these methods will always mirror each other. A new
// scope will need to know what type it acts on, such a `CodeBuffer` or
// `MacroAssembler`, and what policies in accepts, represented as a mask. For
// example:
//
// ~~~
// class ReserveBufferScope : public Scope<ReserveBufferScope,
//                                         CodeBuffer,
//                                         Policy::kReserveBufferSpaceMask> {
//   typedef Scope<ReserveBufferScope,
//                 CodeBuffer,
//                 Policy::kReserveBufferSpaceMask> Base;
//
//  public:
//   ReserveBufferScope() : Base() {}
//   ReserveBufferScope(CodeBuffer* buffer, size_t size, Policy policy)
//       : Base(buffer, size, policy) {}
//
//  private:
//   void DoOpen(CodeBuffer* buffer, size_t size, Policy policy) {
//     if (policy.GetValue(Policy::kReserveBufferSpaceMask) ==
//         Policy::kReserveBufferSpace) {
//       buffer->EnsureSpaceFor(size);
//     }
//   }
//   void DoClose(CodeBuffer* buffer, size_t size, Policy policy) {
//     // Do nothing
//   }
//
//   friend void Base::Open(CodeBuffer*, size_t, Policy);
//   friend void Base::Close();
// };
// ~~~
//
// The example above shows we are using the "Curious Recurring Template
// Pattern", which means we have to pass the derived type as the first template
// parameter. The `Scope` base class will call back to `DoOpen` and `DoClose`
// accordingly. However, since they are private methods of the derived class, we
// have to specifically mark the `Open` and `Close` methods as friends.
//
// The main idea behind this design is that all the core functionality of a
// scope is in one place: `DoOpen`/`DoClose`. And if one wants to create a new
// scope by extending an existing one, they cannot redefine these methods to do
// something else. This might be seen as a drawback but it forces us to keep
// scopes self-contained and therefore easy to understand.

// The template arguments are:
//   S: Derived class implementing the scope.
//   O: Object the scope acts on.
//   M: uint32_t mask representing the policies the scope accepts.

template <typename S, typename O, uint32_t M>
class Scope {
 public:
  Scope() : initialised_(false) {}

  Scope(O* object, size_t size, Policy policy) : initialised_(false) {
    // TODO: Consider not allowing the object to be NULL.
    if (object != NULL) {
      Open(object, size, policy);
    }
  }

  virtual ~Scope() { Close(); }

  void Open(O* object, size_t size, Policy policy) {
    // Check that the bits set in the given policy are consistent with what the
    // scope accepts. In practice, that means that all bits outside of M should
    // be cleared.
    VIXL_ASSERT((policy.value_ & ~M) == 0);
    VIXL_ASSERT(!initialised_);
    static_cast<S*>(this)->DoOpen(object, size, policy);
    object_ = object;
    size_ = size;
    policy_ = policy;
    initialised_ = true;
  }

  void Close() {
    if (!initialised_) {
      return;
    }
    static_cast<S*>(this)->DoClose(object_, size_, policy_);
    initialised_ = false;
  }

 private:
  O* object_;
  size_t size_;
  Policy policy_;
  bool initialised_;
};

// We want to allow the assembler temporarily and emit exactly a sequence of
// instructions, but at the same time, we cannot emit literal or veneer
// pools. This is useful internally for macro-assemblers to implement pools
// themselves.
// This scope will:
//   - Make sure enough space is allocated in the buffer.
//   - Restrict usage to the Assembler.
//   - Check that we haven't emitted more than we said according to the
//     CheckSize policy.
class ExactAssemblyScopeWithoutPoolsCheck
    : public Scope<ExactAssemblyScopeWithoutPoolsCheck,
                   MacroAssemblerInterface,
                   Policy::kCheckSizeMask> {
  typedef Scope<ExactAssemblyScopeWithoutPoolsCheck,
                MacroAssemblerInterface,
                Policy::kCheckSizeMask> Base;

 public:
  ExactAssemblyScopeWithoutPoolsCheck(MacroAssemblerInterface* masm,
                                      size_t size,
                                      Policy policy = Policy::kExactSize)
      : Base(masm, size, policy) {}

 private:
  void DoOpen(MacroAssemblerInterface* masm, size_t size, Policy /*policy*/) {
    masm->AsAssemblerBase()->GetBuffer()->EnsureSpaceFor(size);

    limit_ = masm->AsAssemblerBase()->GetSizeOfCodeGenerated() + size;

    previous_allow_macro_assembler_ = masm->AllowMacroInstructions();
    masm->SetAllowMacroInstructions(false);
    previous_allow_assembler_ = masm->AsAssemblerBase()->AllowAssembler();
    masm->AsAssemblerBase()->SetAllowAssembler(true);
  }

  void DoClose(MacroAssemblerInterface* masm, size_t /*size*/, Policy policy) {
    switch (policy.GetValue(Policy::kCheckSizeMask)) {
      case Policy::kExactSize:
        VIXL_ASSERT(masm->AsAssemblerBase()->GetSizeOfCodeGenerated() ==
                    limit_);
        break;
      case Policy::kMaximumSize:
        VIXL_ASSERT(masm->AsAssemblerBase()->GetSizeOfCodeGenerated() <=
                    limit_);
        break;
      default:
        VIXL_UNREACHABLE();
        break;
    }

    masm->SetAllowMacroInstructions(previous_allow_macro_assembler_);
    masm->AsAssemblerBase()->SetAllowAssembler(previous_allow_assembler_);
  }

  // Needed so that the base class may call `DoOpen` and `DoClose` which are
  // private.
  friend void Base::Open(MacroAssemblerInterface*, size_t, Policy);
  friend void Base::Close();

  size_t limit_;
  bool previous_allow_assembler_;
  bool previous_allow_macro_assembler_;
};

}  // namespace internal

// This scope will:
//   - Make sure enough space is allocated in the buffer according to the
//     ReserveBufferSpace policy.
//   - Allow usage of the Assembler directly.
//   - Check that we haven't emitted more than we said according to the
//     CheckSize policy.
class CodeBufferCheckScope
    : public internal::Scope<CodeBufferCheckScope,
                             internal::AssemblerBase,
                             Policy::kReserveBufferSpace |
                                 Policy::kCheckSizeMask> {
  typedef Scope<CodeBufferCheckScope,
                internal::AssemblerBase,
                Policy::kReserveBufferSpace | Policy::kCheckSizeMask> Base;

 public:
  CodeBufferCheckScope() : Base() {}

  CodeBufferCheckScope(internal::AssemblerBase* assembler,
                       size_t size,
                       Policy reserve_policy = Policy::kReserveBufferSpace,
                       Policy size_policy = Policy::kMaximumSize)
      : Base(assembler, size, Policy::Combine(reserve_policy, size_policy)) {}
  void Open(internal::AssemblerBase* assembler,
            size_t size,
            Policy reserve_policy = Policy::kReserveBufferSpace,
            Policy size_policy = Policy::kMaximumSize) {
    Base::Open(assembler, size, Policy::Combine(reserve_policy, size_policy));
  }

 private:
  void DoOpen(internal::AssemblerBase* assembler, size_t size, Policy policy) {
    if (policy.GetValue(Policy::kReserveBufferSpaceMask) ==
        Policy::kReserveBufferSpace) {
      assembler->GetBuffer()->EnsureSpaceFor(size);
    }

    limit_ = assembler->GetSizeOfCodeGenerated() + size;

    previous_allow_assembler_ = assembler->AllowAssembler();
    assembler->SetAllowAssembler(true);
  }

  void DoClose(internal::AssemblerBase* assembler,
               size_t /*size*/,
               Policy policy) {
    switch (policy.GetValue(Policy::kCheckSizeMask)) {
      case Policy::kExactSize:
        VIXL_ASSERT(assembler->GetSizeOfCodeGenerated() == limit_);
        break;
      case Policy::kMaximumSize:
        VIXL_ASSERT(assembler->GetSizeOfCodeGenerated() <= limit_);
        break;
      default:
        break;
    }

    assembler->SetAllowAssembler(previous_allow_assembler_);
  }

  // Needed so that the base class may call `DoOpen` and `DoClose` which are
  // private.
  friend void Base::Open(internal::AssemblerBase*, size_t, Policy);
  friend void Base::Close();

  size_t limit_;
  bool previous_allow_assembler_;
};

// This scope will:
//   - Emit pools if the specified size would push them out of range.
//   - Block pools emission for the duration of the scope.
//   - Allocate space in the buffer if needed.
//   - Allow usage of the Assembler directly.
//   - Check that we haven't emitted more than we said according to the
//     CheckSize policy.
// This scope allows the `Assembler` and `MacroAssembler` to be freely and
// safely mixed for its duration.
class EmissionCheckScope : public internal::Scope<EmissionCheckScope,
                                                  MacroAssemblerInterface,
                                                  Policy::kCheckSizeMask> {
  typedef Scope<EmissionCheckScope,
                MacroAssemblerInterface,
                Policy::kCheckSizeMask> Base;

 public:
  EmissionCheckScope() : Base() {}

  EmissionCheckScope(MacroAssemblerInterface* masm,
                     size_t size,
                     Policy policy = Policy::kMaximumSize)
      : Base(masm, size, policy) {}
  void Open(MacroAssemblerInterface* masm,
            size_t size,
            Policy policy = Policy::kMaximumSize) {
    Base::Open(masm, size, policy);
  }

 private:
  void DoOpen(MacroAssemblerInterface* masm, size_t size, Policy /*policy*/) {
    masm->EnsureEmitPoolsFor(size);
    masm->BlockPools();

    masm->AsAssemblerBase()->GetBuffer()->EnsureSpaceFor(size);

    limit_ = masm->AsAssemblerBase()->GetSizeOfCodeGenerated() + size;

    previous_allow_assembler_ = masm->AsAssemblerBase()->AllowAssembler();
    masm->AsAssemblerBase()->SetAllowAssembler(true);
  }

  void DoClose(MacroAssemblerInterface* masm, size_t /*size*/, Policy policy) {
    switch (policy.GetValue(Policy::kCheckSizeMask)) {
      case Policy::kExactSize:
        VIXL_ASSERT(masm->AsAssemblerBase()->GetSizeOfCodeGenerated() ==
                    limit_);
        break;
      case Policy::kMaximumSize:
        VIXL_ASSERT(masm->AsAssemblerBase()->GetSizeOfCodeGenerated() <=
                    limit_);
        break;
      default:
        VIXL_UNREACHABLE();
        break;
    }

    masm->AsAssemblerBase()->SetAllowAssembler(previous_allow_assembler_);

    masm->ReleasePools();
  }

  // Needed so that the base class may call `DoOpen` and `DoClose` which are
  // private.
  friend void Base::Open(MacroAssemblerInterface*, size_t, Policy);
  friend void Base::Close();

  size_t limit_;
  bool previous_allow_assembler_;
};

// This scope will:
//   - Emit pools if the specified size would push them out of range.
//   - Block pools emission for the duration of the scope.
//   - Allocate space in the buffer if needed.
//   - Restrict usage to the Assembler.
//   - Check that we haven't emitted more than we said according to the
//     CheckSize policy.
class ExactAssemblyScope : public internal::Scope<ExactAssemblyScope,
                                                  MacroAssemblerInterface,
                                                  Policy::kCheckSizeMask> {
  typedef Scope<ExactAssemblyScope,
                MacroAssemblerInterface,
                Policy::kCheckSizeMask> Base;

 public:
  ExactAssemblyScope() : Base() {}

  ExactAssemblyScope(MacroAssemblerInterface* masm,
                     size_t size,
                     Policy policy = Policy::kExactSize)
      : Base(masm, size, policy) {}

  void Open(MacroAssemblerInterface* masm,
            size_t size,
            Policy policy = Policy::kExactSize) {
    Base::Open(masm, size, policy);
  }

 private:
  void DoOpen(MacroAssemblerInterface* masm, size_t size, Policy /*policy*/) {
    masm->EnsureEmitPoolsFor(size);
    masm->BlockPools();

    masm->AsAssemblerBase()->GetBuffer()->EnsureSpaceFor(size);

    limit_ = masm->AsAssemblerBase()->GetSizeOfCodeGenerated() + size;

    previous_allow_macro_assembler_ = masm->AllowMacroInstructions();
    masm->SetAllowMacroInstructions(false);
    previous_allow_assembler_ = masm->AsAssemblerBase()->AllowAssembler();
    masm->AsAssemblerBase()->SetAllowAssembler(true);
  }

  void DoClose(MacroAssemblerInterface* masm, size_t /*size*/, Policy policy) {
    switch (policy.GetValue(Policy::kCheckSizeMask)) {
      case Policy::kExactSize:
        VIXL_ASSERT(masm->AsAssemblerBase()->GetSizeOfCodeGenerated() ==
                    limit_);
        break;
      case Policy::kMaximumSize:
        VIXL_ASSERT(masm->AsAssemblerBase()->GetSizeOfCodeGenerated() <=
                    limit_);
        break;
      default:
        break;
    }

    masm->SetAllowMacroInstructions(previous_allow_macro_assembler_);
    masm->AsAssemblerBase()->SetAllowAssembler(previous_allow_assembler_);

    masm->ReleasePools();
  }

  // Needed so that the base class may call `DoOpen` and `DoClose` which are
  // private.
  friend void Base::Open(MacroAssemblerInterface*, size_t, Policy);
  friend void Base::Close();

  size_t limit_;
  bool previous_allow_assembler_;
  bool previous_allow_macro_assembler_;
};

}  // namespace vixl

#endif  // VIXL_CODE_GENERATION_SCOPES_H_
