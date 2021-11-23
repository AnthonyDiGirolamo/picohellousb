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
#include "pico/stdlib.h"

#include "pw_preprocessor/compiler.h"
#include "pw_sys_io/sys_io.h"

namespace {

#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY UART_PARITY_NONE

#define UART_TX_PIN 0
#define UART_RX_PIN 1

static int chars_rxed = 0;

// // RX interrupt handler
// void on_uart_rx() {
//   while (uart_is_readable(UART_ID)) {
//     uint8_t ch = uart_getc(UART_ID);
//     // Can we send it back?
//     if (uart_is_writable(UART_ID)) {
//       // Change it slightly first!
//       ch++;
//       uart_putc(UART_ID, ch);
//     }
//     chars_rxed++;
//   }
// }

} // namespace

extern "C" void pw_sys_io_pico_Init() {
  // Set up our UART with a basic baud rate.
  uart_init(UART_ID, 2400);

  // Set the TX and RX pins by using the function select on the GPIO
  // Set datasheet for more information on function select
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

  // Actually, we want a different speed
  // The call will return the actual baud rate selected, which will be as close
  // as possible to that requested
  int __unused actual = uart_set_baudrate(UART_ID, BAUD_RATE);

  // Set UART flow control CTS/RTS, we don't want these, so turn them off
  uart_set_hw_flow(UART_ID, false, false);

  // Set our data format
  uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

  // Turn off FIFO's - we want to do this character by character
  uart_set_fifo_enabled(UART_ID, false);

  // Set up a RX interrupt
  // We need to set up the handler first
  // Select correct interrupt for the UART we are using

  // int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

  // And set up and enable the interrupt handlers

  // irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
  // irq_set_enabled(UART_IRQ, true);

  // Now enable the UART to send interrupts - RX only

  // uart_set_irq_enables(UART_ID, true, false);

  // OK, all set up.
  // Lets send a basic string out, and then run a loop and wait for RX
  // interrupts The handler will count them, but also reflect the incoming data
  // back with a slight change!

  // uart_puts(UART_ID, "\nHello, uart interrupts\n");
}

namespace pw::sys_io {

Status ReadByte(std::byte *dest) {
  while (true) {
    if (TryReadByte(dest).ok()) {
      return OkStatus();
    }
  }
}

Status TryReadByte(std::byte *dest) {
  if (!uart_is_readable(UART_ID)) {
    return Status::Unavailable();
  }
  *dest = static_cast<std::byte>(uart_getc(UART_ID));
  return OkStatus();
}

Status WriteByte(std::byte b) {
  uart_putc(UART_ID, (uint8_t)b);
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
