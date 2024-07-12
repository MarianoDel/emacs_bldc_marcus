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
unsigned char Circular_Diff (unsigned char a, unsigned char b);


// Module Functions ------------------------------------------------------------
// void Pb5_On (void)
// {
//     // PB5_ON;
// }


// void Pb6_On (void)
// {
//     // PB6_ON;
// }


// void Pb7_On (void)
// {
//     // PB7_ON;
// }


// void Pb5_Off (void)
// {
//     // PB5_OFF;
// }


// void Pb6_Off (void)
// {
//     // PB6_OFF;
// }


// void Pb7_Off (void)
// {
//     // PB7_OFF;
// }


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


unsigned char hall_u_last = 0;
unsigned char hall_v_last = 0;
unsigned char hall_w_last = 0;
unsigned char hall_rising_check_u = 0;
unsigned char hall_rising_check_v = 0;
unsigned char hall_rising_check_w = 0;
void Hall_Update_Supervisor (void)
{
    unsigned char hall_u_new = 0;
    unsigned char hall_v_new = 0;
    unsigned char hall_w_new = 0;

    hall_u_new = Hall_U();
    hall_v_new = Hall_V();
    hall_w_new = Hall_W();

    // send speed on u phase
    if (hall_u_new)
        SPEED_OUT_ON;
    else
        SPEED_OUT_OFF;

    // check rising edge on U
    if (hall_u_last != hall_u_new)
    {
        if ((!hall_u_last) &&
            (hall_u_new))
        {
            hall_rising_check_u |= 0x01;
            hall_rising_check_v |= 0x01;
            hall_rising_check_w |= 0x01;
            
            // have rising edge on others?
            if (hall_rising_check_u == 0x07)
                HALL_FAIL_OFF;
            else
                HALL_FAIL_ON;

            hall_rising_check_u = 0;
        }
        hall_u_last = hall_u_new;
    }

    // check rising edge on V
    if (hall_v_last != hall_v_new)
    {
        if ((!hall_v_last) &&
            (hall_v_new))
        {
            hall_rising_check_u |= 0x02;
            hall_rising_check_v |= 0x02;
            hall_rising_check_w |= 0x02;
            
            // have rising edge on others?
            if (hall_rising_check_v == 0x07)
                HALL_FAIL_OFF;
            else
                HALL_FAIL_ON;

            hall_rising_check_v = 0;
        }
        hall_v_last = hall_v_new;
    }

    // check rising edge on W
    if (hall_w_last != hall_w_new)
    {
        if ((!hall_w_last) &&
            (hall_w_new))
        {
            hall_rising_check_u |= 0x04;
            hall_rising_check_v |= 0x04;
            hall_rising_check_w |= 0x04;
            
            // have rising edge on others?
            if (hall_rising_check_w == 0x07)
                HALL_FAIL_OFF;
            else
                HALL_FAIL_ON;

            hall_rising_check_w = 0;
        }
        hall_w_last = hall_w_new;
    }
}


unsigned char Hall_Check_Invalid (void)
{
    unsigned char hall_sum = 0;    

    // check for invalid state
    hall_sum = Hall_U() + Hall_V() + Hall_W();
    
    if ((hall_sum == 0) ||
        (hall_sum == 3))
    {
        return 1;
    }

    return 0;
}

unsigned char Circular_Diff (unsigned char a, unsigned char b)
{
    unsigned char diff = 0;
    char aa = a;
    char bb = b;

    if (aa > bb)
        diff = aa - bb;
    else
        diff = bb - aa;

    return diff;
}


unsigned char Hall_U (void)
{
    if (hall_u_cnt > 4)
        return 1;

    return 0;
}


unsigned char Hall_V (void)
{
    if (hall_v_cnt > 4)
        return 1;

    return 0;
}


unsigned char Hall_W (void)
{
    if (hall_w_cnt > 4)
        return 1;

    return 0;
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


// filter and validate inputs
volatile unsigned char inputs_timer = 0;
volatile unsigned char alarma_on_cntr = 0;
volatile unsigned char brake_cntr = 0;
volatile unsigned char direction_cntr = 0;
volatile unsigned char control_i0_cntr = 0;
volatile unsigned char control_i1_cntr = 0;
void HARD_Timeouts (void)
{
    // check every ms
    if (BRAKE)
    {
        if (brake_cntr < 20)
            brake_cntr++;
    }
    else if (brake_cntr)
        brake_cntr--;    

    // check every 5ms
    if (inputs_timer)
    {
        inputs_timer--;
        return;
    }

    inputs_timer = 5;
    if (ALARMA_ON)
    {
        if (alarma_on_cntr < 20)
            alarma_on_cntr++;
    }
    else if (alarma_on_cntr)
        alarma_on_cntr--;

    if (DIRECTION)
    {
        if (direction_cntr < 20)
            direction_cntr++;
    }
    else if (direction_cntr)
        direction_cntr--;

    if (CONTROL_I0)
    {
        if (control_i0_cntr < 20)
            control_i0_cntr++;
    }
    else if (control_i0_cntr)
        control_i0_cntr--;

    if (CONTROL_I1)
    {
        if (control_i1_cntr < 20)
            control_i1_cntr++;
    }
    else if (control_i1_cntr)
        control_i1_cntr--;

}


unsigned char HARD_Check_Current_Config (void)
{
    unsigned char result = 0;
    
    if (control_i0_cntr > 10)
        result |= 0x01;

    if (control_i1_cntr > 10)
        result |= 0x02;

    return result;
}


unsigned char Brake (void)
{
    if (brake_cntr > 10)
        return 1;

    return 0;
}


unsigned char Alarm_On (void)
{
    if (alarma_on_cntr > 10)
        return 1;

    return 0;
}


unsigned char Direction (void)
{
    if (direction_cntr > 10)
        return 1;

    return 0;
}


//--- end of file ---//
