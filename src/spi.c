//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### SPI.C #################################
//---------------------------------------------

// Includes --------------------------------------------------------------------
#include "spi.h"
#include "stm32f10x.h"
// #include "hard.h"
  
#include <stdio.h>
#include <string.h>


// Module Private Types Constants and Macros -----------------------------------
//Clock Peripherals
#define RCC_SPI1_CLK    (RCC->APB2ENR & 0x00001000)
#define RCC_SPI1_CLK_ON    (RCC->APB2ENR |= 0x00001000)
#define RCC_SPI1_CLK_OFF    (RCC->APB2ENR &= ~0x00001000)


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------


// Module Exported Functions ---------------------------------------------------
  /* SPI3 parameter configuration*/
  // hspi3.Instance = SPI3;
  // hspi3.Init.Mode = SPI_MODE_MASTER;
  // hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  // hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  // hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  // hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  // hspi3.Init.NSS = SPI_NSS_SOFT;
  // hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  // hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  // hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  // hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  // hspi3.Init.CRCPolynomial = 10;

void SPI1_Config(void)
{
    if (!RCC_SPI1_CLK)
        RCC_SPI1_CLK_ON;

    //Configuracion SPI
    SPI1->CR1 = 0;

#if defined SPI_MASTER
    //SPI speed; clk / 256; master
    SPI1->CR1 |= SPI_CR1_MSTR | SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2;
    //SPI speed; clk / 16; master
    // SPI1->CR1 |=  SPI_CR1_BR_1 | SPI_CR1_BR_0;
    //SPI speed; clk / 8; master
    // SPI1->CR1 |=  SPI_CR1_BR_1;
    //SPI speed; clk / 4; master
    // SPI1->CR1 |=  SPI_CR1_BR_0;
#elif defined SPI_SLAVE
    //SPI speed; clk / 256; slave
    SPI1->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2 |SPI_CR1_SSM;
#else
#error "Select Peripheral use on spi.h"
#endif

#ifdef SPI_DATA_VALID_ON_FALLING_CLK
    //CPOL High; CPHA first clock
    SPI1->CR1 |= SPI_CR1_CPOL | SPI_CR1_SSM | SPI_CR1_SSI;
#endif
#ifdef SPI_DATA_VALID_ON_RISING_CLK
    //CPOL High; CPHA second clock
    SPI1->CR1 |= SPI_CR1_CPOL | SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_CPHA;
#endif
#ifdef SPI_DATA_VALID_ON_RISING_CLK_DIRECT_POLARITY
    //CPOL High; CPHA on riding
    SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
#endif

    // peripheral enable
    SPI1->CR1 |= SPI_CR1_SPE;    
}


unsigned char SPI1_Send_Receive (unsigned char a)
{
    // wait for transmit buffer empty
    while ((SPI1->SR & SPI_SR_TXE) == 0);

    *(__IO uint8_t *) ((uint32_t)SPI1 + (uint32_t)0x0C) = a;    // compiler problems

    // wait for received data
    while ((SPI1->SR & SPI_SR_RXNE) == 0);

    return (SPI1->DR & 0x00FF);
}


void SPI1_Wait_Clk256 (void)
{
    for (unsigned char i = 0; i < 15; i++)
        asm ("nop \n\t");

}


void SPI1_Busy_Wait (void)
{
    // wait for complete transfer
    while ((SPI1->SR & SPI_SR_BSY) != 0);
}


void SPI1_Send_Multiple (unsigned char a)
{
    //espero que haya lugar en el buffer
    while ((SPI1->SR & SPI_SR_TXE) == 0);

    //*(__IO uint8_t *) SPI1->DR = a;
    *(__IO uint8_t *) ((uint32_t)SPI1 + (uint32_t)0x0C) = a; //evito enviar 16bits problemas de compilador

}


void SPI1_Send_Single (unsigned char a)
{
    //espero que se libere el buffer
    while ((SPI1->SR & SPI_SR_TXE) == 0);

    //tengo espacio
    //SPI1->DR = a;
    //SPI1->DR = a;
    *(__IO uint8_t *) ((uint32_t)SPI1 + (uint32_t)0x0C) = a; //evito enviar 16bits problemas de compilador

    //espero que se transfiera el dato
    while ((SPI1->SR & SPI_SR_BSY) != 0);
}


unsigned char SPI1_Receive_Single (void)
{
    unsigned char dummy;

    //espero que se libere el buffer
    while (((SPI1->SR & SPI_SR_TXE) == 0) || ((SPI1->SR & SPI_SR_BSY) != 0));

    //limpio buffer RxFIFO
    while ((SPI1->SR & SPI_SR_RXNE) != 0)
        dummy = SPI1->DR;

    *(__IO uint8_t *) ((uint32_t)SPI1 + (uint32_t)0x0C) = 0xff; //evito enviar 16bits problemas de compilador

    //espero que se transfiera el dato
    while ((SPI1->SR & SPI_SR_BSY) != 0);

    dummy = (unsigned char) SPI1->DR;
    return dummy;
}



//---- End of File ----//
