/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h> 
#include <stdio.h>
#include "stack-c.h"
#include "sciprint.h"

/*************************************************************
 * Example of using a character string sent by the interface.
 * All 'a' are replaced by 'o'.
 *************************************************************/

int as2osc(char *thechain)
{
  static int k, l;
  l = strlen(thechain);
  sciprint("character string length %d\n",l);
  for (k = 0 ; k < l; k++) 
    {
      if ( thechain[k] == 'a' ) thechain[k]='o';
    }
  return 0;
} 

