# ORIGIN Boot Protocol

The ORIGIN boot protocol defines the interface between the UEFI bootloader
and the ORIGIN kernel.

The bootloader prepares the information required by the kernel before
terminating UEFI Boot Services and transfers control to the kernel entry
point.

## Boot Information Structure

The bootloader passes a pointer to:

```c
ORIGIN_BOOT_INFO
```

The current structure is:

```c
typedef struct {
    uint64_t memory_map;
    uint64_t memory_map_size;
    uint64_t memory_descriptor_size;

    uint64_t fb_base;
    uint64_t fb_size;
    uint32_t fb_width;
    uint32_t fb_height;
    uint32_t fb_pixels_per_scanline;
    uint32_t fb_pixel_format;
} ORIGIN_BOOT_INFO;
```

The structure contains two groups of boot information:

```text
ORIGIN_BOOT_INFO
├── Memory map information
│   ├── memory_map
│   ├── memory_map_size
│   └── memory_descriptor_size
│
└── Framebuffer information
    ├── fb_base
    ├── fb_size
    ├── fb_width
    ├── fb_height
    ├── fb_pixels_per_scanline
    └── fb_pixel_format
```

## Memory Map Fields

### `memory_map`

`memory_map` contains the address of the final UEFI memory map obtained before
`ExitBootServices()`.

The value is stored as a 64-bit integer in the boot protocol structure.

The map represents the memory state associated with the final
`ExitBootServices()` transition.

### `memory_map_size`

`memory_map_size` contains the actual size of the final memory map returned by
`GetMemoryMap()`.

It represents the valid byte range of the memory map beginning at
`memory_map`.

### `memory_descriptor_size`

`memory_descriptor_size` contains the descriptor size reported by UEFI.

The kernel must use this value when walking the memory map rather than
assuming that descriptors have a fixed stride based only on
`sizeof(EFI_MEMORY_DESCRIPTOR)`.

## Framebuffer Fields

### `fb_base`

`fb_base` contains the physical address of the framebuffer provided by the
UEFI Graphics Output Protocol.

The kernel uses this address as the base of its framebuffer access.

### `fb_size`

`fb_size` contains the framebuffer size reported by the Graphics Output
Protocol.

### `fb_width`

`fb_width` contains the framebuffer horizontal resolution in pixels.

### `fb_height`

`fb_height` contains the framebuffer vertical resolution in pixels.

### `fb_pixels_per_scanline`

`fb_pixels_per_scanline` contains the number of pixels between the beginning
of consecutive framebuffer scanlines.

The kernel uses this value when calculating framebuffer offsets.

### `fb_pixel_format`

`fb_pixel_format` contains the pixel format reported by the Graphics Output
Protocol.

The current definition uses:

```text
0 = RGB
1 = BGR
```

The current kernel output uses white pixels, which have the same byte
representation for both RGB and BGR formats. General pixel-format handling
is therefore not yet part of the current kernel framebuffer implementation.

## Kernel Entry Contract

The bootloader transfers control to:

```text
kernel_entry
```

The kernel image defines `kernel_entry` as its ELF entry point.

The bootloader invokes the kernel entry point with:

```text
RDI = ORIGIN_BOOT_INFO *
```

The kernel entry assembly preserves the incoming `RDI` value and calls:

```c
kernel_main(ORIGIN_BOOT_INFO *BootInfo);
```

The current kernel therefore receives the boot information pointer directly
through the System V AMD64 calling convention.

## ABI Boundary

The bootloader and kernel are built using different x86-64 calling
conventions.

The bootloader is compiled with the MinGW x86-64 environment, while the
kernel uses the System V AMD64 ABI.

The bootloader's kernel entry function pointer is explicitly declared with:

```c
typedef void (__attribute__((sysv_abi)) *KERNEL_ENTRY)(
    ORIGIN_BOOT_INFO *
);
```

This causes the kernel entry call to use the System V AMD64 calling
convention.

Under this convention, the first pointer argument is passed in:

