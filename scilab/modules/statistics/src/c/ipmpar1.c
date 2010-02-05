/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA -
 * Copyright (C) 2009 - Digiteo - Vincent LIARD
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
  if (!memoized) {
    if (sizeof(int) == sizeof(long))
      largest = LARGEST_LONG_INTEGER;
    else if (sizeof(int) == sizeof(short))
      largest = LARGEST_SHORT_INTEGER;
    memoized = 1;
  }
  return largest;
}
