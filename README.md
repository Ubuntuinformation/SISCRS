# Fortini OS - Sistema Operacional Livre

Um sistema operacional de código aberto desenvolvido com C, Assembly e ferramentas modernas.

## Características

- **Bootloader**: GRUB com suporte a Multiboot
- **Kernel**: Modo protegido 32-bit
- **Shell**: SiscrShell com 13+ comandos
- **Compiladores**: GCC e Clang suportados
- **Executáveis**: Formato EFF proprietário

## Início Rápido

```bash
cd fortini-os
make all      # Compila tudo
make iso      # Gera ISO
make test     # Testa em QEMU (ou qemu-system-i386 -cdrom build/fortini.iso -m 512)
```

## Estrutura

```
fortini-os/
├── bootloader/    # GRUB
├── kernel/        # C + Assembly 32-bit
├── shell/         # SiscrShell
├── libc/          # Biblioteca C
└── utils/         # Ferramentas (conversor EFF)
```

## Compiladores

✅ GCC, Clang, NASM, QEMU instalados
✅ Kernel compilado com sucesso
✅ Shell compilado com sucesso  
✅ ISO gerada com sucesso

## Status

| Componente | Status |
|-----------|--------|
| Bootloader | ✅ Funcional |
| Kernel | ✅ Modo protegido |
| Shell | ✅ 13+ comandos |
| ISO | ✅ Bootável |
| Sistema de Arquivos | ⏳ Planejado |
| Multitarefa | ⏳ Planejado |

---

**Fortini OS** - SO livre, modular e educacional