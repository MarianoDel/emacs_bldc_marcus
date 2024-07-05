//----------------------------------------------------------
// #### MARCUS BLDC MOTOR - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### PWM.C ##############################################
//----------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "pwm.h"
#include "hard.h"
#include "stm32f10x.h"
#include "tim.h"
#include "dsp.h"
#include "adc.h"


// Module Private Types & Macros -----------------------------------------------
// #define SOFT_PWM_STEPS    256


// Externals -------------------------------------------------------------------
extern volatile unsigned short adc_ch [];

// Globals ---------------------------------------------------------------------
ma16_u16_data_obj_t pote_filter;


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
void Pwm_U (unsigned short a)
{
    TIM1_Update_CH3 (a);
}


void Pwm_V (unsigned short a)
{
    TIM1_Update_CH2 (a);
}


void Pwm_W (unsigned short a)
{
    TIM1_Update_CH1 (a);
}


void Low_U_Reset (void)
{
    // Pb5_Off();
    LOW_U_OFF;
}


void Low_V_Reset (void)
{
    // Pb6_Off();
    LOW_V_OFF;
}


void Low_W_Reset (void)
{
    // Pb7_Off();
    LOW_W_OFF;
}


void Low_U_Set (void)
{
    // Pb5_On();
    LOW_U_ON;
}


void Low_V_Set (void)
{
    // Pb6_On();
    LOW_V_ON;
}


void Low_W_Set (void)
{
    // Pb7_On();
    LOW_W_ON;
}


volatile unsigned short filter_timer = 0;
unsigned short pwm_set_filtered = 0;
unsigned short Pwm_Setting (void)
{
    return (pwm_set_filtered >> 1);
}


void Pwm_Setting_Update (void)
{
    if (!filter_timer)
    {
        filter_timer = 5;
        pwm_set_filtered = MA16_U16Circular (&pote_filter, Sense_Pote);
    }
}


void Pwm_Setting_Update_Reset (void)
{
    MA16_U16Circular_Reset (&pote_filter);
}


void Pwm_Setting_Timeout (void)
{
    if (filter_timer)
        filter_timer--;
}

//--- end of file ---//
