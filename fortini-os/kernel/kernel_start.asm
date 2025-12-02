[BITS 32]

extern kernel_main

section .text
    global kernel_entry

kernel_entry:
    ; Stack já foi configurado pelo bootloader em 0x20000
    ; Registradores de segmento já estão configurados (protected mode ativo)
    
    ; Limpa registradores de propósito geral
    xor eax, eax
    xor ebx, ebx
    xor ecx, ecx
    xor edx, edx
    xor esi, esi
    xor edi, edi
    xor ebp, ebp
    
    ; Alinha stack
    mov esp, 0x20000
    
    ; Chama kernel_main
    call kernel_main
    
    ; Se kernel_main retornar, faz halt
    jmp halt

halt:
    cli
    hlt
    jmp halt

section .data
    ; Dados estáticos vazios (para linker não reclamar)
