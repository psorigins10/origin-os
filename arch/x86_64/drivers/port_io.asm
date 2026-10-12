bits 64

section .text

; Export these functions so the linker can find them
; when C code calls outb() or inb().
global outb
global inb

; --------------------------------------------------
; outb(uint16_t port, uint8_t value)
;
; Writes one byte to an I/O port.
;
; x86-64 System V calling convention:
;   DI  = port (first argument)
;   SIL = value (second argument)
; --------------------------------------------------
outb:
    mov dx, di       ; Copy the port number into DX.
    mov al, sil      ; Copy the byte to write into AL.
    out dx, al       ; Write AL to the I/O port specified by DX.
    ret              ; Return to the C caller.

; --------------------------------------------------
; inb(uint16_t port)
;
; Reads one byte from an I/O port and returns it.
;
; x86-64 System V calling convention:
;   DI  = port (first argument)
;   AL  = returned byte
; --------------------------------------------------
inb:
    mov dx, di       ; Copy the port number into DX.
    xor rax, rax     ; Clear RAX so the return value starts at zero.
    in al, dx        ; Read one byte from the port into AL.
    ret              ; Return to the C caller; RAX contains the result.
