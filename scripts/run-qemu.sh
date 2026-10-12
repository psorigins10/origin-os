qemu-system-x86_64 \
    -device VGA,edid=on,xres=1920,yres=1080 \
    -drive if=pflash,format=raw,readonly=on,file=/usr/share/edk2/x64/OVMF_CODE.4m.fd \
    -drive format=raw,file=bootloader/build/esp/esp.img \
    -serial stdio