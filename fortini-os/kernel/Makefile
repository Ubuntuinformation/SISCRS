CC = gcc
AS = nasm
LD = ld
OBJCOPY = objcopy

CFLAGS = -Wall -Wextra -ffreestanding -fno-stack-protector -m32 -O2 -fno-builtin
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T linker.ld -nostdlib

SOURCES = kernel.c
OBJECTS = kernel_start.o kernel.o
TARGET_ELF = kernel.elf
TARGET_BIN = kernel.bin

.PHONY: all clean

all: $(TARGET_BIN)

$(TARGET_BIN): $(TARGET_ELF)
	@echo "[OBJCOPY] Convertendo ELF para binário..."
	$(OBJCOPY) -O binary $< $@

$(TARGET_ELF): $(OBJECTS) linker.ld
	@echo "[LD] Ligando kernel..."
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS)

kernel_start.o: kernel_start.asm
	@echo "[AS] Compilando kernel_start.asm..."
	$(AS) $(ASFLAGS) -o $@ $<

kernel.o: kernel.c
	@echo "[CC] Compilando kernel.c..."
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(TARGET_ELF) $(TARGET_BIN)

.PHONY: all clean
