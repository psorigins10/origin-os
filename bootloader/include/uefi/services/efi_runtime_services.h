#ifndef EFI_RUNTIME_SERVICES_H
#define EFI_RUNTIME_SERVICES_H

// ============================================================================
// Includes
// ============================================================================

#include "../efi_table_header.h"
#include "../efi_types.h"
#include "../efi_annotations.h"
#include "../efi_memory.h"


// ============================================================================
// Time Services
// ============================================================================

// Retrieves the current system time from the UEFI real-time clock.
//
// Time         - Receives the current date and time.
// Capabilities - Optionally receives information about the clock's
//                supported resolution and accuracy.
typedef
EFI_STATUS
(EFIAPI *EFI_GET_TIME)(
    OUT EFI_TIME              *Time,
    OUT EFI_TIME_CAPABILITIES *Capabilities OPTIONAL
);


// Sets the current system time in the UEFI real-time clock.
typedef
EFI_STATUS
(EFIAPI *EFI_SET_TIME)(
    IN EFI_TIME *Time
);


// Retrieves the current wake-up timer configuration.
//
// Enabled - Indicates whether the wake-up timer is enabled.
// Pending - Indicates whether a wake-up event is pending.
// Time    - Receives the scheduled wake-up time.
typedef
EFI_STATUS
(EFIAPI *EFI_GET_WAKEUP_TIME)(
    OUT BOOLEAN  *Enabled,
    OUT BOOLEAN  *Pending,
    OUT EFI_TIME *Time
);


// Enables or disables the wake-up timer.
//
// If Enable is TRUE, Time specifies when the system should wake up.
// Time may be NULL when disabling the wake-up timer.
typedef
EFI_STATUS
(EFIAPI *EFI_SET_WAKEUP_TIME)(
    IN BOOLEAN  Enable,
    IN EFI_TIME *Time OPTIONAL
);


// ============================================================================
// Virtual Memory Services
// ============================================================================

// Supplies UEFI Runtime Services with the virtual memory map that the
// operating system will use after switching to virtual addressing.
//
// MemoryMapSize    - Size of the memory map in bytes.
// DescriptorSize   - Size of each memory descriptor.
// DescriptorVersion - Version of the memory descriptor format.
// VirtualMap       - Array containing the virtual memory map.
typedef
EFI_STATUS
(EFIAPI *EFI_SET_VIRTUAL_ADDRESS_MAP)(
    IN UINTN                 MemoryMapSize,
    IN UINTN                 DescriptorSize,
    IN UINT32                DescriptorVersion,
    IN EFI_MEMORY_DESCRIPTOR *VirtualMap
);


// Converts a firmware pointer from its physical address to its
// corresponding virtual address after virtual memory is enabled.
typedef
EFI_STATUS
(EFIAPI *EFI_CONVERT_POINTER)(
    IN UINTN DebugDisposition,
    IN VOID  **Address
);


// ============================================================================
// Variable Services
// ============================================================================

// Retrieves the value and attributes of a UEFI variable.
//
// VariableName - Name of the variable.
// VendorGuid   - GUID identifying the variable's vendor.
// Attributes   - Optionally receives the variable attributes.
// DataSize     - Size of the Data buffer; receives the required/returned size.
// Data         - Optionally receives the variable's stored data.
typedef
EFI_STATUS
(EFIAPI *EFI_GET_VARIABLE)(
    IN CHAR16   *VariableName,
    IN EFI_GUID *VendorGuid,
    OUT UINT32  *Attributes OPTIONAL,
    IN OUT UINTN *DataSize,
    OUT VOID    *Data OPTIONAL
);


// Retrieves the next variable name when enumerating UEFI variables.
//
// VariableNameSize - Size of the VariableName buffer.
// VariableName     - Receives the next variable name.
// VendorGuid       - Receives the GUID associated with that variable.
typedef
EFI_STATUS
(EFIAPI *EFI_GET_NEXT_VARIABLE_NAME)(
    IN OUT UINTN   *VariableNameSize,
    IN OUT CHAR16  *VariableName,
    IN OUT EFI_GUID *VendorGuid
);


// Creates, modifies, or deletes a UEFI variable.
//
// Data contains the value to store. A variable can be deleted by using
// the appropriate attributes and a DataSize of zero.
typedef
EFI_STATUS
(EFIAPI *EFI_SET_VARIABLE)(
    IN CHAR16 *VariableName,
    IN EFI_GUID *VendorGuid,
    IN UINT32 Attributes,
    IN UINTN DataSize,
    IN VOID *Data
);


