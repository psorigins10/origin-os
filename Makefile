CC = gcc
LD = ld

KERNEL_CFLAGS = -ffreestanding \
                -fno-stack-protector \
                -mno-red-zone

KERNEL_OBJ = kernel/init/main.o
KERNEL_ELF = kernel/kernel.elf
KERNEL_LD  = kernel/linker.ld

.PHONY: all kernel bootloader run clean

all: kernel bootloader

kernel: $(KERNEL_ELF)

$(KERNEL_OBJ): kernel/init/main.c
	$(CC) $(KERNEL_CFLAGS) -c $< -o $@

$(KERNEL_ELF): $(KERNEL_OBJ) $(KERNEL_LD)
	$(LD) -nostdlib -T $(KERNEL_LD) $(KERNEL_OBJ) -o $(KERNEL_ELF)

bootloader: kernel
	mcopy -o -i bootloader/build/disk.img $(KERNEL_ELF) ::/kernel.elf
	$(MAKE) -C bootloader

run: all
	./bootloader/scripts/run-qemu.sh

clean:
	rm -f $(KERNEL_OBJ) $(KERNEL_ELF)
	$(MAKE) -C bootloader clean