GNU_EFI = /home/psorigins/gnu-efi
ARCH = x86_64

CC = gcc
LD = ld
OBJCOPY = objcopy

CFLAGS = -I./include \
		 -I$(GNU_EFI)/inc \
         -I$(GNU_EFI)/inc/$(ARCH) \
         -ffreestanding \
         -fno-stack-protector \
         -fno-stack-check \
         -fshort-wchar \
         -mno-red-zone \
         -maccumulate-outgoing-args \
         -fpic

LDFLAGS = -nostdlib \
          --warn-common \
          --no-undefined \
          --fatal-warnings \
          --build-id=sha1 \
          -z norelro \
          -z nocombreloc \
          -L$(GNU_EFI)/$(ARCH)/lib \
          -L$(GNU_EFI)/$(ARCH)/gnuefi \
          $(GNU_EFI)/$(ARCH)/gnuefi/crt0-efi-x86_64.o \
          -pie \
          -Bsymbolic \
          --no-dynamic-linker

LIBS = -lefi -lgnuefi \
       /usr/lib/gcc/x86_64-pc-linux-gnu/16/libgcc.a

all: build/BOOTX64.EFI build/disk.img

build/disk.img: build/BOOTX64.EFI scripts/mkimage.sh
	./scripts/mkimage.sh

build/obj/main.o: src/main.c
	mkdir -p build/obj
	$(CC) $(CFLAGS) -c $< -o $@

build/obj/file.o: src/file.c
	mkdir -p build/obj
	$(CC) $(CFLAGS) -c $< -o $@

build/obj/elf.o: src/elf.c
	mkdir -p build/obj
	$(CC) $(CFLAGS) -c $< -o $@

build/originboot.so: build/obj/main.o build/obj/file.o build/obj/elf.o
	$(LD) $(LDFLAGS) $^ \
		-o $@ \
		-T $(GNU_EFI)/gnuefi/elf_x86_64_efi.lds \
		$(LIBS)

build/BOOTX64.EFI: build/originboot.so
	$(OBJCOPY) \
		-j .text \
		-j .sdata \
		-j .data \
		-j .dynamic \
		-j .rodata \
		-j .rel \
		-j .rela \
		-j .rel.* \
		-j .rela.* \
		-j .rel* \
		-j .rela* \
		-j .areloc \
		-j .reloc \
		-O efi-app-x86_64 \
		$< $@

clean:
	rm -rf build/obj build/originboot.so build/BOOTX64.EFI