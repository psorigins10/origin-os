#ifndef EFI_BOOT_SERVICES_H
#define EFI_BOOT_SERVICES_H

// ============================================================================
// Includes
// ============================================================================

#include "../efi_table_header.h"
#include "../efi_memory.h"
#include "../efi_types.h"
#include "../efi_annotations.h"


// ============================================================================
// Event Types
// ============================================================================
//
// Event type flags may be combined with bitwise OR.
// ============================================================================

#define EVT_TIMER                           0x80000000
#define EVT_RUNTIME                         0x40000000

#define EVT_NOTIFY_WAIT                     0x00000100
#define EVT_NOTIFY_SIGNAL                   0x00000200

#define EVT_SIGNAL_EXIT_BOOT_SERVICES       0x00000201
#define EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE   0x60000202


// ============================================================================
// Task Priority Services
// ============================================================================

// Raises the current Task Priority Level.
typedef EFI_TPL (EFIAPI *EFI_RAISE_TPL)(
    IN EFI_TPL NewTpl
);


// Restores the previous Task Priority Level.
typedef VOID (EFIAPI *EFI_RESTORE_TPL)(
    IN EFI_TPL OldTpl
);


// ============================================================================
// Memory Services
// ============================================================================

// Retrieves the current UEFI memory map.
typedef EFI_STATUS (EFIAPI *EFI_GET_MEMORY_MAP)(
    IN OUT UINTN                  *MemoryMapSize,
    OUT EFI_MEMORY_DESCRIPTOR     *MemoryMap,
    OUT UINTN                     *MapKey,
    OUT UINTN                     *DescriptorSize,
    OUT UINT32                    *DescriptorVersion
);


// Allocates physical memory pages.
typedef EFI_STATUS (EFIAPI *EFI_ALLOCATE_PAGES)(
    IN EFI_ALLOCATE_TYPE          Type,
    IN EFI_MEMORY_TYPE            MemoryType,
    IN UINTN                      Pages,
    IN OUT EFI_PHYSICAL_ADDRESS   *Memory
);


// Frees previously allocated physical memory pages.
typedef EFI_STATUS (EFIAPI *EFI_FREE_PAGES)(
    IN EFI_PHYSICAL_ADDRESS       Memory,
    IN UINTN                      Pages
);


// Allocates a pool of memory.
typedef EFI_STATUS (EFIAPI *EFI_ALLOCATE_POOL)(
    IN EFI_MEMORY_TYPE            PoolType,
    IN UINTN                      Size,
    OUT VOID                      **Buffer
);


// Frees a previously allocated memory pool.
typedef EFI_STATUS (EFIAPI *EFI_FREE_POOL)(
    IN VOID                       *Buffer
);


// ============================================================================
// Event & Timer Services
// ============================================================================

// Notification callback used by UEFI events.
typedef VOID (EFIAPI *EFI_EVENT_NOTIFY)(
    IN EFI_EVENT Event,
    IN VOID      *Context
);


// Creates a UEFI event.
typedef EFI_STATUS (EFIAPI *EFI_CREATE_EVENT)(
    IN UINT32             Type,
    IN EFI_TPL            NotifyTpl,
    IN EFI_EVENT_NOTIFY   NotifyFunction OPTIONAL,
    IN VOID               *NotifyContext OPTIONAL,
    OUT EFI_EVENT         *Event
);


// Sets the timer associated with an event.
typedef EFI_STATUS (EFIAPI *EFI_SET_TIMER)(
    IN EFI_EVENT        Event,
    IN EFI_TIMER_DELAY  Type,
    IN UINT64           TriggerTime
);


// Waits for one or more events to be signaled.
typedef EFI_STATUS (EFIAPI *EFI_WAIT_FOR_EVENT)(
    IN UINTN             NumberOfEvents,
    IN EFI_EVENT         *Event,
    OUT UINTN            *Index
);


// Signals an event.
typedef EFI_STATUS (EFIAPI *EFI_SIGNAL_EVENT)(
    IN EFI_EVENT Event
);


// Closes an event.
typedef EFI_STATUS (EFIAPI *EFI_CLOSE_EVENT)(
    IN EFI_EVENT Event
);


// Checks whether an event is ready.
typedef EFI_STATUS (EFIAPI *EFI_CHECK_EVENT)(
    IN EFI_EVENT Event
);


// ============================================================================
// Protocol Handler Services
// ============================================================================

// Installs a protocol interface onto a handle.
typedef EFI_STATUS (EFIAPI *EFI_INSTALL_PROTOCOL_INTERFACE)(
    IN OUT EFI_HANDLE        *Handle,
    IN EFI_GUID              *Protocol,
    IN EFI_INTERFACE_TYPE    InterfaceType,
    IN VOID                  *Interface
);


