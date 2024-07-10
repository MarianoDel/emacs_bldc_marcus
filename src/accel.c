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
#include "accel.h"
// #include "hard.h"
// #include "stm32f10x.h"
// #include "tim.h"
#include "dsp.h"
// #include "adc.h"


// Module Private Types & Macros -----------------------------------------------
// #define SOFT_PWM_STEPS    256


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
ma16_u16_data_obj_t input_filter;
unsigned short accel_duty_filtered = 0;
volatile unsigned short accel_input_timeout = 0;
volatile unsigned short accel_input_filter_timer = 0;
volatile unsigned short accel_duty = 0;
volatile unsigned short accel_period = 0;



// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
void Accel_Setting_Timeout (void)
{
    if (accel_input_timeout)
        accel_input_timeout--;

    if (accel_input_filter_timer)
        accel_input_filter_timer--;
}


void Accel_Setting_Update (void)
{
    if (!accel_input_filter_timer)
    {
        accel_input_filter_timer = 5;
        accel_duty_filtered = MA16_U16Circular (&input_filter, accel_duty);
    }
}


void Accel_Setting_Reset (void)
{
    MA16_U16Circular_Reset (&input_filter);
}


unsigned short Accel_Get_Duty (void)
{
    if (!accel_input_timeout)
        return 0;

    if (accel_duty_filtered < ACCEL_MIN_FOR_START)    // 10% min
        return 0;
    
    return accel_duty_filtered - ACCEL_MIN_FOR_START;
}


void Accel_Set_Values (unsigned short period, unsigned short duty)
{
    accel_period = period;
    accel_duty = duty;

    accel_input_timeout = 500;    // 500ms reload
    
}
//--- end of file ---//