// Returns information about the storage available for UEFI variables.
//
// Reports the maximum storage capacity, remaining storage, and maximum
// size of a single variable for the requested attributes.
typedef
EFI_STATUS
(EFIAPI *EFI_QUERY_VARIABLE_INFO)(
    IN UINT32 Attributes,
    OUT UINT64 *MaximumVariableStorageSize,
    OUT UINT64 *RemainingVariableStorageSize,
    OUT UINT64 *MaximumVariableSize
);


// ============================================================================
// Miscellaneous Services
// ============================================================================

// Retrieves the next high-order portion of the monotonic counter.
//
// Used together with the monotonic counter mechanism when the lower
// portion of the counter rolls over.
typedef
EFI_STATUS
(EFIAPI *EFI_GET_NEXT_HIGH_MONO_COUNT)(
    OUT UINT32 *HighCount
);


// Resets the system.
//
// This function does not return to its caller.
typedef
VOID
(EFIAPI *EFI_RESET_SYSTEM)(
    IN EFI_RESET_TYPE ResetType,
    IN EFI_STATUS     ResetStatus,
    IN UINTN          DataSize,
    IN VOID           *ResetData OPTIONAL
);


// ============================================================================
// Capsule Services
// ============================================================================

// Passes one or more UEFI capsules to firmware for processing.
//
// CapsuleHeaderArray - Array of pointers to capsule headers.
// CapsuleCount       - Number of capsules in the array.
// ScatterGatherList  - Optional physical address of a scatter/gather list.
typedef
EFI_STATUS
(EFIAPI *EFI_UPDATE_CAPSULE)(
    IN EFI_CAPSULE_HEADER   **CapsuleHeaderArray,
    IN UINTN                CapsuleCount,
    IN EFI_PHYSICAL_ADDRESS ScatterGatherList OPTIONAL
);


// Determines whether firmware supports the supplied capsules.
//
// Returns the maximum capsule size supported by the firmware and the
// type of reset required to complete the capsule update.
typedef
EFI_STATUS
(EFIAPI *EFI_QUERY_CAPSULE_CAPABILITIES)(
    IN EFI_CAPSULE_HEADER **CapsuleHeaderArray,
    IN UINTN              CapsuleCount,
    OUT UINT64             *MaximumCapsuleSize,
    OUT EFI_RESET_TYPE     *ResetType
);




// ============================================================================
// EFI Runtime Services Table
// ============================================================================
//
// This structure contains function pointers supplied by UEFI firmware.
// The ordering of these fields is defined by the UEFI specification and
// MUST NOT be changed.
//
// Runtime Services remain available after ExitBootServices(), although
// some services require the operating system to establish virtual
// addressing first.
// ============================================================================
typedef struct _EFI_RUNTIME_SERVICES
{
    EFI_TABLE_HEADER Hdr;

    // ------------------------------------------------------------------------
    // Time Services
    // ------------------------------------------------------------------------

    EFI_GET_TIME         GetTime;
    EFI_SET_TIME         SetTime;
    EFI_GET_WAKEUP_TIME  GetWakeupTime;
    EFI_SET_WAKEUP_TIME  SetWakeupTime;


    // ------------------------------------------------------------------------
    // Virtual Memory Services
    // ------------------------------------------------------------------------

    EFI_SET_VIRTUAL_ADDRESS_MAP  SetVirtualAddressMap;
    EFI_CONVERT_POINTER          ConvertPointer;


    // ------------------------------------------------------------------------
    // Variable Services
    // ------------------------------------------------------------------------

    EFI_GET_VARIABLE           GetVariable;
    EFI_GET_NEXT_VARIABLE_NAME GetNextVariableName;
    EFI_SET_VARIABLE           SetVariable;


    // ------------------------------------------------------------------------
    // Miscellaneous Services
    // ------------------------------------------------------------------------

    EFI_GET_NEXT_HIGH_MONO_COUNT GetNextHighMonotonicCount;
    EFI_RESET_SYSTEM             ResetSystem;


    // ------------------------------------------------------------------------
    // Capsule Services
    // ------------------------------------------------------------------------

    EFI_UPDATE_CAPSULE             UpdateCapsule;
    EFI_QUERY_CAPSULE_CAPABILITIES QueryCapsuleCapabilities;


    // ------------------------------------------------------------------------
    // Variable Information Services
    // ------------------------------------------------------------------------

    EFI_QUERY_VARIABLE_INFO QueryVariableInfo;

} EFI_RUNTIME_SERVICES;


#endif // EFI_RUNTIME_SERVICES