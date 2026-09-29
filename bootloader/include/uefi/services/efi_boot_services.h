#ifndef EFI_BOOT_SERVICES_H
#define EFI_BOOT_SERVICES_H

// Calling convention used by UEFI function interfaces.
#ifndef EFIAPI
#define EFIAPI
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#include "../efi_table_header.h"
#include "../efi_memory.h"
#include "../efi_types.h"


// Boot Service function that raises the current Task Priority Level.
typedef EFI_TPL (EFIAPI *EFI_RAISE_TPL) (IN EFI_TPL NewTpl);


typedef EFI_STATUS (EFIAPI *EFI_GET_MEMORY_MAP)(
    IN OUT UINTN *MemoryMapSize,
    OUT EFI_MEMORY_DESCRIPTOR *MemoryMap,
    OUT UINTN *MapKey,
    OUT UINTN *DescriptorSize,
    OUT UINT32 *DescriptorVersion
);


typedef EFI_STATUS (EFIAPI *EFI_ALLOCATE_PAGES) (
    IN EFI_ALLOCATE_TYPE Type,
    IN EFI_MEMORY_TYPE MemoryType,
    IN UINTN Pages,
    IN OUT EFI_PHYSICAL_ADDRESS *Memory
);


typedef EFI_STATUS (EFIAPI *EFI_FREE_PAGES) (
    IN EFI_PHYSICAL_ADDRESS Memory,
    IN UINTN Pages
);


typedef VOID (EFIAPI *EFI_RESTORE_TPL)(
    IN EFI_TPL OldTpl
);


/*
It is used to exit boot services,
take control from and give control to the kernel
*/
typedef EFI_STATUS (EFIAPI *EFI_EXIT_BOOT_SERVICES)(
    IN EFI_HANDLE ImageHandle,
    IN UINTN MapKey
);


typedef struct _EFI_BOOT_SERVICES {
    
    EFI_TABLE_HEADER Hdr;
    EFI_RAISE_TPL RaiseTPL;
    EFI_RESTORE_TPL RestoreTPL;
    EFI_ALLOCATE_PAGES AllocatePages;
    EFI_FREE_PAGES FreePages;
    EFI_GET_MEMORY_MAP GetMemoryMap;
    EFI_EXIT_BOOT_SERVICES ExitBootServices;

} EFI_BOOT_SERVICES;

#endif