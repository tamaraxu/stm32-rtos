#ifndef STM32F446_H
#define STM32F446_H
#include <stdint.h>

#define RCC_BASE    0x40023800u
#define GPIOA_BASE  0x40020000u
#define PWR_BASE    0x40007000u
#define FLASH_BASE  0x40023C00u

// blinking LED
#define RCC_AHB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE))
#define GPIOA_ODR   (*(volatile uint32_t *)(GPIOA_BASE + 0x14))

// clock
#define RCC_CR (*(volatile uint32_t *)RCC_BASE)
#define RCC_PLLCFGR (*(volatile uint32_t *)(RCC_BASE + 0x04))
#define RCC_CFGR (*(volatile uint32_t *)(RCC_BASE + 0x08))
#define RCC_APB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x40))
#define PWR_CR (*(volatile uint32_t *)(PWR_BASE))
#define PWR_CSR (*(volatile uint32_t *)(PWR_BASE + 0x04))
#define FLASH_ACR (*(volatile uint32_t *)(FLASH_BASE))
#endif