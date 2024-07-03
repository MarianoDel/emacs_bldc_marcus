//---------------------------------------------
// #### MARCUS BLDC MOTOR - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### HARD.H #################################
//---------------------------------------------

#ifndef _HARD_H_
#define _HARD_H_


// Defines For Configuration ---------------------------------------------------

//----- Board Configuration -------------------//
//--- Hardware ------------------//
#define HARDWARE_VERSION_1_0        //placa Arduino Blue Pill

//--- Software ------------------//
// #define SOFTWARE_VERSION_1_2		
// #define SOFTWARE_VERSION_1_1	     //habla contra pc o rpi con nuevo protocolo camilla
#define SOFTWARE_VERSION_1_0        //habla contra rpi con programa magneto y traduce a micros potencia

//-------- Type of Program (depending on software version) ----------------

//-------- Type of Program and Features ----------------

//-------- Kind of Reports Sended ----------------

//-------- Others Configurations depending on the formers ------------

//-------- Hysteresis Conf ------------------------

//-------- PWM Conf ------------------------



//-------- End Of Defines For Configuration ------




//--- Hardware & Software Messages ------------------//
#ifdef HARDWARE_VERSION_1_0
#define HARD "Hardware Version: 1.0\r\n"
#endif
#ifdef HARDWARE_VERSION_2_1
#define HARD "Hardware Version: 2.1\r\n"
#endif
#ifdef SOFTWARE_VERSION_2_2
#define SOFT "Software Version: 2.2\r\n"
#endif
#ifdef SOFTWARE_VERSION_1_0
#define SOFT "Software Version: 1.0\r\n"
#endif
#ifdef SOFTWARE_VERSION_1_1
#define SOFT "Software Version: 1.1\r\n"
#endif
//--- Type of Program Announcement ----------------

//--- End of Hardware & Software Messages ------------------//



// Module Exported Constants ---------------------------------------------------
typedef enum {
	resp_ok = 0,
	resp_not_own,
	resp_error

} resp_t;


// Gpios Configuration ---------------------------------------------------------
#ifdef HARDWARE_VERSION_1_0

//--- Port A ---//
// PA1 input
#define HALL_U    ((GPIOA->IDR & 0x0002) != 0)

// PA2 input
#define HALL_V    ((GPIOA->IDR & 0x0004) != 0)

// PA3 input
#define HALL_W    ((GPIOA->IDR & 0x0008) != 0)

//PA4 SPI_CS
// #define SPI_CS    ((GPIOA->ODR & 0x0010) == 0)
// #define SPI_CS_OFF    (GPIOA->BSRR = 0x00000010)
// #define SPI_CS_ON    (GPIOA->BSRR = 0x00100000)

// PA8 PA9 PA10 Alterantive (TIM1_CH1, TIM1_CH2, TIM1_CH3)
// PA12 input alternative TIM1_ETR


//--- Port B ---//
// PB3 output
// PB4 output    Special Function

// PB5 output
#define PB5    ((GPIOB->ODR & 0x0020) != 0)
#define PB5_ON    (GPIOB->BSRR = 0x00000020)
#define PB5_OFF    (GPIOB->BSRR = 0x00200000)

// PB6 output
#define PB6    ((GPIOB->ODR & 0x0040) != 0)
#define PB6_ON    (GPIOB->BSRR = 0x00000040)
#define PB6_OFF    (GPIOB->BSRR = 0x00400000)

// PB7 output
#define PB7    ((GPIOB->ODR & 0x0080) != 0)
#define PB7_ON    (GPIOB->BSRR = 0x00000080)
#define PB7_OFF    (GPIOB->BSRR = 0x00800000)

//PB8 
// #define P0_14 ((GPIOB->ODR & 0x0100) != 0)
// #define P0_14_ON (GPIOB->BSRR = 0x00000100)
// #define P0_14_OFF (GPIOB->BSRR = 0x01000000)

//PB9
// #define RESET ((GPIOB->ODR & 0x0200) != 0)
// #define RESET_ON (GPIOB->BSRR = 0x00000200)
// #define RESET_OFF (GPIOB->BSRR = 0x02000000)

//PB10, PB11 Alternative Usart 3 Tx Rx


//--- Port C ---//
//PC13
#define LED    ((GPIOC->ODR & 0x2000) == 0)
#define LED_OFF    (GPIOC->BSRR = 0x00002000)
#define LED_ON    (GPIOC->BSRR = 0x20000000)


#endif //HARDWARE_VERSION_1_0


// Module Exported Functions ---------------------------------------------------
void Pb5_On (void);
void Pb6_On (void);
void Pb7_On (void);

void Pb5_Off (void);
void Pb6_Off (void);
void Pb7_Off (void);

void Hall_Update (void);
unsigned char Hall_U (void);
unsigned char Hall_V (void);
unsigned char Hall_W (void);

void Led_Toggle (unsigned short on_off_time);
void Led_Toggle_Update (void);
// void Hard_Timeouts (void);

#endif

//--- end of file ---//
