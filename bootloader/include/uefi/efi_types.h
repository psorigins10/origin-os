#ifndef EFI_TYPES_H
#define EFI_TYPES_H

typedef unsigned long long UINT64;
typedef unsigned int UINT32;
typedef unsigned short CHAR16;
typedef void VOID;

// Unsigned integer whose size matches the native CPU pointer size.
// On our x86-64 UEFI target, UINTN is 64 bits.
typedef unsigned long long UINTN;

// Boolean value used by UEFI interfaces.
typedef unsigned char BOOLEAN;

// Opaque identifier used by UEFI to refer to an object managed by firmware.
typedef VOID *EFI_HANDLE;

// Task Priority Level used by UEFI to control execution priority.
typedef UINTN EFI_TPL;

// Status code returned by UEFI functions.
// On our x86-64 target, this is a 64-bit unsigned value.
typedef unsigned long long EFI_STATUS;

#endif