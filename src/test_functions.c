//------------------------------------------------
// ## Internal Test Functions Module
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### TEST_FUNCTIONS.C #########################
//------------------------------------------------

// Includes --------------------------------------------------------------------
#include "test_functions.h"
#include "stm32f10x.h"
#include "hard.h"
#include "tim.h"
#include "usart.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "spi.h"

#include <stdio.h>
#include <string.h>
#include <math.h>



// Module Private Types Constants & Macros -------------------------------------


// Externals -------------------------------------------------------------------
extern volatile unsigned char adc_int_seq_ready;
extern volatile unsigned short adc_ch [];
extern volatile unsigned short pwm_input_period;
extern volatile unsigned short pwm_input_duty;
extern volatile unsigned char pwm_input_int;



// Globals ---------------------------------------------------------------------


// Module Private Functions ----------------------------------------------------
void TF_Led (void);
void TF_Timer1_Ch1_Ch2_Ch3_Pwm (void);
void TF_Adc_Led_test1 (void);
void TF_Adc_Led_test2 (void);
void Pb5_Pb6_Pb7_On_Off (void);
void TF_Tim2_Pwm_Input (void);
void TF_Tim2_Pwm_Input_Tim3 (void);


// Module Functions ------------------------------------------------------------
void TF_Hardware_Tests (void)
{
    // TF_Led ();

    // TF_Timer1_Ch1_Ch2_Ch3_Pwm ();

    // TF_Adc_Led_test2 ();

    // TF_Adc_Led_test1 ();

    // Pb5_Pb6_Pb7_On_Off ();

    // TF_Tim2_Pwm_Input ();

    TF_Tim2_Pwm_Input_Tim3 ();
}


void TF_Led (void)
{
    while (1)
    {
        if (LED)
            LED_OFF;
        else
            LED_ON;

        Wait_ms(500);
    }
}


void TF_Timer1_Ch1_Ch2_Ch3_Pwm (void)
{
    // Activate Pulses on Timer1
    TIM1_Init();
    TIM1_Update_CH1(1800);    // pwm = 3/4
    TIM1_Update_CH2(1200);    // pwm = 2/4
    TIM1_Update_CH3(600);    // pwm = 1/4
    
    while (1)
    {
        if (LED)
            LED_OFF;
        else
            LED_ON;

        Wait_ms(330);
    }
}


void TF_Adc_Led_test1 (void)
{
    //-- DMA configuration and Init
    DMAConfig ();
    DMA_ENABLE;

    //-- ADC Init
    AdcConfig();
    AdcStart();
    
    int in_on = 0;
    while (1)
    {
        in_on = Sense_Pote;

        LED_ON;
        Wait_ms(in_on);
        LED_OFF;
        Wait_ms(500);
    }
}


void TF_Adc_Led_test2 (void)
{
    //-- DMA configuration and Init
    DMAConfig ();
    DMA_ENABLE;

    //-- ADC Init
    AdcConfig();
    AdcStart();
    
    unsigned short in_on = 0;
    unsigned short in_off = 0;
    while (1)
    {
        in_on = Sense_Pote;

        if (in_on > 4095)
            in_on = 4095;
        
        in_off = 4095 - in_on;
        
        LED_ON;
        Wait_ms(in_on);
        LED_OFF;
        Wait_ms(in_off);
    }
}


void Pb5_Pb6_Pb7_On_Off (void)
{

    for (int i = 0; i < 3; i++)
    {
        LED_ON;
        Wait_ms(250);
        LED_OFF;
        Wait_ms(250);
    }
    
    while (1)
    {
        Low_U_Set ();
        Low_V_Set ();
        Low_W_Set ();
        Wait_ms (5000);
    
        Low_U_Reset ();
        Low_V_Reset ();
        Low_W_Reset ();
        Wait_ms (5000);
    }
}


void TF_Tim2_Pwm_Input (void)
{
    TIM2_Init();

    while (1)
    {
        if (pwm_input_int < 2)
        {
            SPEED_OUT_ON;
        }
        else
            SPEED_OUT_OFF;

        // Wait_ms(1);
        // LOW_U_ON;
        // Wait_ms(1);
        // LOW_U_OFF;
    }
}


void TF_Tim2_Pwm_Input_Tim3 (void)
{
    // cnt for pwm output updates
    unsigned char input_cnt = 0;
    
    // tim2 pwm input
    TIM2_Init();

    // tim3 pwm output
    TIM3_Init();

    while (1)
    {
        if (pwm_input_int)
        {
            pwm_input_int = 0;
            input_cnt++;
            if (SPEED_OUT)
                SPEED_OUT_OFF;
            else
                SPEED_OUT_ON;
            
        }

        if (input_cnt >= 10)
        {
            input_cnt = 0;
            TIM3_Update_CH1(pwm_input_duty);
        }
    }
}


//--- end of file ---//
