#include "../../include/memory/memory_map_dump.h"
#include "../../include/console/console.h"

static void PrintHex64(
    EFI_SYSTEM_TABLE *SystemTable,
    UINT64 Value
)
{
    CHAR16 Buffer[17];

    for (int i = 15; i >= 0; i--) {
        UINTN Digit = Value & 0xF;

        if (Digit < 10) {
            Buffer[i] = (CHAR16)('0' + Digit);
        } else {
            Buffer[i] = (CHAR16)('A' + (Digit - 10));
        }

        Value >>= 4;
    }

    Buffer[16] = L'\0';

    Print(L"0x", SystemTable);
    Print(Buffer, SystemTable);
}

void DumpMemoryMap(
    EFI_SYSTEM_TABLE *SystemTable,
    EFI_MEMORY_DESCRIPTOR *MemoryMap,
    UINTN MemoryMapSize,
    UINTN DescriptorSize
)
{
    UINTN Offset = 0;

    while (Offset < MemoryMapSize) {

        EFI_MEMORY_DESCRIPTOR *Descriptor =
            (EFI_MEMORY_DESCRIPTOR *)(
                (UINT8 *)MemoryMap + Offset
            );

        Print(L"Type: ", SystemTable);
        PrintHex64(SystemTable, Descriptor->Type);

        Print(L"  Pages: ", SystemTable);
        PrintHex64(SystemTable, Descriptor->NumberOfPages);

        Print(L"  Physical: ", SystemTable);
        PrintHex64(SystemTable, Descriptor->PhysicalStart);

        Print(L"\r\n", SystemTable);

        Offset += DescriptorSize;
    }
}