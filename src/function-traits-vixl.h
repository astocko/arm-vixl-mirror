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

#if __cplusplus >= 201103L
#define VIXL_FUNCTION_TRAITS_SUPPORT
#endif

#ifdef VIXL_FUNCTION_TRAITS_SUPPORT
#ifndef VIXL_FUNCTION_TRAITS_H_
#define VIXL_FUNCTION_TRAITS_H_

#include <tuple>
#include <type_traits>

namespace vixl {

// TODO: Comments

template <typename f_type>
struct function_traits;

// Function pointer.
template <typename r_type, typename... p_types>
struct function_traits<r_type (*)(p_types...)>
    : public function_traits<r_type(p_types...)> {};

template <typename r_type, typename... p_types>
struct function_traits<r_type(p_types...)> {
  using return_type = r_type;
  using parameter_types = std::tuple<p_types...>;
  static constexpr std::size_t arity = sizeof...(p_types);

  template <unsigned index>
  struct parameter {
    static_assert(index < arity, "ERROR: Invalid parameter index.");
    using type = typename std::tuple_element<index, parameter_types>::type;
  };
};

}  // namespace vixl

#endif  // VIXL_FUNCTION_TRAITS_H_
#endif  // VIXL_FUNCTION_TRAITS_SUPPORT
