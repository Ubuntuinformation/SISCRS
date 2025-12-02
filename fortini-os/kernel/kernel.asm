; kernel.asm - Ponto de entrada do kernel em modo protegido
; Assinatura Multiboot

MBOOT_MAGIC     equ 0x1BADB002
MBOOT_FLAGS     equ 0x00000001
MBOOT_CHECKSUM  equ -(MBOOT_MAGIC + MBOOT_FLAGS)

section .multiboot
align 4
    dd MBOOT_MAGIC
    dd MBOOT_FLAGS
    dd MBOOT_CHECKSUM

; Stack
STACK_SIZE equ 0x4000
section .bss
align 16
stack_bottom:
    resb STACK_SIZE
stack_top:

section .note.GNU-stack noalloc noexec

section .text
extern _start
global start
align 4

start:
    ; Configurar stack
    mov esp, stack_top
    
    ; Chamar main C
    call _start
    
    ; Loop infinito
    cli
    hlt
    jmp $
