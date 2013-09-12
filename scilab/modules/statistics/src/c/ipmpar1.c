/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA -
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
/*
 *  MACHINE CONSTANTS
 *  These numbers must be updated when the program is ported to a new machine.
 *  Using spConfig.h to get the largest int
 */
/*--------------------------------------------------------------------------*/
#define spINSIDE_SPARSE
#include "../../sparse/includes/spConfig.h"
#include "ipmpar1.h"
/*--------------------------------------------------------------------------*/
int C2F(largestint)()
{
    static int first = 0, large = 0;
    if ( first == 0)
    {
        if (sizeof(int) == sizeof(long))
        {
            large = LARGEST_LONG_INTEGER ;
        }
        else if (sizeof(int) == sizeof(short))
        {
            large = LARGEST_SHORT_INTEGER;
        }
        else
        {
            large = 2147483647 ;    /** using default value **/
        }

        first++;
        return large ;
    }
    return large;
}
/*--------------------------------------------------------------------------*/
