#!/bin/bash

set -e

ESP_IMAGE="bootloader/build/esp/esp.img"
MOUNT_POINT="/tmp/origin-esp"
BOOTLOADER="bootloader/build/efi/BOOTX64.EFI"

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

# Copy bootloader
echo "==> Installing BOOTX64.EFI..."
sudo cp "$BOOTLOADER" "$MOUNT_POINT/EFI/BOOT/BOOTX64.EFI"

echo "==> ESP ready."