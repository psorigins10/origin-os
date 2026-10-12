.DEFAULT_GOAL := all

.PHONY: all bootloader kernel clean run debug debug-gdb

# ---------------------------------------------------------------------------
# Toolchain
# ---------------------------------------------------------------------------

BOOT_CC   := x86_64-w64-mingw32-gcc
KERNEL_CC := gcc
NASM      := nasm

# ---------------------------------------------------------------------------
# Include directories
# ---------------------------------------------------------------------------

SHARED_INC := -Iinclude
BOOT_INC   := -Ibootloader/include
KERNEL_INC := -Ikernel/include

# ---------------------------------------------------------------------------
# Compiler and linker flags
# ---------------------------------------------------------------------------

BOOT_CFLAGS  := -ffreestanding $(SHARED_INC) $(BOOT_INC) -MMD -MP
BOOT_LDFLAGS := -nostdlib -Wl,--subsystem,10 -Wl,--entry,efi_main

KERNEL_CFLAGS := -g -ffreestanding -fno-pie -fno-stack-protector \
                 -mno-red-zone $(SHARED_INC) $(KERNEL_INC) -MMD -MP

KERNEL_LDFLAGS := -nostdlib -nodefaultlibs -nostartfiles -no-pie \
                  -Wl,-e,kernel_entry

NASMFLAGS := -f elf64

# ---------------------------------------------------------------------------
# Build directories and output files
# ---------------------------------------------------------------------------

BOOT_OBJ_DIR   := bootloader/build/obj
BOOT_EFI_DIR   := bootloader/build/efi
KERNEL_OBJ_DIR := kernel/build/obj
KERNEL_BIN_DIR := kernel/build/bin

BOOTLOADER := $(BOOT_EFI_DIR)/BOOTX64.EFI
KERNEL     := $(KERNEL_BIN_DIR)/kernel.elf

KERNEL_LINKER_SCRIPT := kernel/linker.ld

# ---------------------------------------------------------------------------
# Source files
# ---------------------------------------------------------------------------

BOOT_SRCS := \
	bootloader/src/main.c \
	bootloader/src/console/console.c \
	bootloader/src/memory/memory_map.c \
	bootloader/src/memory/memory_map_dump.c \
	bootloader/src/elf/elf_loader.c

KERNEL_C_SRCS := \
	kernel/init/kernel.c \
	kernel/video/framebuffer.c \
	kernel/console/console.c \
	kernel/printk/printk.c \
	kernel/panic/panic.c \
	kernel/panic/format.c \
	drivers/serial/serial.c \
	arch/x86_64/gdt/gdt.c \
	arch/x86_64/idt/idt.c \
	arch/x86_64/idt/exception.c

KERNEL_ASM_SRCS := \
	arch/x86_64/boot/kernel_entry.asm \
	arch/x86_64/drivers/port_io.asm \
	arch/x86_64/panic/register_capture.asm \
	arch/x86_64/gdt/gdt_load.asm \
	arch/x86_64/idt/idt_load.asm \
	arch/x86_64/gdt/gdt_read.asm \
	arch/x86_64/idt/exception_handlers.asm

# ---------------------------------------------------------------------------
# Object-file lists
# ---------------------------------------------------------------------------

BOOT_OBJS := \
	$(patsubst bootloader/src/%.c,$(BOOT_OBJ_DIR)/%.o,$(BOOT_SRCS))

KERNEL_C_OBJS := \
	$(patsubst kernel/%.c,$(KERNEL_OBJ_DIR)/%.o,$(filter kernel/%.c,$(KERNEL_C_SRCS))) \
	$(patsubst drivers/%.c,$(KERNEL_OBJ_DIR)/drivers/%.o,$(filter drivers/%.c,$(KERNEL_C_SRCS))) \
	$(patsubst arch/%.c,$(KERNEL_OBJ_DIR)/arch/%.o,$(filter arch/%.c,$(KERNEL_C_SRCS)))

KERNEL_ASM_OBJS := \
	$(patsubst arch/%.asm,$(KERNEL_OBJ_DIR)/arch/%.o,$(KERNEL_ASM_SRCS))

# ---------------------------------------------------------------------------
# Default target
# ---------------------------------------------------------------------------

all: bootloader kernel

# ---------------------------------------------------------------------------
# Bootloader
# ---------------------------------------------------------------------------

bootloader: $(BOOTLOADER)
	@echo "Bootloader built: $(BOOTLOADER)"

$(BOOTLOADER): $(BOOT_OBJS)
	@mkdir -p $(dir $@)
	$(BOOT_CC) $(BOOT_LDFLAGS) $^ -o $@

$(BOOT_OBJ_DIR)/%.o: bootloader/src/%.c
	@mkdir -p $(dir $@)
	$(BOOT_CC) $(BOOT_CFLAGS) -c $< -o $@

# ---------------------------------------------------------------------------
# Kernel
# ---------------------------------------------------------------------------

kernel: $(KERNEL)
	@echo "Kernel built: $(KERNEL)"

$(KERNEL): $(KERNEL_ASM_OBJS) $(KERNEL_C_OBJS) $(KERNEL_LINKER_SCRIPT)
	@mkdir -p $(dir $@)
	$(KERNEL_CC) $(KERNEL_LDFLAGS) -T $(KERNEL_LINKER_SCRIPT) \
		$(KERNEL_ASM_OBJS) $(KERNEL_C_OBJS) -o $@

# C sources under kernel/
$(KERNEL_OBJ_DIR)/%.o: kernel/%.c
	@mkdir -p $(dir $@)
	$(KERNEL_CC) $(KERNEL_CFLAGS) -c $< -o $@

# C sources under drivers/
$(KERNEL_OBJ_DIR)/drivers/%.o: drivers/%.c
	@mkdir -p $(dir $@)
	$(KERNEL_CC) $(KERNEL_CFLAGS) -c $< -o $@

# Architecture assembly sources
$(KERNEL_OBJ_DIR)/arch/%.o: arch/%.asm
	@mkdir -p $(dir $@)
	$(NASM) $(NASMFLAGS) $< -o $@

# Architecture C sources
$(KERNEL_OBJ_DIR)/arch/%.o: arch/%.c
	@mkdir -p $(dir $@)
	$(KERNEL_CC) $(KERNEL_CFLAGS) -c $< -o $@

# ---------------------------------------------------------------------------
# Header dependencies
# ---------------------------------------------------------------------------

-include $(BOOT_OBJS:.o=.d)
-include $(KERNEL_C_OBJS:.o=.d)

# ---------------------------------------------------------------------------
# Clean
# ---------------------------------------------------------------------------

clean:
	rm -rf bootloader/build/obj
	rm -rf bootloader/build/efi
	rm -rf bootloader/build/esp
	rm -rf kernel/build

# ---------------------------------------------------------------------------
# Run
# ---------------------------------------------------------------------------

run: all
	@mkdir -p bootloader/build/esp
	./scripts/build-esp.sh
	./scripts/run-qemu.sh

# ---------------------------------------------------------------------------
# Debug
# ---------------------------------------------------------------------------

debug-gdb:
	gdb -x scripts/kernel.gdb $(KERNEL)

debug: all
	@mkdir -p bootloader/build/esp
	./scripts/build-esp.sh
	./scripts/debug-qemu.sh