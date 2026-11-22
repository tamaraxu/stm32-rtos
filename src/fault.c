#include <stdint.h>

#define CFSR    (*(volatile uint32_t *)0xE000ED28)
#define HFSR    (*(volatile uint32_t *)0xE000ED2C)
#define MMFAR   (*(volatile uint32_t *)0xE000ED34)
#define BFAR    (*(volatile uint32_t *)0xE000ED38)

typedef struct {
    uint32_t r0, r1, r2, r3, r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t xpsr;
    uint32_t sp;
    uint32_t cfsr;
    uint32_t hfsr;
    uint32_t mmfar;
    uint32_t bfar;
} fault_info_t;

volatile fault_info_t fault_reg;

// Receives a pointer to the stacked registers.
void hardfault_c(uint32_t *frame) {
    fault_reg.r0 = frame[0];
    fault_reg.r1 = frame[1];
    fault_reg.r2 = frame[2];
    fault_reg.r3 = frame[3];
    fault_reg.r12 = frame[4];
    fault_reg.lr = frame[5];
    fault_reg.pc = frame[6];
    fault_reg.xpsr = frame[7];
    fault_reg.sp = (uint32_t)frame;
    fault_reg.cfsr = CFSR;
    fault_reg.hfsr = HFSR;
    fault_reg.mmfar = MMFAR;
    fault_reg.bfar = BFAR;

    __asm volatile ("bkpt #0"); // halt here if a debugger is attached
    for (;;) {} // never return
}

// Entry point for HardFault. Finds the exception stack frame and
// passes it to hardfault_c(). Declared naked so the compiler adds no
// setup code that would change SP or LR before we read them.
__attribute__((naked))
void HardFault_Handler(void)
{
    __asm volatile (
        "tst   lr, #4      \n"   // test bit 2 of EXC_RETURN (which stack was in use?)
        "ite   eq          \n"   // if-then-else block for the next two instructions
        "mrseq r0, msp     \n"   // bit 2 == 0: frame is on the main stack (MSP)
        "mrsne r0, psp     \n"   // bit 2 == 1: frame is on the process stack (PSP)
        "b     hardfault_c \n"   // jump to hardfault_c(r0 = frame pointer)
    );
}