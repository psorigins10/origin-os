#!/bin/bash
set -e

qemu-system-x86_64 \
  -serial stdio \
  -drive if=pflash,format=raw,readonly=on,file=/usr/share/edk2/x64/OVMF_CODE.4m.fd \
  -drive format=raw,file=bootloader/build/disk.img
