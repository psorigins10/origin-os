#ifndef SIMPLE_TEXT_OUTPUT_PROTOCOL_H
#define SIMPLE_TEXT_OUTPUT_PROTOCOL_H


#include "../efi_types.h"


#ifndef EFIAPI
#define EFIAPI
#endif

typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

// Function used by UEFI's text output protocol to reset the device.
typedef EFI_STATUS (EFIAPI *EFI_TEXT_RESET) (
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    BOOLEAN ExtendedVerification
);


// Function used by UEFI's text output protocol to print a CHAR16 string.
typedef EFI_STATUS (EFIAPI *EFI_TEXT_STRING) (
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    CHAR16 *String
);


// Function used by UEFI to test whether a string can be displayed.
typedef EFI_STATUS (EFIAPI *EFI_TEXT_TEST_STRING) (
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    CHAR16 *String
);


typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {

    EFI_TEXT_RESET Reset;
    EFI_TEXT_STRING OutputString;
    EFI_TEXT_TEST_STRING TextString;

} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

#endif