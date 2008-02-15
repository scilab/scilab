/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) ENPC
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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
