#include <stdint.h>

extern uint32_t _estack;
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

int main(void);

void Reset_Handler(void)
{
    // copy initial values of globals from FLASH to SRAM
    uint32_t *src = &_sidata;   // start of initial data
    uint32_t *dst = &_sdata;    // start of data
    // copy initial data to data while we haven't reached end of data
    while (dst < &_edata) {
        *dst = *src;
        dst++;
        src++;
    }

    // zero out uninitialized globals
    dst = &_sbss;   // start of bss
    // zero out bss while we haven't reached the end
    while (dst < &_ebss) {
        *dst = 0;
        dst++;
    }

    // run the main application
    main();

    // safety net in case main() returns
    for (;;) {
        // loop forever
    }
}

void Default_Handler(void)
{
    for (;;) {
        // loop forever
    }
}

void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));


// vector table is an array of handler addresses that the CPU reads at reset
// section attribute puts it in .isr_vector, which linker.ld places at start of FLASH
__attribute__((section(".isr_vector"), used))
void (*const vector_table[])(void) = {  // defining vector_table as an array of constant pointers to functions
                                            // functions that take no arguments and return nothing
    (void (*)(void))&_estack,   // [0] is initial stack pointer
    Reset_Handler,              // [1] is reset (first code CPU runs)
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0, 0, 0, 0,
    SVC_Handler,
    DebugMon_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler,
};