bits 64

section .text
global gdt_load
global tss_load

gdt_load:
    ; RDI points to the packed GDT pointer structure.
    lgdt [rdi]

    ; Reload CS using a far return.
    push qword 0x08
    lea rax, [rel .reload_cs]
    push rax
    retfq

.reload_cs:
    ; Reload kernel data segment selectors.
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax

    xor eax, eax
    mov fs, ax
    mov gs, ax

    ret

; void tss_load(uint16_t selector)
; SysV ABI: selector arrives in EDI.
tss_load:
    mov ax, di
    ltr ax
    ret

global tss_read

tss_read:
    xor eax, eax
    str ax
    ret