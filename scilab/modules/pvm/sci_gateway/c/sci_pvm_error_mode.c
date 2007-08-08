/*-----------------------------------------------------------------------------------*/
/* PVM functions interfaces */
/* Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */
/* Jean-Philippe Chancelier July 2002  */
/* Modified by Allan Cornet  INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#include "gw_pvm.h"
#include "sci_pvm.h"
/******************************************
 * SCILAB function : pvm_error_mode
 ******************************************/
/*-----------------------------------------------------------------------------------*/ 
static int pvm_error = 0;
/*-----------------------------------------------------------------------------------*/ 
int intspvm_error_mode _PARAMS((char *fname,unsigned long fname_len))
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
		Scierror(999,"%s: first argument should be \"stop\" or \"continue\"\r\n",fname);
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
/*-----------------------------------------------------------------------------------*/ 
void pvm_error_check _PARAMS((char *fname,int err, unsigned long fname_len))
{
  if ( pvm_error == 1 && err != 0 ) 
    {
      Scierror(999,"%s: %s\r\n",fname,scipvm_error_msg(err));
    }
}
/*-----------------------------------------------------------------------------------*/ 

