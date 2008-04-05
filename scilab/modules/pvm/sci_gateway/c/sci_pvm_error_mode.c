/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1997-1999 - INRIA - Eric FLEURY
 * Copyright (C) 2002 - ENPC - Jean-Philippe CHANCELIER
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
/* PVM functions interfaces */
/*--------------------------------------------------------------------------*/ 
#include "gw_pvm.h"
#include "sci_pvm.h"
#include "localization.h"
/******************************************
 * SCILAB function : pvm_error_mode
 ******************************************/
/*--------------------------------------------------------------------------*/ 
static int pvm_error = 0;
/*--------------------------------------------------------------------------*/ 
int intspvm_error_mode(char *fname,unsigned long fname_len)
{ 
  char *loc;
  static char stop[]="stop"; 
  static char cont[]="continue"; 
  int m1,n1,l1,un=1; 
  CheckRhs(0,1);
  CheckLhs(1,1);
  if ( Rhs == 1 ) 
  { 
      GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
      if ( strcmp(cstk(l1),stop) == 0) pvm_error = 1; 
      else if ( strcmp(cstk(l1),cont) == 0) pvm_error = 0; 
      else 
	  {
		Scierror(999,_("%s: Wrong first input argument: '%s' or '%s' expected.\n"),fname,stop,cont);
		return 0;
      }
      LhsVar(1)=0;
  }
  else 
  {
      int m;
      if ( pvm_error == 1 ) 
	  { 
		m = strlen(stop);
		CreateVarFromPtr(Rhs+1,STRING_DATATYPE,&m,&un,(loc=stop,&loc));
      } 
	  else 
	  { 
		m = strlen(cont);
		CreateVarFromPtr(Rhs+1,STRING_DATATYPE,&m,&un,(loc=cont,&loc));
      }
      LhsVar(1)=Rhs+1;
  }
  C2F(putlhsvar)();
  return 0;
}
/*--------------------------------------------------------------------------*/ 
void pvm_error_check(char *fname,int err, unsigned long fname_len)
{
  if ( pvm_error == 1 && err != 0 ) 
    {
      Scierror(999,"%s: %s\n",fname,scipvm_error_msg(err));
    }
}
/*--------------------------------------------------------------------------*/ 

