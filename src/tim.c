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
volatile unsigned int tim2_increment = 0;


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

    TIM1->ARR = 2400 - 1;    //48MHz / 2400 = 20KHz

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
    
    // TIM2->SMCR |= TIM_SMCR_SMS_2 |TIM_SMCR_SMS_1 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0;    //reset mode
    // TIM2->SMCR |= TIM_SMCR_SMS_2;    //reset mode link timer1    OJO no anda
    // TIM2->SMCR |= TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1;    //trigger mode link timer1
    // TIM2->SMCR = 0x0000;    //

    // TIM2->CCMR1 = 0x6000;            //CH2 output PWM mode 1
    // TIM2->CCMR1 = 0x0060;            //CH1 output PWM mode 1
    // TIM2->CCER |= TIM_CCER_CC1E | TIM_CCER_CC1P;    //CH1 enable on pin active high
    // TIM2->CCER |= TIM_CCER_CC2E | TIM_CCER_CC2P;    //CH2 enable on pin active high
    
    // TIM2->ARR = 48 - 1;    // 1us @ 48MHz
    TIM2->ARR = 480 - 1;    // 10us @ 48MHz    
    TIM2->CNT = 0;
    TIM2->PSC = 0;

    // Enable timer
    TIM2->DIER |= TIM_DIER_UIE;    // int on update
    TIM2->CR1 |= TIM_CR1_CEN;

    // NVIC Irq Enable
    // Timer2 interrupt
    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_SetPriority(TIM2_IRQn, 6);
}


unsigned short signal_table [] = {524,537,549,562,574,586,599,611,623,636,
                                  648,660,672,684,695,707,719,730,741,752,
                                  763,774,785,795,806,816,826,836,845,855,
                                  864,873,882,890,898,907,914,922,929,936,
                                  943,950,956,962,968,973,979,984,988,993,
                                  997,1000,1004,1007,1010,1013,1015,1017,1019,1020,
                                  1021,1022,1022,1023,1022,1022,1021,1020,1019,1017,
                                  1015,1013,1010,1007,1004,1000,997,993,988,984,
                                  979,973,968,962,956,950,943,936,929,922,
                                  914,907,898,890,882,873,864,855,845,836,
                                  826,816,806,795,785,774,763,752,741,730,
                                  719,707,695,684,672,660,648,636,623,611,
                                  599,586,574,562,549,537,524,512,499,486,
                                  474,461,449,437,424,412,400,387,375,363,
                                  351,339,328,316,304,293,282,271,260,249,
                                  238,228,217,207,197,187,178,168,159,150,
                                  141,133,125,116,109,101,94,87,80,73,
                                  67,61,55,50,44,39,35,30,26,23,
                                  19,16,13,10,8,6,4,3,2,1,
                                  1,1,1,1,2,3,4,6,8,10,
                                  13,16,19,23,26,30,35,39,44,50,
                                  55,61,67,73,80,87,94,101,109,116,
                                  125,133,141,150,159,168,178,187,197,207,
                                  217,228,238,249,260,271,282,293,304,316,
                                  328,339,351,363,375,387,400,412,424,437,
                                  449,461,474,486,499,511};


volatile unsigned short last_value = 0;
volatile unsigned int tim2_cnt = 0;
void TIM2_IRQHandler (void)
{
    //Code Handler    
    if (LED)
        LED_OFF;
    else
        LED_ON;

    tim2_cnt += tim2_increment;

    // right shift precision bits to get table index
    // unsigned char table_index = (unsigned char) (tim2_cnt >> 8);
    unsigned char table_index = (unsigned char) (tim2_cnt >> 12);    

    unsigned short new_value = signal_table[table_index];
    if (new_value != last_value)
    {
        TIM4_Update_CH1(new_value);
        last_value = new_value;
    }
    
    // low flag
    if (TIM2->SR & 0x01)
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
    TIM3->CR1 |= TIM_CR1_OPM;        //clk int / 1; upcounting; one pulse
    //TIM3->CR1 = 0x0000;        //clk int / 1; upcounting;
    //TIM3->CR2 |= TIM_CR2_MMS_1;        //UEV -> TRG0
    TIM3->CR2 = 0x0000;
    //TIM3->SMCR |= TIM_SMCR_SMS_2 |TIM_SMCR_SMS_1 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0;    //reset mode
    //TIM3->SMCR |= TIM_SMCR_SMS_2;    //reset mode link timer1    OJO no anda
    TIM3->SMCR |= TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1;    //trigger mode link timer1
    //TIM3->SMCR = 0x0000;    //
    //TIM3->CCMR1 = 0x6000;            //CH2 output PWM mode 1
    //  TIM3->CCMR1 = 0x0060;            //CH1 output PWM mode 1
    TIM3->CCMR1 = 0x0070;            //CH1 output PWM mode 2 (channel inactive TIM3->CNT < TIM3->CCR1)
    TIM3->CCMR2 = 0x0000;            //
    //  TIM3->CCER |= TIM_CCER_CC1E | TIM_CCER_CC1P;    //CH1 enable on pin active high
    TIM3->CCER |= TIM_CCER_CC1E;    //CH1 enable on pin active high
    //TIM3->CCER |= TIM_CCER_CC2E | TIM_CCER_CC2P;    //CH2 enable on pin active high
    TIM3->ARR = DUTY_50_PERCENT;
    TIM3->CNT = 0;
    TIM3->PSC = 0;
    //TIM3->EGR = TIM_EGR_UG;    //generate event

    // Enable timer ver UDIS
    //TIM3->DIER |= TIM_DIER_UIE;
    TIM3->CR1 |= TIM_CR1_CEN;

    //TIM3->CCR2 = 512;        //delay = TIM3->CCRx = 512 - TIM1->CCR2
    TIM3->CCR1 = (DUTY_50_PERCENT + 1);        //delay = TIM3->CCRx = 512 - TIM1->CCR2
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
