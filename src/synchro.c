//------------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### SYNCHRO.C ################################
//------------------------------------------------

// Includes --------------------------------------------------------------------
#include "synchro.h"
// #include "stm32f10x.h"
#include "hard.h"
// #include "tim.h"

#include "pwm.h"

// #include "usart.h"
// #include "adc.h"
// #include "dma.h"
// #include "tim.h"
// #include "spi.h"

// #include <stdio.h>
// #include <string.h>
// #include <math.h>



// Module Private Types Constants & Macros -------------------------------------
typedef enum {
    SEQ_NO_UPDATE,
    SEQ_NEW_SYNC,
    SEQ_OUT_OF_SYNC
    
} sequence_update_e;


typedef enum {
    SYNC_INIT,
    SYNC_ACC,
    SYNC_RUNNING,
    SYNC_BREAK,
    SYNC_WAIT_STOP,
    SYNC_STALL
        
} synchro_state_e;


typedef enum {
    SEQ_120_1,
    SEQ_120_2,
    SEQ_120_3,
    SEQ_120_4,
    SEQ_120_5,
    SEQ_120_6,
    SEQ_120_OUT_OFF_SYNC
    
} sequence_state_e;

#define NO_ACCELERATION
#define NO_MOTOR_STALL_CHECK
// Externals -------------------------------------------------------------------



// Globals ---------------------------------------------------------------------
volatile unsigned short motor_timer = 0;
volatile unsigned short motor_stop_timer = 0;
synchro_state_e synchro_state = SYNC_INIT;
sequence_state_e seq_st = SEQ_120_OUT_OFF_SYNC;


unsigned short pwm_current = 0;
unsigned short pwm_step = 0;
unsigned short pwm_current_sended = 0;
unsigned char generating = 0;

// Module Private Functions ----------------------------------------------------
void Sync_Stop_All (void);
void Synchro_Motor_Stop_Update (sequence_update_e new_seq);
unsigned char Synchro_Motor_Stopped (void);
sequence_update_e Synchro_State_Update (void);
sequence_update_e Synchro_State_Update_Texas (void);


// Module Functions ------------------------------------------------------------
void Synchro_Timeouts (void)
{
    if (motor_stop_timer)
        motor_stop_timer--;

    if (motor_timer)
        motor_timer--;
}


void Synchro (void)
{
    unsigned short pwm_set = 0;
    sequence_update_e seq_update = SEQ_NO_UPDATE;
    
    switch (synchro_state)
    {
    case SYNC_INIT:
        if (Synchro_Check_Valid_State () &&
            Synchro_Motor_Stopped())
        {
            if (Pwm_Setting () > PWM_MIN_START)
            {
                // accelerate the motor over some time
                motor_timer = MOTOR_ACCELERATION_TIMER_TT;
                generating = 1;
                pwm_current = 0;
                synchro_state++;
                
                Led_Toggle(400);
            }
        }
        break;
        
    case SYNC_ACC:
#ifndef NO_ACCELERATION
        if (motor_timer != pwm_step)
        {
            pwm_step = motor_timer;

            pwm_set = Pwm_Setting ();
            if (pwm_set > pwm_current)
                pwm_current++;
            else if (pwm_set < pwm_current)
                pwm_current--;
        
        }

        if ((!motor_timer) &&
            (!Synchro_Motor_Stopped()))
        {
            synchro_state++;
            Led_Toggle(200);
        }        
#else
        synchro_state++;
        Led_Toggle(200);
#endif
        break;

    case SYNC_RUNNING:
        
        pwm_set = Pwm_Setting ();

        if (pwm_set < PWM_MIN_START)
        {
            generating = 0;
            Sync_Stop_All ();
            synchro_state = SYNC_WAIT_STOP;
            motor_timer = 400 * 6;
            Led_Toggle (400);
        }
        else if (pwm_set != pwm_current)
        {
            // check limits before asingment
            if (pwm_set > 2100)
                pwm_current = 2100;
            else
                pwm_current = pwm_set;
        }        

#ifndef NO_MOTOR_STALL_CHECK
        if (Synchro_Motor_Stopped())
        {
            generating = 0;
            Sync_Stop_All ();
            motor_timer = 1000;    // 1sec on stall
            synchro_state = SYNC_STALL;
            Led_Toggle(100);
        }
#endif
        break;

    case SYNC_BREAK:
        break;

    case SYNC_WAIT_STOP:
        if (!motor_timer)
        {
            Led_Toggle(0);
            synchro_state = SYNC_INIT;
        }
        break;
        
    case SYNC_STALL:
        // motor stalled, wait pwm_set be 0 to unstall

        pwm_set = Pwm_Setting ();        
        if ((!motor_timer) &&
            (pwm_set < PWM_MIN_START))
        {
            synchro_state = SYNC_INIT;
            Led_Toggle (0);
        }        
        break;

    default:
        synchro_state = SYNC_INIT;
        break;
    }

    // concurrent functions
    // seq_update = Synchro_State_Update ();

    seq_update = Synchro_State_Update_Texas ();

    Synchro_Motor_Stop_Update (seq_update);

    Pwm_Setting_Update ();
    
}


