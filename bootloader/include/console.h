#ifndef CONSOLE_H
#define CONSOLE_H

#include "uefi/efi_types.h"

typedef struct _EFI_SYSTEM_TABLE EFI_SYSTEM_TABLE;

void Print(CHAR16 *_String, EFI_SYSTEM_TABLE *SystemTable);

#endif