// Replaces an existing protocol interface.
typedef EFI_STATUS (EFIAPI *EFI_REINSTALL_PROTOCOL_INTERFACE)(
    IN EFI_HANDLE    Handle,
    IN EFI_GUID      *Protocol,
    IN VOID          *OldInterface,
    IN VOID          *NewInterface
);


// Removes a protocol interface from a handle.
typedef EFI_STATUS (EFIAPI *EFI_UNINSTALL_PROTOCOL_INTERFACE)(
    IN EFI_HANDLE    Handle,
    IN EFI_GUID      *Protocol,
    IN VOID          *Interface
);


// Retrieves a protocol interface from a handle.
typedef EFI_STATUS (EFIAPI *EFI_HANDLE_PROTOCOL)(
    IN EFI_HANDLE    Handle,
    IN EFI_GUID      *Protocol,
    OUT VOID         **Interface
);


// Registers a notification for a protocol installation.
typedef EFI_STATUS (EFIAPI *EFI_REGISTER_PROTOCOL_NOTIFY)(
    IN EFI_GUID      *Protocol,
    IN EFI_EVENT     Event,
    OUT VOID         **Registration
);


// Locates handles matching a specified search condition.
typedef EFI_STATUS (EFIAPI *EFI_LOCATE_HANDLE)(
    IN EFI_LOCATE_SEARCH_TYPE  SearchType,
    IN EFI_GUID                *Protocol OPTIONAL,
    IN VOID                    *SearchKey OPTIONAL,
    IN OUT UINTN               *BufferSize,
    OUT EFI_HANDLE             *Buffer
);


// Locates a device handle using a device path.
typedef EFI_STATUS (EFIAPI *EFI_LOCATE_DEVICE_PATH)(
    IN EFI_GUID                     *Protocol,
    IN OUT EFI_DEVICE_PATH_PROTOCOL **DevicePath,
    OUT EFI_HANDLE                  *Device
);


// Installs or updates a configuration table.
typedef EFI_STATUS (EFIAPI *EFI_INSTALL_CONFIGURATION_TABLE)(
    IN EFI_GUID *Guid,
    IN VOID     *Table
);


// ============================================================================
// Image Services
// ============================================================================

// Loads an EFI image into memory.
typedef EFI_STATUS (EFIAPI *EFI_IMAGE_LOAD)(
    IN BOOLEAN                       BootPolicy,
    IN EFI_HANDLE                    ParentImageHandle,
    IN EFI_DEVICE_PATH_PROTOCOL      *DevicePath OPTIONAL,
    IN VOID                          *SourceBuffer OPTIONAL,
    IN UINTN                         SourceSize,
    OUT EFI_HANDLE                   *ImageHandle
);


// Starts a previously loaded EFI image.
typedef EFI_STATUS (EFIAPI *EFI_IMAGE_START)(
    IN EFI_HANDLE    ImageHandle,
    OUT UINTN        *ExitDataSize,
    OUT CHAR16       **ExitData OPTIONAL
);


// Unloads a previously loaded EFI image.
typedef EFI_STATUS (EFIAPI *EFI_IMAGE_UNLOAD)(
    IN EFI_HANDLE ImageHandle
);


// Exits the current EFI image.
typedef EFI_STATUS (EFIAPI *EFI_EXIT)(
    IN EFI_HANDLE    ImageHandle,
    IN EFI_STATUS    ExitStatus,
    IN UINTN         ExitDataSize,
    IN CHAR16        *ExitData OPTIONAL
);


// Terminates UEFI Boot Services and transfers control to the OS.
typedef EFI_STATUS (EFIAPI *EFI_EXIT_BOOT_SERVICES)(
    IN EFI_HANDLE ImageHandle,
    IN UINTN      MapKey
);


// ============================================================================
// Miscellaneous Services
// ============================================================================

// Returns the next monotonic counter value.
typedef EFI_STATUS (EFIAPI *EFI_GET_NEXT_MONOTONIC_COUNT)(
    OUT UINT64 *Count
);


// Pauses execution for the specified number of microseconds.
typedef EFI_STATUS (EFIAPI *EFI_STALL)(
    IN UINTN Microseconds
);


// Configures the UEFI watchdog timer.
typedef EFI_STATUS (EFIAPI *EFI_SET_WATCHDOG_TIMER)(
    IN UINTN   Timeout,
    IN UINT64  WatchdogCode,
    IN UINTN   DataSize,
    IN CHAR16  *WatchdogData OPTIONAL
);


// ============================================================================
// Driver Support Services
// ============================================================================

