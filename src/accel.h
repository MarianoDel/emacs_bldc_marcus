//----------------------------------------------------------
// #### MARCUS BLDC MOTOR - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### ACCEL.H ############################################
//----------------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef __ACCEL_H_
#define __ACCEL_H_


// Exported Types Constants and Macros -----------------------------------------
#define ACCEL_MIN_FOR_START    320


// Exported Functions ----------------------------------------------------------
void Accel_Setting_Timeout (void);
void Accel_Setting_Update (void);
unsigned short Accel_Get_Duty (void);
void Accel_Set_Values (unsigned short period, unsigned short duty);
void Accel_Setting_Reset (void);


#endif    /* __ACCEL_H_ */

//--- end of file ---//

