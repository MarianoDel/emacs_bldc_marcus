//----------------------------------------------------------
// #### MARCUS BLDC MOTOR - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### PWM.H ##############################################
//----------------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef __PWM_H_
#define __PWM_H_


// Exported Types --------------------------------------------------------------



// Exported Constants or Macros ------------------------------------------------
// #define POWER_CONTROL_GENERAL_THRESHOLD    512    // este es un valor total de dmx no pwm

// #define EnablePreload_PWM1    EnablePreload_TIM3_CH1
// #define EnablePreload_PWM2    EnablePreload_TIM3_CH2

// #define DisablePreload_PWM1    DisablePreload_TIM3_CH1
// #define DisablePreload_PWM2    DisablePreload_TIM3_CH2

// #define PWM_Update_CH1(X)    Update_TIM16_CH1N(X)
// #define PWM_Update_CH2(X)    Update_TIM14_CH1(X)

// #define EnablePreload_ENA1    EnablePreload_TIM1_CH2
// #define EnablePreload_ENA2    EnablePreload_TIM1_CH4

// #define DisablePreload_ENA1    DisablePreload_TIM1_CH2
// #define DisablePreload_ENA2    DisablePreload_TIM1_CH4

// #define PWM_Update_ENA1(X)    Update_TIM1_CH2(X)
// #define PWM_Update_ENA2(X)    Update_TIM1_CH4(X)



// Exported Functions ----------------------------------------------------------
void Pwm_U (unsigned short a);
void Pwm_V (unsigned short a);
void Pwm_W (unsigned short a);

void Low_U_Reset (void);
void Low_V_Reset (void);
void Low_W_Reset (void);

void Low_U_Set (void);
void Low_V_Set (void);
void Low_W_Set (void);


unsigned short Pwm_Setting (void);
void Pwm_Setting_Update (void);
void Pwm_Setting_Update_Reset (void);
void Pwm_Setting_Timeout (void);



#endif    /* __PWM_H_ */

//--- end of file ---//

