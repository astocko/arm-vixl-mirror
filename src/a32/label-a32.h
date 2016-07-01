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

#ifndef VIXL_A32_LABEL_A32_H_
#define VIXL_A32_LABEL_A32_H_

#include <stdint.h>
#include <stddef.h>
#include <list>
#include <iomanip>

namespace vixl {
namespace aarch32 {

class Label {
 public:
  typedef int32_t Offset;
  static const Offset kMaxOffset = 0x7fffffff;

  class LabelEmitOperator {
    Label::Offset max_backward_;
    Label::Offset max_forward_;

   public:
    LabelEmitOperator(Label::Offset max_backward, Label::Offset max_forward)
        : max_backward_(max_backward), max_forward_(max_forward) {}
    virtual ~LabelEmitOperator() {}
    virtual uint32_t Encode(uint32_t /*instr*/,
                            Label::Offset /*pc*/,
                            const Label* /*label*/) const {
      return 0;
    }
    Label::Offset GetMaxForwardDistance() const { return max_forward_; }
    Label::Offset GetMaxBackwardDistance() const { return max_backward_; }
  };

  class ForwardReference {
   public:
    ForwardReference(ptrdiff_t location, const LabelEmitOperator& op, bool t32)
        : location_(location), op_(op), is_t32_(t32), is_branch_(false) {}
    Offset GetMaxForwardDistance() const { return op_.GetMaxForwardDistance(); }
    ptrdiff_t GetLocation() const { return location_; }
    uint32_t GetStatePCOffset() const {
      return is_t32_ ? kT32PcDelta : kA32PcDelta;
    }
    bool IsT32() const { return is_t32_; }
    bool IsBranch() const { return is_branch_; }
    void SetIsBranch() { is_branch_ = true; }
    const LabelEmitOperator& GetEmitOperator() const { return op_; }
    Offset GetCheckpoint() const {
      return GetMaxForwardDistance() + (GetLocation() + GetStatePCOffset());
    }

   private:
    ptrdiff_t location_;
    const LabelEmitOperator& op_;
    bool is_t32_;
    bool is_branch_;
  };

  typedef std::list<ForwardReference> ForwardRefList;

  enum UpdateCheckpointOption { kNoUpdateNecessary, kRecomputeCheckpoint };

  static bool CompareCheckpoints(const ForwardReference& a,
                                 const ForwardReference& b) {
    return a.GetCheckpoint() < b.GetCheckpoint();
  }

  Offset GetNextCheckpoint() {
    if (!forward_.empty()) {
      ForwardRefList::iterator min_checkpoint =
          std::min_element(forward_.begin(),
                           forward_.end(),
                           CompareCheckpoints);
      return (*min_checkpoint).GetCheckpoint();
    }
    return kMaxOffset;
  }

 public:
  Label()
      : imm_offset_(kMaxOffset),
        pc_offset_(0),
        is_bound_(false),
        minus_zero_(false),
        is_t32_(false),
        in_veneer_pool_(false),
        checkpoint_(kMaxOffset) {}
  explicit Label(Offset offset, uint32_t pc_offset, bool minus_zero = false)
      : imm_offset_(offset),
        pc_offset_(pc_offset),
        is_bound_(true),
        minus_zero_(minus_zero),
        is_t32_(false),
        in_veneer_pool_(false),
        checkpoint_(kMaxOffset) {}
  ~Label() {}
  bool IsBound() const { return is_bound_; }
  void Bind(Offset offset, bool isT32) {
    VIXL_ASSERT(!IsBound());
    imm_offset_ = offset;
    is_bound_ = true;
    is_t32_ = isT32;
  }
  Offset GetLocation() const {
    VIXL_ASSERT(IsBound());
    return imm_offset_ + static_cast<Offset>(pc_offset_);
  }
  bool IsT32() const {
    VIXL_ASSERT(IsBound());  // Must be bound to know if it's a T32 label
    return is_t32_;
  }
  bool IsMinusZero() const {
    VIXL_ASSERT(IsBound());
    return minus_zero_;
  }
  bool IsInVeneerPool() const { return in_veneer_pool_; }
  void SetInVeneerPool() { in_veneer_pool_ = true; }
  void ResetInVeneerPool() { in_veneer_pool_ = false; }
  void SetCheckpoint(Offset checkpoint) { checkpoint_ = checkpoint; }
  Offset GetCheckpoint() const { return checkpoint_; }
  Offset GetAlignedCheckpoint(int byte_align) const {
    return AlignDown(GetCheckpoint(), byte_align);
  }
  void AddForwardRef(ptrdiff_t instr_location,
                     bool isT32,
                     const LabelEmitOperator& op) {
    forward_.push_back(ForwardReference(instr_location, op, isT32));
  }

