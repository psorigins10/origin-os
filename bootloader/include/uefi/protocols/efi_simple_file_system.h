#ifndef EFI_SIMPLE_FILE_SYSTEM_H
#define EFI_SIMPLE_FILE_SYSTEM_H

#include "../efi_types.h"
#include "../efi_file.h"


/*
 * Forward declaration of the UEFI Simple File System Protocol.
 *
 * The protocol provides access to the root directory of a filesystem
 * exposed by the firmware.
 */
typedef struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;


/*
 * Opens the root volume of the filesystem.
 *
 * Parameters:
 *   This  - Simple File System Protocol instance.
 *   Root  - Receives the EFI_FILE_PROTOCOL handle for the root directory.
 */
typedef EFI_STATUS (*EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME)(
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *This,
    EFI_FILE_PROTOCOL **Root
);


/*
 * UEFI Simple File System Protocol.
 *
 * This protocol provides access to a filesystem through its root
 * directory. File and directory operations are performed through
 * the EFI_FILE_PROTOCOL returned by OpenVolume().
 *
 * The member order is part of the UEFI protocol ABI and must not
 * be changed.
 */
struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL {

    /* Revision of the Simple File System Protocol interface. */
    UINT64 Revision;

    /*
     * Opens the filesystem's root directory.
     *
     * The returned EFI_FILE_PROTOCOL is used for subsequent file
     * and directory operations.
     */
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME OpenVolume;
};

#endif