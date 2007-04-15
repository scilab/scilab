/* Copyright INRIA/ENPC */
#ifdef _MSC_VER
#define True 1
#define False 0
#else
#include <X11/Xlib.h>
#endif
#include "flags.h"

static int echo_mode = True;
static int reading = False;

void set_echo_mode(int mode)
{
    echo_mode = mode;
}

int get_echo_mode(void)
{
    return(echo_mode);
}

void set_is_reading(int mode)
{
    reading = mode;
}

int get_is_reading(void)
{
    return(reading);
}
