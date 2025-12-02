# Makefile para bootloader Fortini real (stage1+2)

AS = nasm
ASFLAGS = -f bin

TARGET = boot.img
SOURCE = boot.asm

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCE)
	@echo "[AS] Compilando bootloader..."
	$(AS) $(ASFLAGS) -o $@ $(SOURCE)

clean:
	rm -f $(TARGET)
