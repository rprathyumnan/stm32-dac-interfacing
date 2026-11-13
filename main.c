/*******************************************************************
 * Experiment 11: Interfacing of Internal DAC
 * Board   : LOGSUN STM32 M4 DVK (STM32F407VET6, ARM Cortex-M4)
 * Output  : DAC channel 1 on PA4 -> observe on DSO (PA4 to GND)
 * Toolchain: Keil uVision with Keil.STM32F4xx_DFP 2.13.0
 *
 * NOTE: Reference implementation using direct register access.
 * Replace this file with the exact code you flashed if it differs.
 *******************************************************************/

#include <stm32f4xx.h>

#define DAC_MAX 4095U   /* 12-bit DAC full scale */

static void Delay(volatile unsigned int count)
{
    while (count--) { __NOP(); }
}

static void DAC1_Init(void)
{
    /* Enable clocks: GPIOA (AHB1) and DAC (APB1) */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;

    /* PA4 in analog mode (MODER4 = 0b11), no pull-up/pull-down */
    GPIOA->MODER |= (3U << (4U * 2U));
    GPIOA->PUPDR &= ~(3U << (4U * 2U));

    /* Enable DAC channel 1, output buffer enabled (BOFF1 = 0), no trigger */
    DAC->CR |= DAC_CR_EN1;
}

static inline void DAC1_Write(uint16_t value)
{
    DAC->DHR12R1 = (value & 0x0FFFU);   /* 12-bit right-aligned data */
}

int main(void)
{
    uint16_t i;

    DAC1_Init();

    while (1)
    {
        /* Triangle wave: ramp up then ramp down */
        for (i = 0; i < DAC_MAX; i += 16)
        {
            DAC1_Write(i);
            Delay(50);
        }
        for (i = DAC_MAX; i > 0; i -= 16)
        {
            DAC1_Write(i);
            Delay(50);
        }
    }
}
