/* Copyright INRIA/ENPC */
#if defined(THINK_C)||defined(__MWERKS__)||defined(WIN32)
#define True 1
#define False 0
#else
#include <X11/Xlib.h>
#endif


static int echo_mode = True;
static int reading = False;
/* 
   functions to handle echo of typed scilab commands 
*/
void set_echo_mode(int mode)
{
    echo_mode = mode;
}
int get_echo_mode(void)
{
    return(echo_mode);
}
/* 
   functions to handle when scilab waits for commands 
*/
void set_is_reading(int mode)
{
    reading = mode;
}
int get_is_reading(void)
{
    return(reading);
}
