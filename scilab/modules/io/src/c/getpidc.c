/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifdef _MSC_VER
#include <Windows.h>
#else
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#endif
#include "getpidc.h"
#include "machine.h"
/*--------------------------------------------------------------------------*/
int getpidc(int *id1)
{
#if _MSC_VER
    *id1 = GetCurrentProcessId();
#else
    *id1 = getpid();
#endif

    return (0) ;
}
/*--------------------------------------------------------------------------*/
