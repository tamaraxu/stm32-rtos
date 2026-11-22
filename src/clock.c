#include <stdint.h>
#include "stm32f446.h"

void clock_init(void) {
    RCC_APB1ENR |= (1u << 28);
    
    PWR_CR = (PWR_CR & ~(0x3u << 14)) | (0x3u << 14);
    
    RCC_CR |= (1u << 18);
    RCC_CR |= (1u << 16);
    while (!(RCC_CR & (1u << 17)));

    RCC_PLLCFGR = (RCC_PLLCFGR & ~(0x3Fu)) | 0x4;   // set PLL M to 4
    RCC_PLLCFGR = (RCC_PLLCFGR & ~(0x1FFu << 6)) | (180u << 6); // set PLL N to 180
    RCC_PLLCFGR = (RCC_PLLCFGR & ~(0x3u << 16));    // set PLL P to 2 (encoded as 0)
    RCC_PLLCFGR |= (1u << 22);  // set PLL SRC to 1
    RCC_CR |= (1u << 24);   // set PLL ON

    // enable over-drive and check SR
    PWR_CR |= (1u << 16);
    while (!(PWR_CSR & (1u << 16)));

    // enable over-drive switching and check SR
    PWR_CR |= (1u << 17);
    while (!(PWR_CSR & (1u << 17)));

    FLASH_ACR = (FLASH_ACR & ~(0xFu)) | 0x5u;  // set latency to 5
    FLASH_ACR |= (1u << 8);     // set PRFTEN
    FLASH_ACR |= (1u << 9);     // set ICEN
    FLASH_ACR |= (1u << 10);    // set DCEN

    RCC_CFGR = (RCC_CFGR & ~(0xFu << 4));   // HPRE (AHB) = / 1
    RCC_CFGR = (RCC_CFGR & ~(0x7u << 10)) | (0x5u << 10);   // PPRE1 (APB1) = / 4
    RCC_CFGR = (RCC_CFGR & ~(0x7u << 13)) | (0x4u << 13);   // PPRE2 (APB2) = / 2

    // wait for PLL to be ready
    while (!(RCC_CR & (1u << 25)));

    RCC_CFGR = (RCC_CFGR & ~(3u)) | 0x2;    // set PLL as system clock
    // check SWS
    while ((RCC_CFGR & (0x3u << 2)) != (0x2u << 2));
}