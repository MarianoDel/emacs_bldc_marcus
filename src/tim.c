//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### TIM.C #################################
//---------------------------------------------

// Includes --------------------------------------------------------------------
#include "tim.h"
#include "stm32f10x.h"
#include "hard.h"
#include "accel.h"


// Module Private Types Constants and Macros -----------------------------------
#define RCC_TIM1_CLK    (RCC->APB2ENR & 0x00000800)
#define RCC_TIM1_CLKEN    (RCC->APB2ENR |= 0x00000800)
#define RCC_TIM1_CLKDIS    (RCC->APB2ENR &= ~0x00000800)

#define RCC_TIM2_CLK    (RCC->APB1ENR & 0x00000001)
#define RCC_TIM2_CLKEN    (RCC->APB1ENR |= 0x00000001)
#define RCC_TIM2_CLKDIS    (RCC->APB1ENR &= ~0x00000001)

#define RCC_TIM3_CLK    (RCC->APB1ENR & 0x00000002)
#define RCC_TIM3_CLKEN    (RCC->APB1ENR |= 0x00000002)
#define RCC_TIM3_CLKDIS    (RCC->APB1ENR &= ~0x00000002)

#define RCC_TIM4_CLK    (RCC->APB1ENR & 0x00000004)
#define RCC_TIM4_CLKEN    (RCC->APB1ENR |= 0x00000004)
#define RCC_TIM4_CLKDIS    (RCC->APB1ENR &= ~0x00000004)

#ifdef STM32F10X_HD
#define RCC_TIM5_CLK    (RCC->APB1ENR & 0x00000008)
#define RCC_TIM5_CLKEN    (RCC->APB1ENR |= 0x00000008)
#define RCC_TIM5_CLKDIS    (RCC->APB1ENR &= ~0x00000008)

#define RCC_TIM6_CLK    (RCC->APB1ENR & 0x00000010)
#define RCC_TIM6_CLKEN    (RCC->APB1ENR |= 0x00000010)
#define RCC_TIM6_CLKDIS    (RCC->APB1ENR &= ~0x00000010)

#define RCC_TIM7_CLK    (RCC->APB1ENR & 0x00000020)
#define RCC_TIM7_CLKEN    (RCC->APB1ENR |= 0x00000020)
#define RCC_TIM7_CLKDIS    (RCC->APB1ENR &= ~0x00000020)
#endif


// Externals -------------------------------------------------------------------
extern volatile unsigned short wait_ms_var;


// Globals ---------------------------------------------------------------------
// volatile unsigned short pwm_input_period = 0;
// volatile unsigned short pwm_input_duty = 0;
// volatile unsigned char pwm_input_int = 0;


// Module Functions ------------------------------------------------------------
void Wait_ms (unsigned short a)
{
    wait_ms_var = a;
    while (wait_ms_var);
}

// inline void UpdateTIMSync (unsigned short a)
// {
//     //primero cargo TIM1
//     TIM1->CCR1 = a;
//     TIM3->ARR = DUTY_50_PERCENT + a;    //TIM3->CCR1 es el delay entre timers
//                                         //lo cargo en el timer init
// }

// inline void UpdateTIM_MosfetA (unsigned short a)
// {
//     TIM3->ARR = DUTY_50_PERCENT + a;    
// }

// inline void UpdateTIM_MosfetB (unsigned short a)
// {
//     TIM1->CCR1 = a;
// }

// inline void EnablePreload_MosfetA (void)
// {
//     // TIM3->CCMR1 |= TIM_CCMR1_OC1PE;
//     TIM3->CR1 |= TIM_CR1_ARPE;
// }

// inline void DisablePreload_MosfetA (void)
// {
//     // TIM3->CCMR1 &= ~TIM_CCMR1_OC1PE;
//     TIM3->CR1 &= ~TIM_CR1_ARPE;    
// }

// inline void EnablePreload_MosfetB (void)
// {
//     TIM1->CCMR1 |= TIM_CCMR1_OC1PE;
// }

// inline void DisablePreload_MosfetB (void)
// {
//     TIM1->CCMR1 &= ~TIM_CCMR1_OC1PE;
// }


