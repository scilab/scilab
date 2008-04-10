/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*-----------------------------------------------------------------------------------*/
#include "machine.h"
#include "stack-c.h"
#include "gw_matio.h"
#include "../../src/c/sciloadmatfile.h"
/*-----------------------------------------------------------------------------------*/
int sci_loadmatfile(char *fname,unsigned long fname_len)
{
  int m=1,n=1,l=0;
  sciloadmatfile(fname,fname_len);

  CreateVar(1,"d",&m,&n,&l);
  *stk(l)=2110;
  LhsVar(1) = 1; 
  return 0;
}
/*-----------------------------------------------------------------------------------*/
