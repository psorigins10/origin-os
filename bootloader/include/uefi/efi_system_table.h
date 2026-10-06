#ifndef EFI_SYSTEM_TABLE_H
#define EFI_SYSTEM_TABLE_H


// ------------------------------------------
// -------------------------------------------
// Included Files
#include "efi_types.h"

#include "protocols/efi_simple_text_output.h"
#include "protocols/efi_simple_text_input.h"

#include "services/efi_runtime_services.h"
#include "services/efi_boot_services.h"

#include "tables/efi_configuration_table.h"
// -------------------------------------------
// ------------------------------------------



// Main table provided by UEFI firmware to our application.
// Contains firmware information and pointers to UEFI services/protocols.
typedef struct _EFI_SYSTEM_TABLE {
    
    EFI_TABLE_HEADER Hdr;
    CHAR16 *FirmwareVendor;
    UINT32 FirmwareRevision;
    EFI_HANDLE ConsoleInHandle;
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL *ConIn;
    EFI_HANDLE ConsoleOutHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
    EFI_HANDLE StandardErrorHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *StdErr;
    EFI_RUNTIME_SERVICES *RuntimeServices;
    EFI_BOOT_SERVICES *BootServices;
    UINTN NumberOfTableEntries;
    EFI_CONFIGURATION_TABLE *ConfigurationTable;

} EFI_SYSTEM_TABLE;


#endif