```text
RDI
```

The kernel entry assembly therefore does not move the argument from `RCX` to
`RDI`.

## Kernel Entry Initialization

The current `kernel_entry` performs the following initialization:

```text
kernel_entry
    │
    ├── cli
    ├── initialize bootstrap stack
    ├── clear RBP
    └── call kernel_main
```

The bootstrap stack is reserved by the kernel entry assembly and is 16 KiB in
size.

The incoming `ORIGIN_BOOT_INFO *` value remains available in `RDI` when
`kernel_main` is called.

## UEFI Boundary

The boot protocol is established before the bootloader calls
`ExitBootServices()`.

The bootloader prepares the boot information while UEFI Boot Services are
still available.

The transition is:

```text
UEFI Boot Services
       │
       ├── filesystem access
       ├── kernel loading
       ├── graphics discovery
       ├── memory-map acquisition
       └── ORIGIN_BOOT_INFO construction
       │
       ▼
ExitBootServices()
       │
       ▼
kernel_entry
       │
       ▼
kernel_main(ORIGIN_BOOT_INFO *)
```

After successful `ExitBootServices()`, the kernel must not depend on UEFI
Boot Services.

The UEFI System Table and Boot Services table are not passed to the kernel as
the primary boot interface.

## Memory Map Lifetime

The memory map contained in `ORIGIN_BOOT_INFO` is the final map obtained for
the `ExitBootServices()` transition.

The bootloader obtains the map and its `MapKey`, calls
`ExitBootServices()`, and then transfers control to the kernel.

The `MapKey` itself is not part of `ORIGIN_BOOT_INFO` because it is a
bootloader-side value required to terminate UEFI Boot Services.

After successful termination of Boot Services, the bootloader performs no
additional UEFI memory-map operations.

## Framebuffer Lifetime

The framebuffer information is discovered through the UEFI Graphics Output
Protocol before `ExitBootServices()`.

The bootloader copies the relevant framebuffer parameters into
`ORIGIN_BOOT_INFO` before the kernel handoff.

The kernel uses those values after UEFI Boot Services have been terminated.

The current kernel accesses the framebuffer directly through `fb_base` and
uses:

```text
fb_width
fb_height
fb_pixels_per_scanline
```

to determine pixel positions.

## Current Kernel Consumer

The current kernel receives the complete `ORIGIN_BOOT_INFO` structure but
currently consumes only the framebuffer-related fields.

Its current initialization uses the framebuffer to:

1. Clear the display.
2. Render the initial ORIGIN OS text.
3. Enter a halt loop.

The memory-map fields are passed through the boot protocol but are not yet
consumed by `kernel_main`.

## Protocol Scope

The current boot protocol contains:

- Final UEFI memory map information.
- Framebuffer information.
- Kernel entry calling convention.

It currently does not contain:

- A protocol magic value.
- A protocol version.
- Feature flags.
- ACPI information.
- Firmware configuration-table information.
- Memory allocator state.
- CPU topology information.
- Additional platform-specific boot data.

Those items are not part of the current `ORIGIN_BOOT_INFO` contract.

## Contract Stability

`ORIGIN_BOOT_INFO` is an ABI-level structure shared between the bootloader
and kernel.

Changes to the structure must be coordinated between both sides of the
handoff.

In particular, changes to:

- Field order.
- Field type.
- Field width.
- Structure layout.

can change the binary contract between the bootloader and kernel.

The bootloader and kernel must therefore be built against compatible
definitions of `ORIGIN_BOOT_INFO`.

## Handoff Summary

The current boot contract can be summarized as:

```text
Bootloader
    │
    │ prepares ORIGIN_BOOT_INFO
    │
    │ obtains final memory map
    │
    │ ExitBootServices()
    │
    │ sysv_abi call
    │
    ▼
kernel_entry
    │
    │ RDI = ORIGIN_BOOT_INFO *
    │
    ▼
kernel_main
```

This interface is the current boundary between the UEFI bootloader and the
ORIGIN kernel.