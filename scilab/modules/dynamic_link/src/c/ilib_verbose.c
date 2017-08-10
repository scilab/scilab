/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
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

/*---------------------------------------------------------------------------*/
#include "ilib_verbose.h"
/*---------------------------------------------------------------------------*/
ilib_verbose_level verboseLevel = ILIB_VERBOSE_STANDARD;
/*---------------------------------------------------------------------------*/
BOOL setIlibVerboseLevel(ilib_verbose_level _level)
{
    switch (_level)
    {
        case ILIB_VERBOSE_NO_OUTPUT:
        case ILIB_VERBOSE_STANDARD:
        case ILIB_VERBOSE_MAX:
        {
            verboseLevel = _level;
            return TRUE;
        }
    }
    return FALSE;
}
/*---------------------------------------------------------------------------*/
ilib_verbose_level getIlibVerboseLevel(void)
{
    return verboseLevel;
}
/*---------------------------------------------------------------------------*/
int C2F(getilibverboselevel)(int *_level)
{
    *_level = (int)verboseLevel;
    return 0;
}
/*---------------------------------------------------------------------------*/