// Connects a controller to the appropriate driver.
typedef EFI_STATUS (EFIAPI *EFI_CONNECT_CONTROLLER)(
    IN EFI_HANDLE                       ControllerHandle,
    IN EFI_HANDLE                       *DriverImageHandle OPTIONAL,
    IN EFI_DEVICE_PATH_PROTOCOL         *RemainingDevicePath OPTIONAL,
    IN BOOLEAN                          Recursive
);


// Disconnects a controller from its driver.
typedef EFI_STATUS (EFIAPI *EFI_DISCONNECT_CONTROLLER)(
    IN EFI_HANDLE ControllerHandle,
    IN EFI_HANDLE DriverImageHandle OPTIONAL,
    IN EFI_HANDLE ChildHandle OPTIONAL
);


// ============================================================================
// Open & Close Protocol Services
// ============================================================================

// Opens a protocol interface.
typedef EFI_STATUS (EFIAPI *EFI_OPEN_PROTOCOL)(
    IN EFI_HANDLE    Handle,
    IN EFI_GUID      *Protocol,
    OUT VOID         **Interface OPTIONAL,
    IN EFI_HANDLE    AgentHandle,
    IN EFI_HANDLE    ControllerHandle,
    IN UINT32        Attributes
);


// Closes a previously opened protocol interface.
typedef EFI_STATUS (EFIAPI *EFI_CLOSE_PROTOCOL)(
    IN EFI_HANDLE Handle,
    IN EFI_GUID   *Protocol,
    IN EFI_HANDLE AgentHandle,
    IN EFI_HANDLE ControllerHandle
);


// Retrieves information about protocol openings.
typedef EFI_STATUS (EFIAPI *EFI_OPEN_PROTOCOL_INFORMATION)(
    IN EFI_HANDLE                         Handle,
    IN EFI_GUID                           *Protocol,
    OUT EFI_OPEN_PROTOCOL_INFORMATION_ENTRY **EntryBuffer,
    OUT UINTN                             *EntryCount
);


// ============================================================================
// Library Services
// ============================================================================

// Retrieves all protocols installed on a handle.
typedef EFI_STATUS (EFIAPI *EFI_PROTOCOLS_PER_HANDLE)(
    IN EFI_HANDLE Handle,
    OUT EFI_GUID   ***ProtocolBuffer,
    OUT UINTN      *ProtocolBufferCount
);


// Locates handles and automatically allocates the result buffer.
typedef EFI_STATUS (EFIAPI *EFI_LOCATE_HANDLE_BUFFER)(
    IN EFI_LOCATE_SEARCH_TYPE SearchType,
    IN EFI_GUID               *Protocol OPTIONAL,
    IN VOID                   *SearchKey OPTIONAL,
    OUT UINTN                 *NoHandles,
    OUT EFI_HANDLE            **Buffer
);


// Locates the first handle supporting a protocol.
typedef EFI_STATUS (EFIAPI *EFI_LOCATE_PROTOCOL)(
    IN EFI_GUID *Protocol,
    IN VOID     *Registration OPTIONAL,
    OUT VOID    **Interface
);


// Installs multiple protocol interfaces onto a handle.
typedef EFI_STATUS (EFIAPI *EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES)(
    IN OUT EFI_HANDLE *Handle,
    ...
);


// Uninstalls multiple protocol interfaces from a handle.
typedef EFI_STATUS (EFIAPI *EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES)(
    IN EFI_HANDLE Handle,
    ...
);


// ============================================================================
// 32-bit CRC Services
// ============================================================================

// Calculates a 32-bit CRC for a memory buffer.
typedef EFI_STATUS (EFIAPI *EFI_CALCULATE_CRC32)(
    IN VOID    *Data,
    IN UINTN   DataSize,
    OUT UINT32 *Crc32
);


// ============================================================================
// Memory Utility Services
// ============================================================================

// Copies a block of memory.
typedef VOID (EFIAPI *EFI_COPY_MEM)(
    IN VOID  *Destination,
    IN VOID  *Source,
    IN UINTN Length
);


// Fills a memory buffer with a specified byte value.
typedef VOID (EFIAPI *EFI_SET_MEM)(
    IN VOID  *Buffer,
    IN UINTN Size,
    IN UINT8 Value
);


// ============================================================================
// Extended Event Services
// ============================================================================

// Creates an event associated with an event group.
typedef EFI_STATUS (EFIAPI *EFI_CREATE_EVENT_EX)(
    IN UINT32             Type,
    IN EFI_TPL            NotifyTpl,
    IN EFI_EVENT_NOTIFY   NotifyFunction OPTIONAL,
    IN CONST VOID         *NotifyContext OPTIONAL,
    IN CONST EFI_GUID     *EventGroup OPTIONAL,
    OUT EFI_EVENT         *Event
);


