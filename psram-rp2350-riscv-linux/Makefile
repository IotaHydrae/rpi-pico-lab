ELF := build/psram-bootloader.elf

all: build/build.ninja
	ninja -C build

build:
	mkdir -p build

build/build.ninja: CMakeLists.txt | build
	cd build && cmake -DPICO_PLATFORM=rp2350-riscv .. -G Ninja

flash: all
	openocd -f interface/cmsis-dap.cfg -f target/rp2350-riscv.cfg -s tcl -c "adapter speed 10000" -c "program $(ELF) reset exit"

flash-kernel:
	sudo picotool load -fxup 0 prebuilt/flash-image.bin

openocd-server:
	openocd -f interface/cmsis-dap.cfg -f target/rp2350-riscv.cfg -s tcl

debug: $(ELF)
	gdb-multiarch $^ --command="debug.gdb"
