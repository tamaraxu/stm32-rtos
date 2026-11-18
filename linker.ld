ENTRY(Reset_Handler)

/* Define physical memory regions */
MEMORY {
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
    SRAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 128K
}

/* Define top of stack at the end of RAM */
/* (stack grows downward) */
_estack = ORIGIN(SRAM) + LENGTH(SRAM);

/* Map input sections to output memory regions */
SECTIONS {
    .text :
    {
        . = ALIGN(4);           /* forces 4 byte alignment */
        KEEP(*(.isr_vector))    /* prevent linker from removing unused vector table */
        *(.text*)               /* all program code */
        *(.rodata*)             /* read only constants */
        . = ALIGN(4);
        _etext = .;             /* end of .text in FLASH */
    } > FLASH

    _sidata = LOADADDR(.data);  /* stores FLASH address where .data is saved before being copied to RAM */
    .data :
    {
        . = ALIGN(4);
        _sdata = .;             /* start of .data in RAM */
        *(.data*)               /* initialized global/static variables */
        . = ALIGN(4);
        _edata = .;             /* end of .data in RAM */
    } > SRAM AT > FLASH

    .bss :
    {
        . = ALIGN(4);
        _sbss = .;              /* start of .bss in RAM */
        *(.bss*)                /* uninitialized global/static variables */
        . = ALIGN(4);
        _ebss = .;              /* end of .bss in RAM */
    } > SRAM
}