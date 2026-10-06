#ifndef EFI_ANNOTATIONS_H
#define EFI_ANNOTATIONS_H

// ============================================================================
// UEFI Annotations
// ============================================================================
//
// These macros document how UEFI function parameters are used.
//
// IN       - Parameter is provided by the caller.
// OUT      - Parameter is written by the function.
// OPTIONAL - Parameter may be NULL.
// CONST    - Data must not be modified through this reference.
//
// EFIAPI   - Calling convention used by UEFI interfaces.
//
// On x86-64, UEFI uses the Microsoft x64 calling convention.
// MinGW x86-64 already uses that ABI, so no additional calling-
// convention attribute is required here.
//
// ============================================================================

#ifndef EFIAPI
#define EFIAPI
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef OPTIONAL
#define OPTIONAL
#endif

#ifndef CONST
#define CONST const
#endif

#endif // EFI_ANNOTATIONS_H