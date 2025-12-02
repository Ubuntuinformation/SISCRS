OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(kernel_entry)

SECTIONS {
    . = 0x0;
    
    .text ALIGN(0x1000) : {
        *(.text)
        *(.rodata)
    }
    
    .data ALIGN(0x1000) : {
        *(.data)
    }
    
    .bss ALIGN(0x1000) : {
        *(COMMON)
        *(.bss)
    }
}
