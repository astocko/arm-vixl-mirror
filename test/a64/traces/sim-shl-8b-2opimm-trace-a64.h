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


// ---------------------------------------------------------------------
// This file is auto generated using tools/generate_simulator_traces.py.
//
// PLEASE DO NOT EDIT.
// ---------------------------------------------------------------------

#ifndef VIXL_SIM_SHL_8B_2OPIMM_TRACE_A64_H_
#define VIXL_SIM_SHL_8B_2OPIMM_TRACE_A64_H_

const uint8_t kExpected_NEON_shl_8B_2OPIMM[] = {
  0x33, 0x55, 0x7d, 0x7e, 0x7f, 0x80, 0x81, 0x82,
  0x66, 0xaa, 0xfa, 0xfc, 0xfe, 0x00, 0x02, 0x04,
  0xcc, 0x54, 0xf4, 0xf8, 0xfc, 0x00, 0x04, 0x08,
  0x98, 0xa8, 0xe8, 0xf0, 0xf8, 0x00, 0x08, 0x10,
  0x30, 0x50, 0xd0, 0xe0, 0xf0, 0x00, 0x10, 0x20,
  0x60, 0xa0, 0xa0, 0xc0, 0xe0, 0x00, 0x20, 0x40,
  0xc0, 0x40, 0x40, 0x80, 0xc0, 0x00, 0x40, 0x80,
  0x80, 0x80, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00,
  0x55, 0x7d, 0x7e, 0x7f, 0x80, 0x81, 0x82, 0x83,
  0xaa, 0xfa, 0xfc, 0xfe, 0x00, 0x02, 0x04, 0x06,
  0x54, 0xf4, 0xf8, 0xfc, 0x00, 0x04, 0x08, 0x0c,
  0xa8, 0xe8, 0xf0, 0xf8, 0x00, 0x08, 0x10, 0x18,
  0x50, 0xd0, 0xe0, 0xf0, 0x00, 0x10, 0x20, 0x30,
  0xa0, 0xa0, 0xc0, 0xe0, 0x00, 0x20, 0x40, 0x60,
  0x40, 0x40, 0x80, 0xc0, 0x00, 0x40, 0x80, 0xc0,
  0x80, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80,
  0x7d, 0x7e, 0x7f, 0x80, 0x81, 0x82, 0x83, 0xaa,
  0xfa, 0xfc, 0xfe, 0x00, 0x02, 0x04, 0x06, 0x54,
  0xf4, 0xf8, 0xfc, 0x00, 0x04, 0x08, 0x0c, 0xa8,
  0xe8, 0xf0, 0xf8, 0x00, 0x08, 0x10, 0x18, 0x50,
  0xd0, 0xe0, 0xf0, 0x00, 0x10, 0x20, 0x30, 0xa0,
  0xa0, 0xc0, 0xe0, 0x00, 0x20, 0x40, 0x60, 0x40,
  0x40, 0x80, 0xc0, 0x00, 0x40, 0x80, 0xc0, 0x80,
  0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00,
  0x7e, 0x7f, 0x80, 0x81, 0x82, 0x83, 0xaa, 0xcc,
  0xfc, 0xfe, 0x00, 0x02, 0x04, 0x06, 0x54, 0x98,
  0xf8, 0xfc, 0x00, 0x04, 0x08, 0x0c, 0xa8, 0x30,
  0xf0, 0xf8, 0x00, 0x08, 0x10, 0x18, 0x50, 0x60,
  0xe0, 0xf0, 0x00, 0x10, 0x20, 0x30, 0xa0, 0xc0,
  0xc0, 0xe0, 0x00, 0x20, 0x40, 0x60, 0x40, 0x80,
  0x80, 0xc0, 0x00, 0x40, 0x80, 0xc0, 0x80, 0x00,
  0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00,
  0x7f, 0x80, 0x81, 0x82, 0x83, 0xaa, 0xcc, 0xf8,
  0xfe, 0x00, 0x02, 0x04, 0x06, 0x54, 0x98, 0xf0,
  0xfc, 0x00, 0x04, 0x08, 0x0c, 0xa8, 0x30, 0xe0,
  0xf8, 0x00, 0x08, 0x10, 0x18, 0x50, 0x60, 0xc0,
  0xf0, 0x00, 0x10, 0x20, 0x30, 0xa0, 0xc0, 0x80,
  0xe0, 0x00, 0x20, 0x40, 0x60, 0x40, 0x80, 0x00,
  0xc0, 0x00, 0x40, 0x80, 0xc0, 0x80, 0x00, 0x00,
  0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00,
  0x80, 0x81, 0x82, 0x83, 0xaa, 0xcc, 0xf8, 0xfd,
  0x00, 0x02, 0x04, 0x06, 0x54, 0x98, 0xf0, 0xfa,
  0x00, 0x04, 0x08, 0x0c, 0xa8, 0x30, 0xe0, 0xf4,
  0x00, 0x08, 0x10, 0x18, 0x50, 0x60, 0xc0, 0xe8,
  0x00, 0x10, 0x20, 0x30, 0xa0, 0xc0, 0x80, 0xd0,
  0x00, 0x20, 0x40, 0x60, 0x40, 0x80, 0x00, 0xa0,
  0x00, 0x40, 0x80, 0xc0, 0x80, 0x00, 0x00, 0x40,
  0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80,
  0x81, 0x82, 0x83, 0xaa, 0xcc, 0xf8, 0xfd, 0xfe,
  0x02, 0x04, 0x06, 0x54, 0x98, 0xf0, 0xfa, 0xfc,
  0x04, 0x08, 0x0c, 0xa8, 0x30, 0xe0, 0xf4, 0xf8,
  0x08, 0x10, 0x18, 0x50, 0x60, 0xc0, 0xe8, 0xf0,
  0x10, 0x20, 0x30, 0xa0, 0xc0, 0x80, 0xd0, 0xe0,
  0x20, 0x40, 0x60, 0x40, 0x80, 0x00, 0xa0, 0xc0,
  0x40, 0x80, 0xc0, 0x80, 0x00, 0x00, 0x40, 0x80,
  0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00,
  0x82, 0x83, 0xaa, 0xcc, 0xf8, 0xfd, 0xfe, 0xff,
  0x04, 0x06, 0x54, 0x98, 0xf0, 0xfa, 0xfc, 0xfe,
  0x08, 0x0c, 0xa8, 0x30, 0xe0, 0xf4, 0xf8, 0xfc,
  0x10, 0x18, 0x50, 0x60, 0xc0, 0xe8, 0xf0, 0xf8,
  0x20, 0x30, 0xa0, 0xc0, 0x80, 0xd0, 0xe0, 0xf0,
  0x40, 0x60, 0x40, 0x80, 0x00, 0xa0, 0xc0, 0xe0,
  0x80, 0xc0, 0x80, 0x00, 0x00, 0x40, 0x80, 0xc0,
  0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80,
  0x83, 0xaa, 0xcc, 0xf8, 0xfd, 0xfe, 0xff, 0x00,
  0x06, 0x54, 0x98, 0xf0, 0xfa, 0xfc, 0xfe, 0x00,
  0x0c, 0xa8, 0x30, 0xe0, 0xf4, 0xf8, 0xfc, 0x00,
  0x18, 0x50, 0x60, 0xc0, 0xe8, 0xf0, 0xf8, 0x00,
  0x30, 0xa0, 0xc0, 0x80, 0xd0, 0xe0, 0xf0, 0x00,
  0x60, 0x40, 0x80, 0x00, 0xa0, 0xc0, 0xe0, 0x00,
  0xc0, 0x80, 0x00, 0x00, 0x40, 0x80, 0xc0, 0x00,
  0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00,
  0xaa, 0xcc, 0xf8, 0xfd, 0xfe, 0xff, 0x00, 0x01,
  0x54, 0x98, 0xf0, 0xfa, 0xfc, 0xfe, 0x00, 0x02,
  0xa8, 0x30, 0xe0, 0xf4, 0xf8, 0xfc, 0x00, 0x04,
  0x50, 0x60, 0xc0, 0xe8, 0xf0, 0xf8, 0x00, 0x08,
  0xa0, 0xc0, 0x80, 0xd0, 0xe0, 0xf0, 0x00, 0x10,
  0x40, 0x80, 0x00, 0xa0, 0xc0, 0xe0, 0x00, 0x20,
  0x80, 0x00, 0x00, 0x40, 0x80, 0xc0, 0x00, 0x40,
  0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80,
  0xcc, 0xf8, 0xfd, 0xfe, 0xff, 0x00, 0x01, 0x02,
  0x98, 0xf0, 0xfa, 0xfc, 0xfe, 0x00, 0x02, 0x04,
  0x30, 0xe0, 0xf4, 0xf8, 0xfc, 0x00, 0x04, 0x08,
  0x60, 0xc0, 0xe8, 0xf0, 0xf8, 0x00, 0x08, 0x10,
  0xc0, 0x80, 0xd0, 0xe0, 0xf0, 0x00, 0x10, 0x20,
  0x80, 0x00, 0xa0, 0xc0, 0xe0, 0x00, 0x20, 0x40,
  0x00, 0x00, 0x40, 0x80, 0xc0, 0x00, 0x40, 0x80,
  0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00,
  0xf8, 0xfd, 0xfe, 0xff, 0x00, 0x01, 0x02, 0x08,
  0xf0, 0xfa, 0xfc, 0xfe, 0x00, 0x02, 0x04, 0x10,
  0xe0, 0xf4, 0xf8, 0xfc, 0x00, 0x04, 0x08, 0x20,
  0xc0, 0xe8, 0xf0, 0xf8, 0x00, 0x08, 0x10, 0x40,
  0x80, 0xd0, 0xe0, 0xf0, 0x00, 0x10, 0x20, 0x80,
  0x00, 0xa0, 0xc0, 0xe0, 0x00, 0x20, 0x40, 0x00,
  0x00, 0x40, 0x80, 0xc0, 0x00, 0x40, 0x80, 0x00,
  0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00,
  0xfd, 0xfe, 0xff, 0x00, 0x01, 0x02, 0x08, 0x33,
  0xfa, 0xfc, 0xfe, 0x00, 0x02, 0x04, 0x10, 0x66,
  0xf4, 0xf8, 0xfc, 0x00, 0x04, 0x08, 0x20, 0xcc,
  0xe8, 0xf0, 0xf8, 0x00, 0x08, 0x10, 0x40, 0x98,
  0xd0, 0xe0, 0xf0, 0x00, 0x10, 0x20, 0x80, 0x30,
  0xa0, 0xc0, 0xe0, 0x00, 0x20, 0x40, 0x00, 0x60,
  0x40, 0x80, 0xc0, 0x00, 0x40, 0x80, 0x00, 0xc0,
  0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x80,
  0xfe, 0xff, 0x00, 0x01, 0x02, 0x08, 0x33, 0x55,
  0xfc, 0xfe, 0x00, 0x02, 0x04, 0x10, 0x66, 0xaa,
  0xf8, 0xfc, 0x00, 0x04, 0x08, 0x20, 0xcc, 0x54,
  0xf0, 0xf8, 0x00, 0x08, 0x10, 0x40, 0x98, 0xa8,
  0xe0, 0xf0, 0x00, 0x10, 0x20, 0x80, 0x30, 0x50,
  0xc0, 0xe0, 0x00, 0x20, 0x40, 0x00, 0x60, 0xa0,
  0x80, 0xc0, 0x00, 0x40, 0x80, 0x00, 0xc0, 0x40,
  0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x80, 0x80,
  0xff, 0x00, 0x01, 0x02, 0x08, 0x33, 0x55, 0x7d,
  0xfe, 0x00, 0x02, 0x04, 0x10, 0x66, 0xaa, 0xfa,
  0xfc, 0x00, 0x04, 0x08, 0x20, 0xcc, 0x54, 0xf4,
  0xf8, 0x00, 0x08, 0x10, 0x40, 0x98, 0xa8, 0xe8,
  0xf0, 0x00, 0x10, 0x20, 0x80, 0x30, 0x50, 0xd0,
  0xe0, 0x00, 0x20, 0x40, 0x00, 0x60, 0xa0, 0xa0,
  0xc0, 0x00, 0x40, 0x80, 0x00, 0xc0, 0x40, 0x40,
  0x80, 0x00, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80,
  0x00, 0x01, 0x02, 0x08, 0x33, 0x55, 0x7d, 0x7e,
  0x00, 0x02, 0x04, 0x10, 0x66, 0xaa, 0xfa, 0xfc,
  0x00, 0x04, 0x08, 0x20, 0xcc, 0x54, 0xf4, 0xf8,
  0x00, 0x08, 0x10, 0x40, 0x98, 0xa8, 0xe8, 0xf0,
  0x00, 0x10, 0x20, 0x80, 0x30, 0x50, 0xd0, 0xe0,
  0x00, 0x20, 0x40, 0x00, 0x60, 0xa0, 0xa0, 0xc0,
  0x00, 0x40, 0x80, 0x00, 0xc0, 0x40, 0x40, 0x80,
  0x00, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00,
  0x01, 0x02, 0x08, 0x33, 0x55, 0x7d, 0x7e, 0x7f,
  0x02, 0x04, 0x10, 0x66, 0xaa, 0xfa, 0xfc, 0xfe,
  0x04, 0x08, 0x20, 0xcc, 0x54, 0xf4, 0xf8, 0xfc,
  0x08, 0x10, 0x40, 0x98, 0xa8, 0xe8, 0xf0, 0xf8,
  0x10, 0x20, 0x80, 0x30, 0x50, 0xd0, 0xe0, 0xf0,
  0x20, 0x40, 0x00, 0x60, 0xa0, 0xa0, 0xc0, 0xe0,
  0x40, 0x80, 0x00, 0xc0, 0x40, 0x40, 0x80, 0xc0,
  0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x80,
  0x02, 0x08, 0x33, 0x55, 0x7d, 0x7e, 0x7f, 0x80,
  0x04, 0x10, 0x66, 0xaa, 0xfa, 0xfc, 0xfe, 0x00,
  0x08, 0x20, 0xcc, 0x54, 0xf4, 0xf8, 0xfc, 0x00,
  0x10, 0x40, 0x98, 0xa8, 0xe8, 0xf0, 0xf8, 0x00,
  0x20, 0x80, 0x30, 0x50, 0xd0, 0xe0, 0xf0, 0x00,
  0x40, 0x00, 0x60, 0xa0, 0xa0, 0xc0, 0xe0, 0x00,
  0x80, 0x00, 0xc0, 0x40, 0x40, 0x80, 0xc0, 0x00,
  0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x80, 0x00,
  0x08, 0x33, 0x55, 0x7d, 0x7e, 0x7f, 0x80, 0x81,
  0x10, 0x66, 0xaa, 0xfa, 0xfc, 0xfe, 0x00, 0x02,
  0x20, 0xcc, 0x54, 0xf4, 0xf8, 0xfc, 0x00, 0x04,
  0x40, 0x98, 0xa8, 0xe8, 0xf0, 0xf8, 0x00, 0x08,
  0x80, 0x30, 0x50, 0xd0, 0xe0, 0xf0, 0x00, 0x10,
  0x00, 0x60, 0xa0, 0xa0, 0xc0, 0xe0, 0x00, 0x20,
  0x00, 0xc0, 0x40, 0x40, 0x80, 0xc0, 0x00, 0x40,
  0x00, 0x80, 0x80, 0x80, 0x00, 0x80, 0x00, 0x80,
};
const unsigned kExpectedCount_NEON_shl_8B_2OPIMM = 152;

#endif  // VIXL_SIM_SHL_8B_2OPIMM_TRACE_A64_H_
