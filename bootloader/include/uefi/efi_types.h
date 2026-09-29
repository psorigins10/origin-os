#ifndef EFI_TYPES_H
#define EFI_TYPES_H

#ifndef NULL
#define NULL ((VOID *)0)
#endif

typedef unsigned long long UINT64;
typedef unsigned int UINT32;
typedef unsigned short CHAR16;
typedef void VOID;
typedef unsigned char UINT8;

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

// Memory maps used by UEFI Interface
typedef UINT64 EFI_PHYSICAL_ADDRESS;
typedef UINT64 EFI_VIRTUAL_ADDRESS;


#define EFI_BUFFER_TOO_SMALL 0x8000000000000005ULL
#define EFI_SUCCESS 0

#endif