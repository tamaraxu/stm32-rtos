#include <stdint.h>

#define RCC_AHB1ENR (*(volatile uint32_t *)0x40023830)
#define GPIOA_MODER (*(volatile uint32_t *)0x40020000)
#define GPIOA_ODR   (*(volatile uint32_t *)0x40020014)

int main(void) {

    RCC_AHB1ENR |= 1u;
    // GPIOA_MODER &= ~(0x3u << 10);
    // GPIOA_MODER |= (0x1u << 10);
    GPIOA_MODER = (((GPIOA_MODER) & ~(0x3u << 10)) | (0x1u << 10));

    for (;;) {
        // runs forever
        GPIOA_ODR ^= (0x1 << 5u);

        for (volatile uint32_t i = 0; i < 500000; i++) {}
    }
}