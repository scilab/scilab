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

#include "stack-c.h"

/*****************************************
 *  Accessing the Scilab Stack 
 *   Creation of a Scilab variable Str of type string
 *   from a name and an a C string 
 *****************************************/

int intex16c(char* fname)
{ 
  static int minlhs=1, minrhs=0, maxlhs=1, maxrhs=0;
  static char str[] ="Pilgrim said he, where can it be this land of Eldorado";
  int strl;

  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  strl=strlen(str);
  /* Creating the Scilab variable Str from str */
  WriteString("Str", &strl, str);

  /* No output */
  LhsVar(1) = 0;
  return(0);
}


