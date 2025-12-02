#ifndef EFF_H
#define EFF_H

#include <stdint.h>

/* EFF - Executable For FortiniOS
 * Formato proprietário de executáveis para o Fortini OS
 */

#define EFF_MAGIC 0x45464600  /* "EFF\0" */
#define EFF_VERSION 1

typedef struct {
    uint32_t magic;           /* 0x45464600 */
    uint32_t version;         /* Versão do formato */
    uint32_t entry_point;     /* Endereço de início */
    uint32_t code_offset;     /* Offset do código */
    uint32_t code_size;       /* Tamanho do código */
    uint32_t data_offset;     /* Offset dos dados */
    uint32_t data_size;       /* Tamanho dos dados */
    uint32_t bss_size;        /* Tamanho da BSS */
    uint32_t flags;           /* Flags do executável */
    uint32_t checksum;        /* Checksum CRC32 */
} eff_header_t;

/* Flags */
#define EFF_FLAG_EXECUTABLE 0x00000001
#define EFF_FLAG_SHARED     0x00000002
#define EFF_FLAG_PIE        0x00000004

typedef struct {
    uint32_t name_offset;     /* Offset do nome da seção */
    uint32_t offset;          /* Offset no arquivo */
    uint32_t size;            /* Tamanho da seção */
    uint32_t flags;           /* Flags da seção */
} eff_section_t;

#endif
