#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "eff.h"

/* CRC32 implementation para checksum EFF */
static uint32_t crc32_table[256];

void init_crc32_table(void) {
    for (int i = 0; i < 256; i++) {
        uint32_t crc = i;
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320UL;
            } else {
                crc >>= 1;
            }
        }
        crc32_table[i] = crc;
    }
}

uint32_t calculate_crc32(const uint8_t* data, size_t len) {
    uint32_t crc = 0xFFFFFFFFUL;
    for (size_t i = 0; i < len; i++) {
        crc = crc32_table[(crc ^ data[i]) & 0xFF] ^ (crc >> 8);
    }
    return crc ^ 0xFFFFFFFFUL;
}

/* Converter ELF para EFF */
int elf_to_eff(const char* elf_file, const char* eff_file) {
    FILE* in = fopen(elf_file, "rb");
    FILE* out = fopen(eff_file, "wb");
    
    if (!in || !out) {
        fprintf(stderr, "Erro ao abrir arquivo\n");
        return -1;
    }
    
    /* Ler entrada de ELF (simplificado) */
    uint8_t elf_header[64];
    fread(elf_header, 1, 64, in);
    
    /* Criar cabeçalho EFF */
    eff_header_t eff;
    eff.magic = EFF_MAGIC;
    eff.version = EFF_VERSION;
    eff.entry_point = 0x400000;
    eff.code_offset = sizeof(eff_header_t);
    eff.flags = EFF_FLAG_EXECUTABLE;
    
    /* Copiar dados */
    fseek(in, 0, SEEK_END);
    size_t file_size = ftell(in);
    fseek(in, 0, SEEK_SET);
    
    uint8_t* buffer = malloc(file_size);
    fread(buffer, 1, file_size, in);
    
    eff.code_size = file_size - 64;
    eff.checksum = calculate_crc32(buffer, file_size);
    
    fwrite(&eff, sizeof(eff_header_t), 1, out);
    fwrite(buffer + 64, 1, eff.code_size, out);
    
    free(buffer);
    fclose(in);
    fclose(out);
    
    printf("Convertido para EFF: %s\n", eff_file);
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <elf_file> <eff_file>\n", argv[0]);
        return 1;
    }
    
    init_crc32_table();
    return elf_to_eff(argv[1], argv[2]);
}
