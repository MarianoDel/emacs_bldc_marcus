//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS.C ###############################
//---------------------------------------------

// Includes Modules for tests --------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <math.h>


// Types Constants and Macros --------------------------------------------------


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------



// Module Functions to Test ----------------------------------------------------
unsigned char Circular_Diff (unsigned char a, unsigned char b);


// Module Auxiliary Functions --------------------------------------------------


// Module Functions ------------------------------------------------------------
int main (int argc, char *argv[])
{
    unsigned char a = 255;
    unsigned char b = 254;

    printf ("a: %d b: %d diff: %d\n",
            a,
            b,
            Circular_Diff(a, b));

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



//--- end of file ---//


