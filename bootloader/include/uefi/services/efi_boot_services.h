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
 * Placeholder for Boot Service entries whose exact prototypes are not
 * needed yet. They are still required here so that the EFI_BOOT_SERVICES
 * structure matches the UEFI-defined layout.
 */
typedef VOID (EFIAPI *EFI_BOOT_SERVICE_PLACEHOLDER)(VOID);

/*
 * Used to exit boot services and transfer control away from UEFI boot
 * services. The MapKey must come from the most recent GetMemoryMap call.
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

    /*
     * These entries must remain in the structure even though ORIGIN does
     * not use them yet. Removing them shifts every following function
     * pointer and makes later calls target the wrong UEFI service.
     */
    EFI_BOOT_SERVICE_PLACEHOLDER AllocatePool;
    EFI_BOOT_SERVICE_PLACEHOLDER FreePool;
    EFI_BOOT_SERVICE_PLACEHOLDER CreateEvent;
    EFI_BOOT_SERVICE_PLACEHOLDER SetTimer;
    EFI_BOOT_SERVICE_PLACEHOLDER WaitForEvent;
    EFI_BOOT_SERVICE_PLACEHOLDER SignalEvent;
    EFI_BOOT_SERVICE_PLACEHOLDER CloseEvent;
    EFI_BOOT_SERVICE_PLACEHOLDER CheckEvent;

    EFI_BOOT_SERVICE_PLACEHOLDER InstallProtocolInterface;
    EFI_BOOT_SERVICE_PLACEHOLDER ReinstallProtocolInterface;
    EFI_BOOT_SERVICE_PLACEHOLDER UninstallProtocolInterface;
    EFI_BOOT_SERVICE_PLACEHOLDER HandleProtocol;

    VOID *Reserved;

    EFI_BOOT_SERVICE_PLACEHOLDER RegisterProtocolNotify;
    EFI_BOOT_SERVICE_PLACEHOLDER LocateHandle;
    EFI_BOOT_SERVICE_PLACEHOLDER LocateDevicePath;
    EFI_BOOT_SERVICE_PLACEHOLDER InstallConfigurationTable;

    EFI_BOOT_SERVICE_PLACEHOLDER LoadImage;
    EFI_BOOT_SERVICE_PLACEHOLDER StartImage;
    EFI_BOOT_SERVICE_PLACEHOLDER Exit;
    EFI_BOOT_SERVICE_PLACEHOLDER UnloadImage;

    EFI_EXIT_BOOT_SERVICES ExitBootServices;

} EFI_BOOT_SERVICES;

#endif
