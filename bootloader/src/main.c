#include <origin/boot/boot_info.h>

#include <console/console.h>
#include <elf/elf_loader.h>
#include <memory/memory_map.h>
#include <memory/memory_map_dump.h>
#include <uefi/efi_system_table.h>
#include <uefi/protocols/efi_graphics_output.h>
#include <uefi/protocols/efi_loaded_image.h>
#include <uefi/protocols/efi_simple_file_system.h>

static EFI_GUID GopGuid = {
    0x9042A9DE, 0x23DC, 0x4A38,
    {0x96, 0xFB, 0x7A, 0xDE, 0xD0, 0x80, 0x51, 0x6A}
};

/* GUID for EFI_LOADED_IMAGE_PROTOCOL */
static EFI_GUID LoadedImageProtocolGuid = {
    0x5B1B31A1,
    0x9562,
    0x11D2,
    {0x8E, 0x3F, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B}
};


/* GUID for EFI_SIMPLE_FILE_SYSTEM_PROTOCOL */
static EFI_GUID SimpleFileSystemProtocolGuid = {
    0x964E5B22,
    0x6459,
    0x11D2,
    {0x8E, 0x39, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B}
};


/*
 * The kernel is compiled for the System V ABI (first argument in RDI),
 * while this bootloader is compiled for the Microsoft ABI (RCX).
 *
 * The sysv_abi attribute makes the compiler pass BootInfo in RDI
 * when calling the kernel entry function.
 *
 * IMPORTANT:
 *
 * kernel_entry.asm must NOT do:
 *
 *     mov rdi, rcx
 *
 * because this function pointer already uses the System V ABI.
 */
typedef void (__attribute__((sysv_abi)) *KERNEL_ENTRY)(ORIGIN_BOOT_INFO *);


/*
 * Halt the CPU forever.
 *
 * Used whenever continuing execution would be unsafe.
 */
static void Halt(void)
{
    for (;;) {
        __asm__ volatile ("hlt");
    }
}


/*
 * Print a 64-bit value in hexadecimal.
 *
 * Example:
 *
 *     0x8000000000000002
 *
 * This is mainly used for debugging UEFI status codes.
 */
static void PrintHex64(UINT64 Value, EFI_SYSTEM_TABLE *SystemTable)
{
    CHAR16 Buffer[19];

    const CHAR16 Hex[] = L"0123456789ABCDEF";

    Buffer[0] = L'0';
    Buffer[1] = L'x';

    for (int i = 0; i < 16; i++) {
        Buffer[17 - i] = Hex[Value & 0xFULL];
        Value >>= 4;
    }

    Buffer[18] = L'\0';

    Print(Buffer, SystemTable);
}


