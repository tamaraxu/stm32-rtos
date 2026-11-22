#include <stdint.h>
#include "stm32f446.h"
#include "clock.h"

int main(void) {

    // initialize clocks
    clock_init();

    // turns on clock for GPIOA
    RCC_AHB1ENR |= 1u;
    // makes PA5 an output
    GPIOA_MODER = (((GPIOA_MODER) & ~(0x3u << 10)) | (0x1u << 10));

    // runs forever
    for (;;) {
        // toggles LED2
        GPIOA_ODR ^= (0x1u << 5);
        // delay
        for (volatile uint32_t i = 0; i < 500000; i++) {}
    }
}