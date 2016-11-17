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

#include <sstream>
#include <string>
#include <list>

#include "test-runner.h"
#include "test-utils.h"

#include "aarch32/macro-assembler-aarch32.h"
#include "aarch32/disasm-aarch32.h"

#ifdef VIXL_NEGATIVE_TESTING
#include <stdexcept>
#endif

namespace vixl {
namespace aarch32 {

#define __ masm.
#define TEST(name)  TEST_(AARCH32_DISASM_##name)

#define BUF_SIZE (4096)

#define SETUP()                   \
  MacroAssembler masm(BUF_SIZE);

#define CLEANUP()

#define START_COMPARE()                                                        \
  {                                                                            \
    int32_t start = masm.GetCursorOffset();

#define END_COMPARE(EXP)                                                       \
    int32_t end = masm.GetCursorOffset();                                      \
    masm.FinalizeCode();                                                       \
    std::ostringstream ss;                                                     \
    TestDisassembler disassembler(ss, 0);                                      \
    if (masm.IsUsingT32()) {                                                   \
      disassembler.DisassembleT32(*masm.GetBuffer(), start, end);              \
    } else {                                                                   \
      disassembler.DisassembleA32(*masm.GetBuffer(), start, end);              \
    }                                                                          \
    masm.GetBuffer()->Reset();                                                 \
    if (Test::disassemble()) {                                                 \
      printf("%s", ss.str().c_str());                                          \
    }                                                                          \
    if (std::string(EXP) != ss.str()) {                                        \
      printf("\nFound:\n%sExpected:\n%s", ss.str().c_str(), EXP);              \
      abort();                                                                 \
    }                                                                          \
  }

#define COMPARE_A32(ASM, EXP)                                                  \
  masm.UseA32();                                                               \
  START_COMPARE()                                                              \
  masm.ASM;                                                                    \
  END_COMPARE(EXP)

#define COMPARE_T32(ASM, EXP)                                                  \
  masm.UseT32();                                                               \
  START_COMPARE()                                                              \
  masm.ASM;                                                                    \
  END_COMPARE(EXP)

#define COMPARE_BOTH(ASM, EXP)                                                 \
  COMPARE_A32(ASM, EXP)                                                        \
  COMPARE_T32(ASM, EXP)

#define NEGATIVE_TEST(ASM, EXP)                                                \
  {                                                                            \
    try {                                                                      \
      masm.ASM;                                                                \
      masm.FinalizeCode();                                                     \
      printf("\nNo exception raised. Expected:\n%s", EXP);                     \
      abort();                                                                 \
    } catch (std::runtime_error e) {                                           \
      const char *msg = e.what();                                              \
      if (std::strcmp(EXP, msg) != 0) {                                        \
        printf("\nFound:\n%sExpected:\n%s", msg, EXP);                         \
        abort();                                                               \
      }                                                                        \
    }                                                                          \
  }

#ifdef VIXL_NEGATIVE_TESTING
#define MUST_FAIL_TEST_A32(ASM, EXP)                                            \
  masm.UseA32();                                                               \
  NEGATIVE_TEST(ASM, EXP)                                                      \
  masm.GetBuffer()->Reset();

#define MUST_FAIL_TEST_T32(ASM, EXP)                                            \
  masm.UseT32();                                                               \
  NEGATIVE_TEST(ASM, EXP)                                                      \
  masm.GetBuffer()->Reset();

#define MUST_FAIL_TEST_BOTH(ASM, EXP)                                           \
  MUST_FAIL_TEST_A32(ASM, EXP)                                                  \
  MUST_FAIL_TEST_T32(ASM, EXP)
#else
// Skip negative tests.
#define MUST_FAIL_TEST_A32(ASM, EXP) \
  printf("Skipping negative tests. To enable them, build with 'negative_testing=on'.\n");
#define MUST_FAIL_TEST_T32(ASM, EXP) \
  printf("Skipping negative tests. To enable them, build with 'negative_testing=on'.\n");
#define MUST_FAIL_TEST_BOTH(ASM, EXP) \
  printf("Skipping negative tests. To enable them, build with 'negative_testing=on'.\n");
#endif

class TestDisassembler : public PrintDisassembler {
 public:
  TestDisassembler(std::ostream& os, uint32_t pc)  // NOLINT(runtime/references)
      : PrintDisassembler(os, pc) {
  }

  virtual void PrintCodeAddress(uint32_t code_address) VIXL_OVERRIDE {
    USE(code_address);
  }

  virtual void PrintOpcode16(uint32_t opcode) VIXL_OVERRIDE {
    USE(opcode);
  }

  virtual void PrintOpcode32(uint32_t opcode) VIXL_OVERRIDE {
    USE(opcode);
  }

  void DisassembleA32(const CodeBuffer& buffer, ptrdiff_t start,
                      ptrdiff_t end) {
    DisassembleA32Buffer(buffer.GetOffsetAddress<const uint32_t*>(start),
                         end - start);
  }

