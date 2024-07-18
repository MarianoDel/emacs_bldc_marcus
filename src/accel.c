//----------------------------------------------------------
// #### MARCUS BLDC MOTOR - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### ACCEL.C ############################################
//----------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "accel.h"
#include "dsp.h"


// Module Private Types & Macros -----------------------------------------------


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
ma16_u16_data_obj_t input_filter;
unsigned short accel_duty_filtered = 0;
volatile unsigned short accel_input_timeout = 0;
volatile unsigned short accel_input_filter_timer = 0;
volatile unsigned short accel_duty = 0;


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

        // if we are getting values, use it, else input will be 0
        if (accel_input_timeout)
            accel_duty_filtered = MA16_U16Circular (&input_filter, accel_duty);
        else
            accel_duty_filtered = MA16_U16Circular (&input_filter, 0);
    }
}


void Accel_Setting_Reset (void)
{
    MA16_U16Circular_Reset (&input_filter);
}


unsigned short Accel_Get_Duty (void)
{
    if (accel_duty_filtered < ACCEL_MIN_FOR_START)    // 5% min
        return 0;
    
    return accel_duty_filtered - ACCEL_MIN_FOR_START;    // 95% max
}


void Accel_Set_Values (unsigned short period, unsigned short duty)
{
    unsigned int calc = 0;

    if (period)
    {
        calc = duty * 1000;
        calc = calc / period;
    }
    else
        calc = 0;
    
    // accel_period = period;
    accel_duty = (unsigned short) calc;

    accel_input_timeout = 500;    // 500ms reload
    
}
//--- end of file ---//
