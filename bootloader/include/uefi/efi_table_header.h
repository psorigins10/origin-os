#ifndef EFI_TABLE_HEADER_H
#define EFI_TABLE_HEADER_H

#include "efi_types.h"

// Common header present at the beginning of UEFI service tables.
// Used by firmware to identify, version, size, and validate a table.
typedef struct {

    UINT64 Signature;
    UINT32 Revision;
    UINT32 HeaderSize;
    UINT32 CRC32;
    UINT32 Reserved;

} EFI_TABLE_HEADER;

#endif