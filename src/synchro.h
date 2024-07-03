//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### SYNCHRO.H #################################
//---------------------------------------------

#ifndef _SYNCHRO_H_
#define _SYNCHRO_H_


// Module Exported Types Constants and Macros ----------------------------------
#define PWM_MIN_START    200

#define MOTOR_STOP_TIMER_TT    1000
#define MOTOR_ACCELERATION_TIMER_TT    2000


// Module Exported Functions ---------------------------------------------------
unsigned char Synchro_Check_Valid_State (void);
void Synchro_Timeouts (void);
void Synchro (void);



#endif /* _SYNCHRO_H_ */
