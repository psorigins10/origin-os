#ifndef EFI_TYPES_H
#define EFI_TYPES_H

// ============================================================================
// NULL
// ============================================================================

#ifndef NULL
#define NULL ((VOID *)0)
#endif

#define UINT64_MAX 0xFFFFFFFFFFFFFFFFULL

// ============================================================================
// Basic Types
// ============================================================================

typedef void            VOID;

typedef unsigned char   UINT8;
typedef unsigned short  UINT16;
typedef unsigned int    UINT32;
typedef unsigned long long UINT64;

typedef unsigned short  CHAR16;

typedef signed short INT16;


// ============================================================================
// Native Integer Type
// ============================================================================
//
// UINTN is an unsigned integer whose size matches the native CPU pointer size.
//
// ORIGIN currently targets x86-64 UEFI, where UINTN is 64 bits.
// ============================================================================

typedef unsigned long long UINTN;


// ============================================================================
// Boolean
// ============================================================================

typedef unsigned char BOOLEAN;


// ============================================================================
// UEFI Handles
// ============================================================================
//
// EFI_HANDLE is an opaque pointer used by UEFI to identify firmware objects.
// ============================================================================

typedef VOID *EFI_HANDLE;


// ============================================================================
// Task Priority Level
// ============================================================================

typedef UINTN EFI_TPL;


// ============================================================================
// EFI Status
// ============================================================================
//
// UEFI functions return EFI_STATUS values.
//
// ORIGIN currently targets x86-64 UEFI, where EFI_STATUS is 64 bits.
// ============================================================================

typedef unsigned long long EFI_STATUS;

#define EFI_SUCCESS             0
#define EFI_INVALID_PARAMETER   0x8000000000000002ULL
#define EFI_BUFFER_TOO_SMALL    0x8000000000000005ULL


// ============================================================================
// Memory Addresses
// ============================================================================

typedef UINT64 EFI_PHYSICAL_ADDRESS;
typedef UINT64 EFI_VIRTUAL_ADDRESS;


// ============================================================================
// EFI GUID
// ============================================================================

typedef struct
{
    UINT32 Data1;
    UINT16 Data2;
    UINT16 Data3;
    UINT8  Data4[8];

} EFI_GUID;


// ============================================================================
// EFI Interface Type
// ============================================================================

typedef enum
{
    EFI_NATIVE_INTERFACE

} EFI_INTERFACE_TYPE;


// ============================================================================
// EFI Locate Search Type
// ============================================================================

typedef enum
{
    AllHandles,
    ByRegisterNotify,
    ByProtocol

} EFI_LOCATE_SEARCH_TYPE;


// ============================================================================
// EFI Event
// ============================================================================

typedef VOID *EFI_EVENT;


// ============================================================================
// EFI Timer Delay
// ============================================================================

typedef enum
{
    TimerCancel,
    TimerPeriodic,
    TimerRelative

} EFI_TIMER_DELAY;


// ============================================================================
// EFI Device Path Protocol
// ============================================================================

typedef struct _EFI_DEVICE_PATH_PROTOCOL
{
    UINT8 Type;
    UINT8 SubType;
    UINT8 Length[2];

} EFI_DEVICE_PATH_PROTOCOL;


// ============================================================================
// EFI Open Protocol Information Entry
// ============================================================================

typedef struct
{
    EFI_HANDLE AgentHandle;
    EFI_HANDLE ControllerHandle;
    UINT32     Attributes;
    UINT32     OpenCount;

} EFI_OPEN_PROTOCOL_INFORMATION_ENTRY;


//************************************************
//EFI_TIME
//************************************************
// This represents the current time information
typedef struct {
   UINT16    Year;              // 1900 - 9999
   UINT8     Month;             // 1 - 12
   UINT8     Day;               // 1 - 31
   UINT8     Hour;              // 0 - 23
   UINT8     Minute;            // 0 - 59
   UINT8     Second;            // 0 - 59
   UINT8     Pad1;
   UINT32    Nanosecond;        // 0 - 999,999,999
   INT16     TimeZone;          // —1440 to 1440 or 2047
   UINT8     Daylight;
   UINT8     Pad2;
 }   EFI_TIME;


//******************************************************
// EFI_TIME_CAPABILITIES
//******************************************************
// This provides the capabilities of the
// real time clock device as exposed through the EFI interfaces.
typedef struct {
   UINT32                  Resolution;
   UINT32                  Accuracy;
   BOOLEAN                 SetsToZero;
}   EFI_TIME_CAPABILITIES;


//******************************************************
// EFI_RESET_TYPE
//******************************************************
typedef enum {
   EfiResetCold,
   EfiResetWarm,
   EfiResetShutdown,
   EfiResetPlatformSpecific
}   EFI_RESET_TYPE;


//******************************************************
// EFI_CAPSULE_HEADER
//******************************************************
typedef struct {
   EFI_GUID             CapsuleGuid;
   UINT32               HeaderSize;
   UINT32               Flags;
   UINT32               CapsuleImageSize;
 } EFI_CAPSULE_HEADER;


#endif // EFI_TYPES_H