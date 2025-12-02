# Fortini OS - Download e Instalação

## Download da ISO

A ISO do Fortini OS está disponível no repositório GitHub:

```bash
# Clone o repositório
git clone https://github.com/Ubuntuinformation/SISCRS.git
cd SISCRS

# A ISO estará em:
fortini-os/build/fortini.iso
```

## Testando com QEMU (Linux/Mac/Windows)

### Opção 1: Executar direto do Makefile
```bash
cd fortini-os
make test
```

### Opção 2: QEMU manual
```bash
qemu-system-i386 -cdrom fortini-os/build/fortini.iso -m 512
```

### Opção 3: Com mais recursos
```bash
qemu-system-i386 -cdrom fortini-os/build/fortini.iso -m 1024 -smp 2 -enable-kvm
```

## Testando em Hardware Real

### Criar USB bootável (Linux):
```bash
# Encontre o dispositivo USB
lsblk

# Escrever ISO no USB (substitua sdX pelo seu dispositivo)
sudo dd if=fortini-os/build/fortini.iso of=/dev/sdX bs=4M status=progress
sudo sync
```

### Criar USB bootável (Windows):
1. Baixe o Rufus: https://rufus.ie/
2. Selecione a ISO: `fortini-os/build/fortini.iso`
3. Selecione o USB
4. Clique em "START"

### Criar USB bootável (Mac):
```bash
# Converta IMG primeiro
hdiutil convert -format UDRW -o fortini.img fortini-os/build/fortini.iso

# Encontre o disco
diskutil list

# Desmontar (substitua diskX)
diskutil unmountDisk /dev/diskX

# Escrever
sudo dd if=fortini.img of=/dev/rdiskX bs=1m
```

## Requisitos

### Para rodar em QEMU:
- QEMU instalado
- 512 MB de RAM mínimo
- Processador x86/x64

### Para compilar do zero:
```bash
cd fortini-os
make clean
make all
make iso
```

## Comandos disponíveis no SiscrShell

Após inicializar o sistema, você terá acesso aos seguintes comandos:

- `help` - Mostra ajuda
- `echo` - Exibe um texto
- `pwd` - Diretório atual
- `cd` - Muda de diretório
- `ls` - Lista arquivos
- `mkdir` - Cria diretório
- `rm` - Remove arquivo
- `cat` - Exibe conteúdo de arquivo
- `gcc` - Compilador GCC
- `clang` - Compilador Clang
- `clear` - Limpa tela
- `whoami` - Exibe nome do usuário
- `date` - Exibe data e hora
- `exit` - Sai do shell

## Estrutura da ISO

```
/
├── boot/
│   ├── kernel.elf (kernel Fortini OS)
│   ├── grub/
│   │   └── grub.cfg (configuração GRUB)
├── bin/
│   └── siscrshell (shell principal)
```

## Características

✅ Kernel em Assembly (x86) e C
✅ Modo protegido 32-bit
✅ Shell Unix-like (SiscrShell)
✅ Suporte a GCC e Clang
✅ Formato EFF (Executable for FortiniOS)
✅ Bootável com GRUB
✅ Testável com QEMU
✅ Código aberto no GitHub

## Documentação

- `fortini-os/DEVELOPMENT.md` - Guia de desenvolvimento
- `BUILD_SUMMARY.txt` - Resumo da compilação
- `README.md` - Informações gerais

## Troubleshooting

**QEMU não funciona:**
- Verifique se QEMU está instalado: `qemu-system-i386 --version`
- Tente desabilitar KVM: remova `-enable-kvm`

**ISO não inicia:**
- Verifique se o arquivo existe: `ls -la fortini-os/build/fortini.iso`
- Recompile: `cd fortini-os && make clean && make iso`

**Erro de boot:**
- Isso é normal! O sistema está em desenvolvimento
- Use QEMU para testes

## Contribuindo

O projeto está aberto para contribuições!

1. Fork o repositório
2. Crie uma branch: `git checkout -b feature/sua-feature`
3. Commit suas mudanças: `git commit -am 'Adiciona feature'`
4. Push: `git push origin feature/sua-feature`
5. Abra um Pull Request

## Licença

Este projeto é de código aberto e disponível sob a licença MIT.

## Autores

- Fortini OS Team
- Comunidade SISCRS

---

**Divirta-se explorando o Fortini OS! 🚀**
