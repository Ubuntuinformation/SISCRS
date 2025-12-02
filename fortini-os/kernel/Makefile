CC = gcc
AS = nasm
LD = ld
CFLAGS = -Wall -Wextra -ffreestanding -fno-stack-protector -m32 -O2
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T linker.ld -nostdlib

SOURCES = kernel.c
OBJECTS = kernel.o kernel_asm.o
TARGET = kernel.elf

all: $(TARGET)

$(TARGET): $(OBJECTS) linker.ld
	@echo "[LD] Ligando kernel..."
	$(LD) $(LDFLAGS) -o $@ kernel_asm.o kernel.o

kernel_asm.o: kernel.asm
	@echo "[AS] Compilando Assembly..."
	$(AS) $(ASFLAGS) -o $@ kernel.asm

kernel.o: kernel.c
	@echo "[CC] Compilando kernel.c..."
	$(CC) $(CFLAGS) -c -o $@ kernel.c

clean:
	rm -f $(OBJECTS) $(TARGET)

install: $(TARGET)
	mkdir -p /tmp/iso/boot
	cp $(TARGET) /tmp/iso/boot/

.PHONY: all clean install
