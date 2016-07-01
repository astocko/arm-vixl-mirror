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

#include "examples.h"

#include "a64/disasm-a64.h"

#define __ masm->

void print_integers(int32_t a, int32_t b) {
  printf("a = %d, b = %d\n", a, b);
}

float add_integers_as_float(int32_t a, int32_t b) {
  return static_cast<float>(a) + static_cast<float>(b);
}

void GenerateRuntimeCalls(MacroAssembler* masm) {
  // __ Call<void, print_integers, int32_t, int32_t>(print_integers);
  // __ float<int32_t, add_integers_as_float, int32_t, int32_t>(add_integers_as_float);

  // The return value is in s0.
  __ Ret();
}


#ifndef TEST_EXAMPLES
#ifdef VIXL_INCLUDE_SIMULATOR

int main(void) {
  MacroAssembler masm;

  // Generate the code for the example function.
  Label call_runtime_add_floats;
  masm.Bind(&call_runtime_add_floats);
  GenerateRuntimeCalls(&masm);
  masm.FinalizeCode();

  Instruction* start = masm.GetLabelAddress<Instruction*>(&call_runtime_add_floats);

  // Disassemble the generated code.
  PrintDisassembler disassembler(stdout);
  disassembler.DisassembleBuffer(start, masm.GetSizeOfCodeGenerated());

  Decoder decoder;
  Simulator simulator(&decoder);

  int32_t input_a = 123;
  int32_t input_b = 321;
  simulator.WriteWRegister(0, input_a);
  simulator.WriteWRegister(0, input_b);
  simulator.RunFrom(start);
  printf("add_integers_as_float(%d, %d) = %f\n",
         input_a, input_b, simulator.ReadSRegister(0));

  return 0;
}
#else
// Without the simulator there is nothing to test.
int main(void) { return 0; }
#endif  // VIXL_INCLUDE_SIMULATOR
#endif  // TEST_EXAMPLES
