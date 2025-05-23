//------------------------------------------------------------------------------
// #### TEMPLATE FOR PROJECTS WITH ARDUINO BLUEPILL - Processor STM32F103C8T6
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### MAIN.C #################################################################
//------------------------------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "stm32f10x.h"
#include "hard.h"
#include "test_functions.h"
#include "tim.h"
#include "gpio.h"
#include "usart.h"
#include "adc.h"
#include "dma.h"

#include "pwm.h"
#include "synchro.h"
#include "accel.h"

#include <stdio.h>
#include <string.h>



// Externals -------------------------------------------------------------------
// -- Externals for timers
volatile unsigned short wait_ms_var = 0;
volatile unsigned short timer_standby = 0;
volatile unsigned short timer_led = 0;

// -- Externals for adc
volatile unsigned short adc_ch [ADC_CHANNEL_QUANTITY];


// Globals ---------------------------------------------------------------------


// Module Private Functions ----------------------------------------------------
void TimingDelay_Decrement(void);
void SysTickError (void);
extern void EXTI0_IRQHandler (void);


// Module Functions ------------------------------------------------------------
int main (void)
{
    // System Clock already configured at this point

    // Gpio Configuration.
    GpioInit ();
    
    // Systick Timer Activation
    // if (SysTick_Config(72000))
    if (SysTick_Config(64000))        
    // if (SysTick_Config(48000))
        SysTickError();

    // Hardware Tests Functions
    // TF_Hardware_Tests ();
    // End of Hardware Tests Functions

    // Hardware Setup
    for (int i = 0; i < 3; i++)
    {
        LED_ON;
        Wait_ms(250);
        LED_OFF;
        Wait_ms(250);
    }
    
    // //-- DMA configuration and Init
    // DMAConfig ();
    // DMA_ENABLE;

    // //-- ADC Init
    // AdcConfig();
    // AdcStart();

    // all channels to 0    
    Low_U_Reset();
    Low_V_Reset();
    Low_W_Reset();
    
    // Timer 1 for PWM outputs
    TIM1_Init();

    // all channels to 0
    Pwm_U(0);
    Pwm_V(0);
    Pwm_W(0); 

    // Timer 3 for Current Setting
    TIM3_Init();
    TIM3_Update_CH1 (250);    // starts on 1/4
    // TIM3_Update_CH1 (1000);    // starts on max 

    // Timer 2 for speed setting input
    TIM2_Init();
    Accel_Setting_Reset ();

    
    while (1)
    {
        Synchro ();

        // Led_Toggle_Update ();

        Hall_Update ();    // check hall changes with fast filter

        // Hall_Update_Supervisor ();    // check hall fail

        Hall_Update_Supervisor_With_Counter ();    // check hall fail with counter	

        Accel_Setting_Update ();
    }

}
//--- End of Main ---//

void TimingDelay_Decrement(void)
{
    if (wait_ms_var)
        wait_ms_var--;

    if (timer_standby)
        timer_standby--;

    if (timer_led)
        timer_led--;

    HARD_Timeouts ();

    Synchro_Timeouts ();

    // Pwm_Setting_Timeout ();

    Accel_Setting_Timeout ();
}


// void EXTI0_IRQHandler (void)
// {
//     if(EXTI->PR & 0x00000001)	//Line0
//     {
//         if (LED)
//             LED_OFF;
//         else
//             LED_ON;

        
//         EXTI->PR |= 0x00000001;
//     }
// }


void SysTickError (void)
{
    //Capture systick error...
    while (1)
    {
        if (LED)
            LED_OFF;
        else
            LED_ON;

        for (unsigned char i = 0; i < 255; i++)
        {
            asm ("nop \n\t"
                 "nop \n\t"
                 "nop \n\t" );
        }
    }
}

//--- end of file ---//

