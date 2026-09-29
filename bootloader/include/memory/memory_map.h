#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

#include "../uefi/efi_types.h"
#include "../uefi/efi_memory.h"

typedef struct _EFI_SYSTEM_TABLE EFI_SYSTEM_TABLE;

EFI_STATUS GetMemoryMap(
    EFI_SYSTEM_TABLE *SystemTable,
    EFI_MEMORY_DESCRIPTOR **MemoryMap,
    UINTN *MemoryMapSize,
    UINTN *MapKey,
    UINTN *DescriptorSize,
    UINT32 *DescriptorVersion
);

#endif