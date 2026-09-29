.PHONY: all bootloader clean run

all: bootloader

bootloader: bootloader/build/efi/BOOTX64.EFI
	@echo "Building bootloader..."

bootloader/build/efi/BOOTX64.EFI: \
	bootloader/build/obj/main.o \
	bootloader/build/obj/console.o \
	bootloader/build/obj/memory_map.o \
	bootloader/build/obj/memory_map_dump.o
	x86_64-w64-mingw32-gcc -nostdlib -Wl,--subsystem,10 -Wl,--entry,efi_main \
		bootloader/build/obj/main.o \
		bootloader/build/obj/console.o \
		bootloader/build/obj/memory_map.o \
		bootloader/build/obj/memory_map_dump.o \
		-o bootloader/build/efi/BOOTX64.EFI

bootloader/build/obj/main.o: bootloader/src/main.c
	x86_64-w64-mingw32-gcc -ffreestanding -c bootloader/src/main.c -o bootloader/build/obj/main.o

bootloader/build/obj/console.o: bootloader/src/console/console.c
	x86_64-w64-mingw32-gcc -ffreestanding -c bootloader/src/console/console.c -o bootloader/build/obj/console.o

bootloader/build/obj/memory_map.o: bootloader/src/memory/memory_map.c
	x86_64-w64-mingw32-gcc -ffreestanding -c bootloader/src/memory/memory_map.c -o bootloader/build/obj/memory_map.o

bootloader/build/obj/memory_map_dump.o: bootloader/src/memory/memory_map_dump.c
	x86_64-w64-mingw32-gcc -ffreestanding -c bootloader/src/memory/memory_map_dump.c -o bootloader/build/obj/memory_map_dump.o

clean:
	rm -rf bootloader/build/obj/*
	rm -rf bootloader/build/efi/*
	rm -rf bootloader/build/esp/*

run: all
	./scripts/build-esp.sh
	./scripts/run-qemu.sh