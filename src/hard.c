//---------------------------------------------
// #### MARCUS BLDC MOTOR - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### HARD.C #################################
//---------------------------------------------
#include "hard.h"
#include "stm32f10x.h"


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
void Pb5_On (void)
{
    // PB5_ON;
}


void Pb6_On (void)
{
    // PB6_ON;
}


void Pb7_On (void)
{
    // PB7_ON;
}


void Pb5_Off (void)
{
    // PB5_OFF;
}


void Pb6_Off (void)
{
    // PB6_OFF;
}


void Pb7_Off (void)
{
    // PB7_OFF;
}


unsigned char hall_u_cnt = 0;
unsigned char hall_v_cnt = 0;
unsigned char hall_w_cnt = 0;
void Hall_Update (void)
{
    if (HALL_U)
    {
        if (hall_u_cnt < 8)
            hall_u_cnt++;
    }
    else if (hall_u_cnt)
        hall_u_cnt--;

    if (HALL_V)
    {
        if (hall_v_cnt < 8)
            hall_v_cnt++;
    }
    else if (hall_v_cnt)
        hall_v_cnt--;

    if (HALL_W)
    {
        if (hall_w_cnt < 8)
            hall_w_cnt++;
    }
    else if (hall_w_cnt)
        hall_w_cnt--;
    

}


unsigned char Hall_U (void)
{
    if (hall_u_cnt > 4)
        return 1;

    return 0;
    // return HALL_U;
}


unsigned char Hall_V (void)
{
    if (hall_v_cnt > 4)
        return 1;

    return 0;
    // return HALL_V;
}


unsigned char Hall_W (void)
{
    if (hall_w_cnt > 4)
        return 1;

    return 0;
    // return HALL_W;
}


unsigned short led_on_off_time;
extern volatile unsigned short timer_led;
void Led_Toggle (unsigned short on_off_time)
{
    led_on_off_time = on_off_time;
}


void Led_Toggle_Update (void)
{
    if (!timer_led)
    {
        if (led_on_off_time)
        {
            timer_led = led_on_off_time;
            if (LED)
                LED_OFF;
            else
                LED_ON;
        }
        else
        {
            // check in next 100ms
            timer_led = 100;
            LED_OFF;
        }
    }
}


//--- end of file ---//
