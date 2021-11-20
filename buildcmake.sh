#!/bin/bash
mkdir -p outcmake
cd outcmake
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 .. && make -j4

