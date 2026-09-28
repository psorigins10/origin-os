#ifndef EFI_BOOT_SERVICES_H
#define EFI_BOOT_SERVICES_H

// Calling convention used by UEFI function interfaces.
#ifndef EFIAPI
#define EFIAPI
#endif

#ifndef IN
#define IN
#endif

#include "../efi_table_header.h"
#include "../efi_types.h"


// Boot Service function that raises the current Task Priority Level.
typedef EFI_TPL (EFIAPI *EFI_RAISE_TPL) (IN EFI_TPL NewTpl);


typedef struct _EFI_BOOT_SERVICES {
    EFI_TABLE_HEADER Hdr;
    EFI_RAISE_TPL RaiseTPL;
} EFI_BOOT_SERVICES;

#endif