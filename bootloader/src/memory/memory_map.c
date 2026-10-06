#include "../../include/memory/memory_map.h"
#include "../../include/uefi/efi_system_table.h"
#include "../../include/uefi/efi_types.h"


EFI_STATUS GetMemoryMap(
    EFI_SYSTEM_TABLE *SystemTable,
    EFI_MEMORY_DESCRIPTOR **MemoryMap,
    UINTN *MemoryMapSize,
    UINTN *MapKey,
    UINTN *DescriptorSize,
    UINT32 *DescriptorVersion
)
{
    EFI_STATUS Status;

    /*
     * The memory-map buffer is kept between calls.
     *
     * This is important because AllocatePages() changes the
     * UEFI memory map. We do NOT want to allocate a new buffer
     * every time GetMemoryMap() is called.
     */
    static EFI_PHYSICAL_ADDRESS BufferAddress = 0;
    static UINTN BufferPages = 0;


    /*
     * ------------------------------------------------------------
     * First call:
     *
     * Ask UEFI how large the memory map currently is.
     * ------------------------------------------------------------
     */

    UINTN RequiredSize = 0;

    Status = SystemTable->BootServices->GetMemoryMap(
        &RequiredSize,
        NULL,
        MapKey,
        DescriptorSize,
        DescriptorVersion
    );


    if (Status != EFI_BUFFER_TOO_SMALL) {
        return Status;
    }


    /*
     * Add some extra room because the memory map can grow while
     * we are working with Boot Services.
     */
    RequiredSize += 2 * (*DescriptorSize);


    /*
     * Calculate how many 4 KiB pages are required.
     */
    UINTN RequiredPages =
        (RequiredSize + 0xFFF) / 0x1000;


    /*
     * ------------------------------------------------------------
     * Allocate the buffer only if we don't already have one large
     * enough.
     * ------------------------------------------------------------
     */

    if (BufferAddress == 0 ||
        BufferPages < RequiredPages) {

        /*
         * If we already had a buffer but it is too small,
         * allocate a larger one.
         *
         * For this early bootloader implementation we leave the
         * old buffer allocated. The important part is that we
         * don't allocate on every GetMemoryMap() call.
         */
        BufferAddress = 0;
        BufferPages = 0;


        Status = SystemTable->BootServices->AllocatePages(
            AllocateAnyPages,
            EfiLoaderData,
            RequiredPages,
            &BufferAddress
        );

        if (Status != EFI_SUCCESS) {
            return Status;
        }

        BufferPages = RequiredPages;
    }


    /*
     * Return the persistent buffer to the caller.
     */
    *MemoryMap =
        (EFI_MEMORY_DESCRIPTOR *)(UINTN)BufferAddress;


    /*
     * Give GetMemoryMap() the actual capacity of our buffer.
     */
    *MemoryMapSize =
        BufferPages * 0x1000;


    /*
     * ------------------------------------------------------------
     * Second call:
     *
     * Fill our already allocated buffer with the current
     * memory map.
     * ------------------------------------------------------------
     */

    Status = SystemTable->BootServices->GetMemoryMap(
        MemoryMapSize,
        *MemoryMap,
        MapKey,
        DescriptorSize,
        DescriptorVersion
    );


    if (Status != EFI_SUCCESS) {
        return Status;
    }


    return EFI_SUCCESS;
}