////////////////////
// TIM1 Functions //
////////////////////
void TIM1_Init (void)    //for pwm
{
    if (!RCC_TIM1_CLK)
        RCC_TIM1_CLKEN;

    //Configuracion del timer.
    TIM1->CR1 = 0x00;        //clk int / 1;
    TIM1->CR2 = 0x00;
    TIM1->SMCR = TIM_SMCR_ETF_1 | TIM_SMCR_ETF_0;    // ETR filtered N = 8

    TIM1->CCMR1 = 0x6060;    //CH1 CH2 output PWM mode 1 (channel active TIMx->CNT < TIMx->CCRx)
    TIM1->CCMR2 = 0x0060;    //CH3 output PWM mode 1 (channel active TIMx->CNT < TIMx->CCRx)

    TIM1->CCMR1 |= TIM_CCMR1_OC2CE | TIM_CCMR1_OC1CE;    // CH2 CH1 clear enable
    TIM1->CCMR2 |= TIM_CCMR2_OC3CE;    // CH3 clear enable    
    

    TIM1->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E;        
    TIM1->BDTR |= TIM_BDTR_MOE;    

    TIM1->ARR = 1000 - 1;    // 16MHz / 1000 = 16KHz
    TIM1->PSC = 4 - 1;    // 64MHz / 4 = 16MHz
    TIM1->CNT = 0;

    TIM1->CR1 |= TIM_CR1_CEN;
}


void TIM1_Update_CH1 (unsigned short a)
{
    TIM1->CCR1 = a;
}


void TIM1_Update_CH2 (unsigned short a)
{
    TIM1->CCR2 = a;
}


void TIM1_Update_CH3 (unsigned short a)
{
    TIM1->CCR3 = a;
}


// void TIM_1_Init (void)    //for one pulse mode
// {
//     if (!RCC_TIM1_CLK)
//         RCC_TIM1_CLKEN;

//     //Configuracion del timer.
//     TIM1->CR1 |= TIM_CR1_OPM;        //clk int / 1; upcounting; one pulse
//     // TIM1->CR1 = 0x00;        //clk int / 1;
//     // TIM1->CR1 |= TIM_CR1_ARPE;        //clk int / 1, Preload;
//     // TIM1->CR2 |= TIM_CR2_MMS_1;        //UEV -> TRG0
//     TIM1->CR2 = 0x00;
//     //TIM1->SMCR |= TIM_SMCR_MSM | TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1 | TIM_SMCR_TS_1;    //link timer3
//     TIM1->SMCR = 0x0000;

//     TIM1->CCMR1 = 0x0060;            //CH1 output PWM mode 1 (channel active TIM1->CNT < TIM1->CCR1)
// #ifdef USE_CHANNELS_WITH_PRELOAD
//     TIM1->CCMR1 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE;
// #endif
//     TIM1->CCMR2 = 0x0000;
//     // TIM1->CCER |= TIM_CCER_CC1NE | TIM_CCER_CC1NP | TIM_CCER_CC2NE | TIM_CCER_CC2NP;
//     TIM1->CCER |= TIM_CCER_CC1E;
        
//     TIM1->BDTR |= TIM_BDTR_MOE;
//     TIM1->ARR = DUTY_100_PERCENT;    //cada tick 20.83ns con PSC = 0

//     TIM1->CNT = 0;
// // #if defined USE_FREQ_18KHZ
// //     TIM1->PSC = 3;
// // #elif defined USE_FREQ_9KHZ
// //     TIM1->PSC = 7;
// // #elif defined USE_FREQ_4_5KHZ
// //     TIM1->PSC = 15;
// // #else
// // #error "Select working frequency on hard.h"
// // #endif

//     // Enable timer ver UDIS
//     //TIM1->DIER |= TIM_DIER_UIE;
//     // TIM1->CR1 |= TIM_CR1_CEN;

//     TIM1->CCR1 = 0;
// }
void TIM1_OPM_us (unsigned short a)
{
    TIM1->CCR1 = a;
    ENABLE_TIM1;
}




