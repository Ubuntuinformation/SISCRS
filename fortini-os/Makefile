.PHONY: all kernel shell bootloader iso clean test

KERNEL_DIR = kernel
SHELL_DIR = shell
BOOTLOADER_DIR = bootloader
ISO_DIR = iso
BUILD_DIR = build

all: kernel shell bootloader iso

kernel:
	@echo "=== Compilando kernel ==="
	$(MAKE) -C $(KERNEL_DIR)

shell:
	@echo "=== Compilando shell ==="
	$(MAKE) -C $(SHELL_DIR)

bootloader:
	@echo "=== Configurando bootloader ==="
	$(MAKE) -C $(BOOTLOADER_DIR)

install: kernel shell
	@mkdir -p /tmp/iso/boot
	@mkdir -p /tmp/iso/bin
	@mkdir -p /tmp/iso/boot/grub
	@cp $(KERNEL_DIR)/kernel.elf /tmp/iso/boot/
	@cp $(SHELL_DIR)/siscrshell /tmp/iso/bin/
	@cp $(BOOTLOADER_DIR)/grub.cfg /tmp/iso/boot/grub/ 2>/dev/null || true

iso: install
	@echo "=== Gerando ISO ==="
	@mkdir -p $(BUILD_DIR)
	@grub-mkrescue -o $(BUILD_DIR)/fortini.iso /tmp/iso/ 2>/dev/null || \
	  xorriso -as mkisofs -R -b boot/grub/i386-pc/eltorito.img \
	    -no-emul-boot -boot-load-size 4 -boot-info-table \
	    -o $(BUILD_DIR)/fortini.iso /tmp/iso/
	@echo "ISO criada em: $(BUILD_DIR)/fortini.iso"

test: iso
	@echo "=== Testando com QEMU ==="
	qemu-system-i386 -cdrom $(BUILD_DIR)/fortini.iso -m 512

clean:
	$(MAKE) -C $(KERNEL_DIR) clean
	$(MAKE) -C $(SHELL_DIR) clean
	$(MAKE) -C $(BOOTLOADER_DIR) clean
	rm -rf $(BUILD_DIR)
	rm -rf /tmp/iso

help:
	@echo "Fortini OS - Compilação"
	@echo ""
	@echo "Alvos disponíveis:"
	@echo "  make all       - Compila tudo"
	@echo "  make kernel    - Compila o kernel"
	@echo "  make shell     - Compila o shell"
	@echo "  make bootloader- Configura o bootloader"
	@echo "  make iso       - Gera a ISO"
	@echo "  make test      - Testa com QEMU"
	@echo "  make clean     - Remove arquivos compilados"
	@echo "  make help      - Exibe esta mensagem"
