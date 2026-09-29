#include "../../include/memory/memory_map.h"
#include "../../include/uefi/efi_system_table.h"
#include "../../include/uefi/efi_types.h"


EFI_STATUS GetMemoryMap(
    EFI_SYSTEM_TABLE *SystemTable,
    EFI_MEMORY_DESCRIPTOR **MemoryMap,
    UINTN *MemoryMapSize,
    UINTN *MapKey,
    UINTN *DescriptorSize,
    UINT32 *DescriptorVersion
)
{
    EFI_STATUS Status;

    *MemoryMapSize = 0;

    Status = SystemTable->BootServices->GetMemoryMap(
        MemoryMapSize,
        NULL,
        MapKey,
        DescriptorSize,
        DescriptorVersion
    );

    if (Status != EFI_BUFFER_TOO_SMALL) {
        return Status;
    }

    *MemoryMapSize += 2 * *DescriptorSize;

    UINTN Pages = (*MemoryMapSize + 0xFFF) / 0x1000;

    EFI_PHYSICAL_ADDRESS BufferAddress = 0;

    Status = SystemTable->BootServices->AllocatePages(
        AllocateAnyPages,
        EfiLoaderData,
        Pages,
        &BufferAddress
    );

    if (Status != EFI_SUCCESS) {
        return Status;
    }

    *MemoryMap = (EFI_MEMORY_DESCRIPTOR *)(UINTN)BufferAddress;

    Status = SystemTable->BootServices->GetMemoryMap(
        MemoryMapSize,
        *MemoryMap,
        MapKey,
        DescriptorSize,
        DescriptorVersion
    );

    if (Status != EFI_SUCCESS) {
        return Status;
    }

    return EFI_SUCCESS;
}