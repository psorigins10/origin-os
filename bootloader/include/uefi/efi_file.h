#ifndef EFI_FILE_H
#define EFI_FILE_H

#include "efi_types.h"

/*
 * Forward declaration of the UEFI File Protocol structure.
 *
 * The protocol is represented as a table of function pointers. The
 * structure is defined below after all operation types have been declared.
 */
typedef struct _EFI_FILE_PROTOCOL EFI_FILE_PROTOCOL;


/*
 * Opens a file or directory relative to the current file handle.
 *
 * Parameters:
 *   This       - File protocol instance used to perform the operation.
 *   NewHandle  - Receives the handle of the opened file or directory.
 *   FileName   - Null-terminated UTF-16 path of the target.
 *   OpenMode   - Requested access mode.
 *   Attributes - File attributes when creating a new file.
 */
typedef EFI_STATUS (*EFI_FILE_OPEN)(
    EFI_FILE_PROTOCOL *This,
    EFI_FILE_PROTOCOL **NewHandle,
    CHAR16 *FileName,
    UINT64 OpenMode,
    UINT64 Attributes
);


/*
 * Closes the current file or directory handle.
 */
typedef EFI_STATUS (*EFI_FILE_CLOSE)(
    EFI_FILE_PROTOCOL *This
);


/*
 * Deletes the file or directory represented by the current handle.
 */
typedef EFI_STATUS (*EFI_FILE_DELETE)(
    EFI_FILE_PROTOCOL *This
);


/*
 * Reads data from the current file position.
 *
 * BufferSize is both an input and output parameter:
 *   - On input, it specifies the size of the destination buffer.
 *   - On output, it contains the number of bytes actually read.
 */
typedef EFI_STATUS (*EFI_FILE_READ)(
    EFI_FILE_PROTOCOL *This,
    UINTN *BufferSize,
    VOID *Buffer
);


/*
 * Writes data to the current file position.
 *
 * BufferSize specifies the number of bytes to write and may be updated
 * by the implementation to report the number of bytes actually written.
 */
typedef EFI_STATUS (*EFI_FILE_WRITE)(
    EFI_FILE_PROTOCOL *This,
    UINTN *BufferSize,
    VOID *Buffer
);


/*
 * Retrieves the current byte position within the file.
 */
typedef EFI_STATUS (*EFI_FILE_GET_POSITION)(
    EFI_FILE_PROTOCOL *This,
    UINT64 *Position
);


/*
 * Sets the current byte position within the file.
 */
typedef EFI_STATUS (*EFI_FILE_SET_POSITION)(
    EFI_FILE_PROTOCOL *This,
    UINT64 Position
);


/*
 * Retrieves information about the file or directory.
 *
 * InformationType identifies the requested information structure.
 */
typedef EFI_STATUS (*EFI_FILE_GET_INFO)(
    EFI_FILE_PROTOCOL *This,
    VOID *InformationType,
    UINTN *BufferSize,
    VOID *Buffer
);


/*
 * Updates information associated with the file or directory.
 */
typedef EFI_STATUS (*EFI_FILE_SET_INFO)(
    EFI_FILE_PROTOCOL *This,
    VOID *InformationType,
    UINTN BufferSize,
    VOID *Buffer
);


/*
 * Flushes pending file data and metadata associated with the handle.
 */
typedef EFI_STATUS (*EFI_FILE_FLUSH)(
    EFI_FILE_PROTOCOL *This
);


/*
 * Asynchronous version of the Open operation.
 *
 * Token provides the completion context for the asynchronous request.
 */
typedef EFI_STATUS (*EFI_FILE_OPEN_EX)(
    EFI_FILE_PROTOCOL *This,
    EFI_FILE_PROTOCOL **NewHandle,
    CHAR16 *FileName,
    UINT64 OpenMode,
    UINT64 Attributes,
    VOID *Token
);


/*
 * Asynchronous version of the Read operation.
 *
 * Token describes the asynchronous request and its completion state.
 */
typedef EFI_STATUS (*EFI_FILE_READ_EX)(
    EFI_FILE_PROTOCOL *This,
    VOID *Token
);


/*
 * Asynchronous version of the Write operation.
 *
 * Token describes the asynchronous request and its completion state.
 */
typedef EFI_STATUS (*EFI_FILE_WRITE_EX)(
    EFI_FILE_PROTOCOL *This,
    VOID *Token
);


/*
 * Asynchronous version of the Flush operation.
 *
 * Token describes the asynchronous request and its completion state.
 */
typedef EFI_STATUS (*EFI_FILE_FLUSH_EX)(
    EFI_FILE_PROTOCOL *This,
    VOID *Token
);


/*
 * UEFI File Protocol.
 *
 * This structure represents a file or directory handle and exposes
 * operations through a table of function pointers.
 *
 * The member order is part of the UEFI protocol ABI and must not be
 * changed when reconstructing the protocol definition.
 */
struct _EFI_FILE_PROTOCOL {

    /* Revision of the File Protocol interface. */
    UINT64 Revision;

    /* Synchronous file and directory operations. */
    EFI_FILE_OPEN Open;
    EFI_FILE_CLOSE Close;
    EFI_FILE_DELETE Delete;
    EFI_FILE_READ Read;
    EFI_FILE_WRITE Write;
    EFI_FILE_GET_POSITION GetPosition;
    EFI_FILE_SET_POSITION SetPosition;
    EFI_FILE_GET_INFO GetInfo;
    EFI_FILE_SET_INFO SetInfo;
    EFI_FILE_FLUSH Flush;

    /* Extended/asynchronous file operations. */
    EFI_FILE_OPEN_EX OpenEx;
    EFI_FILE_READ_EX ReadEx;
    EFI_FILE_WRITE_EX WriteEx;
    EFI_FILE_FLUSH_EX FlushEx;
};

#endif