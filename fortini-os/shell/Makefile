CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
LDFLAGS = 

TARGET = siscrshell
SOURCE = main.c
OBJECT = main.o

all: $(TARGET)

$(TARGET): $(OBJECT)
	@echo "[LD] Ligando shell..."
	$(CC) $(LDFLAGS) -o $@ $(OBJECT)

$(OBJECT): $(SOURCE)
	@echo "[CC] Compilando shell..."
	$(CC) $(CFLAGS) -c -o $@ $(SOURCE)

install: $(TARGET)
	mkdir -p /tmp/iso/bin
	cp $(TARGET) /tmp/iso/bin/

clean:
	rm -f $(OBJECT) $(TARGET)

.PHONY: all clean install
