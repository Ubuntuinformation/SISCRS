#!/bin/bash

# Script de teste do Fortini OS

echo "╔═══════════════════════════════════════╗"
echo "║     Fortini OS - Test Runner         ║"
echo "╚═══════════════════════════════════════╝"
echo ""

cd "$(dirname "$0")" || exit 1

# Verificar se a ISO existe
if [ ! -f "build/fortini.iso" ]; then
    echo "❌ ISO não encontrada. Gerando..."
    make iso || exit 1
fi

echo "✅ ISO encontrada: build/fortini.iso"
echo ""
echo "📊 Informações do Kernel:"
file kernel/kernel.elf
ls -lh kernel/kernel.elf
echo ""
echo "📊 Informações do Shell:"
file shell/siscrshell
ls -lh shell/siscrshell
echo ""
echo "📊 Informações da ISO:"
ls -lh build/fortini.iso
echo ""

echo "🚀 Iniciando QEMU..."
echo "   Comando: qemu-system-i386 -cdrom build/fortini.iso -m 512"
echo "   Pressione Ctrl+C para sair"
echo ""

# Iniciar QEMU em modo gráfico (se disponível) ou sem display
if command -v qemu-system-i386 &> /dev/null; then
    qemu-system-i386 -cdrom build/fortini.iso -m 512 -boot d
else
    echo "❌ QEMU não encontrado"
    exit 1
fi