// ============================================================================
// EFI BOOT SERVICES TABLE
// ============================================================================

typedef struct _EFI_BOOT_SERVICES
{
    EFI_TABLE_HEADER Hdr;

    // ------------------------------------------------------------------------
    // Task Priority Services
    // ------------------------------------------------------------------------

    EFI_RAISE_TPL    RaiseTPL;
    EFI_RESTORE_TPL  RestoreTPL;


    // ------------------------------------------------------------------------
    // Memory Services
    // ------------------------------------------------------------------------

    EFI_ALLOCATE_PAGES  AllocatePages;
    EFI_FREE_PAGES      FreePages;
    EFI_GET_MEMORY_MAP   GetMemoryMap;
    EFI_ALLOCATE_POOL   AllocatePool;
    EFI_FREE_POOL       FreePool;


    // ------------------------------------------------------------------------
    // Event & Timer Services
    // ------------------------------------------------------------------------

    EFI_CREATE_EVENT  CreateEvent;
    EFI_SET_TIMER     SetTimer;
    EFI_WAIT_FOR_EVENT WaitForEvent;
    EFI_SIGNAL_EVENT  SignalEvent;
    EFI_CLOSE_EVENT   CloseEvent;
    EFI_CHECK_EVENT   CheckEvent;


    // ------------------------------------------------------------------------
    // Protocol Handler Services
    // ------------------------------------------------------------------------

    EFI_INSTALL_PROTOCOL_INTERFACE     InstallProtocolInterface;
    EFI_REINSTALL_PROTOCOL_INTERFACE   ReinstallProtocolInterface;
    EFI_UNINSTALL_PROTOCOL_INTERFACE   UninstallProtocolInterface;
    EFI_HANDLE_PROTOCOL                HandleProtocol;

    VOID *Reserved;

    EFI_REGISTER_PROTOCOL_NOTIFY       RegisterProtocolNotify;
    EFI_LOCATE_HANDLE                  LocateHandle;
    EFI_LOCATE_DEVICE_PATH             LocateDevicePath;
    EFI_INSTALL_CONFIGURATION_TABLE    InstallConfigurationTable;


    // ------------------------------------------------------------------------
    // Image Services
    // ------------------------------------------------------------------------

    EFI_IMAGE_LOAD         LoadImage;
    EFI_IMAGE_START        StartImage;
    EFI_EXIT               Exit;
    EFI_IMAGE_UNLOAD       UnloadImage;
    EFI_EXIT_BOOT_SERVICES ExitBootServices;


    // ------------------------------------------------------------------------
    // Miscellaneous Services
    // ------------------------------------------------------------------------

    EFI_GET_NEXT_MONOTONIC_COUNT  GetNextMonotonicCount;
    EFI_STALL                     Stall;
    EFI_SET_WATCHDOG_TIMER        SetWatchdogTimer;


    // ------------------------------------------------------------------------
    // Driver Support Services
    // ------------------------------------------------------------------------

    EFI_CONNECT_CONTROLLER     ConnectController;
    EFI_DISCONNECT_CONTROLLER  DisconnectController;


    // ------------------------------------------------------------------------
    // Open & Close Protocol Services
    // ------------------------------------------------------------------------

    EFI_OPEN_PROTOCOL              OpenProtocol;
    EFI_CLOSE_PROTOCOL             CloseProtocol;
    EFI_OPEN_PROTOCOL_INFORMATION  OpenProtocolInformation;


    // ------------------------------------------------------------------------
    // Library Services
    // ------------------------------------------------------------------------

    EFI_PROTOCOLS_PER_HANDLE                  ProtocolsPerHandle;
    EFI_LOCATE_HANDLE_BUFFER                  LocateHandleBuffer;
    EFI_LOCATE_PROTOCOL                       LocateProtocol;
    EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES   InstallMultipleProtocolInterfaces;
    EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES UninstallMultipleProtocolInterfaces;


    // ------------------------------------------------------------------------
    // 32-bit CRC Services
    // ------------------------------------------------------------------------

    EFI_CALCULATE_CRC32 CalculateCrc32;


    // ------------------------------------------------------------------------
    // Memory Utility Services
    // ------------------------------------------------------------------------

    EFI_COPY_MEM        CopyMem;
    EFI_SET_MEM         SetMem;
    EFI_CREATE_EVENT_EX CreateEventEx;

} EFI_BOOT_SERVICES;


#endif // EFI_BOOT_SERVICES_H