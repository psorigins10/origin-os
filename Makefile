.PHONY: all bootloader clean run

all: bootloader

bootloader: bootloader/build/efi/BOOTX64.EFI
	@echo "Building bootloader..."

bootloader/build/efi/BOOTX64.EFI: bootloader/build/obj/main.o bootloader/build/obj/console.o
	x86_64-w64-mingw32-gcc -nostdlib -Wl,--subsystem,10 -Wl,--entry,efi_main bootloader/build/obj/main.o bootloader/build/obj/console.o -o bootloader/build/efi/BOOTX64.EFI

bootloader/build/obj/main.o: bootloader/src/main.c
	x86_64-w64-mingw32-gcc -ffreestanding -c bootloader/src/main.c -o bootloader/build/obj/main.o

bootloader/build/obj/console.o: bootloader/src/console.c
	x86_64-w64-mingw32-gcc -ffreestanding -c bootloader/src/console.c -o bootloader/build/obj/console.o

clean:
	rm -rf bootloader/build/obj/*
	rm -rf bootloader/build/efi/*
	rm -rf bootloader/build/esp/*

run: all
	./scripts/build-esp.sh
	./scripts/run-qemu.sh