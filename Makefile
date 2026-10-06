.PHONY: all bootloader kernel clean run

# ---------------------------------------------------------------------------
# Toolchain
# ---------------------------------------------------------------------------

BOOT_CC := x86_64-w64-mingw32-gcc
KERNEL_CC := gcc
NASM := nasm

# ---------------------------------------------------------------------------
# Flags
# ---------------------------------------------------------------------------

BOOT_CFLAGS := -ffreestanding -Iinclude
BOOT_LDFLAGS := -nostdlib -Wl,--subsystem,10 -Wl,--entry,efi_main

KERNEL_CFLAGS := -ffreestanding -fno-pie -fno-stack-protector -mno-red-zone -Iinclude
KERNEL_LDFLAGS := -nostdlib -nodefaultlibs -nostartfiles -no-pie -Wl,-e,kernel_entry

NASMFLAGS := -f elf64

# ---------------------------------------------------------------------------
# Paths
# ---------------------------------------------------------------------------

BOOT_OBJ_DIR := bootloader/build/obj
BOOT_EFI_DIR := bootloader/build/efi

KERNEL_OBJ_DIR := kernel/build/obj
KERNEL_BIN_DIR := kernel/build/bin

BOOTLOADER := $(BOOT_EFI_DIR)/BOOTX64.EFI
KERNEL := $(KERNEL_BIN_DIR)/kernel.elf

# Kernel linker script
KERNEL_LINKER_SCRIPT := kernel/linker.ld

# ---------------------------------------------------------------------------
# Default
# ---------------------------------------------------------------------------

all: bootloader kernel

# ---------------------------------------------------------------------------
# Bootloader
# ---------------------------------------------------------------------------

bootloader: $(BOOTLOADER)
	@echo "Bootloader built: $(BOOTLOADER)"

$(BOOTLOADER): \
	$(BOOT_OBJ_DIR)/main.o \
	$(BOOT_OBJ_DIR)/console.o \
	$(BOOT_OBJ_DIR)/memory_map.o \
	$(BOOT_OBJ_DIR)/memory_map_dump.o \
	$(BOOT_OBJ_DIR)/elf_loader.o

	$(BOOT_CC) $(BOOT_LDFLAGS) \
		$(BOOT_OBJ_DIR)/main.o \
		$(BOOT_OBJ_DIR)/console.o \
		$(BOOT_OBJ_DIR)/memory_map.o \
		$(BOOT_OBJ_DIR)/memory_map_dump.o \
		$(BOOT_OBJ_DIR)/elf_loader.o \
		-o $(BOOTLOADER)

$(BOOT_OBJ_DIR)/main.o: bootloader/src/main.c
	@mkdir -p $(BOOT_OBJ_DIR)
	$(BOOT_CC) $(BOOT_CFLAGS) -c $< -o $@

$(BOOT_OBJ_DIR)/console.o: bootloader/src/console/console.c
	@mkdir -p $(BOOT_OBJ_DIR)
	$(BOOT_CC) $(BOOT_CFLAGS) -c $< -o $@

$(BOOT_OBJ_DIR)/memory_map.o: bootloader/src/memory/memory_map.c
	@mkdir -p $(BOOT_OBJ_DIR)
	$(BOOT_CC) $(BOOT_CFLAGS) -c $< -o $@

$(BOOT_OBJ_DIR)/memory_map_dump.o: bootloader/src/memory/memory_map_dump.c
	@mkdir -p $(BOOT_OBJ_DIR)
	$(BOOT_CC) $(BOOT_CFLAGS) -c $< -o $@

$(BOOT_OBJ_DIR)/elf_loader.o: bootloader/src/elf/elf_loader.c
	@mkdir -p $(BOOT_OBJ_DIR)
	$(BOOT_CC) $(BOOT_CFLAGS) -c $< -o $@

# ---------------------------------------------------------------------------
# Kernel
# ---------------------------------------------------------------------------

kernel: $(KERNEL)
	@echo "Kernel built: $(KERNEL)"

$(KERNEL): \
	$(KERNEL_OBJ_DIR)/kernel.o \
	$(KERNEL_OBJ_DIR)/kernel_entry.o \
	$(KERNEL_LINKER_SCRIPT)

	@mkdir -p $(KERNEL_BIN_DIR)
	$(KERNEL_CC) $(KERNEL_LDFLAGS) \
		-T $(KERNEL_LINKER_SCRIPT) \
		$(KERNEL_OBJ_DIR)/kernel_entry.o \
		$(KERNEL_OBJ_DIR)/kernel.o \
		-o $(KERNEL)

$(KERNEL_OBJ_DIR)/kernel.o: kernel/init/kernel.c
	@mkdir -p $(KERNEL_OBJ_DIR)
	$(KERNEL_CC) $(KERNEL_CFLAGS) -c $< -o $@

$(KERNEL_OBJ_DIR)/kernel_entry.o: arch/x86_64/boot/kernel_entry.asm
	@mkdir -p $(KERNEL_OBJ_DIR)
	$(NASM) $(NASMFLAGS) $< -o $@

# ---------------------------------------------------------------------------
# Clean
# ---------------------------------------------------------------------------

clean:
	rm -rf bootloader/build/obj/*
	rm -rf bootloader/build/efi/*
	rm -rf bootloader/build/esp/*
	rm -rf kernel/build

# ---------------------------------------------------------------------------
# Run
# ---------------------------------------------------------------------------

run: all
	./scripts/build-esp.sh
	./scripts/run-qemu.sh