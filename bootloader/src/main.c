#include "../include/console/console.h"
#include "../include/uefi/efi_system_table.h"
#include "../include/memory/memory_map.h"
#include "../include/memory/memory_map_dump.h"

EFI_STATUS EFIAPI efi_main(
    EFI_HANDLE ImageHandle,
    EFI_SYSTEM_TABLE *SystemTable
)
{
    (void)ImageHandle;

    Print(L"Welcome To ORIGIN Boot\r\n", SystemTable);

    EFI_MEMORY_DESCRIPTOR *MemoryMap;
    UINTN MemoryMapSize;
    UINTN MapKey;
    UINTN DescriptorSize;
    UINT32 DescriptorVersion;

    EFI_STATUS Status = GetMemoryMap(
        SystemTable,
        &MemoryMap,
        &MemoryMapSize,
        &MapKey,
        &DescriptorSize,
        &DescriptorVersion
    );

    if (Status != EFI_SUCCESS) {
    Print(L"GetMemoryMap failed\r\n", SystemTable);

    while (1) {
    }
}

    Print(L"Memory map acquired\r\n", SystemTable);

    DumpMemoryMap(
        SystemTable,
        MemoryMap,
        MemoryMapSize,
        DescriptorSize
    );

    Print(L"\r\nMemory map dump complete\r\n", SystemTable);

    while (1) {
    }
}