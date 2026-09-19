#include <efi.h>
#include <efilib.h>
#include "elf.h"
#include "file.h"

typedef void (*KernelEntry)(void);

EFI_STATUS EFIAPI efi_main(
    EFI_HANDLE ImageHandle,
    EFI_SYSTEM_TABLE *SystemTable
)
{
    InitializeLib(ImageHandle, SystemTable);

    void *kernel_buffer;
    UINTN kernel_size;
    UINT64 kernel_entry;

    EFI_STATUS status;

    Print(L"ORIGIN bootloader starting...\n");

    status = load_kernel(
        ImageHandle,
        &kernel_buffer,
        &kernel_size
    );

    if (EFI_ERROR(status)) {
        Print(L"Failed to load kernel. Status: %r\n", status);
        return status;
    }

    Print(L"Kernel loaded: %lu bytes\n", kernel_size);

    if (elf_load(kernel_buffer, &kernel_entry) != 0) {
        Print(L"Invalid kernel ELF\n");
        return EFI_LOAD_ERROR;
    }

    Print(L"Starting ORIGIN OS kernel...\n");

    KernelEntry entry = (KernelEntry)kernel_entry;

    entry();

    return EFI_SUCCESS;
}