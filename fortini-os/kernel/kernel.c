#include <stddef.h>
#include <stdint.h>

/* Multiboot header structure */
struct multiboot_info {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t syms[4];
    uint32_t mmap_length;
    uint32_t mmap_addr;
};

/* VGA text mode buffer */
#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

typedef struct {
    uint8_t character;
    uint8_t color;
} vga_entry_t;

static vga_entry_t* vga_buffer = (vga_entry_t*) VGA_ADDRESS;
static size_t terminal_row = 0;
static size_t terminal_column = 0;

/* Cores VGA */
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

static uint8_t make_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

void terminal_clear(void) {
    uint8_t color = make_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[y * VGA_WIDTH + x] = (vga_entry_t) {
                character: ' ',
                color: color,
            };
        }
    }
    terminal_row = 0;
    terminal_column = 0;
}

void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        terminal_row++;
        if (terminal_row >= VGA_HEIGHT) {
            terminal_row = VGA_HEIGHT - 1;
            // Scroll
            for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
                for (size_t x = 0; x < VGA_WIDTH; x++) {
                    vga_buffer[y * VGA_WIDTH + x] = vga_buffer[(y + 1) * VGA_WIDTH + x];
                }
            }
            uint8_t color = make_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
            for (size_t x = 0; x < VGA_WIDTH; x++) {
                vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = (vga_entry_t) {
                    character: ' ',
                    color: color,
                };
            }
        }
        return;
    }
    
    uint8_t color = make_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    size_t index = terminal_row * VGA_WIDTH + terminal_column;
    vga_buffer[index] = (vga_entry_t) {
        character: c,
        color: color,
    };
    
    terminal_column++;
    if (terminal_column >= VGA_WIDTH) {
        terminal_column = 0;
        terminal_row++;
        if (terminal_row >= VGA_HEIGHT) {
            terminal_row = VGA_HEIGHT - 1;
        }
    }
}

void terminal_write_string(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        terminal_putchar(str[i]);
    }
}

void _start(struct multiboot_info* __attribute__((unused)) multiboot) {
    terminal_clear();
    terminal_write_string("=== Fortini OS ===\n");
    terminal_write_string("Kernel iniciado em modo protegido!\n\n");
    terminal_write_string("Sistema de arquivos: Fortini OS\n");
    terminal_write_string("Shell: SiscrShell\n");
    terminal_write_string("Compiladores: GCC, Clang\n");
    terminal_write_string("Formato executavel: EFF (Executable for FortiniOS)\n\n");
    terminal_write_string("Inicializando...\n");
    
    while (1) {
        __asm__ __volatile__("hlt");
    }
}
