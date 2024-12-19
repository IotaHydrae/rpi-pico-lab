#!/bin/bash

ninja -C build

openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg &
gdb-multiarch build/psram-mpu.elf --command="opencod.gdb"
