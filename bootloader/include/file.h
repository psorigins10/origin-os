#ifndef ORIGIN_FILE_H
#define ORIGIN_FILE_H

#include <efi.h>

EFI_STATUS load_kernel(
    EFI_HANDLE ImageHandle,
    void **buffer,
    UINTN *size
);

#endif