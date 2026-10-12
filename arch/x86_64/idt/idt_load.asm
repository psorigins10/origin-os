bits 64

section .text
global idt_load
global idt_read

; void idt_load(const IDTR *idtr_pointer)
; System V AMD64 ABI: RDI contains the pointer.

idt_load:
    lidt [rdi]
    ret

idt_read:
    sidt [rdi]
    ret