unsigned char Synchro_Check_Valid_State (void)
{
    if ((!Hall_U()) &&
        (!Hall_V()) &&
        (!Hall_W()))
        return 0;

    if ((Hall_U()) &&
        (Hall_V()) &&
        (Hall_W()))
        return 0;

    return 1;

    // another with seq
    // if (seq_last_update == SEQ_OUT_OF_SYNC)
    //     return 0;

    // return 1;
}


sequence_update_e seq_last_update = SEQ_NO_UPDATE;
void Synchro_Motor_Stop_Update (sequence_update_e new_seq)
{
    if (new_seq != seq_last_update)
    {
        seq_last_update = new_seq;
        motor_stop_timer = MOTOR_STOP_TIMER_TT;
    }
}


unsigned char Synchro_Motor_Stopped (void)
{
    if (motor_stop_timer)
        return 0;

    return 1;
}


#define HALL_CW_STAGE_1    0x01
#define HALL_CW_STAGE_2    0x03
#define HALL_CW_STAGE_3    0x02
#define HALL_CW_STAGE_4    0x06
#define HALL_CW_STAGE_5    0x04
#define HALL_CW_STAGE_6    0x05

unsigned char last_hall = 0;
sequence_update_e Synchro_State_Update_Texas (void)
{
    sequence_update_e seq = SEQ_NO_UPDATE;
    unsigned char current_hall = 0;

    // first check change in hall effect
    current_hall |= (Hall_W() << 0);
    current_hall |= (Hall_V() << 1);
    current_hall |= (Hall_U() << 2);

    if (last_hall != current_hall)
    {
        last_hall = current_hall;
        seq = SEQ_NEW_SYNC;
        
        // change in stage
        if (generating)
        {
            Pwm_U (0);
            Pwm_V (0);
            Pwm_W (0);
            pwm_current_sended = 0;
            
            Low_U_Reset();
            Low_V_Reset();
            Low_W_Reset();            
        }
    }

    // check for change in pwm
    if ((generating) &&
        (pwm_current != pwm_current_sended))
    {
        pwm_current_sended = pwm_current;
        
        switch (last_hall)
        {
        case HALL_CW_STAGE_1:
            Pwm_V (pwm_current);
            Low_W_Set();            
            break;
        case HALL_CW_STAGE_2:
            Pwm_U (pwm_current);
            Low_W_Set();
            break;
        case HALL_CW_STAGE_3:
            Pwm_U (pwm_current);            
            Low_V_Set();
            break;
        case HALL_CW_STAGE_4:
            Pwm_W (pwm_current);            
            Low_V_Set();
            break;
        case HALL_CW_STAGE_5:
            Pwm_W (pwm_current);
            Low_U_Set();            
            break;
        case HALL_CW_STAGE_6:
            Pwm_V (pwm_current);
            Low_U_Set();            
            break;
        }
    }

    return seq;
}


