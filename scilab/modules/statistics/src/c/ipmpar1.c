/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA -
 * Copyright (C) 2009 - Digiteo - Vincent LIARD
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

/*
 *  MACHINE CONSTANTS
 *  These numbers must be updated when the program is ported to a new machine.
 *  Using spConfig.h to get the largest int
 */

#define spINSIDE_SPARSE
#include "../../sparse/includes/spConfig.h"
#include "ipmpar1.h"

int
C2F(largestint)(void)
{
    static int memoized = 0;
    static int largest = (1uL << 31) - 1; /* default */
    if (!memoized)
    {
        if (sizeof(int) == sizeof(long))
        {
            largest = LARGEST_LONG_INTEGER;
        }
        else if (sizeof(int) == sizeof(short))
        {
            largest = LARGEST_SHORT_INTEGER;
        }
        memoized = 1;
    }
    return largest;
}
