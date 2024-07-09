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
#define HARDWARE_VERSION_1_0

//--- Software ------------------//
// #define SOFTWARE_VERSION_1_2		
// #define SOFTWARE_VERSION_1_1
#define SOFTWARE_VERSION_1_0    // works as a bldc driver for esp32

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
// PA1 input Alternative TIM2_CH2
#define SPEED_CTRL    ((GPIOA->IDR & 0x0002) != 0)

// PA2
#define HALL_FAIL    ((GPIOA->ODR & 0x0004) != 0)
#define HALL_FAIL_ON    (GPIOA->BSRR = 0x00000004)
#define HALL_FAIL_OFF    (GPIOA->BSRR = 0x00040000)

// PA3
// PA4
// PA5    NC

// PA6 CURRENT_CTRL output Alternative TIM3_CH1

// PA7

// PA8 PA9 PA10 Alternative (TIM1_CH1, TIM1_CH2, TIM1_CH3)
// PA12 input alternative TIM1_ETR

// PA15 input
#define BRAKE    ((GPIOA->IDR & 0x8000) != 0)

//--- Port B ---//
// PB0 input
#define HALL_U    ((GPIOB->IDR & 0x0001) != 0)

// PB1 input
#define HALL_V    ((GPIOB->IDR & 0x0002) != 0)

// PB2 input
#define HALL_W    ((GPIOB->IDR & 0x0004) != 0)

// PB3 input Special Function on reset
#define CONTROL_I1    ((GPIOB->IDR & 0x0008) != 0)

// PB4 input Special Function on reset
#define CONTROL_I0    ((GPIOB->IDR & 0x0010) != 0)

// PB5 output
#define LOW_W    ((GPIOB->ODR & 0x0020) != 0)
#define LOW_W_ON    (GPIOB->BSRR = 0x00000020)
#define LOW_W_OFF    (GPIOB->BSRR = 0x00200000)

// PB6 output
#define LOW_V    ((GPIOB->ODR & 0x0040) != 0)
#define LOW_V_ON    (GPIOB->BSRR = 0x00000040)
#define LOW_V_OFF    (GPIOB->BSRR = 0x00400000)

// PB7 output
#define LOW_U    ((GPIOB->ODR & 0x0080) != 0)
#define LOW_U_ON    (GPIOB->BSRR = 0x00000080)
#define LOW_U_OFF    (GPIOB->BSRR = 0x00800000)

// PB8 output
#define SPEED_OUT    ((GPIOB->ODR & 0x0100) != 0)
#define SPEED_OUT_ON    (GPIOB->BSRR = 0x00000100)
#define SPEED_OUT_OFF    (GPIOB->BSRR = 0x01000000)

// PB9 input
#define DIRECTION    ((GPIOB->IDR & 0x0200) != 0)

// PB10
// PB11 NC

// PB14 input
#define ALARMA_ON    ((GPIOB->IDR & 0x4000) != 0)


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
void Hall_Update_Supervisor (void);
unsigned char Hall_U (void);
unsigned char Hall_V (void);
unsigned char Hall_W (void);

void Led_Toggle (unsigned short on_off_time);
void Led_Toggle_Update (void);

void HARD_Timeouts (void);
unsigned char HARD_Check_Current_Config (void);

unsigned char Brake (void);
unsigned char Alarm_On (void);


#endif

//--- end of file ---//