  void DisassembleT32(const CodeBuffer& buffer, ptrdiff_t start,
                      ptrdiff_t end) {
    DisassembleT32Buffer(buffer.GetOffsetAddress<const uint16_t*>(start),
                         end - start);
  }
};


TEST(t32_disassembler_limit1) {
  SETUP();

  masm.UseT32();
  START_COMPARE()
  masm.Add(r9, r10, r11);
  masm.GetBuffer()->Emit16(kLowestT32_32Opcode >> 16);
  END_COMPARE("add r9, r10, r11\n"
              "?\n");

  CLEANUP();
}


TEST(t32_disassembler_limit2) {
  SETUP();

  masm.UseT32();
  START_COMPARE()
  masm.Add(r9, r10, r11);
  masm.Add(r0, r0, r1);
  END_COMPARE("add r9, r10, r11\n"
              "add r0, r1\n");

  CLEANUP();
}


TEST(macro_assembler_orn) {
  SETUP();

  // - Identities.

  COMPARE_BOTH(Orn(r0, r1, 0),
               "mvn r0, #0\n");
  COMPARE_BOTH(Orn(r0, r0, 0xffffffff),
               "");

  // - Immediate form. This form does not need macro-assembler support
  //   for T32.

  // Use r0 as the temporary register.
  COMPARE_A32(Orn(r0, r1, 1),
              "mvn r0, #1\n"
              "orr r0, r1, r0\n");
  // Use ip as the temporary register.
  COMPARE_A32(Orn(r0, r0, 1),
              "mvn ip, #1\n"
              "orr r0, ip\n");

  //  - Too large immediate form.

  // TODO: optimize this.
  COMPARE_A32(Orn(r0, r1, 0x00ffffff),
              "mvn ip, #4278190080\n"
              "mvn r0, ip\n"
              "orr r0, r1, r0\n");
  COMPARE_T32(Orn(r0, r1, 0x00ffffff),
              "orr r0, r1, #0xff000000\n");

  COMPARE_A32(Orn(r0, r1, 0xabcd2345),
              "mov ip, #9029\n"
              "movt ip, #43981\n"
              "mvn r0, ip\n"
              "orr r0, r1, r0\n");
  COMPARE_T32(Orn(r0, r1, 0xabcd2345),
              "mov r0, #9029\n"
              "movt r0, #43981\n"
              "orn r0, r1, r0\n");

  // - Plain register form. This form does not need macro-assembler
  //   support for T32.

  // Use r0 as the temporary register.
  COMPARE_A32(Orn(r0, r1, r2),
              "mvn r0, r2\n"
              "orr r0, r1, r0\n");
  // Use r1 as the temporary register.
  COMPARE_A32(Orn(r0, r0, r1),
              "mvn r1, r1\n"
              "orr r0, r1\n");
  // Use r0 as the temporary register.
  COMPARE_A32(Orn(r0, r1, r0),
              "mvn r0, r0\n"
              "orr r0, r1, r0\n");
  // Use ip as the temporary register.
  COMPARE_A32(Orn(r0, r0, r0),
              "mvn ip, r0\n"
              "orr r0, ip\n");

  // - Shifted register form. This form does not need macro-assembler
  //   support for T32.

  // Use r0 as the temporary register.
  COMPARE_A32(Orn(r0, r1, Operand(r2, LSL, 1)),
              "mvn r0, r2, lsl #1\n"
              "orr r0, r1, r0\n");
  // Use r2 as the temporary register.
  COMPARE_A32(Orns(r0, r0, Operand(r2, LSR, 2)),
              "mvn r2, r2, lsr #2\n"
              "orrs r0, r2\n");

  // - Register shifted register form.

  // Use r0 as the temporary register.
  COMPARE_A32(Orn(r0, r1, Operand(r2, LSL, r3)),
              "mvn r0, r2, lsl r3\n"
              "orr r0, r1, r0\n");
  COMPARE_T32(Orn(r0, r1, Operand(r2, LSL, r3)),
              "lsl r0, r2, r3\n"
              "orn r0, r1, r0\n");
  // Use r2 as the temporary register.
  COMPARE_A32(Orns(r0, r0, Operand(r2, LSR, r3)),
              "mvn r2, r2, lsr r3\n"
              "orrs r0, r2\n");
  COMPARE_T32(Orns(r0, r0, Operand(r2, LSR, r3)),
              "lsr r2, r3\n"
              "orns r0, r2\n");
  // Use r3 as the temporary register.
  COMPARE_A32(Orn(r0, r0, Operand(r0, ASR, r3)),
              "mvn r3, r0, asr r3\n"
              "orr r0, r3\n");
  COMPARE_T32(Orn(r0, r0, Operand(r0, ASR, r3)),
              "asr r3, r0, r3\n"
              "orn r0, r3\n");
  CLEANUP();
}


TEST(macro_assembler_t32_rsc) {
  SETUP();

  // - Immediate form. We can always re-use `rn`.

  // No need for temporay registers.
  COMPARE_T32(Rsc(r0, r1, 1),
              "mvn r1, r1\n"
              "adc r0, r1, #1\n");
  // No need for temporay registers.
  COMPARE_T32(Rscs(r0, r0, 2),
              "mvn r0, r0\n"
              "adcs r0, #2\n");

  //  - Too large immediate form.

  // TODO: optimize this.
  COMPARE_A32(Rsc(r0, r1, 0x00ffffff),
              "mvn r0, #4278190080\n"
              "rsc r0, r1, r0\n");
  COMPARE_T32(Rscs(r0, r1, 0x00ffffff),
              "mvn r1, r1\n"
              "mvn r0, #4278190080\n"
              "adcs r0, r1, r0\n");

  COMPARE_A32(Rsc(r0, r1, 0xabcd2345),
              "mov r0, #9029\n"
              "movt r0, #43981\n"
              "rsc r0, r1, r0\n");
  COMPARE_T32(Rscs(r0, r1, 0xabcd2345),
              "mvn r1, r1\n"
              "mov r0, #56506\n"
              "movt r0, #21554\n"
              "sbcs r0, r1, r0\n");

  // - Plain register form.

  // No need for temporary registers.
  COMPARE_T32(Rscs(r0, r1, r2),
              "mvn r1, r1\n"
              "adcs r0, r1, r2\n");
  // Use r0 as the temporary register.
  COMPARE_T32(Rscs(r0, r1, r1),
              "mvn r0, r1\n"
              "adcs r0, r1\n");
  // Use ip as the temporary register.
  COMPARE_T32(Rscs(r0, r0, r0),
              "mvn ip, r0\n"
              "adcs r0, ip, r0\n");

  // - Shifted register form.

  // No need for temporay registers.
  COMPARE_T32(Rsc(r0, r1, Operand(r2, LSL, 1)),
              "mvn r1, r1\n"
              "adc r0, r1, r2, lsl #1\n");
  // No need for temporay registers.
  COMPARE_T32(Rscs(r0, r1, Operand(r0, LSR, 2)),
              "mvn r1, r1\n"
              "adcs r0, r1, r0, lsr #2\n");
  // Use r0 as the temporary register.
  COMPARE_T32(Rsc(r0, r1, Operand(r1, ASR, 3)),
              "mvn r0, r1\n"
              "adc r0, r1, asr #3\n");
  // Use ip as the temporary register.
  COMPARE_T32(Rscs(r0, r0, Operand(r0, ROR, 4)),
              "mvn ip, r0\n"
              "adcs r0, ip, r0, ror #4\n");

  // - Register shifted register form. The macro-assembler handles this form in
  //   two steps. First, a shift instruction is generated from the operand. And
  //   finally the operation is reduced to its plain register form.

  COMPARE_T32(Rsc(r0, r1, Operand(r2, LSL, r3)),
              "lsl r0, r2, r3\n"
              "mvn r1, r1\n"
              "adc r0, r1, r0\n");
  // Use r0 as the temporary register.
  COMPARE_T32(Rscs(r0, r1, Operand(r1, LSR, r3)),
              "lsr r0, r1, r3\n"
              "mvn r1, r1\n"
              "adcs r0, r1, r0\n");
  // Use r2 as the temporary register.
  COMPARE_T32(Rsc(r0, r0, Operand(r2, ASR, r3)),
              "asr r2, r3\n"
              "mvn r0, r0\n"
              "adc r0, r2\n");
  // Use r3 as the temporary register.
  COMPARE_T32(Rscs(r0, r0, Operand(r0, ROR, r3)),
              "ror r3, r0, r3\n"
              "mvn r0, r0\n"
              "adcs r0, r3\n");
  // Use ip as the temporary register.
  COMPARE_T32(Rsc(r0, r0, Operand(r0, LSL, r0)),
              "lsl ip, r0, r0\n"
              "mvn r0, r0\n"
              "adc r0, ip\n");

  CLEANUP();
}


TEST(macro_assembler_t32_register_shift_register) {
  SETUP();

  COMPARE_T32(Adc(r0, r1, Operand(r2, LSL, r3)),
              "lsl r0, r2, r3\n"
              "adc r0, r1, r0\n");
  COMPARE_T32(Adcs(r0, r0, Operand(r2, LSR, r3)),
              "lsr r2, r3\n"
              "adcs r0, r2\n");
  COMPARE_T32(Add(r0, r0, Operand(r0, ASR, r3)),
              "asr r3, r0, r3\n"
              "add r0, r3\n");
  COMPARE_T32(Adds(r0, r0, Operand(r0, ROR, r0)),
              "ror ip, r0, r0\n"
              "adds r0, ip\n");

  CLEANUP();
}


TEST(macro_assembler_big_offset) {
  SETUP();

  COMPARE_BOTH(Ldr(r0, MemOperand(r1, 0xfff123)),
               "mov r0, #61440\n"  // #0xf000
               "movt r0, #255\n"   // #0x00ff
               "add r0, r1, r0\n"
               "ldr r0, [r0, #291]\n");  // #0x123
  COMPARE_BOTH(Ldr(r0, MemOperand(r1, 0xff123)),
               "add r0, r1, #1044480\n"  // #0xff000
               "ldr r0, [r0, #291]\n");  // #0x123
  COMPARE_BOTH(Ldr(r0, MemOperand(r1, -0xff123)),
               "sub r0, r1, #1048576\n"  // #0x100000
               "ldr r0, [r0, #3805]\n");  // #0xedd

  COMPARE_A32(Ldr(r0, MemOperand(r1, 0xfff123, PreIndex)),
              "mov ip, #61440\n"  // #0xf000
              "movt ip, #255\n"   // #0x00ff
              "add r1, ip\n"
              "ldr r0, [r1, #291]!\n");  // #0x123
  COMPARE_A32(Ldr(r0, MemOperand(r1, 0xff123, PreIndex)),
              "add r1, #1044480\n"  // #0xff000
              "ldr r0, [r1, #291]!\n");  // #0x123
  COMPARE_A32(Ldr(r0, MemOperand(r1, -0xff123, PreIndex)),
              "sub r1, #1048576\n"  // #0x100000
              "ldr r0, [r1, #3805]!\n");  // #0xedd

  COMPARE_T32(Ldr(r0, MemOperand(r1, 0xfff12, PreIndex)),
              "mov ip, #65280\n"  // #0xff00
              "movt ip, #15\n"   // #0x000f
              "add r1, ip\n"
              "ldr r0, [r1, #18]!\n");  // #0x12
  COMPARE_T32(Ldr(r0, MemOperand(r1, 0xff12, PreIndex)),
              "add r1, #65280\n"  // #0xff00
              "ldr r0, [r1, #18]!\n");  // #0x12
  COMPARE_T32(Ldr(r0, MemOperand(r1, -0xff12, PreIndex)),
              "sub r1, #65536\n"  // #0x10000
              "ldr r0, [r1, #238]!\n");  // #0xee
  COMPARE_A32(Ldr(r0, MemOperand(r1, 0xfff123, PreIndex)),
              "mov ip, #61440\n"  // #0xf000
              "movt ip, #255\n"   // #0x00ff
              "add r1, ip\n"
              "ldr r0, [r1, #291]!\n");  // #0x123

  COMPARE_A32(Ldr(r0, MemOperand(r1, 0xfff123, PostIndex)),
              "ldr r0, [r1], #291\n"  // #0x123
              "mov ip, #61440\n"  // #0xf000
              "movt ip, #255\n"   // #0x00ff
              "add r1, ip\n");
  COMPARE_A32(Ldr(r0, MemOperand(r1, 0xff123, PostIndex)),
              "ldr r0, [r1], #291\n"  // #0x123
              "add r1, #1044480\n");  // #0xff000
  COMPARE_A32(Ldr(r0, MemOperand(r1, -0xff123, PostIndex)),
              "ldr r0, [r1], #3805\n"  // #0xedd
              "sub r1, #1048576\n");  // #0x100000

  COMPARE_T32(Ldr(r0, MemOperand(r1, 0xfff12, PostIndex)),
              "ldr r0, [r1], #18\n"  // #0x12
              "mov ip, #65280\n"  // #0xff00
              "movt ip, #15\n"   // #0x000f
              "add r1, ip\n");
  COMPARE_T32(Ldr(r0, MemOperand(r1, 0xff12, PostIndex)),
              "ldr r0, [r1], #18\n"  // #0x12
              "add r1, #65280\n");  // #0xff00
  COMPARE_T32(Ldr(r0, MemOperand(r1, -0xff12, PostIndex)),
              "ldr r0, [r1], #238\n"  // #0xee
              "sub r1, #65536\n");  // #0x10000
  CLEANUP();
}


TEST(macro_assembler_wide_immediate) {
  SETUP();

  COMPARE_BOTH(Adc(r0, r1, 0xbadbeef),
              "mov r0, #48879\n"
              "movt r0, #2989\n"
              "adc r0, r1, r0\n");

  COMPARE_BOTH(Add(r0, r0, 0xbadbeef),
              "mov ip, #48879\n"
              "movt ip, #2989\n"
              "add r0, ip\n");

  COMPARE_BOTH(Mov(r0, 0xbadbeef),
              "mov r0, #48879\n"
              "movt r0, #2989\n");
  COMPARE_A32(Mov(eq, r0, 0xbadbeef),
              "moveq r0, #48879\n"
              "movteq r0, #2989\n");
  COMPARE_T32(Mov(eq, r0, 0xbadbeef),
              "bne 0x0000000a\n"
              "mov r0, #48879\n"
              "movt r0, #2989\n");

  COMPARE_BOTH(Movs(r0, 0xbadbeef),
              "mov r0, #48879\n"
              "movt r0, #2989\n"
              "tst r0, r0\n");
  COMPARE_A32(Movs(eq, r0, 0xbadbeef),
              "moveq r0, #48879\n"
              "movteq r0, #2989\n"
              "tsteq r0, r0\n");
  COMPARE_T32(Movs(eq, r0, 0xbadbeef),
              "bne 0x0000000c\n"
              "mov r0, #48879\n"
              "movt r0, #2989\n"
              "tst r0, r0\n");

  COMPARE_BOTH(Mov(pc, 0xbadbeef),
              "mov ip, #48879\n"
              "movt ip, #2989\n"
              "bx ip\n");
  COMPARE_A32(Mov(eq, pc, 0xbadbeef),
              "mov ip, #48879\n"
              "movt ip, #2989\n"
              "bxeq ip\n");
  COMPARE_T32(Mov(eq, pc, 0xbadbeef),
              "bne 0x0000000c\n"
              "mov ip, #48879\n"
              "movt ip, #2989\n"
              "bx ip\n");

  CLEANUP();
}


TEST(macro_assembler_And) {
  SETUP();

  // Identities.
  COMPARE_BOTH(And(r0, r1, 0),
               "mov r0, #0\n");
  COMPARE_BOTH(And(r0, r0, 0xffffffff),
               "");
  CLEANUP();
}


TEST(macro_assembler_Bic) {
  SETUP();

  // Identities.
  COMPARE_BOTH(Bic(r0, r1, 0xffffffff),
               "mov r0, #0\n");
  COMPARE_BOTH(Bic(r0, r0, 0),
               "");
  CLEANUP();
}


TEST(macro_assembler_Orr) {
  SETUP();

  // Identities.
  COMPARE_BOTH(Orr(r0, r1, 0xffffffff),
               "mvn r0, #0\n");
  COMPARE_BOTH(Orr(r0, r0, 0),
               "");
  CLEANUP();
}


TEST(macro_assembler_InstructionCondSizeRROp) {
  SETUP();

  // Special case for Orr <-> Orn correspondance.

  COMPARE_T32(Orr(r0, r1, 0x00ffffff),
              "orn r0, r1, #0xff000000\n");
  COMPARE_T32(Orrs(r0, r1, 0x00ffffff),
              "orns r0, r1, #0xff000000\n");

  // Encodable immediates.

  COMPARE_A32(Add(r0, r1, -1),
              "sub r0, r1, #1\n");
  COMPARE_A32(Adds(r0, r1, -1),
              "subs r0, r1, #1\n");
  // 0xffffffff is encodable in a T32 ADD.
  COMPARE_T32(Add(r0, r1, -1),
              "add r0, r1, #4294967295\n");
  COMPARE_T32(Adds(r0, r1, -1),
              "adds r0, r1, #4294967295\n");

  COMPARE_BOTH(Add(r0, r1, -4),
              "sub r0, r1, #4\n");
  COMPARE_BOTH(Adds(r0, r1, -4),
              "subs r0, r1, #4\n");

  COMPARE_BOTH(Adc(r0, r1, -2),
              "sbc r0, r1, #1\n");
  COMPARE_BOTH(Adcs(r0, r1, -2),
              "sbcs r0, r1, #1\n");

  COMPARE_A32(Sub(r0, r1, -1),
              "add r0, r1, #1\n");
  COMPARE_A32(Subs(r0, r1, -1),
              "adds r0, r1, #1\n");
  // 0xffffffff is encodable in a T32 SUB.
  COMPARE_T32(Sub(r0, r1, -1),
              "sub r0, r1, #4294967295\n");
  COMPARE_T32(Subs(r0, r1, -1),
              "subs r0, r1, #4294967295\n");

  COMPARE_BOTH(Sub(r0, r1, -4),
              "add r0, r1, #4\n");
  COMPARE_BOTH(Subs(r0, r1, -4),
              "adds r0, r1, #4\n");

  COMPARE_BOTH(Sbc(r0, r1, -5),
              "adc r0, r1, #4\n");
  COMPARE_BOTH(Sbcs(r0, r1, -5),
              "adcs r0, r1, #4\n");

  // Non-encodable immediates

  COMPARE_BOTH(Adc(r0, r1, 0xabcd),
               "mov r0, #43981\n"
               "adc r0, r1, r0\n");

  COMPARE_BOTH(Adc(r0, r1, -0xabcd),
               "mov r0, #43980\n" // This represents #0xabcd - 1.
               "sbc r0, r1, r0\n");

  COMPARE_BOTH(Adc(r0, r1, 0x1234abcd),
               "mov r0, #43981\n"
               "movt r0, #4660\n"
               "adc r0, r1, r0\n");

  COMPARE_BOTH(Adc(r0, r1, -0x1234abcd),
               "mov r0, #43980\n" // This represents #0x1234abcd - 1.
               "movt r0, #4660\n"
               "sbc r0, r1, r0\n");

  // Non-encodable immediates with the same source and destination registers.

  COMPARE_BOTH(Sbc(r0, r0, 0xabcd),
               "mov ip, #43981\n"
               "sbc r0, ip\n");

  COMPARE_BOTH(Sbc(r0, r0, -0xabcd),
               "mov ip, #43980\n" // This represents #0xabcd - 1.
               "adc r0, ip\n");

  COMPARE_BOTH(Sbc(r0, r0, 0x1234abcd),
               "mov ip, #43981\n"
               "movt ip, #4660\n"
               "sbc r0, ip\n");

  COMPARE_BOTH(Sbc(r0, r0, -0x1234abcd),
               "mov ip, #43980\n" // This represents #0x1234abcd - 1.
               "movt ip, #4660\n"
               "adc r0, ip\n");


  // Test that we can pass a register shifted register operand in T32.

  COMPARE_T32(Adc(r0, r1, Operand(r2, LSL, r3)),
              "lsl r0, r2, r3\n"
              "adc r0, r1, r0\n");

  COMPARE_T32(Add(r3, r2, Operand(r2, ASR, r3)),
              "asr r3, r2, r3\n"
              "add r3, r2, r3\n");

  COMPARE_T32(Ands(r3, r2, Operand(r2, LSR, r2)),
              "lsr r3, r2, r2\n"
              "ands r3, r2, r3\n");

  COMPARE_T32(Asr(r2, r2, Operand(r2, ROR, r2)),
              "ror ip, r2, r2\n"
              "asr r2, ip\n");

  COMPARE_T32(Asr(r2, r2, Operand(r2, ROR, r2)),
              "ror ip, r2, r2\n"
              "asr r2, ip\n");


  CLEANUP();
}


TEST(macro_assembler_InstructionCondRO) {
  SETUP();

  COMPARE_BOTH(Teq(r0, 0xbadbeef),
               "mov ip, #48879\n"
               "movt ip, #2989\n"
               "teq r0, ip\n");

  CLEANUP();
}


TEST(macro_assembler_too_large_immediate) {
  SETUP();

  // Attempting to use a 17-bit immediate with movt.
  MUST_FAIL_TEST_BOTH(Movt(r0, 0x10000), "`Movt` expects a 16-bit immediate.");

  CLEANUP();
}

TEST(macro_assembler_Cbz) {
  SETUP();

  // Cbz/Cbnz are not available in A32 mode.
  Label label_64(__ GetCursorOffset() + __ GetArchitectureStatePCOffset(), 64);
  MUST_FAIL_TEST_A32(Cbz(r0, &label_64), "Cbz is only available for T32.\n");
  MUST_FAIL_TEST_A32(Cbnz(r0, &label_64), "Cbnz is only available for T32.\n");

  // Make sure GetArchitectureStatePCOffset() returns the correct value.
  __ UseT32();
  // Largest encodable offset.
  Label label_126(__ GetCursorOffset() + __ GetArchitectureStatePCOffset(), 126);
  COMPARE_T32(Cbz(r0, &label_126),
              "cbz r0, 0x00000082\n");
  COMPARE_T32(Cbnz(r0, &label_126),
              "cbnz r0, 0x00000082\n");

  // Offset cannot be encoded.
  Label label_128(__ GetCursorOffset() + __ GetArchitectureStatePCOffset(), 128);
  COMPARE_T32(Cbz(r0, &label_128),
              "cbnz r0, 0x00000004\n"
              "b 0x00000084\n");
  COMPARE_T32(Cbnz(r0, &label_128),
              "cbz r0, 0x00000004\n"
              "b 0x00000084\n");

  // Offset that cannot be encoded and needs 32-bit branch instruction.
  Label label_8192(__ GetCursorOffset() + __ GetArchitectureStatePCOffset(), 8192);
  COMPARE_T32(Cbz(r0, &label_8192),
              "cbnz r0, 0x00000006\n"
              "b 0x00002004\n");
  COMPARE_T32(Cbnz(r0, &label_8192),
              "cbz r0, 0x00000006\n"
              "b 0x00002004\n");

  // Negative offset.
  Label label_neg(__ GetCursorOffset() + __ GetArchitectureStatePCOffset(), -8);
  COMPARE_T32(Cbz(r0, &label_neg),
              "cbnz r0, 0x00000004\n"
              "b 0xfffffffc\n");
  COMPARE_T32(Cbnz(r0, &label_neg),
              "cbz r0, 0x00000004\n"
              "b 0xfffffffc\n");

  // Large negative offset.
  Label label_neg128(__ GetCursorOffset() + __ GetArchitectureStatePCOffset(), -128);
  COMPARE_T32(Cbz(r0, &label_neg128),
              "cbnz r0, 0x00000004\n"
              "b 0xffffff84\n");
  COMPARE_T32(Cbnz(r0, &label_neg128),
              "cbz r0, 0x00000004\n"
              "b 0xffffff84\n");

  CLEANUP();
}


#define TEST_VMEMOP(MACRO_OP, STRING_OP, DST_REG)                       \
  SETUP();                                                              \
                                                                        \
  COMPARE_T32(MACRO_OP(DST_REG, MemOperand(r8, 1024)),                  \
              "add ip, r8, #1024\n"                                     \
              STRING_OP # DST_REG ", [ip]\n");                          \
  COMPARE_T32(MACRO_OP(DST_REG, MemOperand(r8, 1371)),                  \
              "add ip, r8, #1371\n"                                     \
              STRING_OP # DST_REG ", [ip]\n");                          \
  COMPARE_T32(MACRO_OP(DST_REG, MemOperand(r8, 4113)),                  \
              "mov ip, #4113\n"                                         \
              "add ip, r8, ip\n"                                        \
              STRING_OP # DST_REG ", [ip]\n");                          \
  COMPARE_T32(MACRO_OP(DST_REG, MemOperand(r8, 65808)),                 \
              "mov ip, #272\n"                                          \
              "movt ip, #1\n"                                           \
              "add ip, r8, ip\n"                                        \
              STRING_OP # DST_REG ", [ip]\n");                          \
                                                                        \
  COMPARE_T32(MACRO_OP(DST_REG, MemOperand(r8, -1024)),                 \
              "sub ip, r8, #1024\n"                                     \
              STRING_OP # DST_REG ", [ip]\n");                          \
  COMPARE_T32(MACRO_OP(DST_REG, MemOperand(r8, -1371)),                 \
              "sub ip, r8, #1371\n"                                     \
              STRING_OP # DST_REG ", [ip]\n");                          \
  COMPARE_T32(MACRO_OP(DST_REG, MemOperand(r8, -4113)),                 \
              "mov ip, #4113\n"                                         \
              "sub ip, r8, ip\n"                                        \
              STRING_OP # DST_REG ", [ip]\n");                          \
  COMPARE_T32(MACRO_OP(DST_REG, MemOperand(r8, -65808)),                \
              "mov ip, #272\n"                                          \
              "movt ip, #1\n"                                           \
              "sub ip, r8, ip\n"                                        \
              STRING_OP # DST_REG ", [ip]\n");                          \
                                                                        \
  COMPARE_T32(MACRO_OP(DST_REG, MemOperand(r9, 0, PreIndex)),           \
              STRING_OP # DST_REG ", [r9]\n");                          \
  COMPARE_T32(MACRO_OP(DST_REG, MemOperand(r9, 137, PreIndex)),         \
              "add r9, #137\n"                                          \
              STRING_OP # DST_REG ", [r9]\n");                          \
  COMPARE_T32(MACRO_OP(DST_REG, MemOperand(r9, 4110, PreIndex)),        \
              "mov ip, #4110\n"                                         \
              "add r9, ip\n"                                            \
              STRING_OP # DST_REG ", [r9]\n");                          \
  COMPARE_T32(MACRO_OP(DST_REG, MemOperand(r9, 65623, PreIndex)),       \
              "mov ip, #87\n"                                           \
              "movt ip, #1\n"                                           \
              "add r9, ip\n"                                            \
              STRING_OP # DST_REG ", [r9]\n");                          \
                                                                        \
  COMPARE_T32(MACRO_OP(DST_REG, MemOperand(r9, -137, PreIndex)),        \
              "sub r9, #137\n"                                          \
              STRING_OP # DST_REG ", [r9]\n");                          \
  COMPARE_T32(MACRO_OP(DST_REG, MemOperand(r9, -4110, PreIndex)),       \
              "mov ip, #4110\n"                                         \
              "sub r9, ip\n"                                            \
              STRING_OP # DST_REG ", [r9]\n");                          \
  COMPARE_T32(MACRO_OP(DST_REG, MemOperand(r9, -65623, PreIndex)),      \
              "mov ip, #87\n"                                           \
              "movt ip, #1\n"                                           \
              "sub r9, ip\n"                                            \
              STRING_OP # DST_REG ", [r9]\n");                          \
                                                                        \
  COMPARE_T32(MACRO_OP(DST_REG, MemOperand(r10, 0, PostIndex)),         \
              STRING_OP # DST_REG ", [r10]\n");                         \
  COMPARE_T32(MACRO_OP(DST_REG, MemOperand(r10, 137, PostIndex)),       \
              STRING_OP # DST_REG ", [r10]\n"                           \
              "add r10, #137\n");                                       \
  COMPARE_T32(MACRO_OP(DST_REG, MemOperand(r10, 4110, PostIndex)),      \
              STRING_OP # DST_REG ", [r10]\n"                           \
              "mov ip, #4110\n"                                         \
              "add r10, ip\n");                                         \
  COMPARE_T32(MACRO_OP(DST_REG, MemOperand(r10, 65623, PostIndex)),     \
              STRING_OP # DST_REG ", [r10]\n"                           \
              "mov ip, #87\n"                                           \
              "movt ip, #1\n"                                           \
              "add r10, ip\n");                                         \
                                                                        \
  COMPARE_T32(MACRO_OP(DST_REG, MemOperand(r10, -137, PostIndex)),      \
              STRING_OP # DST_REG ", [r10]\n"                           \
              "sub r10, #137\n");                                       \
  COMPARE_T32(MACRO_OP(DST_REG, MemOperand(r10, -4110, PostIndex)),     \
              STRING_OP # DST_REG ", [r10]\n"                           \
              "mov ip, #4110\n"                                         \
              "sub r10, ip\n");                                         \
  COMPARE_T32(MACRO_OP(DST_REG, MemOperand(r10, -65623, PostIndex)),    \
              STRING_OP # DST_REG ", [r10]\n"                           \
              "mov ip, #87\n"                                           \
              "movt ip, #1\n"                                           \
              "sub r10, ip\n");                                         \
  CLEANUP();

TEST(macro_assembler_T32_Vldr_d) {
  TEST_VMEMOP(Vldr, "vldr ", d0);
}

TEST(macro_assembler_T32_Vstr_d) {
  TEST_VMEMOP(Vstr, "vstr ", d1);
}

TEST(macro_assembler_T32_Vldr_s) {
  TEST_VMEMOP(Vldr, "vldr ", s2);
}

TEST(macro_assembler_T32_Vstr_s) {
  TEST_VMEMOP(Vstr, "vstr ", s3);
}

#undef TEST_VMEMOP

#define TEST_VMEMOP(MACRO_OP, STRING_OP, DST_REG)                       \
  SETUP();                                                              \
                                                                        \
  COMPARE_A32(MACRO_OP(DST_REG, MemOperand(r8, 137)),                   \
              "add ip, r8, #137\n"                                      \
              STRING_OP # DST_REG ", [ip]\n");                          \
  COMPARE_A32(MACRO_OP(DST_REG, MemOperand(r8, 274)),                   \
              "mov ip, #274\n"                                          \
              "add ip, r8, ip\n"                                        \
              STRING_OP # DST_REG ", [ip]\n");                          \
  COMPARE_A32(MACRO_OP(DST_REG, MemOperand(r8, 65623)),                 \
              "mov ip, #87\n"                                           \
              "movt ip, #1\n"                                           \
              "add ip, r8, ip\n"                                        \
              STRING_OP # DST_REG ", [ip]\n");                          \
                                                                        \
  COMPARE_A32(MACRO_OP(DST_REG, MemOperand(r8, -137)),                  \
              "sub ip, r8, #137\n"                                      \
              STRING_OP # DST_REG ", [ip]\n");                          \
  COMPARE_A32(MACRO_OP(DST_REG, MemOperand(r8, -274)),                  \
              "mov ip, #274\n"                                          \
              "sub ip, r8, ip\n"                                        \
              STRING_OP # DST_REG ", [ip]\n");                          \
  COMPARE_A32(MACRO_OP(DST_REG, MemOperand(r8, -65623)),                \
              "mov ip, #87\n"                                           \
              "movt ip, #1\n"                                           \
              "sub ip, r8, ip\n"                                        \
              STRING_OP # DST_REG ", [ip]\n");                          \
                                                                        \
  COMPARE_A32(MACRO_OP(DST_REG, MemOperand(r9, 0, PreIndex)),           \
              STRING_OP # DST_REG ", [r9]\n");                          \
  COMPARE_A32(MACRO_OP(DST_REG, MemOperand(r9, 137, PreIndex)),         \
              "add r9, #137\n"                                          \
              STRING_OP # DST_REG ", [r9]\n");                          \
  COMPARE_A32(MACRO_OP(DST_REG, MemOperand(r9, 274, PreIndex)),         \
              "mov ip, #274\n"                                          \
              "add r9, ip\n"                                            \
              STRING_OP # DST_REG ", [r9]\n");                          \
  COMPARE_A32(MACRO_OP(DST_REG, MemOperand(r9, 65623, PreIndex)),       \
              "mov ip, #87\n"                                           \
              "movt ip, #1\n"                                           \
              "add r9, ip\n"                                            \
              STRING_OP # DST_REG ", [r9]\n");                          \
                                                                        \
  COMPARE_A32(MACRO_OP(DST_REG, MemOperand(r9, -137, PreIndex)),        \
              "sub r9, #137\n"                                          \
              STRING_OP # DST_REG ", [r9]\n");                          \
  COMPARE_A32(MACRO_OP(DST_REG, MemOperand(r9, -274, PreIndex)),        \
              "mov ip, #274\n"                                          \
              "sub r9, ip\n"                                            \
              STRING_OP # DST_REG ", [r9]\n");                          \
  COMPARE_A32(MACRO_OP(DST_REG, MemOperand(r9, -65623, PreIndex)),      \
              "mov ip, #87\n"                                           \
              "movt ip, #1\n"                                           \
              "sub r9, ip\n"                                            \
              STRING_OP # DST_REG ", [r9]\n");                          \
                                                                        \
  COMPARE_A32(MACRO_OP(DST_REG, MemOperand(r10, 0, PostIndex)),         \
              STRING_OP # DST_REG ", [r10]\n");                         \
  COMPARE_A32(MACRO_OP(DST_REG, MemOperand(r10, 137, PostIndex)),       \
              STRING_OP # DST_REG ", [r10]\n"                           \
              "add r10, #137\n");                                       \
  COMPARE_A32(MACRO_OP(DST_REG, MemOperand(r10, 274, PostIndex)),       \
              STRING_OP # DST_REG ", [r10]\n"                           \
              "mov ip, #274\n"                                          \
              "add r10, ip\n");                                         \
  COMPARE_A32(MACRO_OP(DST_REG, MemOperand(r10, 65623, PostIndex)),     \
              STRING_OP # DST_REG ", [r10]\n"                           \
              "mov ip, #87\n"                                           \
              "movt ip, #1\n"                                           \
              "add r10, ip\n");                                         \
                                                                        \
  COMPARE_A32(MACRO_OP(DST_REG, MemOperand(r10, -137, PostIndex)),      \
              STRING_OP # DST_REG ", [r10]\n"                           \
              "sub r10, #137\n");                                       \
  COMPARE_A32(MACRO_OP(DST_REG, MemOperand(r10, -274, PostIndex)),      \
              STRING_OP # DST_REG ", [r10]\n"                           \
              "mov ip, #274\n"                                          \
              "sub r10, ip\n");                                         \
  COMPARE_A32(MACRO_OP(DST_REG, MemOperand(r10, -65623, PostIndex)),    \
              STRING_OP # DST_REG ", [r10]\n"                           \
              "mov ip, #87\n"                                           \
              "movt ip, #1\n"                                           \
              "sub r10, ip\n");                                         \
  CLEANUP();


TEST(macro_assembler_A32_Vldr_d) {
  TEST_VMEMOP(Vldr, "vldr ", d0);
}

TEST(macro_assembler_A32_Vstr_d) {
  TEST_VMEMOP(Vstr, "vstr ", d1);
}

TEST(macro_assembler_A32_Vldr_s) {
  TEST_VMEMOP(Vldr, "vldr ", s2);
}

TEST(macro_assembler_A32_Vstr_s) {
  TEST_VMEMOP(Vstr, "vstr ", s3);
}

#undef TEST_VMEMOP


#define TEST_SHIFT_T32(Inst, name, offset)       \
  COMPARE_T32(Inst(r0, Operand(r1, LSL, r2)),    \
              "lsl ip, r1, r2\n"                 \
              name " r0, ip\n");                 \
  COMPARE_T32(Inst(r0, Operand(r1, LSR, r2)),    \
              "lsr ip, r1, r2\n"                 \
              name " r0, ip\n");                 \
  COMPARE_T32(Inst(r0, Operand(r1, ASR, r2)),    \
              "asr ip, r1, r2\n"                 \
              name " r0, ip\n");                 \
  COMPARE_T32(Inst(r0, Operand(r1, ROR, r2)),    \
              "ror ip, r1, r2\n"                 \
              name " r0, ip\n");                 \
  COMPARE_T32(Inst(eq, r0, Operand(r1, LSL, r2)),\
              "bne "#offset"\n"                  \
              "lsl ip, r1, r2\n"                 \
              name " r0, ip\n");                 \
  COMPARE_T32(Inst(le, r0, Operand(r1, LSL, r2)),\
              "bgt "#offset"\n"                  \
              "lsl ip, r1, r2\n"                 \
              name " r0, ip\n");

#define TEST_MOV_SHIFT_T32(Inst, s, offset)      \
  COMPARE_T32(Inst(r0, Operand(r1, LSL, r2)),    \
              "lsl" s " r0, r1, r2\n");          \
  COMPARE_T32(Inst(r0, Operand(r1, LSR, r2)),    \
              "lsr" s " r0, r1, r2\n");          \
  COMPARE_T32(Inst(r0, Operand(r1, ASR, r2)),    \
              "asr" s " r0, r1, r2\n");          \
  COMPARE_T32(Inst(r0, Operand(r1, ROR, r2)),    \
              "ror" s " r0, r1, r2\n");          \
  COMPARE_T32(Inst(eq, r0, Operand(r1, LSL, r2)),\
              "bne "#offset"\n"                  \
              "lsl" s " r0, r1, r2\n");          \
  COMPARE_T32(Inst(le, r0, Operand(r1, LSL, r2)),\
              "bgt "#offset"\n"                  \
              "lsl" s " r0, r1, r2\n");

#define TEST_WIDE_IMMEDIATE(Inst, name, offset)  \
  COMPARE_BOTH(Inst(r0, 0xbadbeef),              \
              "mov ip, #48879\n"                 \
              "movt ip, #2989\n"                 \
              name " r0, ip\n");                 \
  COMPARE_A32(Inst(eq, r0, 0xbadbeef),           \
              "moveq ip, #48879\n"               \
              "movteq ip, #2989\n"               \
              name "eq r0, ip\n");               \
  COMPARE_T32(Inst(eq, r0, 0xbadbeef),           \
              "bne "#offset"\n"                  \
              "mov ip, #48879\n"                 \
              "movt ip, #2989\n"                 \
              name " r0, ip\n");

#define TEST_WIDE_IMMEDIATE_PC(Inst, name, offset) \
  COMPARE_A32(Inst(pc, 0xbadbeef),                 \
              "mov ip, #48879\n"                   \
              "movt ip, #2989\n"                   \
              name " pc, ip\n");                   \
  COMPARE_A32(Inst(eq, pc, 0xbadbeef),             \
              "moveq ip, #48879\n"                 \
              "movteq ip, #2989\n"                 \
              name "eq pc, ip\n");                 \
  MUST_FAIL_TEST_T32(Inst(pc, 0xbadbeef),          \
                     "Unimplemented delegate\n");  \
  MUST_FAIL_TEST_T32(Inst(eq, pc, 0xbadbeef),      \
                     "Unimplemented delegate\n");

TEST(macro_assembler_InstructionCondSizeROp) {
  SETUP();

  // T32 register shifted register.
  TEST_SHIFT_T32(Cmn,"cmn", 0x0000000a)
  TEST_SHIFT_T32(Cmp,"cmp", 0x00000008)
  TEST_SHIFT_T32(Mvn,"mvn", 0x0000000a)
  TEST_SHIFT_T32(Mvns,"mvns", 0x0000000a)
  TEST_SHIFT_T32(Sxtb,"sxtb", 0x0000000a)
  TEST_SHIFT_T32(Sxth,"sxth", 0x0000000a)
  TEST_SHIFT_T32(Tst,"tst", 0x0000000a)
  TEST_SHIFT_T32(Uxtb,"uxtb", 0x0000000a)
  TEST_SHIFT_T32(Uxth,"uxth", 0x0000000a)

  TEST_MOV_SHIFT_T32(Mov, "", 0x00000006)
  TEST_MOV_SHIFT_T32(Movs, "s", 0x00000006)

  // Wide immediates (Mov and Movs are tested in "macro_assembler_wide_immediate").
  TEST_WIDE_IMMEDIATE(Cmp, "cmp", 0x0000000c);
  TEST_WIDE_IMMEDIATE(Cmn, "cmn", 0x0000000e);
  TEST_WIDE_IMMEDIATE(Tst, "tst", 0x0000000e);
  TEST_WIDE_IMMEDIATE_PC(Cmp, "cmp", 0x0000000c);
  TEST_WIDE_IMMEDIATE_PC(Cmn, "cmn", 0x0000000e);
  TEST_WIDE_IMMEDIATE_PC(Tst, "tst", 0x0000000e);

  // For Mvn and Mvns, we don't allow PC as a destination.
  TEST_WIDE_IMMEDIATE(Mvn, "mvn", 0x0000000e);
  TEST_WIDE_IMMEDIATE(Mvns, "mvns", 0x0000000e);
  MUST_FAIL_TEST_BOTH(Mvn(pc, 0xbadbeef), "Unimplemented delegate\n");
  MUST_FAIL_TEST_BOTH(Mvn(eq, pc, 0xbadbeef), "Unimplemented delegate\n");
  MUST_FAIL_TEST_BOTH(Mvns(pc, 0xbadbeef), "Unimplemented delegate\n");
  MUST_FAIL_TEST_BOTH(Mvns(eq, pc, 0xbadbeef), "Unimplemented delegate\n");

  MUST_FAIL_TEST_BOTH(Sxtb(r0, 0x1), "Unimplemented delegate\n");
  MUST_FAIL_TEST_BOTH(Sxth(r0, 0x1), "Unimplemented delegate\n");
  MUST_FAIL_TEST_BOTH(Uxtb(r0, 0x1), "Unimplemented delegate\n");
  MUST_FAIL_TEST_BOTH(Uxth(r0, 0x1), "Unimplemented delegate\n");

  CLEANUP();
}

#undef TEST_SHIFT_T32
#undef TEST_MOV_SHIFT_T32
#undef TEST_WIDE_IMMEDIATE
#undef TEST_WIDE_IMMEDIATE_PC

TEST(macro_assembler_Msr) {
  SETUP();

  // Msr with immediate for T32.
  COMPARE_T32(Msr(APSR_nzcvq, 0x0),
              "mov ip, #0\n"
              "msr APSR_nzcvq, ip\n");

  // Wide immediate.
  COMPARE_BOTH(Msr(APSR_nzcvq, 0xbadbeef),
              "mov ip, #48879\n"
              "movt ip, #2989\n"
              "msr APSR_nzcvq, ip\n");

  // Other types of operands are not handled.
  MUST_FAIL_TEST_BOTH(Msr(APSR_nzcvq, Operand(r0, LSR, r1)),
                      "Unimplemented delegate\n");
  CLEANUP();
}


TEST(macro_assembler_Vmov_imm) {
  SETUP();

  COMPARE_BOTH(Vmov(s0, 0.0f),
               "mov ip, #0\n"
               "vmov s0, ip\n");
  COMPARE_BOTH(Vmov(s1, 1.0f),
               "vmov.f32 s1, #1\n");
  COMPARE_BOTH(Vmov(s2, RawbitsToFloat(0x0000db6c)),
               "mov ip, #56172\n"
               "vmov s2, ip\n");
  COMPARE_BOTH(Vmov(s3, RawbitsToFloat(0x327b23c6)),
               "mov ip, #9158\n"
               "movt ip, #12923\n"
               "vmov s3, ip\n");
  COMPARE_BOTH(Vmov(s4, RawbitsToFloat(0xffcc7fff)),
               "mvn ip, #3375104\n"
               "vmov s4, ip\n");
  COMPARE_BOTH(Vmov(s5, RawbitsToFloat(0xb72df575)),
               "mov ip, #62837\n"
               "movt ip, #46893\n"
               "vmov s5, ip\n");

  COMPARE_BOTH(Vmov(d6, 0.0),
               "vmov.i64 d6, #0x0000000000000000\n");
  COMPARE_BOTH(Vmov(d7, 1.0),
               "vmov.f64 d7, #1\n");
  COMPARE_BOTH(Vmov(d8, RawbitsToDouble(0x000000000000af8e)),
               "mov ip, #44942\n"
               "vdup.32 d8, ip\n"
               "mov ip, #0\n"
               "vmov.32 d8[1], ip\n");
  COMPARE_BOTH(Vmov(d9, RawbitsToDouble(0x000070210000af8e)),
               "mov ip, #44942\n"
               "vdup.32 d9, ip\n"
               "mov ip, #28705\n"
               "vmov.32 d9[1], ip\n");
  COMPARE_BOTH(Vmov(d10, RawbitsToDouble(0x7021000000000000)),
               "mov ip, #0\n"
               "vdup.32 d10, ip\n"
               "mov ip, #0\n"
               "movt ip, #28705\n"
               "vmov.32 d10[1], ip\n");
  COMPARE_BOTH(Vmov(d11, RawbitsToDouble(0x7021da4b0000af8e)),
               "mov ip, #44942\n"
               "vdup.32 d11, ip\n"
               "mov ip, #55883\n"
               "movt ip, #28705\n"
               "vmov.32 d11[1], ip\n");
  COMPARE_BOTH(Vmov(d12, RawbitsToDouble(0x0cff553204ec4a3f)),
               "mov ip, #19007\n"
               "movt ip, #1260\n"
               "vdup.32 d12, ip\n"
               "mov ip, #21810\n"
               "movt ip, #3327\n"
               "vmov.32 d12[1], ip\n");
  COMPARE_BOTH(Vmov(d13, RawbitsToDouble(0xa2037ad20000f592)),
               "mov ip, #62866\n"
               "vdup.32 d13, ip\n"
               "mov ip, #31442\n"
               "movt ip, #41475\n"
               "vmov.32 d13[1], ip\n");
  COMPARE_BOTH(Vmov(d14, RawbitsToDouble(0xe62556c325a59470)),
               "mov ip, #38000\n"
               "movt ip, #9637\n"
               "vdup.32 d14, ip\n"
               "mov ip, #22211\n"
               "movt ip, #58917\n"
               "vmov.32 d14[1], ip\n");
  CLEANUP();
}

}  // namespace aarch32
}  // namespace vixl
