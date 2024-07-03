//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS_SYNCHRO.C #######################
//---------------------------------------------

// Includes Modules for tests --------------------------------------------------
#include "synchro.h"


//helper modules
#include "tests_ok.h"
// #include "tests_vector_utils.h"

#include <stdio.h>
#include <string.h>


// Globals externals for module to test ----------------------------------------


// Externals globals from module to test ---------------------------------------
extern unsigned short pwm_set;
extern volatile unsigned short motor_stop_timer;
extern volatile unsigned short motor_timer;

// Globals locals --------------------------------------------------------------
unsigned char hall_u = 0;
unsigned char hall_v = 0;
unsigned char hall_w = 0;

unsigned char low_u = 0;
unsigned char low_v = 0;
unsigned char low_w = 0;

unsigned short pwm_u = 0;
unsigned short pwm_v = 0;
unsigned short pwm_w = 0;

char * bin3 [] = {"000", "001", "010", "011", "100", "101", "110", "111"};
// Module Auxialiary Functions -------------------------------------------------
unsigned char Hall_Sequence_Change (unsigned char * hu, unsigned char * hv, unsigned char * hw);


// Module Functions for testing ------------------------------------------------
void Test_Valid_State (void);
void Test_Motor_State (void);
void Test_Squence (void);




// Module Functions ------------------------------------------------------------


int main (int argc, char *argv[])
{
    // Test_Valid_State ();

    Test_Motor_State ();

    Test_Generation_Sequence ();

    // Test_Squence ();    

    return 0;
}


void Test_Valid_State (void)
{
    int error = 0;
    
    printf("\nTest valid states on synchro...\n");

    // check unvalid states
    hall_u = 0;
    hall_v = 0;
    hall_w = 0;
    if (Synchro_Check_Valid_State() != 0)
    {
        printf ("error on hall 0\n");
        error = 1;
    }

    hall_u = 1;
    hall_v = 1;
    hall_w = 1;
    if (Synchro_Check_Valid_State() != 0)
    {
        printf ("error on hall 1\n");
        error = 1;
    }

    printf(" unvalid state test: ");
    if (error)
        PrintERR();
    else
        PrintOK();

    // check valid states
    hall_u = 1;
    hall_v = 0;
    hall_w = 0;
    if (Synchro_Check_Valid_State() != 1)
    {
        printf ("error on hall 1\n");
        error = 1;
    }

    printf(" valid state test: ");
    if (error)
        PrintERR();
    else
        PrintOK();
    
        
}


typedef enum {
    SYNC_INIT,
    SYNC_ACC,
    SYNC_RUNNING,
    SYNC_BREAK,
    SYNC_STALL
        
} synchro_state_e;


extern synchro_state_e synchro_state;
void Test_Motor_State (void)
{
    int error = 0;
    
    printf("\nTest motor states on synchro...\n");

    // check remain init
    hall_u = 1;
    hall_v = 0;
    hall_w = 0;
    for (int i = 0; i < 100; i++)
    {
        Synchro();
        if (synchro_state != SYNC_INIT)
        {
            error = 1;
            printf(" no init state: %d on loop: %d\n", synchro_state, i);
            break;
        }
    }
    
    printf(" motor init state test: ");
    if (error)
        PrintERR();
    else
        PrintOK();

    // check go to acceleration
    hall_u = 1;
    hall_v = 0;
    hall_w = 0;
    motor_stop_timer = 0;
    pwm_set = PWM_MIN_START + 1;
    for (int i = 0; i < 10; i++)
    {
        Synchro();
    }

    if (synchro_state != SYNC_ACC)
    {
        error = 1;
        printf(" no acc state: %d motor stop: %d\n",
               synchro_state,
               Synchro_Motor_Stopped());
    }
    
    
    printf(" motor acc state test: ");
    if (error)
        PrintERR();
    else
        PrintOK();


    // check go to running
    hall_u = 1;
    hall_v = 0;
    hall_w = 0;
    motor_timer = 0;
    motor_stop_timer = 1000;
    pwm_set = PWM_MIN_START + 1;
    for (int i = 0; i < 10; i++)
    {
        Synchro();
    }

    if (synchro_state != SYNC_RUNNING)
    {
        error = 1;
        printf(" no running state: %d motor stop: %d\n",
               synchro_state,
               Synchro_Motor_Stopped());
    }
    
    
    printf(" motor running state test: ");
    if (error)
        PrintERR();
    else
        PrintOK();
    

    // check states on running
    for (int i = 0; i < 10; i++)
    {
        printf("loop: %d pwm_set: %d hu: %d hv: %d hw: %d\n",
               i,
               pwm_set,
               hall_u,
               hall_v,
               hall_w);
        
        Synchro();
    }

    if (synchro_state != SYNC_RUNNING)
    {
        error = 1;
        printf(" no running state: %d motor stop: %d\n",
               synchro_state,
               Synchro_Motor_Stopped());
    }
    
}


