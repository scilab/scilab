/* Copyright INRIA/ENPC */
#include "machine.h"
#include "flags.h"

static int echo_mode = TRUE;
static int reading = FALSE;

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