////////////////////
// TIM2 Functions //
////////////////////
void TIM2_Init (void)
{
    if (!RCC_TIM2_CLK)
        RCC_TIM2_CLKEN;

    //Configuracion del timer.
    TIM2->CR1 = 0x0000;        //clk int / 1; upcounting;
    TIM2->CR2 = 0x0000;

    // pwm input mode (input on CH2), pwm freq on CH2, duty on CH1
    TIM2->SMCR = TIM_SMCR_SMS_2 | TIM_SMCR_TS_2 | TIM_SMCR_TS_1;    //reset mode on TI2FP2 trigger
    
    TIM2->CCMR1 = TIM_CCMR1_CC2S_0 | TIM_CCMR1_CC1S_1;    //CH2 input TI2, CH1 input on TI2FP1
    TIM2->CCMR1 |= TIM_CCMR1_IC2F_3 | TIM_CCMR1_IC2F_2 | TIM_CCMR1_IC2F_1 | TIM_CCMR1_IC2F_0;    // filtered
    TIM2->CCMR2 |= 0x0070;    // ch3 pwm mode2
    // TIM2->CCMR2 |= 0x0010;    // ch3 active on match
    // TIM2->CCMR2 = 0x0000;    //     
    
    TIM2->CCER = TIM_CCER_CC2E | TIM_CCER_CC1E | TIM_CCER_CC1P;    //CH2 CH1 enable capture, CH1 on falling
    // TIM2->CCER = TIM_CCER_CC3E;    //CH3 output
    
    TIM2->ARR = 0xFFFF;    // tops 16bits (65ms on 1MHz) (1.36ms on 48MHz)
    TIM2->CNT = 0;
    TIM2->PSC = 64 - 1;    // 1us counter

    TIM2->CCR3 = 5000;    // 200Hz min pwm input

    // Enable timer
    TIM2->DIER |= TIM_DIER_CC2IE | TIM_DIER_CC3IE;    // int on CH2 capture, int on CH3 capture (for min pwm)
    // TIM2->DIER |= TIM_DIER_CC3IE;    // int on CH2 capture, int on CH3 capture (for min pwm)
    // TIM2->DIER |= TIM_DIER_CC2IE;    // int on CH2 capture, int on CH3 capture (for min pwm)    
    // TIM2->DIER |= TIM_DIER_UIE;    // int on update
    TIM2->CR1 |= TIM_CR1_CEN;

    // NVIC Irq Enable
    // Timer2 interrupt
    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_SetPriority(TIM2_IRQn, 6);
}


void TIM2_IRQHandler (void)
{
    //Code Handler    
    // if (TIM2->SR & TIM_SR_UIF)
    //     TIM2->SR = 0x00;
    
    // low flag
    if (TIM2->SR & TIM_SR_CC2IF)
    {
        // pwm_input_period = TIM2->CCR2;
        // pwm_input_duty = TIM2->CCR1;
        Accel_Set_Values (TIM2->CCR2, TIM2->CCR1);
    }
    else if (TIM2->SR & TIM_SR_CC3IF)
    {
	TIM2->SR = 0x00;
	// TIM2->CNT = 0;
	TIM2->EGR |= TIM_EGR_UG;
        Accel_Set_Values (0, 0);

	if (Led_Is_On())
	    Led_Off();
	else
	    Led_On();
    }

    // if (pwm_input_int < 4)
    //     pwm_input_int++;
    // else
    //     pwm_input_int = 0;
    
    TIM2->SR = 0x00;
}


////////////////////
// TIM3 Functions //
////////////////////
void TIM3_Init (void)
{
    if (!RCC_TIM3_CLK)
        RCC_TIM3_CLKEN;

    //Configuracion del timer.
    TIM3->CR1 = 0x0000;        //clk int / 1; upcounting;
    TIM3->CR2 = 0x0000;

    //TIM3->SMCR |= TIM_SMCR_SMS_2 |TIM_SMCR_SMS_1 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0;    //reset mode
    //TIM3->SMCR |= TIM_SMCR_SMS_2;    //reset mode link timer1    OJO no anda
    // TIM3->SMCR |= TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1;    //trigger mode link timer1
    TIM3->SMCR = 0x0000;
    
    TIM3->CCMR1 = 0x0060;    //CH1 output PWM mode 1 (channel active TIM3->CNT < TIM3->CCR1)
    TIM3->CCMR2 = 0x0000;    //

    //  TIM3->CCER |= TIM_CCER_CC1E | TIM_CCER_CC1P;    //CH1 enable on pin active high
    TIM3->CCER |= TIM_CCER_CC1E;    //CH1 enable on pin active high
    //TIM3->CCER |= TIM_CCER_CC2E | TIM_CCER_CC2P;    //CH2 enable on pin active high

    TIM3->ARR = 1000 - 1;
    TIM3->CNT = 0;
    TIM3->PSC = 64 - 1;    // 1us tick
    //TIM3->EGR = TIM_EGR_UG;    //generate event

    // Enable timer ver UDIS
    //TIM3->DIER |= TIM_DIER_UIE;
    TIM3->CR1 |= TIM_CR1_CEN;

    //TIM3->CCR2 = 512;        //delay = TIM3->CCRx = 512 - TIM1->CCR2
    // TIM3->CCR1 = (DUTY_50_PERCENT + 1);        //delay = TIM3->CCRx = 512 - TIM1->CCR2
}


void TIM3_Update_CH1 (unsigned short a)
{
    TIM3->CCR1 = a;
}


void TIM3_Update_CH2 (unsigned short a)
{
    TIM3->CCR2 = a;
}


void TIM3_Update_CH3 (unsigned short a)
{
    TIM3->CCR3 = a;
}


void TIM3_Update_CH4 (unsigned short a)
{
    TIM3->CCR4 = a;
}


