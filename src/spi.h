//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### SPI.H #################################
//---------------------------------------------
#ifndef _SPI_H_
#define _SPI_H_

// Peripheral configuration
// Master or Slave
#define SPI_MASTER
// #define SPI_SLAVE

//When to validate the Data sended
// #define SPI_DATA_VALID_ON_RISING_CLK
// #define SPI_DATA_VALID_ON_FALLING_CLK
#define SPI_DATA_VALID_ON_RISING_CLK_DIRECT_POLARITY

// Exported Module Functions ---------------------------------------------------
void SPI1_Config(void);
unsigned char SPI1_Send_Receive (unsigned char);
void SPI1_Wait_Clk256 (void);
void SPI1_Busy_Wait (void);
void SPI1_Send_Multiple (unsigned char);
void SPI1_Send_Single (unsigned char);
unsigned char SPI1_Receive_Single (void);

#endif    /* _SPI_H_ */
