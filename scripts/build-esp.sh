#!/bin/bash

set -e

ESP_IMAGE="bootloader/build/esp/esp.img"
MOUNT_POINT="/tmp/origin-esp"
BOOTLOADER="bootloader/build/efi/BOOTX64.EFI"
KERNEL="kernel/build/bin/kernel.elf"

echo "==> Preparing ORIGIN ESP..."

# Create ESP image if it doesn't exist
if [ ! -f "$ESP_IMAGE" ]; then
    echo "==> Creating ESP image..."
    truncate -s 64M "$ESP_IMAGE"

    echo "==> Formatting ESP as FAT32..."
    mkfs.fat -F 32 "$ESP_IMAGE"
fi

# Create mount point
mkdir -p "$MOUNT_POINT"

# Mount ESP
echo "==> Mounting ESP..."
sudo mount -o loop "$ESP_IMAGE" "$MOUNT_POINT"

# Always unmount when the script exits
trap 'sudo umount "$MOUNT_POINT"' EXIT

# Create UEFI boot directory
sudo mkdir -p "$MOUNT_POINT/EFI/BOOT"
sudo mkdir -p "$MOUNT_POINT/boot"

# Copy bootloader
echo "==> Installing BOOTX64.EFI..."
sudo cp "$BOOTLOADER" "$MOUNT_POINT/EFI/BOOT/BOOTX64.EFI"

# Copy kernel
echo "==> Installing kernel.elf..."
sudo cp "$KERNEL" "$MOUNT_POINT/boot/kernel.elf"

echo "==> ESP ready."