sequence_update_e Synchro_State_Update (void)
{
    sequence_update_e seq = SEQ_NO_UPDATE;
    unsigned char first_seq = 0;

    switch (seq_st)
    {
    case SEQ_120_1:
        if ((Hall_V()) && (Hall_U()))
        {
            seq_st = SEQ_120_2;
            seq = SEQ_NEW_SYNC;

            if (generating)
            {
                // 120_2 -- U V W -- HiZ Vs Gnd
                Low_V_Reset();
                Pwm_U (0);
                Low_W_Set();
                Pwm_V (pwm_current);
            }
        }
        else if ((generating) &&
                 (pwm_current != pwm_current_sended))
        {
            Pwm_U (pwm_current);
            pwm_current_sended = pwm_current;
            Low_V_Reset();
            Low_W_Set();
        }
        break;

    case SEQ_120_2:
        if (!Hall_U())
        {            
            seq_st = SEQ_120_3;
            seq = SEQ_NEW_SYNC;            

            if (generating)
            {
                // 120_3 -- U V W -- Gnd Vs HiZ
                Low_U_Set();
                Low_W_Reset();
            }
        }
        else if ((generating) &&
                 (pwm_current != pwm_current_sended))
        {
            Pwm_V (pwm_current);
            pwm_current_sended = pwm_current;
            Low_U_Reset();
            Low_W_Set();
        }
        break;

    case SEQ_120_3:
        if ((Hall_W()) && (Hall_V()))
        {
            seq_st = SEQ_120_4;
            seq = SEQ_NEW_SYNC;

            if (generating)
            {
                // 120_4 -- U V W -- Gnd HiZ Vs
                Low_W_Reset();
                Pwm_V (0);
                Low_U_Set();
                Pwm_W (pwm_current);
            }
        }        
        else if ((generating) &&
                 (pwm_current != pwm_current_sended))
        {
            Pwm_V (pwm_current);
            pwm_current_sended = pwm_current;
            Low_U_Set();
            Low_W_Reset();
        }
        break;

    case SEQ_120_4:
        if (!Hall_V())
        {            
            seq_st = SEQ_120_5;
            seq = SEQ_NEW_SYNC;            

            if (generating)
            {
                // 120_5 -- U V W -- HiZ Gnd Vs
                Low_V_Set();
                Low_U_Reset();
            }
        }
        else if ((generating) &&
                 (pwm_current != pwm_current_sended))
        {
            Pwm_W (pwm_current);
            pwm_current_sended = pwm_current;
            Low_U_Set();
            Low_W_Reset();
        }
        break;

    case SEQ_120_5:
        if ((Hall_U()) && (Hall_W()))
        {
            seq_st = SEQ_120_6;
            seq = SEQ_NEW_SYNC;

            if (generating)
            {
                // 120_6 -- U V W -- Vs Gnd HiZ
                Low_U_Reset();
                Pwm_W (0);
                Low_V_Set();
                Pwm_U (pwm_current);
            }
        }        
        else if ((generating) &&
                 (pwm_current != pwm_current_sended))
        {
            Pwm_W (pwm_current);
            pwm_current_sended = pwm_current;
            Low_V_Set();
            Low_U_Reset();
        }
        break;

    case SEQ_120_6:
        if (!Hall_W())
        {            
            seq_st = SEQ_120_1;
            seq = SEQ_NEW_SYNC;            

            if (generating)
            {
                // 120_1 -- U V W -- Vs HiZ Gnd
                Low_W_Set();
                Low_V_Reset();
            }
        }
        else if ((generating) &&
                 (pwm_current != pwm_current_sended))
        {
            Pwm_U (pwm_current);
            pwm_current_sended = pwm_current;
            Low_V_Set();
            Low_W_Reset();
        }
        break;

    default:
        first_seq = 0;
        first_seq |= (Hall_W() << 2);
        first_seq |= (Hall_V() << 1);
        first_seq |= (Hall_U() << 0);

        switch (first_seq)
        {
        case 1:
            seq_st = SEQ_120_1;
            break;

        case 2:
            seq_st = SEQ_120_3;
            break;

        case 3:
            seq_st = SEQ_120_2;
            break;

        case 4:
            seq_st = SEQ_120_5;
            break;

        case 5:
            seq_st = SEQ_120_6;
            break;

        case 6:
            seq_st = SEQ_120_4;
            break;

        default:
            seq = SEQ_OUT_OF_SYNC;
            break;
        }
        break;
    }

    return seq;
}


void Sync_Stop_All (void)
{
    Pwm_U(0);
    Pwm_V(0);
    Pwm_W(0);
    Low_U_Reset();
    Low_V_Reset();
    Low_W_Reset();
}
//--- end of file ---//