void Test_Generation_Sequence (void)
{
    int error = 0;
    int seq = 0;
    
    printf("\nTest motor runnint states on synchro...\n");

    // check go to running
    hall_u = 1;
    hall_v = 0;
    hall_w = 0;
    motor_timer = 0;
    motor_stop_timer = 1000;
    pwm_set = PWM_MIN_START + 1;
    
    for (int i = 0; i < 10; i++)
    {
        seq = Hall_Sequence_Change (&hall_u, &hall_v, &hall_w);
        // printf("seq: %d loop: %2d hall_u: %d hall_v: %d hall_w: %d\n",
        //        seq,
        //        i,
        //        hall_u,
        //        hall_v,
        //        hall_w);
        printf("\n\ncurrent_seq: %s loop: %2d hall_u: %d hall_v: %d hall_w: %d\n",
               bin3[seq],
               i,
               hall_u,
               hall_v,
               hall_w);

        Synchro();
    }

    if (synchro_state != SYNC_RUNNING)
    {
        error = 1;
        printf(" no running state: %d motor stop: %d\n",
               synchro_state,
               Synchro_Motor_Stopped());
    }
    
    
    printf(" motor running state test: ");
    if (error)
        PrintERR();
    else
        PrintOK();
    
        
}


void Test_Squence (void)
{
    int loops = 2 * 6;
    int seq = 0;
    printf("testing sequence for loops: %d\n", loops);

    for (int i = 0; i < loops; i++)
    {
        seq = Hall_Sequence_Change (&hall_u, &hall_v, &hall_w);
        // printf("seq: %d loop: %2d hall_u: %d hall_v: %d hall_w: %d\n",
        //        seq,
        //        i,
        //        hall_u,
        //        hall_v,
        //        hall_w);
        printf("current_seq: %s loop: %2d hall_u: %d hall_v: %d hall_w: %d\n",
               bin3[seq],
               i,
               hall_u,
               hall_v,
               hall_w);
    }
}


unsigned char Hall_Sequence_Change (unsigned char * hu, unsigned char * hv, unsigned char * hw)
{
    // check current sequence
    unsigned char current_seq = 0;
    
    current_seq |= *hw << 2;
    current_seq |= *hv << 1;
    current_seq |= *hu << 0;
    
    switch (current_seq)
    {
    case 1:
        // SEQ_120_1
        *hu = 1;
        *hv = 1;
        *hw = 0;
        break;

    case 2:
        // SEQ_120_3
        *hu = 0;
        *hv = 1;
        *hw = 1;        
        break;

    case 3:
        // SEQ_120_2
        *hu = 0;
        *hv = 1;
        *hw = 0;        
        break;

    case 4:
        // SEQ_120_5
        *hu = 1;
        *hv = 0;
        *hw = 1;        
        break;

    case 5:
        // SEQ_120_6
        *hu = 1;
        *hv = 0;
        *hw = 0;        
        break;

    case 6:
        // SEQ_120_4
        *hu = 0;
        *hv = 0;
        *hw = 1;        
        break;

    default:
        // to SEQ_120_1
        *hu = 1;
        *hv = 0;
        *hw = 0;
        break;
    }

    return current_seq;
}

// Module Mocked Functions -----------------------------------------------------
unsigned char Hall_U (void)
{
    return hall_u;
}


unsigned char Hall_V (void)
{
    return hall_v;
}


unsigned char Hall_W (void)
{
    return hall_w;
}


void Low_U_Set (void)
{
    low_u = 1;
    printf("low_u: 1 ");
}

void Low_U_Reset (void)
{
    low_u = 0;
    printf("low_u: 0 ");
}

void Low_V_Set (void)
{
    low_v = 1;
    printf("low_v: 1 ");
}

void Low_V_Reset (void)
{
    low_v = 0;
    printf("low_v: 0 ");
}

void Low_W_Set (void)
{
    low_w = 1;
    printf("low_w: 1 ");
}

void Low_W_Reset (void)
{
    low_w = 0;
    printf("low_w: 0 ");
}


void Pwm_U (unsigned short a)
{
    pwm_u = a;
    printf("pwm_u: %4d ", pwm_u);
}


void Pwm_V (unsigned short a)
{
    pwm_v = a;
    printf("pwm_v: %4d ", pwm_v);
}


void Pwm_W (unsigned short a)
{
    pwm_w = a;
    printf("pwm_w: %4d ", pwm_w);
}
//--- end of file ---//


