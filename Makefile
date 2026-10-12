.PHONY: all bootloader kernel clean run debug debug-gdb

# ---------------------------------------------------------------------------
# Toolchain
# ---------------------------------------------------------------------------

BOOT_CC   := x86_64-w64-mingw32-gcc
KERNEL_CC := gcc
NASM      := nasm

# ---------------------------------------------------------------------------
# Include roots
# ---------------------------------------------------------------------------

SHARED_INC := -Iinclude
BOOT_INC   := -Ibootloader/include
KERNEL_INC := -Ikernel/include

# ---------------------------------------------------------------------------
# Flags
# ---------------------------------------------------------------------------

BOOT_CFLAGS   := -ffreestanding $(SHARED_INC) $(BOOT_INC) -MMD -MP
BOOT_LDFLAGS  := -nostdlib -Wl,--subsystem,10 -Wl,--entry,efi_main

KERNEL_CFLAGS := -g -ffreestanding -fno-pie -fno-stack-protector -mno-red-zone \
                 $(SHARED_INC) $(KERNEL_INC) -MMD -MP
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
KERNEL     := $(KERNEL_BIN_DIR)/kernel.elf

KERNEL_LINKER_SCRIPT := kernel/linker.ld

# ---------------------------------------------------------------------------
# Sources
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
	drivers/serial/serial.c

KERNEL_ASM_SRCS := \
	arch/x86_64/boot/kernel_entry.asm \
	arch/x86_64/drivers/port_io.asm

# ---------------------------------------------------------------------------
# Objects
# ---------------------------------------------------------------------------

BOOT_OBJS       := $(patsubst bootloader/src/%.c,$(BOOT_OBJ_DIR)/%.o,$(BOOT_SRCS))
KERNEL_C_OBJS   := $(patsubst kernel/%.c,$(KERNEL_OBJ_DIR)/%.o,$(filter kernel/%.c,$(KERNEL_C_SRCS))) \
                   $(patsubst drivers/%.c,$(KERNEL_OBJ_DIR)/drivers/%.o,$(filter drivers/%.c,$(KERNEL_C_SRCS)))
KERNEL_ASM_OBJS := $(patsubst arch/%.asm,$(KERNEL_OBJ_DIR)/arch/%.o,$(KERNEL_ASM_SRCS))

# ---------------------------------------------------------------------------
# Default
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

$(KERNEL_OBJ_DIR)/%.o: kernel/%.c
	@mkdir -p $(dir $@)
	$(KERNEL_CC) $(KERNEL_CFLAGS) -c $< -o $@

$(KERNEL_OBJ_DIR)/drivers/%.o: drivers/%.c
	@mkdir -p $(dir $@)
	$(KERNEL_CC) $(KERNEL_CFLAGS) -c $< -o $@

$(KERNEL_OBJ_DIR)/arch/%.o: arch/%.asm
	@mkdir -p $(dir $@)
	$(NASM) $(NASMFLAGS) $< -o $@

# ---------------------------------------------------------------------------
# Header dependency files
# ---------------------------------------------------------------------------

-include $(BOOT_OBJS:.o=.d) $(KERNEL_C_OBJS:.o=.d)

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

debug-gdb:
	gdb -x scripts/kernel.gdb kernel/build/bin/kernel.elf

debug: all
	@mkdir -p bootloader/build/esp
	./scripts/build-esp.sh
	./scripts/debug-qemu.sh