  ForwardRefList::iterator GetFirstForwardRef() { return forward_.begin(); }
  ForwardRefList::iterator GetEndForwardRef() { return forward_.end(); }
  // Erase an item in the list. We don't have to recompute the checkpoint as
  // the caller does it.
  ForwardRefList::iterator Erase(ForwardRefList::iterator ref) {
    return forward_.erase(ref);
  }
  ForwardReference& GetBackForwardRef() { return forward_.back(); }

  Offset GetLastInsertForwardDistance() const {
    if (!forward_.empty()) {
      return forward_.back().GetMaxForwardDistance();
    }
    return kMaxOffset;
  }

  void ClearForwardRef() { forward_.clear(); }

  // Only used by the literal pool.
  // Removes the last forward reference, in particular because of a rewind.
  // TODO(all): This is hard to test as the checkpoint could be affected only
  //   if the literal has multiple forward references. So, the literal has to be
  //   shared between multiple instructions and part of the literal pool which
  //   is not yet supperted.
  void InvalidateLastForwardReference(
      UpdateCheckpointOption update_checkpoint = kRecomputeCheckpoint) {
    if (!IsBound()) {
      VIXL_ASSERT(!forward_.empty());
      forward_.pop_back();
    }
    VIXL_ASSERT((update_checkpoint == kNoUpdateNecessary) &&
                ((checkpoint_ == GetNextCheckpoint()) ||
                 ((checkpoint_ == Label::kMaxOffset) && forward_.empty())));
    if (update_checkpoint == kRecomputeCheckpoint) {
      checkpoint_ = GetNextCheckpoint();
    }
  }

  // Only used by the literal pool.
  // Update the checkpoint as the shorter distance from the last
  // literal in the pool's reference location to the point
  // where the forward reference will fail.
  // The last forward reference is assumed to be the one freshly
  // added regarding this literal.
  void UpdateCheckpoint() {
    if (!forward_.empty()) {
      const ForwardReference& ref = forward_.back();
      checkpoint_ = std::min(checkpoint_, ref.GetCheckpoint());
    }
    VIXL_ASSERT(GetNextCheckpoint() == checkpoint_);
  }

  static bool CompareLabels(Label* a, Label* b) {
    return a->GetCheckpoint() < b->GetCheckpoint();
  }

 private:
  // Once bound, location of this label in the code buffer.
  Offset imm_offset_;
  uint32_t pc_offset_;
  // Is the label bound.
  bool is_bound_;
  // Special flag for 'pc - 0'.
  bool minus_zero_;
  // Is the label in T32 state.
  bool is_t32_;
  // True if the label has been inserted in the veneer pool.
  bool in_veneer_pool_;
  // Contains the references to the unbound label
  ForwardRefList forward_;
  // Max offset in the code buffer. Must be emitted before this checkpoint.
  Offset checkpoint_;
};


class PrintLabel {
  Label* label_;
  Label::Offset position_;

 public:
  PrintLabel(Label* label, Label::Offset position)
      : label_(label), position_(position) {}
  Label* GetLabel() const { return label_; }
  friend inline std::ostream& operator<<(std::ostream& os,
                                         const PrintLabel& label) {
    if (label.label_->IsMinusZero()) {
      os << "[pc, #-0]";
    } else {
      os << "0x" << std::hex << std::setw(8) << std::setfill('0')
         << static_cast<int32_t>(label.label_->GetLocation() + label.position_)
         << std::dec;
    }
    return os;
  }
};

}  // namespace aarch32
}  // namespace vixl

#endif  // VIXL_A32_LABEL_A32_H_
