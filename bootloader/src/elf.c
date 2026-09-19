#include "elf.h"

int elf_load(void *file, uint64_t *entry)
{
    Elf64_Ehdr *header = (Elf64_Ehdr *)file;

    if (header->e_magic != ELF_MAGIC)
        return -1;

    Elf64_Phdr *phdr =
        (Elf64_Phdr *)((uint8_t *)file + header->e_phoff);

    for (uint16_t i = 0; i < header->e_phnum; i++) {
        if (phdr[i].p_type != PT_LOAD)
            continue;

        uint8_t *src =
            (uint8_t *)file + phdr[i].p_offset;

        uint8_t *dst =
            (uint8_t *)phdr[i].p_paddr;

        for (uint64_t j = 0; j < phdr[i].p_filesz; j++)
            dst[j] = src[j];

        for (uint64_t j = phdr[i].p_filesz;
             j < phdr[i].p_memsz;
             j++)
            dst[j] = 0;
    }

    *entry = header->e_entry;

    return 0;
}