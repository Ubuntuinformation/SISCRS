#!/bin/bash

# Fortini OS v2.0 - Quickstart Build & Test

set -e

echo "╔════════════════════════════════════════════════╗"
echo "║    Fortini OS v2.0 - Build & Boot in QEMU     ║"
echo "╚════════════════════════════════════════════════╝"
echo

# Check dependencies
echo "[1/4] Verificando dependências..."
for cmd in gcc nasm ld objcopy xorriso qemu-system-i386 cargo; do
    if ! command -v $cmd &> /dev/null; then
        echo "❌ Faltando: $cmd"
        echo "   Ubuntu: sudo apt install build-essential nasm qemu-system-x86 xorriso cargo"
        exit 1
    fi
done
echo "✅ Todas as dependências instaladas"
echo

# Build
echo "[2/4] Compilando Bootloader, Kernel e Shell..."
cd fortini-os
make clean > /dev/null 2>&1
make iso > /dev/null 2>&1
echo "✅ Build completo!"
echo

# Verify
echo "[3/4] Verificando artefatos..."
ls -lh bootloader/boot.img kernel/kernel.bin shell-rs/target/release/siscrshell-rs build/fortini.iso
echo

# Boot
echo "[4/4] Iniciando QEMU..."
echo "────────────────────────────────────────────────"
echo "Instruções:"
echo "  - Aguarde boot do kernel"
echo "  - Shell Rust será exibido"
echo "  - Digite: help (para listar comandos)"
echo "  - Digite: exit (para sair)"
echo "  - Pressione: Ctrl+A depois X (para fechar QEMU)"
echo "────────────────────────────────────────────────"
echo

make qemu

echo
echo "✅ Teste concluído!"
