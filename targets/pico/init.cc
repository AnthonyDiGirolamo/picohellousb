// Copyright 2020 The Pigweed Authors
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

#include "pico/stdlib.h"

#include "pw_sys_io_arduino/init.h"

// Arduino target specific init. For Pigweed, this calls pw_sys_io's init. User
// projects may chose to provide something different if they need more pre-main
// init functionality.
extern "C" void pw_pico_Init() { stdio_init_all(); }