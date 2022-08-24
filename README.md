RPI PICO LAB
============
Hi, guys

This is a lab workspace for rpi `RP2040` mcu and based on `rpi-pico`, also contains examples about sensor or display panel ssd1306 etc. So enjoy it.

Cheers,

Clone this repo
---------------
```shell
$ git clone https://github.com/IotaHydrae/rpi-pico-lab.git

$ git submodule update --init
```


Install OpenOCD
---------------
In this case, we use a `CMSIS-DAPLINK` debugger, if you use a st-link or whatever, just enable it.
```shell
# install pre req
$ sudo apt install automake autoconf build-essential texinfo libtool libftdi-dev libusb-1.0-0-dev libhidapi-dev -y

$ git clone https://github.com/raspberrypi/openocd.git --recursive --branch rp2040 --depth=1

$ ./bootstrap
$ ./configure --enable-cmsis-dap
$ make -j12
$ sudo make install
```

Source ENV and export Toolboxs
----------
This will export the path of `pcio-sdk` and some script to environment.

The `load`, `reset` etc. scripts only comptibled with `CMSIS-DAPLINK`, so if you use anthoer
debugger, change the debugger type in script.
```shell
$ source tools/envsetup.sh
```
The `daplink-reset` and `daplink-program` will be added to your shell env

Build examples
--------------
```shell
$ cd epink-allinone
$ mkdir build && cd build
$ cmake ..
$ make -j
```

Load and run program on device
------------------------------
```shell
$ daplink-program epink.elf
```

More
----

### How to use GDB with OpenOCD

First, start a OpenOCD debugger client, you can use `daplink-debug.sh` to start it or just start it maunally like:
```shell
$ openocd -f ... -f ... "gdb_port 3333"
```

Then use `gdb-multiarch` load program
```shell
$ gdb-multiarch ./epink.elf

/* specify a remote target debugger */
(gdb) target remote localhost:3333

/* set a breakpoint at main function 614 line */
(gdb) break main:614

/* look up we have any breakpoints */
(gdb) info breakpoints

/* reset and halt mcu */
(gdb) monitor reset halt

/* run program to first breakpoint */
(gdb) continue

/* run to the next or end */
(gdb) continue
```

### How to create a new example folder
```shell
$ mkdir hello && cd hello
$ cp pico-sdk/external/pico_sdk_import.cmake .
```

a `CMakeLists.txt` like this:
```cmake
cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(hello)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(hello main.c)

target_link_libraries(hello pico_stdlib)
```

add a `main.c` like this :
```c
#include "pico/stdlib.h"

int main(void)
{
    stdio_init_all();

    while(true) {
        printf("Hello, world!\n");
        sleep_ms(500);
    }

    return 0;
}
```

If you want the stdout from uart, add these options to your `CMakeLists.txt`

```cmake
pico_enable_stdio_usb(hello 0)
pico_enable_stdio_uart(hello 1)
```

Also, If you want to get *.bin and *.hex, add this to your `CMakeLists.txt`
```cmake
pico_add_extra_outputs(hello)
```
