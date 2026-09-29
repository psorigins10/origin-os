# UEFI Memory Map

The UEFI memory map describes the physical memory layout provided by the firmware to the bootloader.

## Purpose

ORIGIN uses the UEFI memory map to understand which physical memory regions are available, reserved, or used by firmware and other boot-time components.

## EFI_MEMORY_DESCRIPTOR

Each memory region is described by an `EFI_MEMORY_DESCRIPTOR`:

```c
typedef struct {
    UINT32 Type;
    EFI_PHYSICAL_ADDRESS PhysicalStart;
    EFI_VIRTUAL_ADDRESS VirtualStart;
    UINT64 NumberOfPages;
    UINT64 Attribute;
} EFI_MEMORY_DESCRIPTOR;
```

Important fields:

- `Type` — describes how the memory region is being used.
- `PhysicalStart` — physical address where the region begins.
- `VirtualStart` — virtual address associated with the region.
- `NumberOfPages` — size of the region in 4 KiB pages.
- `Attribute` — memory attributes defined by UEFI.

## GetMemoryMap()

The memory map is obtained through the UEFI Boot Services `GetMemoryMap()` function.

ORIGIN follows the standard two-call pattern.

### First Call

The first call is made without a destination buffer:

```text
GetMemoryMap()
    │
    └── EFI_BUFFER_TOO_SMALL
            │
            └── Required buffer size returned
```

The returned values provide the information needed to allocate a buffer:

- Required memory map size
- Descriptor size
- Descriptor version

### Buffer Allocation

ORIGIN allocates enough pages to hold the memory map.

Additional space is reserved because allocating the buffer itself can modify the UEFI memory map.

### Second Call

After allocating the buffer, `GetMemoryMap()` is called again:

```text
GetMemoryMap()
    │
    └── Memory map populated into allocated buffer
```

The resulting `MapKey` is retained for later use with `ExitBootServices()`.

## Walking the Memory Map

UEFI provides `DescriptorSize` because the size of a memory descriptor is determined by the firmware.

ORIGIN therefore walks the map using the supplied descriptor size rather than assuming that every entry is tightly packed.

```text
MemoryMap
    │
    ├── Descriptor
    ├── Descriptor
    ├── Descriptor
    └── ...
```

Each descriptor is accessed by advancing through the memory map by `DescriptorSize`.

## Debugging

During development, ORIGIN dumps selected descriptor fields to the UEFI console:

```text
Type: 0x0000000000000007  Pages: 0x0000000000000010  Physical: 0x0000000000100000
```

The initial implementation produced invalid-looking repeated values.

The problem was that the allocated buffer had not yet been populated with a second `GetMemoryMap()` call.

After adding the second call, the descriptor values became varied and consistent with a real firmware memory map.

## Current Implementation

ORIGIN currently:

1. Requests the required memory map size.
2. Reads the descriptor size and version.
3. Allocates pages for the memory map.
4. Calls `GetMemoryMap()` again.
5. Stores the resulting memory map and `MapKey`.
6. Walks the descriptors.
7. Dumps descriptor information to the console.

## Important Constraint

The UEFI memory map can change while the bootloader is running.

Operations such as memory allocation can modify the map and therefore invalidate the previously returned `MapKey`.

Before calling `ExitBootServices()`, ORIGIN must obtain the final memory map and use its corresponding `MapKey`.

The final memory map acquisition must therefore be performed immediately before leaving UEFI Boot Services.

## Next Step

The next milestone is:

```text
Memory map validation
        ↓
Correct EFI_BOOT_SERVICES layout
        ↓
Final GetMemoryMap()
        ↓
ExitBootServices()
        ↓
Kernel handoff
```