bits 64
section .text
global gdt_read

; void gdt_read(void *buffer)
; RDI points to a 10-byte buffer for the GDTR.
gdt_read:
    sgdt [rdi]
    ret