void TIM3_IRQHandler (void)	//1 ms
{
    //Code Handler
    
    //bajar flag
    if (TIM3->SR & 0x01)	//bajo el flag
        TIM3->SR = 0x00;
}


////////////////////
// TIM4 Functions //
////////////////////
void TIM4_Init (void)
{
    //---- Clk ----//
    if (!RCC_TIM4_CLK)
        RCC_TIM4_CLKEN;

    //--- Config ----//
    TIM4->CR1 = 0x00;
    TIM4->CR2 = 0x00;
    
    TIM4->CCMR1 = 0x0060;    //CH1 output PWM mode 2 (channel active TIM4->CNT < TIM4->CCR1)
    // TIM4->CCMR1 |= TIM_CCMR1_OC1PE;    // preload enable
    TIM4->CCER |= TIM_CCER_CC1E;
    
    TIM4->ARR = 1023;    //1023 pwm points freq = 48MHz / 1024 = 46.8KHz
    TIM4->PSC = 0;

    TIM4->EGR = TIM_EGR_UG; //update registers

    // Enable timer ver UDIS
    TIM4->CR1 |= TIM_CR1_CEN;
}


void TIM4_Delay_us (unsigned short a)
{
    TIM4->CNT = 0;
    while (a > TIM4->CNT);
}


void TIM4_Update_CH1 (unsigned short a)
{
    TIM4->CCR1 = a;
}

// TIM4 DMA Configs.
void TIM4_Init_DMA_Config (void)
{
    //---- Clk ----//
    if (!RCC_TIM4_CLK)
        RCC_TIM4_CLKEN;

    //--- Config ----//
    TIM4->CR1 = 0x00;
    TIM4->CR2 = 0x00;
    
    TIM4->CCMR1 = 0x0060;    //CH1 output PWM mode 2 (channel active TIM4->CNT < TIM4->CCR1)
    // TIM4->CCMR1 |= TIM_CCMR1_OC1PE;    // preload enable    
    TIM4->CCER |= TIM_CCER_CC1E;
    
    TIM4->ARR = 1023;    //1023 pwm points freq = 48MHz / 1024 = 46.8KHz
    TIM4->PSC = 0;

    // DMA Config
    TIM4->DIER |= TIM_DIER_UDE;    //DMA request on update
    TIM4->DCR |= TIM_DCR_DBL_0;    // 1 transfer
    TIM4->DCR |= (0x34 >> 2);    //TIM4_CCR1 register
        
    // Enable timer ver UDIS
    TIM4->CR1 |= TIM_CR1_CEN;
}


#ifdef STM32F10X_HD
// void TIM6_Init(void)
// {
    // //---- Clk ----//
    // if (!RCC_TIM6_CLK)
    //     RCC_TIM6_CLKEN;

// 	//--- Config ----//
// 	TIM6->ARR = 10000; //100mS.
// 	TIM6->CNT = 0;
// 	TIM6->PSC = 719;
// 	TIM6->EGR = TIM_EGR_UG;

// 	// Enable timer ver UDIS
// 	TIM6->DIER |= TIM_DIER_UIE;
// 	TIM6->CR1 |= TIM_CR1_CEN;

// }

// void TIM6_IRQHandler (void)	//100mS
// {
// 	UART_Tim6 ();

// 	//bajar flag
// 	if (TIM6->SR & 0x01)	//bajo el flag
// 		TIM6->SR = 0x00;
// }


//inicializo el TIM7 para interrupciones
void TIM7_Init(void)
{
//    Counter Register (TIMx_CNT)
//    Prescaler Register (TIMx_PSC)
//    Auto-Reload Register (TIMx_ARR)
//    The counter clock frequency CK_CNT is equal to fCK_PSC / (PSC[15:0] + 1)
//
//    Quiero una interrupcion por ms CK_INT es 72MHz

    //---- Clk ----//
    if (!RCC_TIM7_CLK)
        RCC_TIM7_CLKEN;

    //--- Config ----//
    TIM7->ARR = 48 - 1;    // 1us @ 48MHz
    TIM7->CNT = 0;
    TIM7->PSC = 0;

    // Enable timer ver UDIS
    TIM7->DIER |= TIM_DIER_UIE;
    TIM7->CR1 |= TIM_CR1_CEN;

    //Habilito NVIC
    //Interrupcion timer7.
    NVIC_EnableIRQ(TIM7_IRQn);
    NVIC_SetPriority(TIM7_IRQn, 10);
}


void TIM7_IRQHandler (void)	//1mS
{
    if (TIM7->SR & 0x01)
        TIM7->SR = 0x00;    //bajar flag

    if (LED2)
        LED2_OFF;
    else
        LED2_ON;
    
}
#endif    //STM32F10X_HD

//--- end of file ---//
