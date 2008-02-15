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
#include "sciprint.h"

/*****************************************
 *  Accessing the Scilab Stack 
 *   Accessing a Scilab String (read) by its name. 
 *****************************************/

#define MAXCH 30

int intex15c(char* fname)

{ 
  static int minlhs=1, minrhs=0, maxlhs=1, maxrhs=0;
  char str[MAXCH];
  int strl=MAXCH;

  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  /* We search a Scilab Object named Mystr check that it is a string 
   * and store the string in str. 
   * strl is used on entry to give the maximum number 
   * of characters which can be stored in str 
   * After the call strl contains the number of 
   * copied characters
   */

  ReadString("Mystr", &strl, str);

  sciprint("Mystr= \"%s\", length %d\n", str, strl);

  /*    LhsVar(1) = 0; means "no output"   */
  LhsVar(1) = 0;
  return(0);
}

