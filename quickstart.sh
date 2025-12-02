#!/bin/bash

# Script de Quick Start para Fortini OS
# Baixa a ISO do GitHub e testa com QEMU


# Verificar se a ISO existe
if [ -f "fortini-os/build/fortini.iso" ]; then
    ISO_SIZE=$(ls -lh fortini-os/build/fortini.iso | awk '{print $5}')
    echo "📀 ISO encontrada! (Tamanho: $ISO_SIZE)"
    echo ""
    
    # Oferecer opções
    if command -v qemu-system-i386 &> /dev/null; then
        echo "Opções disponíveis:"
        echo "1. Testar com QEMU agora"
        echo "2. Apenas baixar (sem testar)"
        echo "3. Compilar do zero"
        echo ""
        read -p "Escolha uma opção (1-3): " choice
        
        case $choice in
            1)
                echo ""
                echo "🚀 Iniciando QEMU..."
                echo "   (Pressione Ctrl+A depois X para sair)"
                echo ""
                qemu-system-i386 -cdrom fortini-os/build/fortini.iso -m 512
                ;;
            2)
                echo "✅ ISO pronta em: fortini-os/build/fortini.iso"
                ;;
            3)
                echo "🔨 Compilando do zero..."
                cd fortini-os
                make clean
                make all
                make iso
                echo "✅ Compilação concluída!"
                ;;
            *)
                echo "Opção inválida"
                exit 1
                ;;
        esac
    else
        echo "✅ ISO pronta para download em: fortini-os/build/fortini.iso"
        echo ""
        echo "Próximos passos:"
        echo "1. Instale QEMU"
        echo "2. Execute: qemu-system-i386 -cdrom fortini-os/build/fortini.iso -m 512"
    fi
else
    echo "❌ ISO não encontrada!"
    echo ""
    echo "Compilando do zero..."
    cd fortini-os
    make clean
    make all
    make iso
fi

echo ""
echo "📚 Para mais informações, veja DOWNLOAD_ISO.md"
echo "🌐 GitHub: https://github.com/Ubuntuinformation/SISCRS"
