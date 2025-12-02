# Fortini OS v2.0 - Custom x86 Bootloader + Kernel + Rust Shell

Um sistema operacional x86 completo compilado do zero com:
- **Bootloader**: Real x86 MBR (512 bytes) + Stage2 em Assembly puro
- **Kernel**: Modo protegido 32-bit com drivers (VGA, Keyboard, PIT, kprintf)
- **Shell**: SiscrShell v2.0 em Rust com 25+ comandos
- **Build System**: Make total (sem GRUB, sem dependências Linux)

## Compilação

### Pré-requisitos
```bash
sudo apt update
sudo apt install -y gcc g++ make nasm gdb qemu-system-x86 xorriso binutils gcc-multilib
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
source $HOME/.cargo/env
```

### Build
```bash
cd fortini-os
make all        # Compila tudo
make iso        # Gera ISO bootável (build/fortini.iso)
make qemu       # Executa no QEMU
```

## Estrutura do Projeto

```
fortini-os/
├── bootloader/
│   ├── boot.asm         # Bootloader real x86 (MBR + Stage2)
│   ├── boot.img         # Compilado: flat binary 512+ bytes
│   └── Makefile         # Compila para flat binary via NASM
├── kernel/
│   ├── kernel_start.asm # Entrada de Assembly (ponto de entrada)
│   ├── kernel.c         # Kernel com VGA, keyboard, PIT, kprintf
│   ├── linker.ld        # Linker script para flat binary
│   ├── kernel.bin       # Compilado: flat binary
│   └── Makefile         # Compila C → ELF → binary
├── shell-rs/
│   ├── src/main.rs      # SiscrShell v2.0 em Rust (25+ comandos)
│   ├── Cargo.toml       # Dependências Rust
│   └── target/release/  # Executável compilado
├── Makefile             # Makefile principal (orquestra tudo)
└── build/
    └── fortini.iso      # ISO bootável final
```

## Componentes

### Bootloader (`bootloader/boot.asm`)
- Entra em modo real (MBR at 0x7C00)
- Ativa linha A20 (via PS/2 controller)
- Carrega GDT (Global Descriptor Table)
- Entra em modo protegido 32-bit
- Salta para kernel em 0x10000 (endereço físico)
- Tamanho: ~512 bytes (flat binary)

**Compilação:**
```bash
nasm -f bin bootloader/boot.asm -o bootloader/boot.img
```

### Kernel (`kernel/kernel.c`)
- Entrada: `kernel_entry` (Assembly) → `kernel_main` (C)
- Drivers integrados:
  - **VGA**: Framebuffer em modo texto (80x25), cores suportadas
  - **kprintf**: Printf com %d, %s, %x, %c para debug
  - **Keyboard**: Leitura de scancode PS/2
  - **PIT**: Timer programável (1ms tick)
- Sem dependências externas (ffreestanding)
- Compilado para flat binary via objcopy

**Compilação:**
```bash
cd kernel
gcc -ffreestanding -fno-stack-protector -m32 -c kernel.c -o kernel.o
nasm -f elf32 kernel_start.asm -o kernel_start.o
ld -m elf_i386 -T linker.ld -nostdlib -o kernel.elf kernel_start.o kernel.o
objcopy -O binary kernel.elf kernel.bin
```

### Shell Rust (`shell-rs/src/main.rs`)
25+ comandos implementados:

**Navegação & Filesystem:**
- `pwd` - Print working directory
- `cd <dir>` - Change directory
- `ls [dir]` - List files
- `mkdir <dir>` - Create directory
- `rm <file>` - Remove file
- `touch <file>` - Create empty file
- `cat <file>` - Display file contents
- `cp <src> <dst>` - Copy file
- `mv <src> <dst>` - Move/rename file
- `chmod <mode> <f>` - Change permissions
- `chown <owner> <f>` - Change owner

**System Info:**
- `echo <text>` - Print text
- `whoami` - Current user
- `uname [-a]` - System info
- `date` - System date/time
- `uptime` - System uptime
- `ps` - Running processes
- `kill <pid>` - Terminate process

**Network:**
- `ifconfig` - Network configuration
- `ping <host>` - Ping host (simulator)
- `ssh <host>` - SSH placeholder
- `telnet <host>` - Telnet placeholder

**Development:**
- `gcc` - GCC compiler (placeholder)
- `clang` - Clang compiler (placeholder)
- `make` - GNU Make (placeholder)
- `gdb` - Debugger (placeholder)
- `valgrind` - Memory profiler (placeholder)
- `strace` - System call tracer (placeholder)

**Other:**
- `help` - List all commands
- `version` - Version info
- `clear` - Clear screen
- `export <var>` - Environment variables
- `set` - Shell options
- `exit`/`quit` - Exit shell

**Compilação:**
```bash
cd shell-rs
cargo build --release
# Binário: target/release/siscrshell-rs (501K)
```

## Execução

### Gerar ISO
```bash
make iso
# Saída: build/fortini.iso (876K)
```

### Executar em QEMU
```bash
make qemu
# Ctrl+A depois X para sair do QEMU
```

### Carregar em Máquina Real (USB)
```bash
dd if=build/fortini.iso of=/dev/sdX bs=4M
# Substituir sdX com seu dispositivo USB
```

## Arquitetura & Layout de Memória

```
Real Mode (MBR):           Protected Mode:        Kernel:
0x0000:0x0000 ─────┐       GDT loaded at 0x1000   0x10000
              BIOS │       A20 habilitado        kernel_entry
0x7C00 ───── Boot  │                            kernel_main
       code 512B   │       Switch to PM           +
(MBR)         │       ├─► 0x8000 (stage2)        VGA @ 0xB8000
0x7E00 ─────┘       │       ├─► IDT @ 0x10200    PS/2 @ 0x60/0x64
              Data  │                            PIT @ 0x40/0x43
0x20000 ────────────┘      Stack @ 0x20000
```

## Troubleshooting

### Boot não funciona
- Verificar que boot.img tem exatamente 512 bytes (ou múltiplo)
- Verificar MBR signature 0xAA55 em offset 510-511
- Testar: `hexdump -C bootloader/boot.img | tail -1`

### Kernel não carrega
- Verificar que kernel.bin é flat binary (não ELF)
- Testar: `file kernel/kernel.bin` (deve ser "data")
- Aumentar debug em boot.asm (adicionar prints)

### Shell não aparece
- Verificar que siscrshell-rs está em /bin/ na ISO
- Testar: `xorriso -indev build/fortini.iso -lsl /` em outro terminal

### QEMU não inicia
- Testar: `qemu-system-i386 -version`
- Aumentar memória: `-m 1024` em vez de `-m 512`

## Próximos Passos

- [ ] Implementar sistemas de arquivos (FAT12, ext2)
- [ ] Memory management (paging, segmentação)
- [ ] Multitarefa e scheduling
- [ ] Drivers de rede (E1000, Intel 82540)
- [ ] Compilador integrado (TinyC, Lua)
- [ ] Suporte a modo 64-bit (x86_64)

## Referências

- Intel 80386 Manual
- OSDev.org Barebones
- Writing a Simple Operating System from Scratch (Nick Blundell)
- The Rust Programming Language Book
- GNU Make Manual

## Licença

MIT License - Fortini OS v2.0

---

**Autor:** SISCRS Team  
**Data:** Dezembro 2024  
**Versão:** 2.0  
**Status:** Bootable Proof of Concept ✅
