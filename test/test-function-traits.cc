// Copyright 2016, Linaro Limited
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

#include <typeinfo>

#include "function-traits-vixl.h"
#include "test-runner.h"

#ifdef VIXL_FUNCTION_TRAITS_SUPPORT

namespace vixl {

// This file contains tests for the `function_traits` helper.

#define TEST(name)  TEST_(FUNCTION_TRAITS_##name)

#define CHECK_RETURN_TYPE(function, expected)                                  \
  static_assert(std::is_same<expected,                                         \
                function_traits<decltype(function)>::return_type>::value,      \
                "Unexpected return type.");

#define CHECK_ARITY(function, expected)                                        \
  static_assert(expected == function_traits<decltype(function)>::arity,        \
                "Unexpected arity.");

#define CHECK_PARAMETER_TYPE(function, index, expected)                        \
  static_assert(                                                               \
      std::is_same<expected,                                                   \
      function_traits<decltype(function)>::parameter<index>::type>::value,     \
      "Unexpected parameter type.");


void f0() {}
bool f1() { return 0; }
int8_t f2(int16_t) { return false; }
int32_t f3(int64_t, float, void*) { return 0; }
void* f4(int, int*, int**, int***) { return NULL; }

TEST(traits) {
  CHECK_RETURN_TYPE(f0, void);
  CHECK_ARITY(f0, 0);

  CHECK_RETURN_TYPE(f1, bool);
  CHECK_ARITY(f1, 0);

  CHECK_RETURN_TYPE(f2, int8_t);
  CHECK_ARITY(f2, 1);
  CHECK_PARAMETER_TYPE(f2, 0, int16_t);

  CHECK_RETURN_TYPE(f3, int32_t);
  CHECK_ARITY(f3, 3);
  CHECK_PARAMETER_TYPE(f3, 0, int64_t);
  CHECK_PARAMETER_TYPE(f3, 1, float);
  CHECK_PARAMETER_TYPE(f3, 2, void*);

  CHECK_RETURN_TYPE(f4, void*);
  CHECK_ARITY(f4, 4);
  CHECK_PARAMETER_TYPE(f4, 0, int);
  CHECK_PARAMETER_TYPE(f4, 1, int*);
  CHECK_PARAMETER_TYPE(f4, 2, int**);
  CHECK_PARAMETER_TYPE(f4, 3, int***);
}


TEST(parameter_types) {
  // Check that we can use `parameter_types` as a tuple type.
  function_traits<decltype(f3)>::parameter_types test_tuple {0x0123456789, 1.0, NULL};
}

}  // namespace vixl

#endif  // VIXL_FUNCTION_TRAITS_SUPPORT
