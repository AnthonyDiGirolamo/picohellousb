// Copyright 2019 The Pigweed Authors
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not
// use this file except in compliance with the License. You may obtain a copy of
// the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations under
// the License.

#include <cinttypes>
#include <cstdint>

#include "hardware/irq.h"
#include "hardware/uart.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"

#include "pw_preprocessor/compiler.h"
#include "pw_sys_io/sys_io.h"

extern "C" void pw_sys_io_pico_Init() { stdio_init_all(); }

namespace pw::sys_io {

Status ReadByte(std::byte *dest) {
  while (true) {
    if (TryReadByte(dest).ok()) {
      return OkStatus();
    }
  }
}

Status TryReadByte(std::byte *dest) {
  int c = getchar_timeout_us(0);
  if (c == PICO_ERROR_TIMEOUT) {
    return Status::Unavailable();
  }
  *dest = static_cast<std::byte>(c);
  return OkStatus();
}

Status WriteByte(std::byte b) {
  putchar_raw((uint8_t)b);
  return OkStatus();
}

// Writes a string using pw::sys_io, and add newline characters at the end.
StatusWithSize WriteLine(const std::string_view &s) {
  size_t chars_written = 0;
  StatusWithSize result = WriteBytes(std::as_bytes(std::span(s)));
  if (!result.ok()) {
    return result;
  }
  chars_written += result.size();

  // Write trailing newline.
  result = WriteBytes(std::as_bytes(std::span("\r\n", 2)));
  chars_written += result.size();

  return StatusWithSize(result.status(), chars_written);
}

} // namespace pw::sys_io
