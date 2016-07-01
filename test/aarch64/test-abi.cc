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

#ifdef VIXL_ABI_SUPPORT

#include "test-runner.h"
#include "test-utils-aarch64.h"

#include "aarch64/abi-aarch64.h"

#define TEST(name)  TEST_(AARCH64_ABI_##name)

#define CHECK_NEXT_PARAMETER(type, location) \
    VIXL_CHECK(abi.GetNextParameterLocation<type>().Equals(Location(location)))

namespace vixl {
namespace aarch64 {


TEST(abi) {
  ABI abi;

  VIXL_CHECK(abi.GetStackSpaceRequired() == 0);
  VIXL_CHECK(!abi.GetReturnLocation<void>().IsValid());

  VIXL_CHECK(abi.GetReturnLocation<bool>().Equals(Location(w0)));
  VIXL_CHECK(abi.GetReturnLocation<char>().Equals(Location(w0)));
  VIXL_CHECK(abi.GetReturnLocation<int8_t>().Equals(Location(w0)));
  VIXL_CHECK(abi.GetReturnLocation<uint8_t>().Equals(Location(w0)));
  VIXL_CHECK(abi.GetReturnLocation<short>().Equals(Location(w0)));  // NOLINT(runtime/int)
  VIXL_CHECK(abi.GetReturnLocation<int16_t>().Equals(Location(w0)));
  VIXL_CHECK(abi.GetReturnLocation<uint16_t>().Equals(Location(w0)));
  VIXL_CHECK(abi.GetReturnLocation<int>().Equals(Location(w0)));
  VIXL_CHECK(abi.GetReturnLocation<int32_t>().Equals(Location(w0)));
  VIXL_CHECK(abi.GetReturnLocation<uint32_t>().Equals(Location(w0)));
  VIXL_CHECK(abi.GetReturnLocation<int64_t>().Equals(Location(x0)));
  VIXL_CHECK(abi.GetReturnLocation<uint64_t>().Equals(Location(x0)));

  VIXL_CHECK(abi.GetReturnLocation<float>().Equals(Location(s0)));
  VIXL_CHECK(abi.GetReturnLocation<double>().Equals(Location(d0)));

  abi.Reset();
  CHECK_NEXT_PARAMETER(int,      w0);
  CHECK_NEXT_PARAMETER(char,     w1);
  CHECK_NEXT_PARAMETER(bool,     w2);
  CHECK_NEXT_PARAMETER(float,    s0);
  CHECK_NEXT_PARAMETER(double,   d1);
  CHECK_NEXT_PARAMETER(double,   d2);
  CHECK_NEXT_PARAMETER(float,    s3);
  CHECK_NEXT_PARAMETER(int64_t,  x3);
  CHECK_NEXT_PARAMETER(uint64_t, x4);
  CHECK_NEXT_PARAMETER(uint64_t, x5);
  CHECK_NEXT_PARAMETER(uint32_t, x6);
  typedef short my_type;  // NOLINT(runtime/int)
  CHECK_NEXT_PARAMETER(my_type,  w7);
  CHECK_NEXT_PARAMETER(int,      MemOperand(sp, 0));
  CHECK_NEXT_PARAMETER(int,      MemOperand(sp, 8));
  CHECK_NEXT_PARAMETER(double,   d4);
  CHECK_NEXT_PARAMETER(double,   d5);
  CHECK_NEXT_PARAMETER(double,   d6);
  CHECK_NEXT_PARAMETER(double,   d7);
  CHECK_NEXT_PARAMETER(double,   MemOperand(sp, 16));
  CHECK_NEXT_PARAMETER(bool,     MemOperand(sp, 24));
  CHECK_NEXT_PARAMETER(short,    MemOperand(sp, 32));  // NOLINT(runtime/int)
  CHECK_NEXT_PARAMETER(float,    MemOperand(sp, 40));
  CHECK_NEXT_PARAMETER(float,    MemOperand(sp, 48));
  VIXL_CHECK(abi.GetStackSpaceRequired() == 56);
}


}}  // namespace vixl::aarch64

#endif  // __cplusplus >= 201103L
