#!/bin/bash
set -e

IMG="build/disk.img"
EFI="build/BOOTX64.EFI"

if [ ! -f "$IMG" ]; then
    truncate -s 64M "$IMG"
    mkfs.fat -F 32 "$IMG"
fi

mcopy -o -i "$IMG" "$EFI" ::/EFI/BOOT/BOOTX64.EFI
