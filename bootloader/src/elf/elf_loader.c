#include "elf/elf_loader.h"
#include "uefi/efi_types.h"
#include "console/console.h"

/*
 * EFI status values used for more useful diagnostics.
 *
 * These are defined here so we don't have to depend on whether
 * efi_types.h already contains every UEFI status code.
 */
#define EFI_NOT_FOUND         0x800000000000000EULL
#define EFI_OUT_OF_RESOURCES  0x8000000000000009ULL
#define EFI_ACCESS_DENIED     0x800000000000000FULL


EFI_STATUS LoadKernel(
    EFI_SYSTEM_TABLE *SystemTable,
    VOID *KernelFile,
    UINTN KernelFileSize,
    UINT64 *EntryPoint
)
{
    /*
     * ------------------------------------------------------------
     * Basic argument validation
     * ------------------------------------------------------------
     */

    if (SystemTable == NULL ||
        KernelFile == NULL ||
        EntryPoint == NULL) {

        return EFI_INVALID_PARAMETER;
    }

    if (KernelFileSize < sizeof(ELF64_EHDR)) {
        Print(L"ELF: file too small\r\n", SystemTable);
        return EFI_INVALID_PARAMETER;
    }


    /*
     * ------------------------------------------------------------
     * ELF header validation
     * ------------------------------------------------------------
     */

    ELF64_EHDR *Header = (ELF64_EHDR *)KernelFile;


    /* ELF magic: 0x7F 'E' 'L' 'F' */

    if (Header->e_ident[0] != 0x7F ||
        Header->e_ident[1] != 'E'  ||
        Header->e_ident[2] != 'L'  ||
        Header->e_ident[3] != 'F') {

        Print(L"ELF: invalid magic\r\n", SystemTable);
        return EFI_INVALID_PARAMETER;
    }


    /* ELFCLASS64 */

    if (Header->e_ident[4] != 2) {
        Print(L"ELF: not 64-bit\r\n", SystemTable);
        return EFI_INVALID_PARAMETER;
    }


    /*
     * ELF data encoding.
     *
     * 1 = little endian.
     *
     * x86-64 kernels must use little endian ELF.
     */

    if (Header->e_ident[5] != 1) {
        Print(L"ELF: not little-endian\r\n", SystemTable);
        return EFI_INVALID_PARAMETER;
    }


    /*
     * ELF version.
     *
     * EV_CURRENT = 1.
     */

    if (Header->e_ident[6] != 1) {
        Print(L"ELF: Invalid ELF version\r\n", SystemTable);
        return EFI_INVALID_PARAMETER;
    }

    if (Header->e_version != 1) {
        Print(L"ELF: Invalid ELF e_version\r\n", SystemTable);
        return EFI_INVALID_PARAMETER;
    }

    /*
     * EM_X86_64 = 0x3E
     */

    if (Header->e_machine != 0x3E) {
        Print(L"ELF: not x86-64\r\n", SystemTable);
        return EFI_INVALID_PARAMETER;
    }


    /*
     * ET_EXEC = 2
     *
     * Our current kernel is linked for fixed physical addresses.
     */

    if (Header->e_type != 2) {
        Print(L"ELF: kernel is not executable\r\n", SystemTable);
        return EFI_INVALID_PARAMETER;
    }


    /*
     * ELF header size must match our ELF64 header structure.
     */

    if (Header->e_ehsize != sizeof(ELF64_EHDR)) {
        Print(L"ELF: invalid ELF header size\r\n", SystemTable);
        return EFI_INVALID_PARAMETER;
    }


    /*
     * ------------------------------------------------------------
     * Program-header table validation
     * ------------------------------------------------------------
     */

    if (Header->e_phoff >= KernelFileSize) {
        Print(L"ELF: invalid program-header offset\r\n", SystemTable);
        return EFI_INVALID_PARAMETER;
    }

    if (Header->e_phentsize != sizeof(ELF64_PHDR)) {
        Print(L"ELF: invalid program-header size\r\n", SystemTable);
        return EFI_INVALID_PARAMETER;
    }

    if (Header->e_phnum == 0) {
        Print(L"ELF: no program headers\r\n", SystemTable);
        return EFI_INVALID_PARAMETER;
    }


    /*
     * Calculate the complete size of the program-header table.
     */

    UINT64 ProgramHeaderTableSize =
        (UINT64)Header->e_phnum * Header->e_phentsize;


    /*
     * Make sure the entire program-header table is inside
     * the kernel file.
     */

    if (ProgramHeaderTableSize >
        KernelFileSize - Header->e_phoff) {

        Print(L"ELF: program-header table outside file\r\n",
              SystemTable);

        return EFI_INVALID_PARAMETER;
    }


    ELF64_PHDR *ProgramHeaders =
        (ELF64_PHDR *)(
            (UINT8 *)KernelFile + Header->e_phoff
        );


    /*
     * ------------------------------------------------------------
     * Find the complete physical range required by PT_LOAD
     * segments.
     * ------------------------------------------------------------
     */

    UINT64 LoadBase = UINT64_MAX;
    UINT64 LoadEnd = 0;

    BOOLEAN FoundLoadSegment = 0;


    /*
     * First pass:
     *
     * Validate every PT_LOAD segment and determine the lowest
     * and highest physical addresses used by the kernel.
     */

    for (UINT16 i = 0; i < Header->e_phnum; i++) {

        ELF64_PHDR *ProgramHeader = &ProgramHeaders[i];


        /*
         * Ignore non-loadable program headers.
         */

        if (ProgramHeader->p_type != ELF64_PT_LOAD) {
            continue;
        }

        FoundLoadSegment = 1;


        /*
         * A segment cannot contain more memory than it has
         * file contents.
         */

        if (ProgramHeader->p_filesz > ProgramHeader->p_memsz) {

            Print(L"ELF: filesz > memsz\r\n", SystemTable);

            return EFI_INVALID_PARAMETER;
        }


        /*
         * Make sure the segment's file data exists inside
         * kernel.elf.
         */

        if (ProgramHeader->p_offset > KernelFileSize) {

            Print(L"ELF: segment offset outside file\r\n",
                  SystemTable);

            return EFI_INVALID_PARAMETER;
        }

        if (ProgramHeader->p_filesz >
            KernelFileSize - ProgramHeader->p_offset) {

            Print(L"ELF: segment data outside file\r\n",
                  SystemTable);

            return EFI_INVALID_PARAMETER;
        }


        /*
         * Prevent physical-address overflow.
         */

        if (ProgramHeader->p_paddr >
            UINT64_MAX - ProgramHeader->p_memsz) {

            Print(L"ELF: segment address overflow\r\n",
                  SystemTable);

            return EFI_INVALID_PARAMETER;
        }


        UINT64 SegmentEnd =
            ProgramHeader->p_paddr +
            ProgramHeader->p_memsz;


        /*
         * Determine the lowest physical address.
         */

        if (ProgramHeader->p_paddr < LoadBase) {
            LoadBase = ProgramHeader->p_paddr;
        }


        /*
         * Determine the highest physical address.
         */

        if (SegmentEnd > LoadEnd) {
            LoadEnd = SegmentEnd;
        }
    }


    /*
     * There must be at least one PT_LOAD segment.
     */

    if (!FoundLoadSegment) {

        Print(L"ELF: no PT_LOAD segments\r\n", SystemTable);

        return EFI_INVALID_PARAMETER;
    }


    if (LoadBase == UINT64_MAX ||
        LoadEnd <= LoadBase) {

        Print(L"ELF: invalid load range\r\n", SystemTable);

        return EFI_INVALID_PARAMETER;
    }


    /*
     * ------------------------------------------------------------
     * Validate kernel entry point
     * ------------------------------------------------------------
     *
     * The entry point should live inside the memory occupied by
     * the loaded kernel.
     */

    if (Header->e_entry < LoadBase ||
        Header->e_entry >= LoadEnd) {

        Print(L"ELF: entry point outside kernel\r\n",
              SystemTable);

        return EFI_INVALID_PARAMETER;
    }


    /*
     * ------------------------------------------------------------
     * Calculate page-aligned kernel range
     * ------------------------------------------------------------
     */

    UINT64 PageSize = 0x1000;


    /*
     * Round the beginning down to a page boundary.
     */

    UINT64 AlignedLoadBase =
        LoadBase & ~(PageSize - 1);


    /*
     * Prevent overflow while rounding the end upward.
     */

    if (LoadEnd > UINT64_MAX - (PageSize - 1)) {

        Print(L"ELF: load-end alignment overflow\r\n",
              SystemTable);

        return EFI_INVALID_PARAMETER;
    }


    /*
     * Round the end up to a page boundary.
     */

    UINT64 AlignedLoadEnd =
        (LoadEnd + PageSize - 1) &
        ~(PageSize - 1);


    /*
     * Calculate total number of bytes required.
     */

    UINT64 LoadSize =
        AlignedLoadEnd - AlignedLoadBase;


    if (LoadSize == 0) {

        Print(L"ELF: zero load size\r\n", SystemTable);

        return EFI_INVALID_PARAMETER;
    }


    /*
     * Convert bytes to 4 KiB pages.
     */

    UINTN Pages =
        (UINTN)(LoadSize / PageSize);


    if (Pages == 0) {

        Print(L"ELF: zero pages\r\n", SystemTable);

        return EFI_INVALID_PARAMETER;
    }


    /*
     * ------------------------------------------------------------
     * Reserve the physical memory
     * ------------------------------------------------------------
     *
     * IMPORTANT:
     *
     * AllocateAddress means that UEFI must give us EXACTLY the
     * physical address requested below.
     *
     * This is intentional because the current kernel is linked
     * for fixed addresses.
     */

    EFI_PHYSICAL_ADDRESS KernelAddress =
        (EFI_PHYSICAL_ADDRESS)AlignedLoadBase;


    Print(
        L"ELF: allocating kernel pages...\r\n",
        SystemTable
    );


    EFI_STATUS Status =
        SystemTable->BootServices->AllocatePages(
            AllocateAddress,
            EfiLoaderData,
            Pages,
            &KernelAddress
        );


    /*
     * ------------------------------------------------------------
     * Diagnose allocation failure
     * ------------------------------------------------------------
     */

    if (Status != EFI_SUCCESS) {

        Print(
            L"ELF: AllocatePages FAILED\r\n",
            SystemTable
        );


        if (Status == EFI_NOT_FOUND) {

            /*
             * Most interesting case for AllocateAddress:
             *
             * the requested physical range is unavailable.
             */

            Print(
                L"ELF: requested physical range unavailable\r\n",
                SystemTable
            );

        }
        else if (Status == EFI_OUT_OF_RESOURCES) {

            Print(
                L"ELF: firmware is out of resources\r\n",
                SystemTable
            );

        }
        else if (Status == EFI_INVALID_PARAMETER) {

            Print(
                L"ELF: AllocatePages invalid parameter\r\n",
                SystemTable
            );

        }
        else if (Status == EFI_ACCESS_DENIED) {

            Print(
                L"ELF: requested memory is protected\r\n",
                SystemTable
            );

        }
        else {

            /*
             * We don't have a hexadecimal formatter here yet,
             * so keep the fallback generic.
             */

            Print(
                L"ELF: unknown AllocatePages error\r\n",
                SystemTable
            );
        }


        return Status;
    }


    Print(
        L"ELF: AllocatePages OK\r\n",
        SystemTable
    );


    /*
     * ------------------------------------------------------------
     * Load PT_LOAD segments
     * ------------------------------------------------------------
     *
     * For every loadable segment:
     *
     *   [file data] -> physical destination
     *
     * Then:
     *
     *   [remaining memory] -> zero
     *
     * The second part handles .bss and other zero-filled data.
     */

    for (UINT16 i = 0; i < Header->e_phnum; i++) {

        ELF64_PHDR *ProgramHeader = &ProgramHeaders[i];


        if (ProgramHeader->p_type != ELF64_PT_LOAD) {
            continue;
        }


        UINT8 *Source =
            (UINT8 *)KernelFile +
            ProgramHeader->p_offset;


        UINT8 *Destination =
            (UINT8 *)(UINTN)ProgramHeader->p_paddr;


        /*
         * Copy the part that exists in the ELF file.
         */

        for (UINT64 j = 0;
             j < ProgramHeader->p_filesz;
             j++) {

            Destination[j] = Source[j];
        }


        /*
         * Zero the part that exists in memory but not in
         * the ELF file.
         */

        for (UINT64 j = ProgramHeader->p_filesz;
             j < ProgramHeader->p_memsz;
             j++) {

            Destination[j] = 0;
        }
    }


    /*
     * ------------------------------------------------------------
     * Return kernel entry point
     * ------------------------------------------------------------
     */

    *EntryPoint = Header->e_entry;


    Print(
        L"ELF: kernel loaded successfully\r\n",
        SystemTable
    );


    return EFI_SUCCESS;
}