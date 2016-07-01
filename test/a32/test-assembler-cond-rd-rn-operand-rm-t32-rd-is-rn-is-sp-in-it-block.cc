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

// -----------------------------------------------------------------------------
// This file is auto generated from the
// test/a32/config/template-assembler-a32.cc.in template file using
// tools/generate_tests.py.
//
// PLEASE DO NOT EDIT.
// -----------------------------------------------------------------------------

#include "test-runner.h"

#include "test-utils.h"
#include "test-utils-a32.h"

#include "a32/assembler-a32.h"
#include "a32/macro-assembler-a32.h"

#define BUF_SIZE (4096)

namespace vixl {
namespace aarch32 {

// List of instruction mnemonics.
#define FOREACH_INSTRUCTION(M) M(Add)

// Values to be passed to the assembler to produce the instruction under test.
struct Operands {
  Condition cond;
  Register rd;
  Register rn;
  Register rm;
};

// This structure contains all data needed to test one specific
// instruction.
struct TestData {
  // The `operands` field represents what to pass to the assembler to
  // produce the instruction.
  Operands operands;
  // Optionally expect the MacroAssembler to have generated an extra
  // instruction. This is used when the instruction needs to be in an IT block.
  const char* expect_instruction_before;
  // Description of the operands, used for error reporting.
  const char* operands_description;
  // Unique identifier, used for generating traces.
  const char* identifier;
};

struct TestResult {
  size_t size;
  const byte* encoding;
};

// Each element of this array produce one instruction encoding.
static const TestData kTests[] = {
    {{eq, r13, r13, r0}, "It eq;", "eq r13 r13 r0", "eq_r13_r13_r0"},
    {{eq, r13, r13, r1}, "It eq;", "eq r13 r13 r1", "eq_r13_r13_r1"},
    {{eq, r13, r13, r2}, "It eq;", "eq r13 r13 r2", "eq_r13_r13_r2"},
    {{eq, r13, r13, r3}, "It eq;", "eq r13 r13 r3", "eq_r13_r13_r3"},
    {{eq, r13, r13, r4}, "It eq;", "eq r13 r13 r4", "eq_r13_r13_r4"},
    {{eq, r13, r13, r5}, "It eq;", "eq r13 r13 r5", "eq_r13_r13_r5"},
    {{eq, r13, r13, r6}, "It eq;", "eq r13 r13 r6", "eq_r13_r13_r6"},
    {{eq, r13, r13, r7}, "It eq;", "eq r13 r13 r7", "eq_r13_r13_r7"},
    {{eq, r13, r13, r8}, "It eq;", "eq r13 r13 r8", "eq_r13_r13_r8"},
    {{eq, r13, r13, r9}, "It eq;", "eq r13 r13 r9", "eq_r13_r13_r9"},
    {{eq, r13, r13, r10}, "It eq;", "eq r13 r13 r10", "eq_r13_r13_r10"},
    {{eq, r13, r13, r11}, "It eq;", "eq r13 r13 r11", "eq_r13_r13_r11"},
    {{eq, r13, r13, r12}, "It eq;", "eq r13 r13 r12", "eq_r13_r13_r12"},
    {{eq, r13, r13, r13}, "It eq;", "eq r13 r13 r13", "eq_r13_r13_r13"},
    {{eq, r13, r13, r14}, "It eq;", "eq r13 r13 r14", "eq_r13_r13_r14"},
    {{ne, r13, r13, r0}, "It ne;", "ne r13 r13 r0", "ne_r13_r13_r0"},
    {{ne, r13, r13, r1}, "It ne;", "ne r13 r13 r1", "ne_r13_r13_r1"},
    {{ne, r13, r13, r2}, "It ne;", "ne r13 r13 r2", "ne_r13_r13_r2"},
    {{ne, r13, r13, r3}, "It ne;", "ne r13 r13 r3", "ne_r13_r13_r3"},
    {{ne, r13, r13, r4}, "It ne;", "ne r13 r13 r4", "ne_r13_r13_r4"},
    {{ne, r13, r13, r5}, "It ne;", "ne r13 r13 r5", "ne_r13_r13_r5"},
    {{ne, r13, r13, r6}, "It ne;", "ne r13 r13 r6", "ne_r13_r13_r6"},
    {{ne, r13, r13, r7}, "It ne;", "ne r13 r13 r7", "ne_r13_r13_r7"},
    {{ne, r13, r13, r8}, "It ne;", "ne r13 r13 r8", "ne_r13_r13_r8"},
    {{ne, r13, r13, r9}, "It ne;", "ne r13 r13 r9", "ne_r13_r13_r9"},
    {{ne, r13, r13, r10}, "It ne;", "ne r13 r13 r10", "ne_r13_r13_r10"},
    {{ne, r13, r13, r11}, "It ne;", "ne r13 r13 r11", "ne_r13_r13_r11"},
    {{ne, r13, r13, r12}, "It ne;", "ne r13 r13 r12", "ne_r13_r13_r12"},
    {{ne, r13, r13, r13}, "It ne;", "ne r13 r13 r13", "ne_r13_r13_r13"},
    {{ne, r13, r13, r14}, "It ne;", "ne r13 r13 r14", "ne_r13_r13_r14"},
    {{cs, r13, r13, r0}, "It cs;", "cs r13 r13 r0", "cs_r13_r13_r0"},
    {{cs, r13, r13, r1}, "It cs;", "cs r13 r13 r1", "cs_r13_r13_r1"},
    {{cs, r13, r13, r2}, "It cs;", "cs r13 r13 r2", "cs_r13_r13_r2"},
    {{cs, r13, r13, r3}, "It cs;", "cs r13 r13 r3", "cs_r13_r13_r3"},
    {{cs, r13, r13, r4}, "It cs;", "cs r13 r13 r4", "cs_r13_r13_r4"},
    {{cs, r13, r13, r5}, "It cs;", "cs r13 r13 r5", "cs_r13_r13_r5"},
    {{cs, r13, r13, r6}, "It cs;", "cs r13 r13 r6", "cs_r13_r13_r6"},
    {{cs, r13, r13, r7}, "It cs;", "cs r13 r13 r7", "cs_r13_r13_r7"},
    {{cs, r13, r13, r8}, "It cs;", "cs r13 r13 r8", "cs_r13_r13_r8"},
    {{cs, r13, r13, r9}, "It cs;", "cs r13 r13 r9", "cs_r13_r13_r9"},
    {{cs, r13, r13, r10}, "It cs;", "cs r13 r13 r10", "cs_r13_r13_r10"},
    {{cs, r13, r13, r11}, "It cs;", "cs r13 r13 r11", "cs_r13_r13_r11"},
    {{cs, r13, r13, r12}, "It cs;", "cs r13 r13 r12", "cs_r13_r13_r12"},
    {{cs, r13, r13, r13}, "It cs;", "cs r13 r13 r13", "cs_r13_r13_r13"},
    {{cs, r13, r13, r14}, "It cs;", "cs r13 r13 r14", "cs_r13_r13_r14"},
    {{cc, r13, r13, r0}, "It cc;", "cc r13 r13 r0", "cc_r13_r13_r0"},
    {{cc, r13, r13, r1}, "It cc;", "cc r13 r13 r1", "cc_r13_r13_r1"},
    {{cc, r13, r13, r2}, "It cc;", "cc r13 r13 r2", "cc_r13_r13_r2"},
    {{cc, r13, r13, r3}, "It cc;", "cc r13 r13 r3", "cc_r13_r13_r3"},
    {{cc, r13, r13, r4}, "It cc;", "cc r13 r13 r4", "cc_r13_r13_r4"},
    {{cc, r13, r13, r5}, "It cc;", "cc r13 r13 r5", "cc_r13_r13_r5"},
    {{cc, r13, r13, r6}, "It cc;", "cc r13 r13 r6", "cc_r13_r13_r6"},
    {{cc, r13, r13, r7}, "It cc;", "cc r13 r13 r7", "cc_r13_r13_r7"},
    {{cc, r13, r13, r8}, "It cc;", "cc r13 r13 r8", "cc_r13_r13_r8"},
    {{cc, r13, r13, r9}, "It cc;", "cc r13 r13 r9", "cc_r13_r13_r9"},
    {{cc, r13, r13, r10}, "It cc;", "cc r13 r13 r10", "cc_r13_r13_r10"},
    {{cc, r13, r13, r11}, "It cc;", "cc r13 r13 r11", "cc_r13_r13_r11"},
    {{cc, r13, r13, r12}, "It cc;", "cc r13 r13 r12", "cc_r13_r13_r12"},
    {{cc, r13, r13, r13}, "It cc;", "cc r13 r13 r13", "cc_r13_r13_r13"},
    {{cc, r13, r13, r14}, "It cc;", "cc r13 r13 r14", "cc_r13_r13_r14"},
    {{mi, r13, r13, r0}, "It mi;", "mi r13 r13 r0", "mi_r13_r13_r0"},
    {{mi, r13, r13, r1}, "It mi;", "mi r13 r13 r1", "mi_r13_r13_r1"},
    {{mi, r13, r13, r2}, "It mi;", "mi r13 r13 r2", "mi_r13_r13_r2"},
    {{mi, r13, r13, r3}, "It mi;", "mi r13 r13 r3", "mi_r13_r13_r3"},
    {{mi, r13, r13, r4}, "It mi;", "mi r13 r13 r4", "mi_r13_r13_r4"},
    {{mi, r13, r13, r5}, "It mi;", "mi r13 r13 r5", "mi_r13_r13_r5"},
    {{mi, r13, r13, r6}, "It mi;", "mi r13 r13 r6", "mi_r13_r13_r6"},
    {{mi, r13, r13, r7}, "It mi;", "mi r13 r13 r7", "mi_r13_r13_r7"},
    {{mi, r13, r13, r8}, "It mi;", "mi r13 r13 r8", "mi_r13_r13_r8"},
    {{mi, r13, r13, r9}, "It mi;", "mi r13 r13 r9", "mi_r13_r13_r9"},
    {{mi, r13, r13, r10}, "It mi;", "mi r13 r13 r10", "mi_r13_r13_r10"},
    {{mi, r13, r13, r11}, "It mi;", "mi r13 r13 r11", "mi_r13_r13_r11"},
    {{mi, r13, r13, r12}, "It mi;", "mi r13 r13 r12", "mi_r13_r13_r12"},
    {{mi, r13, r13, r13}, "It mi;", "mi r13 r13 r13", "mi_r13_r13_r13"},
    {{mi, r13, r13, r14}, "It mi;", "mi r13 r13 r14", "mi_r13_r13_r14"},
    {{pl, r13, r13, r0}, "It pl;", "pl r13 r13 r0", "pl_r13_r13_r0"},
    {{pl, r13, r13, r1}, "It pl;", "pl r13 r13 r1", "pl_r13_r13_r1"},
    {{pl, r13, r13, r2}, "It pl;", "pl r13 r13 r2", "pl_r13_r13_r2"},
    {{pl, r13, r13, r3}, "It pl;", "pl r13 r13 r3", "pl_r13_r13_r3"},
    {{pl, r13, r13, r4}, "It pl;", "pl r13 r13 r4", "pl_r13_r13_r4"},
    {{pl, r13, r13, r5}, "It pl;", "pl r13 r13 r5", "pl_r13_r13_r5"},
    {{pl, r13, r13, r6}, "It pl;", "pl r13 r13 r6", "pl_r13_r13_r6"},
    {{pl, r13, r13, r7}, "It pl;", "pl r13 r13 r7", "pl_r13_r13_r7"},
    {{pl, r13, r13, r8}, "It pl;", "pl r13 r13 r8", "pl_r13_r13_r8"},
    {{pl, r13, r13, r9}, "It pl;", "pl r13 r13 r9", "pl_r13_r13_r9"},
    {{pl, r13, r13, r10}, "It pl;", "pl r13 r13 r10", "pl_r13_r13_r10"},
    {{pl, r13, r13, r11}, "It pl;", "pl r13 r13 r11", "pl_r13_r13_r11"},
    {{pl, r13, r13, r12}, "It pl;", "pl r13 r13 r12", "pl_r13_r13_r12"},
    {{pl, r13, r13, r13}, "It pl;", "pl r13 r13 r13", "pl_r13_r13_r13"},
    {{pl, r13, r13, r14}, "It pl;", "pl r13 r13 r14", "pl_r13_r13_r14"},
    {{vs, r13, r13, r0}, "It vs;", "vs r13 r13 r0", "vs_r13_r13_r0"},
    {{vs, r13, r13, r1}, "It vs;", "vs r13 r13 r1", "vs_r13_r13_r1"},
    {{vs, r13, r13, r2}, "It vs;", "vs r13 r13 r2", "vs_r13_r13_r2"},
    {{vs, r13, r13, r3}, "It vs;", "vs r13 r13 r3", "vs_r13_r13_r3"},
    {{vs, r13, r13, r4}, "It vs;", "vs r13 r13 r4", "vs_r13_r13_r4"},
    {{vs, r13, r13, r5}, "It vs;", "vs r13 r13 r5", "vs_r13_r13_r5"},
    {{vs, r13, r13, r6}, "It vs;", "vs r13 r13 r6", "vs_r13_r13_r6"},
    {{vs, r13, r13, r7}, "It vs;", "vs r13 r13 r7", "vs_r13_r13_r7"},
    {{vs, r13, r13, r8}, "It vs;", "vs r13 r13 r8", "vs_r13_r13_r8"},
    {{vs, r13, r13, r9}, "It vs;", "vs r13 r13 r9", "vs_r13_r13_r9"},
    {{vs, r13, r13, r10}, "It vs;", "vs r13 r13 r10", "vs_r13_r13_r10"},
    {{vs, r13, r13, r11}, "It vs;", "vs r13 r13 r11", "vs_r13_r13_r11"},
    {{vs, r13, r13, r12}, "It vs;", "vs r13 r13 r12", "vs_r13_r13_r12"},
    {{vs, r13, r13, r13}, "It vs;", "vs r13 r13 r13", "vs_r13_r13_r13"},
    {{vs, r13, r13, r14}, "It vs;", "vs r13 r13 r14", "vs_r13_r13_r14"},
    {{vc, r13, r13, r0}, "It vc;", "vc r13 r13 r0", "vc_r13_r13_r0"},
    {{vc, r13, r13, r1}, "It vc;", "vc r13 r13 r1", "vc_r13_r13_r1"},
    {{vc, r13, r13, r2}, "It vc;", "vc r13 r13 r2", "vc_r13_r13_r2"},
    {{vc, r13, r13, r3}, "It vc;", "vc r13 r13 r3", "vc_r13_r13_r3"},
    {{vc, r13, r13, r4}, "It vc;", "vc r13 r13 r4", "vc_r13_r13_r4"},
    {{vc, r13, r13, r5}, "It vc;", "vc r13 r13 r5", "vc_r13_r13_r5"},
    {{vc, r13, r13, r6}, "It vc;", "vc r13 r13 r6", "vc_r13_r13_r6"},
    {{vc, r13, r13, r7}, "It vc;", "vc r13 r13 r7", "vc_r13_r13_r7"},
    {{vc, r13, r13, r8}, "It vc;", "vc r13 r13 r8", "vc_r13_r13_r8"},
    {{vc, r13, r13, r9}, "It vc;", "vc r13 r13 r9", "vc_r13_r13_r9"},
    {{vc, r13, r13, r10}, "It vc;", "vc r13 r13 r10", "vc_r13_r13_r10"},
    {{vc, r13, r13, r11}, "It vc;", "vc r13 r13 r11", "vc_r13_r13_r11"},
    {{vc, r13, r13, r12}, "It vc;", "vc r13 r13 r12", "vc_r13_r13_r12"},
    {{vc, r13, r13, r13}, "It vc;", "vc r13 r13 r13", "vc_r13_r13_r13"},
    {{vc, r13, r13, r14}, "It vc;", "vc r13 r13 r14", "vc_r13_r13_r14"},
    {{hi, r13, r13, r0}, "It hi;", "hi r13 r13 r0", "hi_r13_r13_r0"},
    {{hi, r13, r13, r1}, "It hi;", "hi r13 r13 r1", "hi_r13_r13_r1"},
    {{hi, r13, r13, r2}, "It hi;", "hi r13 r13 r2", "hi_r13_r13_r2"},
    {{hi, r13, r13, r3}, "It hi;", "hi r13 r13 r3", "hi_r13_r13_r3"},
    {{hi, r13, r13, r4}, "It hi;", "hi r13 r13 r4", "hi_r13_r13_r4"},
    {{hi, r13, r13, r5}, "It hi;", "hi r13 r13 r5", "hi_r13_r13_r5"},
    {{hi, r13, r13, r6}, "It hi;", "hi r13 r13 r6", "hi_r13_r13_r6"},
    {{hi, r13, r13, r7}, "It hi;", "hi r13 r13 r7", "hi_r13_r13_r7"},
    {{hi, r13, r13, r8}, "It hi;", "hi r13 r13 r8", "hi_r13_r13_r8"},
    {{hi, r13, r13, r9}, "It hi;", "hi r13 r13 r9", "hi_r13_r13_r9"},
    {{hi, r13, r13, r10}, "It hi;", "hi r13 r13 r10", "hi_r13_r13_r10"},
    {{hi, r13, r13, r11}, "It hi;", "hi r13 r13 r11", "hi_r13_r13_r11"},
    {{hi, r13, r13, r12}, "It hi;", "hi r13 r13 r12", "hi_r13_r13_r12"},
    {{hi, r13, r13, r13}, "It hi;", "hi r13 r13 r13", "hi_r13_r13_r13"},
    {{hi, r13, r13, r14}, "It hi;", "hi r13 r13 r14", "hi_r13_r13_r14"},
    {{ls, r13, r13, r0}, "It ls;", "ls r13 r13 r0", "ls_r13_r13_r0"},
    {{ls, r13, r13, r1}, "It ls;", "ls r13 r13 r1", "ls_r13_r13_r1"},
    {{ls, r13, r13, r2}, "It ls;", "ls r13 r13 r2", "ls_r13_r13_r2"},
    {{ls, r13, r13, r3}, "It ls;", "ls r13 r13 r3", "ls_r13_r13_r3"},
    {{ls, r13, r13, r4}, "It ls;", "ls r13 r13 r4", "ls_r13_r13_r4"},
    {{ls, r13, r13, r5}, "It ls;", "ls r13 r13 r5", "ls_r13_r13_r5"},
    {{ls, r13, r13, r6}, "It ls;", "ls r13 r13 r6", "ls_r13_r13_r6"},
    {{ls, r13, r13, r7}, "It ls;", "ls r13 r13 r7", "ls_r13_r13_r7"},
    {{ls, r13, r13, r8}, "It ls;", "ls r13 r13 r8", "ls_r13_r13_r8"},
    {{ls, r13, r13, r9}, "It ls;", "ls r13 r13 r9", "ls_r13_r13_r9"},
    {{ls, r13, r13, r10}, "It ls;", "ls r13 r13 r10", "ls_r13_r13_r10"},
    {{ls, r13, r13, r11}, "It ls;", "ls r13 r13 r11", "ls_r13_r13_r11"},
    {{ls, r13, r13, r12}, "It ls;", "ls r13 r13 r12", "ls_r13_r13_r12"},
    {{ls, r13, r13, r13}, "It ls;", "ls r13 r13 r13", "ls_r13_r13_r13"},
    {{ls, r13, r13, r14}, "It ls;", "ls r13 r13 r14", "ls_r13_r13_r14"},
    {{ge, r13, r13, r0}, "It ge;", "ge r13 r13 r0", "ge_r13_r13_r0"},
    {{ge, r13, r13, r1}, "It ge;", "ge r13 r13 r1", "ge_r13_r13_r1"},
    {{ge, r13, r13, r2}, "It ge;", "ge r13 r13 r2", "ge_r13_r13_r2"},
    {{ge, r13, r13, r3}, "It ge;", "ge r13 r13 r3", "ge_r13_r13_r3"},
    {{ge, r13, r13, r4}, "It ge;", "ge r13 r13 r4", "ge_r13_r13_r4"},
    {{ge, r13, r13, r5}, "It ge;", "ge r13 r13 r5", "ge_r13_r13_r5"},
    {{ge, r13, r13, r6}, "It ge;", "ge r13 r13 r6", "ge_r13_r13_r6"},
    {{ge, r13, r13, r7}, "It ge;", "ge r13 r13 r7", "ge_r13_r13_r7"},
    {{ge, r13, r13, r8}, "It ge;", "ge r13 r13 r8", "ge_r13_r13_r8"},
    {{ge, r13, r13, r9}, "It ge;", "ge r13 r13 r9", "ge_r13_r13_r9"},
    {{ge, r13, r13, r10}, "It ge;", "ge r13 r13 r10", "ge_r13_r13_r10"},
    {{ge, r13, r13, r11}, "It ge;", "ge r13 r13 r11", "ge_r13_r13_r11"},
    {{ge, r13, r13, r12}, "It ge;", "ge r13 r13 r12", "ge_r13_r13_r12"},
    {{ge, r13, r13, r13}, "It ge;", "ge r13 r13 r13", "ge_r13_r13_r13"},
    {{ge, r13, r13, r14}, "It ge;", "ge r13 r13 r14", "ge_r13_r13_r14"},
    {{lt, r13, r13, r0}, "It lt;", "lt r13 r13 r0", "lt_r13_r13_r0"},
    {{lt, r13, r13, r1}, "It lt;", "lt r13 r13 r1", "lt_r13_r13_r1"},
    {{lt, r13, r13, r2}, "It lt;", "lt r13 r13 r2", "lt_r13_r13_r2"},
    {{lt, r13, r13, r3}, "It lt;", "lt r13 r13 r3", "lt_r13_r13_r3"},
    {{lt, r13, r13, r4}, "It lt;", "lt r13 r13 r4", "lt_r13_r13_r4"},
    {{lt, r13, r13, r5}, "It lt;", "lt r13 r13 r5", "lt_r13_r13_r5"},
    {{lt, r13, r13, r6}, "It lt;", "lt r13 r13 r6", "lt_r13_r13_r6"},
    {{lt, r13, r13, r7}, "It lt;", "lt r13 r13 r7", "lt_r13_r13_r7"},
    {{lt, r13, r13, r8}, "It lt;", "lt r13 r13 r8", "lt_r13_r13_r8"},
    {{lt, r13, r13, r9}, "It lt;", "lt r13 r13 r9", "lt_r13_r13_r9"},
    {{lt, r13, r13, r10}, "It lt;", "lt r13 r13 r10", "lt_r13_r13_r10"},
    {{lt, r13, r13, r11}, "It lt;", "lt r13 r13 r11", "lt_r13_r13_r11"},
    {{lt, r13, r13, r12}, "It lt;", "lt r13 r13 r12", "lt_r13_r13_r12"},
    {{lt, r13, r13, r13}, "It lt;", "lt r13 r13 r13", "lt_r13_r13_r13"},
    {{lt, r13, r13, r14}, "It lt;", "lt r13 r13 r14", "lt_r13_r13_r14"},
    {{gt, r13, r13, r0}, "It gt;", "gt r13 r13 r0", "gt_r13_r13_r0"},
    {{gt, r13, r13, r1}, "It gt;", "gt r13 r13 r1", "gt_r13_r13_r1"},
    {{gt, r13, r13, r2}, "It gt;", "gt r13 r13 r2", "gt_r13_r13_r2"},
    {{gt, r13, r13, r3}, "It gt;", "gt r13 r13 r3", "gt_r13_r13_r3"},
    {{gt, r13, r13, r4}, "It gt;", "gt r13 r13 r4", "gt_r13_r13_r4"},
    {{gt, r13, r13, r5}, "It gt;", "gt r13 r13 r5", "gt_r13_r13_r5"},
    {{gt, r13, r13, r6}, "It gt;", "gt r13 r13 r6", "gt_r13_r13_r6"},
    {{gt, r13, r13, r7}, "It gt;", "gt r13 r13 r7", "gt_r13_r13_r7"},
    {{gt, r13, r13, r8}, "It gt;", "gt r13 r13 r8", "gt_r13_r13_r8"},
    {{gt, r13, r13, r9}, "It gt;", "gt r13 r13 r9", "gt_r13_r13_r9"},
    {{gt, r13, r13, r10}, "It gt;", "gt r13 r13 r10", "gt_r13_r13_r10"},
    {{gt, r13, r13, r11}, "It gt;", "gt r13 r13 r11", "gt_r13_r13_r11"},
    {{gt, r13, r13, r12}, "It gt;", "gt r13 r13 r12", "gt_r13_r13_r12"},
    {{gt, r13, r13, r13}, "It gt;", "gt r13 r13 r13", "gt_r13_r13_r13"},
    {{gt, r13, r13, r14}, "It gt;", "gt r13 r13 r14", "gt_r13_r13_r14"},
    {{le, r13, r13, r0}, "It le;", "le r13 r13 r0", "le_r13_r13_r0"},
    {{le, r13, r13, r1}, "It le;", "le r13 r13 r1", "le_r13_r13_r1"},
    {{le, r13, r13, r2}, "It le;", "le r13 r13 r2", "le_r13_r13_r2"},
    {{le, r13, r13, r3}, "It le;", "le r13 r13 r3", "le_r13_r13_r3"},
    {{le, r13, r13, r4}, "It le;", "le r13 r13 r4", "le_r13_r13_r4"},
    {{le, r13, r13, r5}, "It le;", "le r13 r13 r5", "le_r13_r13_r5"},
    {{le, r13, r13, r6}, "It le;", "le r13 r13 r6", "le_r13_r13_r6"},
    {{le, r13, r13, r7}, "It le;", "le r13 r13 r7", "le_r13_r13_r7"},
    {{le, r13, r13, r8}, "It le;", "le r13 r13 r8", "le_r13_r13_r8"},
    {{le, r13, r13, r9}, "It le;", "le r13 r13 r9", "le_r13_r13_r9"},
    {{le, r13, r13, r10}, "It le;", "le r13 r13 r10", "le_r13_r13_r10"},
    {{le, r13, r13, r11}, "It le;", "le r13 r13 r11", "le_r13_r13_r11"},
    {{le, r13, r13, r12}, "It le;", "le r13 r13 r12", "le_r13_r13_r12"},
    {{le, r13, r13, r13}, "It le;", "le r13 r13 r13", "le_r13_r13_r13"},
    {{le, r13, r13, r14}, "It le;", "le r13 r13 r14", "le_r13_r13_r14"}};

// These headers each contain an array of `TestResult` with the reference output
// values. The reference arrays are names `kReference{mnemonic}`.
#include "a32/traces/assembler-cond-rd-rn-operand-rm-t32-rd-is-rn-is-sp-in-it-block-add.h"

// The maximum number of errors to report in detail for each test.
static const unsigned kErrorReportLimit = 8;

typedef void (MacroAssembler::*Fn)(Condition cond, Register rd, Register rn,
                                   const Operand& op);

static void TestHelper(Fn instruction, const char* mnemonic,
                       const TestResult reference[]) {
  unsigned total_error_count = 0;
  MacroAssembler masm(BUF_SIZE);

  masm.SetT32(true);

  for (unsigned i = 0; i < ARRAY_SIZE(kTests); i++) {
    // Values to pass to the macro-assembler.
    Condition cond = kTests[i].operands.cond;
    Register rd = kTests[i].operands.rd;
    Register rn = kTests[i].operands.rn;
    Register rm = kTests[i].operands.rm;
    Operand op(rm);

    uint32_t start = masm.GetCursorOffset();
    (masm.*instruction)(cond, rd, rn, op);
    uint32_t end = masm.GetCursorOffset();

    const byte* result_ptr =
        masm.GetBuffer().GetOffsetAddress<const byte*>(start);
    uint32_t result_size = end - start;

    if (Test::generate_test_trace()) {
      // Print the result bytes.
      printf("static const byte kInstruction_%s_%s[] = {\n", mnemonic,
             kTests[i].identifier);
      for (uint32_t j = 0; j < result_size; j++) {
        if (j == 0) {
          printf("  0x%02" PRIx8, result_ptr[j]);
        } else {
          printf(", 0x%02" PRIx8, result_ptr[j]);
        }
      }
      // This comment is meant to be used by external tools to validate
      // the encoding. We can parse the comment to figure out what
      // instruction this corresponds to.
      printf(" // %s %s %s\n};\n", kTests[i].expect_instruction_before,
             mnemonic, kTests[i].operands_description);
    } else {
      // Check we've emitted the exact same encoding as present in the
      // trace file. Only print up to `kErrorReportLimit` errors.
      if (((result_size != reference[i].size) ||
           (memcmp(result_ptr, reference[i].encoding, reference[i].size) !=
            0)) &&
          (++total_error_count <= kErrorReportLimit)) {
        printf("Error when testing \"%s\" with operands \"%s\":\n", mnemonic,
               kTests[i].operands_description);
        printf("  Expected: ");
        for (uint32_t j = 0; j < reference[i].size; j++) {
          if (j == 0) {
            printf("0x%02" PRIx8, reference[i].encoding[j]);
          } else {
            printf(", 0x%02" PRIx8, reference[i].encoding[j]);
          }
        }
        printf("\n");
        printf("  Found:    ");
        for (uint32_t j = 0; j < result_size; j++) {
          if (j == 0) {
            printf("0x%02" PRIx8, result_ptr[j]);
          } else {
            printf(", 0x%02" PRIx8, result_ptr[j]);
          }
        }
        printf("\n");
      }
    }
  }

  masm.FinalizeCode();

  if (Test::generate_test_trace()) {
    // Finalize the trace file by writing the final `TestResult` array
    // which links all generated instruction encodings.
    printf("static const TestResult kReference%s[] = {\n", mnemonic);
    for (unsigned i = 0; i < ARRAY_SIZE(kTests); i++) {
      printf("  {\n");
      printf("    ARRAY_SIZE(kInstruction_%s_%s),\n", mnemonic,
             kTests[i].identifier);
      printf("    kInstruction_%s_%s,\n", mnemonic, kTests[i].identifier);
      printf("  },\n");
    }
    printf("};\n");
  } else {
    if (total_error_count > kErrorReportLimit) {
      printf("%u other errors follow.\n",
             total_error_count - kErrorReportLimit);
    }
  }
}

// Instantiate tests for each instruction in the list.
#define TEST(mnemonic)                                                      \
  static void Test_##mnemonic() {                                           \
    TestHelper(&MacroAssembler::mnemonic, #mnemonic, kReference##mnemonic); \
  }                                                                         \
  static Test test_##mnemonic(                                              \
      "AARCH32_ASSEMBLER_COND_RD_RN_OPERAND_RM_T32_RD_IS_RN_IS_SP_IN_IT_"   \
      "BLOCK_" #mnemonic,                                                   \
      &Test_##mnemonic);
FOREACH_INSTRUCTION(TEST)
#undef TEST

}  // aarch32
}  // vixl
