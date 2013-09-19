/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
