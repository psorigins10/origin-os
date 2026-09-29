#include "../../include/console/console.h"
#include "../../include/uefi/efi_system_table.h"

void Print(CHAR16 *_String, EFI_SYSTEM_TABLE *SystemTable) {
    SystemTable->ConOut->OutputString(
        SystemTable->ConOut,
        _String
    );
}