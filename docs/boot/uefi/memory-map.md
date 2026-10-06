# UEFI Memory Map

The UEFI memory map describes the physical memory regions reported by
firmware during the UEFI boot phase.

ORIGIN uses the memory map for boot-time memory information and passes the
final map to the kernel as part of `ORIGIN_BOOT_INFO`.

## EFI_MEMORY_DESCRIPTOR

Each memory-map entry is represented by an `EFI_MEMORY_DESCRIPTOR`:

```c
typedef struct {
    UINT32 Type;
    EFI_PHYSICAL_ADDRESS PhysicalStart;
    EFI_VIRTUAL_ADDRESS VirtualStart;
    UINT64 NumberOfPages;
    UINT64 Attribute;
} EFI_MEMORY_DESCRIPTOR;
```

The descriptor contains:

- Memory type.
- Physical start address.
- Virtual start address.
- Number of 4 KiB pages.
- Memory attributes.

The descriptor size returned by UEFI must be used when walking the memory
map. The bootloader does not assume that descriptors are tightly packed
according to `sizeof(EFI_MEMORY_DESCRIPTOR)`.

## GetMemoryMap()

`GetMemoryMap()` provides the current UEFI memory map and its associated
`MapKey`.

The call follows the UEFI two-stage pattern:

```text
GetMemoryMap()
      │
      ├── buffer too small
      │
      ▼
required map size
descriptor size
descriptor version
MapKey
```

The bootloader then provides a buffer large enough for the memory map and
calls `GetMemoryMap()` again to populate it.

The returned values include:

- Memory map size.
- Descriptor size.
- Descriptor version.
- Map key.

The map key identifies the memory-map state used for a subsequent
`ExitBootServices()` call.

## Initial Memory Map

The bootloader obtains an initial memory map during boot.

This map is used for inspection and debugging, including walking and
displaying the memory descriptors.

The initial map is not the map used for the final kernel handoff.

UEFI memory-map state can change while Boot Services remain active. Operations
such as memory allocation can modify the map and therefore invalidate a
previously obtained `MapKey`.

## Final Memory Map

Immediately before terminating UEFI Boot Services, the bootloader obtains a
fresh memory map.

The final map uses a separately allocated buffer with additional capacity so
that the final `GetMemoryMap()` has room for changes caused by the allocation
of the buffer itself and other Boot Services activity.

The implementation distinguishes between:

- `FinalMapCapacity` — size of the allocated buffer.
- `FinalMapSize` — actual size of the memory map returned by
  `GetMemoryMap()`.

Before each final-map query, `FinalMapSize` is reset to `FinalMapCapacity`.
UEFI then updates it with the actual map size.

## Final Map and ExitBootServices()

The final memory-map transition is:

```text
Allocate final map buffer
          │
          ▼
   GetMemoryMap()
          │
          │ returns MapKey
          ▼
 ExitBootServices()
          │
          ├── success ──────────────► Kernel handoff
          │
          └── EFI_INVALID_PARAMETER
                         │
                         ▼
                  Get fresh map
                         │
                         └──────────► retry
```

The `MapKey` passed to `ExitBootServices()` must correspond to the memory
map returned by the immediately preceding `GetMemoryMap()` call.

The bootloader therefore performs no additional Boot Services operation
between the final `GetMemoryMap()` and `ExitBootServices()`.

If `ExitBootServices()` returns `EFI_INVALID_PARAMETER`, the bootloader
obtains a fresh memory map and retries with the new `MapKey`.

Other failure statuses are treated as fatal during the current boot flow.

## Memory Map Passed to the Kernel

After `ExitBootServices()` succeeds, the final memory map is stored in
`ORIGIN_BOOT_INFO`:

```c
BootInfo->memory_map =
    (uint64_t)(UINTN)FinalMap;

BootInfo->memory_map_size =
    (uint64_t)FinalMapSize;

BootInfo->memory_descriptor_size =
    (uint64_t)FinalDescriptorSize;
```

The kernel therefore receives:

- Address of the final memory map.
- Actual size of the final memory map.
- Size of each memory descriptor.

The kernel does not receive the UEFI `MapKey`. The key is only required by
the bootloader to complete the `ExitBootServices()` transition.

## Memory Map Lifetime

After successful `ExitBootServices()`:

- UEFI Boot Services are no longer available.
- The bootloader performs no further memory-map queries.
- The bootloader does not allocate or free Boot Services memory.
- The final memory map is part of the boot information passed to the kernel.

The kernel is responsible for interpreting the memory map after the firmware
boot environment has been terminated.

## Descriptor Walking

When iterating through the memory map, the bootloader advances by the
descriptor size returned by UEFI rather than assuming a fixed structure
stride.

Conceptually:

```text
memory map base
      │
      ▼
descriptor 0
      │
      ├── DescriptorSize
      ▼
descriptor 1
      │
      ├── DescriptorSize
      ▼
descriptor 2
      │
      └── ...
```

This preserves compatibility with the descriptor size reported by the UEFI
implementation.

## Bootloader Boundary

The memory-map handling has two distinct purposes:

```text
Initial memory map
    │
    ├── inspection
    └── debugging

Final memory map
    │
    ├── obtain fresh MapKey
    ├── ExitBootServices()
    └── pass to kernel
```

Only the final memory map represents the memory state used for the
bootloader-to-kernel transition.