EFI_STATUS EFIAPI efi_main(
    EFI_HANDLE ImageHandle,
    EFI_SYSTEM_TABLE *SystemTable
)
{
    Print(L"Welcome To ORIGIN Boot\r\n", SystemTable);


    /* ---------------------------------------------------------
     * Initial memory map (debug dump only).
     *
     * This is NOT the final memory map that the kernel receives.
     * --------------------------------------------------------- */

    EFI_MEMORY_DESCRIPTOR *MemoryMap;
    UINTN MemoryMapSize;
    UINTN MapKey;
    UINTN DescriptorSize;
    UINT32 DescriptorVersion;

    EFI_STATUS Status = GetMemoryMap(
        SystemTable,
        &MemoryMap,
        &MemoryMapSize,
        &MapKey,
        &DescriptorSize,
        &DescriptorVersion
    );

    if (Status != EFI_SUCCESS) {
        Print(L"GetMemoryMap failed\r\n", SystemTable);

        Print(L"Status: ", SystemTable);
        PrintHex64(Status, SystemTable);
        Print(L"\r\n", SystemTable);

        Halt();
    }


    /* ---------------------------------------------------------
     * Loaded Image Protocol
     * --------------------------------------------------------- */

    EFI_LOADED_IMAGE_PROTOCOL *LoadedImage = NULL;

    Status = SystemTable->BootServices->HandleProtocol(
        ImageHandle,
        &LoadedImageProtocolGuid,
        (VOID **)&LoadedImage
    );

    if (Status != EFI_SUCCESS) {
        Print(L"Loaded Image Protocol failed\r\n", SystemTable);

        Print(L"Status: ", SystemTable);
        PrintHex64(Status, SystemTable);
        Print(L"\r\n", SystemTable);

        Halt();
    }


    /* ---------------------------------------------------------
     * Simple File System Protocol
     * --------------------------------------------------------- */

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem = NULL;

    Status = SystemTable->BootServices->HandleProtocol(
        LoadedImage->DeviceHandle,
        &SimpleFileSystemProtocolGuid,
        (VOID **)&FileSystem
    );

    if (Status != EFI_SUCCESS) {
        Print(L"Simple File System Protocol failed\r\n", SystemTable);

        Print(L"Status: ", SystemTable);
        PrintHex64(Status, SystemTable);
        Print(L"\r\n", SystemTable);

        Halt();
    }


    /* ---------------------------------------------------------
     * Open the volume root
     * --------------------------------------------------------- */

    EFI_FILE_PROTOCOL *Root = NULL;

    Status = FileSystem->OpenVolume(
        FileSystem,
        &Root
    );

    if (Status != EFI_SUCCESS) {
        Print(L"OpenVolume failed\r\n", SystemTable);

        Print(L"Status: ", SystemTable);
        PrintHex64(Status, SystemTable);
        Print(L"\r\n", SystemTable);

        Halt();
    }


    /* ---------------------------------------------------------
     * Open kernel.elf
     * --------------------------------------------------------- */

    EFI_FILE_PROTOCOL *KernelFile = NULL;

    Status = Root->Open(
        Root,
        &KernelFile,
        L"\\boot\\kernel.elf",
        0x0000000000000001ULL,
        0
    );

    if (Status != EFI_SUCCESS) {
        Print(L"Failed to open kernel.elf\r\n", SystemTable);

        Print(L"Status: ", SystemTable);
        PrintHex64(Status, SystemTable);
        Print(L"\r\n", SystemTable);

        Halt();
    }


    /* ---------------------------------------------------------
     * Get the file size.
     *
     * Seek to EOF.
     * Read current position.
     * Seek back to beginning.
     * --------------------------------------------------------- */

    UINT64 KernelFileSize;

    Status = KernelFile->SetPosition(
        KernelFile,
        0xFFFFFFFFFFFFFFFFULL
    );

    if (Status != EFI_SUCCESS) {
        Print(L"Failed to seek kernel.elf\r\n", SystemTable);

        Print(L"Status: ", SystemTable);
        PrintHex64(Status, SystemTable);
        Print(L"\r\n", SystemTable);

        Halt();
    }

    Status = KernelFile->GetPosition(
        KernelFile,
        &KernelFileSize
    );

    if (Status != EFI_SUCCESS) {
        Print(L"Failed to get kernel size\r\n", SystemTable);

        Print(L"Status: ", SystemTable);
        PrintHex64(Status, SystemTable);
        Print(L"\r\n", SystemTable);

        Halt();
    }

    Status = KernelFile->SetPosition(
        KernelFile,
        0
    );

    if (Status != EFI_SUCCESS) {
        Print(L"Failed to reset kernel position\r\n", SystemTable);

        Print(L"Status: ", SystemTable);
        PrintHex64(Status, SystemTable);
        Print(L"\r\n", SystemTable);

        Halt();
    }


    /* ---------------------------------------------------------
     * Read the whole kernel file into a pool buffer.
     * --------------------------------------------------------- */

    VOID *KernelBuffer = NULL;

    Status = SystemTable->BootServices->AllocatePool(
        EfiLoaderData,
        KernelFileSize,
        &KernelBuffer
    );

    if (Status != EFI_SUCCESS) {
        Print(L"Kernel buffer allocation failed\r\n", SystemTable);

        Print(L"Status: ", SystemTable);
        PrintHex64(Status, SystemTable);
        Print(L"\r\n", SystemTable);

        Halt();
    }

    UINTN TotalRead = 0;

    while (TotalRead < (UINTN)KernelFileSize) {

        UINTN CurrentRead =
            (UINTN)KernelFileSize - TotalRead;

        Status = KernelFile->Read(
            KernelFile,
            &CurrentRead,
            (UINT8 *)KernelBuffer + TotalRead
        );

        if (Status != EFI_SUCCESS) {
            Print(L"Failed to read kernel.elf\r\n", SystemTable);

            Print(L"Status: ", SystemTable);
            PrintHex64(Status, SystemTable);
            Print(L"\r\n", SystemTable);

            Halt();
        }

        if (CurrentRead == 0) {
            Print(
                L"Kernel read returned zero bytes\r\n",
                SystemTable
            );

            Halt();
        }

        TotalRead += CurrentRead;
    }


    /* ---------------------------------------------------------
     * Load the ELF into memory.
     * --------------------------------------------------------- */

    UINT64 KernelEntryPoint;

    Status = LoadKernel(
        SystemTable,
        KernelBuffer,
        TotalRead,
        &KernelEntryPoint
    );

    if (Status != EFI_SUCCESS) {

        if (Status == EFI_INVALID_PARAMETER) {

            Print(
                L"Kernel load: INVALID_PARAMETER\r\n",
                SystemTable
            );

        } else {

            Print(
                L"Kernel load: ALLOCATE/OTHER ERROR\r\n",
                SystemTable
            );
        }

        Print(L"Status: ", SystemTable);
        PrintHex64(Status, SystemTable);
        Print(L"\r\n", SystemTable);

        Halt();
    }

    Print(
        L"Kernel ELF loaded\r\n",
        SystemTable
    );


    /* ---------------------------------------------------------
     * Debug dump of the INITIAL memory map.
     *
     * This is not the map the kernel will receive.
     * --------------------------------------------------------- */

    Print(
        L"Memory map acquired\r\n",
        SystemTable
    );

    DumpMemoryMap(
        SystemTable,
        MemoryMap,
        MemoryMapSize,
        DescriptorSize
    );

    Print(
        L"\r\nMemory map dump complete\r\n",
        SystemTable
    );


    /* ---------------------------------------------------------
     * Allocate BootInfo.
     * --------------------------------------------------------- */

    ORIGIN_BOOT_INFO *BootInfo;

    Status = SystemTable->BootServices->AllocatePool(
        EfiLoaderData,
        sizeof(ORIGIN_BOOT_INFO),
        (VOID **)&BootInfo
    );

    if (Status != EFI_SUCCESS) {
        Print(
            L"BootInfo allocation failed\r\n",
            SystemTable
        );

        Print(L"Status: ", SystemTable);
        PrintHex64(Status, SystemTable);
        Print(L"\r\n", SystemTable);

        Halt();
    }


    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop = NULL;

    Status = SystemTable->BootServices->LocateProtocol(&GopGuid, NULL, (VOID **)&Gop);
    if (Status != EFI_SUCCESS) {
        Print(L"GOP not found\r\n", SystemTable);
        Halt();
    }

    BootInfo->fb_base                = Gop->Mode->FrameBufferBase;
    BootInfo->fb_size                = Gop->Mode->FrameBufferSize;
    BootInfo->fb_width               = Gop->Mode->Info->HorizontalResolution;
    BootInfo->fb_height              = Gop->Mode->Info->VerticalResolution;
    BootInfo->fb_pixels_per_scanline = Gop->Mode->Info->PixelsPerScanLine;
    BootInfo->fb_pixel_format        = Gop->Mode->Info->PixelFormat;


    /* ---------------------------------------------------------
     * Allocate the FINAL memory map buffer.
     *
     * First ask UEFI how large the current map is.
     *
     * Then allocate extra space because the allocation itself
     * changes the memory map.
     *
     * FinalMapCapacity is kept separate from FinalMapSize:
     *
     *     FinalMapCapacity = total allocated buffer size
     *     FinalMapSize     = actual map size returned by UEFI
     * --------------------------------------------------------- */

    EFI_MEMORY_DESCRIPTOR *FinalMap = NULL;

    UINTN FinalMapSize = 0;
    UINTN FinalMapKey = 0;
    UINTN FinalDescriptorSize = 0;
    UINT32 FinalDescriptorVersion = 0;


    /*
     * Ask for the size of the current memory map.
     *
     * EFI_BUFFER_TOO_SMALL is expected here.
     */
    Status = SystemTable->BootServices->GetMemoryMap(
        &FinalMapSize,
        NULL,
        &FinalMapKey,
        &FinalDescriptorSize,
        &FinalDescriptorVersion
    );

    if (Status != EFI_BUFFER_TOO_SMALL) {

        Print(
            L"Initial final-map GetMemoryMap failed\r\n",
            SystemTable
        );

        Print(L"Status: ", SystemTable);
        PrintHex64(Status, SystemTable);
        Print(L"\r\n", SystemTable);

        Halt();
    }


    /*
     * Give ourselves room for additional descriptors.
     *
     * The final buffer allocation itself modifies the map,
     * so we need some slack.
     */
    UINTN FinalMapCapacity =
        FinalMapSize + 16 * FinalDescriptorSize;


    Status = SystemTable->BootServices->AllocatePool(
        EfiLoaderData,
        FinalMapCapacity,
        (VOID **)&FinalMap
    );

    if (Status != EFI_SUCCESS) {

        Print(
            L"Final map buffer allocation failed\r\n",
            SystemTable
        );

        Print(L"Status: ", SystemTable);
        PrintHex64(Status, SystemTable);
        Print(L"\r\n", SystemTable);

        Halt();
    }


    /*
     * IMPORTANT:
     *
     * This Print happens BEFORE the final GetMemoryMap().
     *
     * Therefore it does not invalidate the MapKey used for
     * ExitBootServices().
     */
    Print(
        L"Exiting UEFI Boot Services...\r\n",
        SystemTable
    );


    /* ---------------------------------------------------------
     * FINAL GetMemoryMap + ExitBootServices loop.
     *
     * The required sequence is:
     *
     *     GetMemoryMap()
     *     ExitBootServices()
     *
     * If ExitBootServices() returns EFI_INVALID_PARAMETER,
     * the memory map changed between those two operations.
     *
     * We then fetch a fresh map and fresh MapKey and retry.
     *
     * IMPORTANT:
     *
     * Do NOT Print(), AllocatePool(), FreePool(), etc.
     * between the final GetMemoryMap() and ExitBootServices().
     * --------------------------------------------------------- */

    for (;;) {

        /*
         * GetMemoryMap() expects the size of the AVAILABLE
         * buffer on input.
         *
         * It overwrites FinalMapSize with the ACTUAL number
         * of bytes used.
         */
        FinalMapSize = FinalMapCapacity;


        Status = SystemTable->BootServices->GetMemoryMap(
            &FinalMapSize,
            FinalMap,
            &FinalMapKey,
            &FinalDescriptorSize,
            &FinalDescriptorVersion
        );


        /*
         * If this fails, we cannot safely call ExitBootServices().
         */
        if (Status != EFI_SUCCESS) {

            Print(
                L"Final GetMemoryMap failed\r\n",
                SystemTable
            );

            Print(L"Status: ", SystemTable);
            PrintHex64(Status, SystemTable);
            Print(L"\r\n", SystemTable);

            Halt();
        }


        /*
         * Immediately call ExitBootServices().
         *
         * No other Boot Service call happens between these
         * two operations.
         */
        Status = SystemTable->BootServices->ExitBootServices(
            ImageHandle,
            FinalMapKey
        );


        /*
         * Success!
         *
         * From this point onward, Boot Services are gone.
         */
        if (Status == EFI_SUCCESS) {
            break;
        }


        /*
         * EFI_INVALID_PARAMETER normally means that the memory
         * map changed after GetMemoryMap() gave us FinalMapKey.
         *
         * We must obtain a fresh memory map and retry.
         *
         * This Print is safe here because ExitBootServices()
         * has already failed, and the next operation in the
         * loop will obtain a completely fresh MapKey.
         */
        if (Status == EFI_INVALID_PARAMETER) {

            Print(
                L"ExitBootServices: INVALID_PARAMETER\r\n",
                SystemTable
            );

            Print(
                L"Retrying with fresh memory map...\r\n",
                SystemTable
            );

            continue;
        }


        /*
         * Any status other than SUCCESS or INVALID_PARAMETER
         * is unexpected.
         *
         * Do not silently loop forever.
         */
        Print(
            L"ExitBootServices failed\r\n",
            SystemTable
        );

        Print(
            L"Status: ",
            SystemTable
        );

        PrintHex64(
            Status,
            SystemTable
        );

        Print(
            L"\r\n",
            SystemTable
        );

        Halt();
    }


    /* ---------------------------------------------------------
     * Boot Services are now gone.
     *
     * DO NOT:
     *
     *     Print()
     *     AllocatePool()
     *     FreePool()
     *     GetMemoryMap()
     *     Call any other Boot Service
     *
     * The memory map we already obtained is now passed to the
     * kernel through BootInfo.
     * --------------------------------------------------------- */

    BootInfo->magic = ORIGIN_BOOT_MAGIC;

    BootInfo->boot_protocol_major =
        ORIGIN_BOOT_PROTOCOL_MAJOR;

    BootInfo->boot_protocol_minor =
        ORIGIN_BOOT_PROTOCOL_MINOR;

    BootInfo->bootloader_major =
        ORIGIN_BOOTLOADER_MAJOR;

    BootInfo->bootloader_minor =
        ORIGIN_BOOTLOADER_MINOR;

    BootInfo->memory_map =
        (uint64_t)(UINTN)FinalMap;

    BootInfo->memory_map_size =
        (uint64_t)FinalMapSize;

    BootInfo->memory_descriptor_size =
        (uint64_t)FinalDescriptorSize;


    /* ---------------------------------------------------------
     * Jump to the kernel.
     *
     * The function pointer uses System V ABI, so BootInfo
     * is passed in RDI.
     * --------------------------------------------------------- */

    __asm__ volatile ("cli");

    ((KERNEL_ENTRY)(UINTN)KernelEntryPoint)(BootInfo);


    /*
     * The kernel should never return.
     */
    Halt();

    return EFI_SUCCESS;
}