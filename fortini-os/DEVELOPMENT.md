# Guia de Desenvolvimento - Fortini OS

## Arquitetura

```
┌─────────────────────────────────────┐
│     SiscrShell (shell/main.c)       │
├─────────────────────────────────────┤
│        Kernel (kernel/*.c)          │
│    Modo Protegido 32-bit i386       │
├─────────────────────────────────────┤
│ GRUB Bootloader (bootloader/*.cfg)  │
├─────────────────────────────────────┤
│         Hardware (QEMU/Real)        │
└─────────────────────────────────────┘
```

## Fluxo de Boot

1. **BIOS** → Carrega GRUB (bootloader)
2. **GRUB** → Lê kernel.elf (Multiboot)
3. **Kernel** → kernel.asm (entry point) → kernel.c (_start)
4. **Kernel** → Inicializa VGA e modo protegido
5. **Shell** → Exibe prompt (em desenvolvimento)

## Adicionar um Novo Comando ao Shell

### Passo 1: Implementar a função

Edite `shell/main.c` e adicione:

```c
int cmd_seu_comando(int argc, char** argv) {
    // Seu código aqui
    printf("Seu comando funcionou!\n");
    return 0;
}
```

### Passo 2: Registrar no array de comandos

Encontre a estrutura `builtins[]` e adicione:

```c
{"seu_comando", "Descrição do comando", cmd_seu_comando},
```

### Passo 3: Recompilar

```bash
cd shell
make clean && make
cd ..
make iso
```

## Estender o Kernel

### Adicionar uma nova função de sistema

Edite `kernel/kernel.c`:

```c
void my_system_call(void) {
    terminal_write_string("Meu sistema chamado!\n");
}
```

Chame de `_start()` para testar.

### Adicionar seção ao linker script

Edite `kernel/linker.ld` se precisar de seções customizadas.

## Estrutura de Memória

```
0x00000000 +----- Tabela de Paginação (futura)
           |
0x00100000 +----- Kernel (0x100000) [1MB]
           |
           | Stack (cresce para baixo)
           |
0xFFFFFFFF +----- Fim
```

## Compiladores

### Usando GCC
```bash
gcc -Wall -Wextra -ffreestanding -m32 -c arquivo.c
```

### Usando Clang
```bash
clang -Wall -Wextra -ffreestanding -m32 -c arquivo.c
```

## Formato EFF (Executable For Fortini)

### Header EFF

```c
struct eff_header {
    uint32_t magic;         // 0x45464600
    uint32_t version;       // 1
    uint32_t entry_point;   // Endereço de início
    uint32_t code_offset;   // Offset do código
    uint32_t code_size;     // Tamanho
    uint32_t data_offset;   // Offset dos dados
    uint32_t data_size;     // Tamanho
    uint32_t bss_size;      // BSS não inicializado
    uint32_t flags;         // FLAGS
    uint32_t checksum;      // CRC32
};
```

### Converter ELF para EFF

```bash
cd utils
gcc eff_converter.c -o converter
./converter ../kernel/kernel.elf kernel.eff
```

## Debugging

### Com QEMU

```bash
# Em um terminal:
qemu-system-i386 -cdrom build/fortini.iso -s -S

# Em outro (GDB):
gdb
(gdb) file kernel/kernel.elf
(gdb) target remote :1234
(gdb) c
```

### Modo serial

```bash
qemu-system-i386 -kernel kernel/kernel.elf -serial stdio
```

## Testes

```bash
# Compilar tudo
make clean && make all

# Gerar ISO
make iso

# Testar em QEMU
./test.sh
# ou
qemu-system-i386 -cdrom build/fortini.iso -m 512
```

## Estrutura de Arquivos

```
kernel/
├── kernel.c         - Código principal do kernel
├── kernel.asm       - Ponto de entrada (multiboot)
├── linker.ld        - Script de linkedição
└── Makefile

shell/
├── main.c           - Shell e comandos built-in
└── Makefile

libc/
├── string.c         - Funções de string
├── stdio.h          - Protótipos (futura)

utils/
├── eff.h            - Header do formato EFF
├── eff_converter.c  - Conversor ELF→EFF
```

## Próximos Passos

1. **Sistema de Arquivos** - Implementar FAT32 básico
2. **Multitarefa** - Scheduler e context switch
3. **Memória Virtual** - Paginação e proteção
4. **Drivers** - Disco, rede, vídeo
5. **Interface Gráfica** - Framebuffer simples

## Recursos

- **Multiboot Spec**: https://www.gnu.org/software/grub/manual/multiboot/
- **Intel 80386 Reference**: https://en.wikibooks.org/wiki/X86_Assembly
- **OSDev Wiki**: https://wiki.osdev.org/

## Dúvidas Comuns

**P: Como adicionar mais memória?**
A: Edite a chamada QEMU em test.sh: `-m 1024` para 1GB

**P: Como compilar sem warnings?**
A: Corrija os warnings ou use `-Wno-error` no CFLAGS

**P: Posso usar Rust?**
A: Sim, compile com `rustc` e ligue com o linker padrão

**P: Como adicionar drivers?**
A: Implemente em `kernel/`, registre em `_start()`

---

**Fortini OS Development Guide**
