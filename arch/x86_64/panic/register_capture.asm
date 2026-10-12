bits 64

section .text
global register_capture

; void register_capture(ORIGIN_REGISTER_CONTEXT *context)
;
; System V AMD64 ABI:
;   RDI = pointer to ORIGIN_REGISTER_CONTEXT
;
; This is a software snapshot taken at function entry.
; RDI contains the context pointer, not the caller's
; pre-call RDI value.
;
; Structure offsets:
;   RAX             0
;   RBX             8
;   RCX            16
;   RDX            24
;   RSI            32
;   RDI            40
;   RBP            48
;   R8             56
;   R9             64
;   R10            72
;   R11            80
;   R12            88
;   R13            96
;   R14           104
;   R15           112
;   RSP           120
;   RIP           128
;   RFLAGS        136
;   exception_vector 144
;   error_code       152

register_capture:
    ; Save all 15 general-purpose registers.
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    ; 15 registers * 8 bytes = 120 bytes.
    ; The saved context pointer is at [rsp + 72].
    mov rdi, [rsp + 72]

    ; Copy saved general-purpose registers.
    mov rax, [rsp + 112]
    mov [rdi + 0], rax

    mov rax, [rsp + 104]
    mov [rdi + 8], rax

    mov rax, [rsp + 96]
    mov [rdi + 16], rax

    mov rax, [rsp + 88]
    mov [rdi + 24], rax

    mov rax, [rsp + 80]
    mov [rdi + 32], rax

    mov rax, [rsp + 72]
    mov [rdi + 40], rax

    mov rax, [rsp + 64]
    mov [rdi + 48], rax

    mov rax, [rsp + 56]
    mov [rdi + 56], rax

    mov rax, [rsp + 48]
    mov [rdi + 64], rax

    mov rax, [rsp + 40]
    mov [rdi + 72], rax

    mov rax, [rsp + 32]
    mov [rdi + 80], rax

    mov rax, [rsp + 24]
    mov [rdi + 88], rax

    mov rax, [rsp + 16]
    mov [rdi + 96], rax

    mov rax, [rsp + 8]
    mov [rdi + 104], rax

    mov rax, [rsp]
    mov [rdi + 112], rax

    ; RSP at function entry, before our pushes.
    lea rax, [rsp + 120]
    mov [rdi + 120], rax

    ; The call instruction placed the return address
    ; at the original stack top.
    mov rax, [rsp + 120]
    mov [rdi + 128], rax

    ; PUSHFQ captures the entry flags. The instructions
    ; above (PUSH, MOV, LEA) do not modify RFLAGS.
    pushfq
    pop rax
    mov [rdi + 136], rax

    ; UINT64_MAX means this is not an exception frame.
    mov qword [rdi + 144], -1
    mov qword [rdi + 152], 0

    ; Restore the saved registers in reverse order.
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax

    ret