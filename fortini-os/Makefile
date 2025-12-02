.PHONY: all kernel shell bootloader iso clean test qemu help

BOOTLOADER_DIR = bootloader
KERNEL_DIR = kernel
SHELL_DIR = shell-rs
BUILD_DIR = build

all: bootloader kernel shell iso

# Compile bootloader (real x86 MBR + stage2)
bootloader:
	@echo "=== Compilando Bootloader ==="
	$(MAKE) -C $(BOOTLOADER_DIR)

# Compile kernel (protected mode + drivers)
kernel: bootloader
	@echo "=== Compilando Kernel ==="
	$(MAKE) -C $(KERNEL_DIR)

# Compile shell (Rust)
shell:
	@echo "=== Compilando Shell Rust ==="
	cd $(SHELL_DIR) && cargo build --release 2>&1 | grep -v "warning: unused"

# Generate ISO bootable image with El-Torito
iso: bootloader kernel shell
	@echo "=== Gerando ISO Bootável ==="
	@mkdir -p /tmp/iso_staging/boot
	@cp $(BOOTLOADER_DIR)/boot.img /tmp/iso_staging/boot/
	@cp $(KERNEL_DIR)/kernel.bin /tmp/iso_staging/boot/
	@mkdir -p /tmp/iso_staging/bin
	@cp $(SHELL_DIR)/target/release/siscrshell-rs /tmp/iso_staging/bin/
	@mkdir -p $(BUILD_DIR)
	@xorriso -as mkisofs \
		-b boot/boot.img \
		-no-emul-boot \
		-boot-load-size 4 \
		-o $(BUILD_DIR)/fortini.iso \
		/tmp/iso_staging 2>&1 | grep -v "Number of files:"
	@rm -rf /tmp/iso_staging
	@echo "ISO criada em: $(BUILD_DIR)/fortini.iso"
	@ls -lh $(BUILD_DIR)/fortini.iso

# Boot ISO in QEMU
qemu: iso
	@echo "=== Iniciando QEMU (pressione Ctrl+A depois X para sair) ==="
	qemu-system-i386 -cdrom $(BUILD_DIR)/fortini.iso -m 512 -serial stdio -nographic

# Clean all artifacts
clean:
	@echo "=== Limpando artefatos ==="
	$(MAKE) -C $(BOOTLOADER_DIR) clean 2>/dev/null || true
	$(MAKE) -C $(KERNEL_DIR) clean 2>/dev/null || true
	cd $(SHELL_DIR) && cargo clean 2>/dev/null || true
	rm -rf $(BUILD_DIR)
	rm -rf /tmp/iso_staging

help:
	@echo "Fortini OS v2.0 - Compilação"
	@echo ""
	@echo "Alvos disponíveis:"
	@echo "  make all           - Compila tudo (padrão)"
	@echo "  make bootloader    - Compila bootloader customizado"
	@echo "  make kernel        - Compila kernel com drivers"
	@echo "  make shell         - Compila shell em Rust"
	@echo "  make iso           - Gera ISO bootável"
	@echo "  make qemu          - Testa ISO no QEMU"
	@echo "  make clean         - Remove arquivos compilados"
	@echo "  make help          - Exibe esta mensagem"
	@echo ""
	@echo "Exemplo: make qemu (compila e executa no QEMU)"
