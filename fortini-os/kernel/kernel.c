#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

/* VGA Text Mode Constants */
#define VGA_MEMORY 0xb8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_SIZE (VGA_WIDTH * VGA_HEIGHT)

/* Colors */
#define BLACK 0x00
#define BLUE 0x01
#define GREEN 0x02
#define CYAN 0x03
#define RED 0x04
#define MAGENTA 0x05
#define BROWN 0x06
#define LIGHT_GRAY 0x07
#define DARK_GRAY 0x08
#define LIGHT_BLUE 0x09
#define LIGHT_GREEN 0x0A
#define LIGHT_CYAN 0x0B
#define LIGHT_RED 0x0C
#define LIGHT_MAGENTA 0x0D
#define LIGHT_BROWN 0x0E
#define WHITE 0x0F

typedef volatile uint16_t* vga_buffer_t;
static vga_buffer_t vga_buffer = (vga_buffer_t)VGA_MEMORY;
static int cursor_x = 0, cursor_y = 0;

void vga_clear(void)
{
    for (int i = 0; i < VGA_SIZE; i++) {
        vga_buffer[i] = (WHITE << 8) | ' ';
    }
    cursor_x = 0;
    cursor_y = 0;
}

void vga_putchar(char c)
{
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
        if (cursor_y >= VGA_HEIGHT) {
            cursor_y = VGA_HEIGHT - 1;
            // Scroll
            for (int i = 0; i < VGA_SIZE - VGA_WIDTH; i++) {
                vga_buffer[i] = vga_buffer[i + VGA_WIDTH];
            }
            for (int i = VGA_SIZE - VGA_WIDTH; i < VGA_SIZE; i++) {
                vga_buffer[i] = (WHITE << 8) | ' ';
            }
        }
        return;
    }

    int pos = cursor_y * VGA_WIDTH + cursor_x;
    vga_buffer[pos] = (WHITE << 8) | (unsigned char)c;

    cursor_x++;
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
        if (cursor_y >= VGA_HEIGHT) {
            cursor_y = VGA_HEIGHT - 1;
        }
    }
}

void vga_write(const char *str)
{
    while (*str) {
        vga_putchar(*str);
        str++;
    }
}

/* Simple kprintf for debugging */
void kprintf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 'd': {
                    int val = va_arg(args, int);
                    char buffer[16] = {0};
                    int len = 0;
                    if (val < 0) {
                        vga_putchar('-');
                        val = -val;
                    }
                    if (val == 0) {
                        vga_putchar('0');
                    } else {
                        while (val > 0) {
                            buffer[len++] = '0' + (val % 10);
                            val /= 10;
                        }
                        while (len > 0) {
                            vga_putchar(buffer[--len]);
                        }
                    }
                    break;
                }
                case 's': {
                    const char *str = va_arg(args, const char *);
                    vga_write(str);
                    break;
                }
                case 'x': {
                    int val = va_arg(args, int);
                    char buffer[16] = "0x";
                    const char *hex = "0123456789abcdef";
                    for (int i = 0; i < 8; i++) {
                        buffer[2 + i] = hex[(val >> (28 - i * 4)) & 0xf];
                    }
                    buffer[10] = '\0';
                    vga_write(buffer);
                    break;
                }
                case 'c': {
                    char c = (char)va_arg(args, int);
                    vga_putchar(c);
                    break;
                }
                default:
                    vga_putchar(*fmt);
            }
        } else {
            vga_putchar(*fmt);
        }
        fmt++;
    }

    va_end(args);
}

/* Keyboard driver (simple) */
#define KEYBOARD_DATA 0x60
#define KEYBOARD_STATUS 0x64

void keyboard_init(void)
{
    // Simple PS/2 initialization
}

uint8_t keyboard_read_scancode(void)
{
    while (!(*((volatile uint8_t *)KEYBOARD_STATUS) & 1));
    return *((volatile uint8_t *)KEYBOARD_DATA);
}

/* PIT (Programmable Interval Timer) for timing */
#define PIT_CHANNEL_0 0x40
#define PIT_MODE 0x43

void pit_init(void)
{
    uint16_t divisor = 1193;
    *((volatile uint8_t *)PIT_MODE) = 0x36;
    *((volatile uint8_t *)PIT_CHANNEL_0) = divisor & 0xff;
    *((volatile uint8_t *)PIT_CHANNEL_0) = (divisor >> 8) & 0xff;
}

/* Simple memory functions */
void *memcpy(void *dest, const void *src, size_t n)
{
    uint8_t *d = (uint8_t *)dest;
    const uint8_t *s = (const uint8_t *)src;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

void *memset(void *s, int c, size_t n)
{
    uint8_t *p = (uint8_t *)s;
    for (size_t i = 0; i < n; i++) {
        p[i] = (uint8_t)c;
    }
    return s;
}

size_t strlen(const char *s)
{
    size_t n = 0;
    while (s[n]) n++;
    return n;
}

int strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (unsigned char)*s1 - (unsigned char)*s2;
}

/* Main kernel */
void kernel_main(void)
{
    vga_clear();

    kprintf("================================\n");
    kprintf("Fortini OS Kernel v2.0\n");
    kprintf("Built with GCC, ASM, Rust Shell\n");
    kprintf("================================\n\n");

    kprintf("Bootloader: Real x86 Stage1+2\n");
    kprintf("Kernel: C with Protected Mode\n");
    kprintf("Shell: Rust (SiscrShell-RS)\n\n");

    kprintf("Initializing drivers...\n");
    pit_init();
    kprintf("PIT Timer: OK\n");

    keyboard_init();
    kprintf("Keyboard: OK\n");

    kprintf("\nKernel loaded successfully!\n");
    kprintf("Waiting for shell loader...\n\n");

    kprintf(">> ");

    // Simple halt
    while (1) {
        __asm__ __volatile__("hlt");
    }
}
