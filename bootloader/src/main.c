#include "../include/console.h"
#include "../include/uefi/efi_system_table.h"

EFI_STATUS EFIAPI efi_main(
    EFI_HANDLE ImageHandle,
    EFI_SYSTEM_TABLE *SystemTable
)
{
    (void)ImageHandle;

    Print(L"Welcome To ORIGIN Boot", SystemTable);

    while (1) {
    }
}