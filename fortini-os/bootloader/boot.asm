[org 0x7c00]
[bits 16]

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    sti

    ; Limpar tela
    mov ax, 0x0600
    xor bh, bh
    xor cx, cx
    mov dx, 0x184f
    int 0x10

    ; Exibir mensagem de boot
    mov si, banner
    call print_string

    ; Carregar estágios seguintes (stage2) de 0x7c00 + 512 bytes (0x7e00)
    ; Simples: usar int 13h para ler setores
    ; Para testes, apenas pular para stage2 que está na próxima posição

    jmp stage2_start

print_string:
    mov ah, 0x0e
.loop:
    lodsb
    test al, al
    jz .done
    int 0x10
    jmp .loop
.done:
    ret

banner:
    db 'Fortini OS Bootloader v1.0', 13, 10
    db 'Real Hardware Boot', 13, 10, 0

; Pad para 512 bytes
times 510 - ($ - $$) db 0
dw 0xaa55

; Stage2: simples, apenas chama kernel
stage2_start:
    cli

    ; Ativar A20
    call enable_a20

    ; Carregar GDT
    lgdt [gdt_ptr]

    ; Modo protegido
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Far jump para 32-bit (selector 8 = code segment)
    jmp 0x08:stage2_32bit

enable_a20:
    ; Método rápido: usar PS/2 controller
    mov al, 0xad
    out 0x64, al
    call wait_kb
    mov al, 0xd0
    out 0x64, al
    call wait_kb
    in al, 0x60
    push ax
    mov al, 0xd1
    out 0x64, al
    call wait_kb
    pop ax
    or al, 0x02
    out 0x60, al
    call wait_kb
    mov al, 0xae
    out 0x64, al
    call wait_kb
    ret

wait_kb:
    push ax
    xor ax, ax
    mov cx, 0xffff
.loop:
    in al, 0x64
    test al, 2
    jz .done
    loop .loop
.done:
    pop ax
    ret

; GDT
gdt_start:
    dq 0x0000000000000000
    dq 0x00cf9a000000ffff
    dq 0x00cf92000000ffff

gdt_ptr:
    dw $ - gdt_start - 1
    dd gdt_start

[bits 32]
stage2_32bit:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov esp, 0x20000

    ; Arquivo kernel.bin deve estar em setores seguintes
    ; Por hora, apenas saltar para um endereço fixo onde kernel será carregado
    ; Assumindo kernel carregado em 0x10000 (pelo bootloader ou firmware)
    jmp 0x10000
