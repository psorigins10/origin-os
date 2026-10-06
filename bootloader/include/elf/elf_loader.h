#ifndef ELF_LOADER_H
#define ELF_LOADER_H

#include "../uefi/efi_system_table.h"
#include "elf64.h"

EFI_STATUS LoadKernel(
    EFI_SYSTEM_TABLE *SystemTable,
    VOID *KernelFile,
    UINTN KernelFileSize,
    UINT64 *EntryPoint
);

#endif