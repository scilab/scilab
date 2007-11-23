/*--------------------------------------------------------------------------*/
/* PVM functions interfaces */
/* Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */
/* Jean-Philippe Chancelier July 2002  */
/* Modified by Allan Cornet  INRIA 2006 */
/*--------------------------------------------------------------------------*/ 
#include "gw_pvm.h"
#include "sci_pvm.h"
/*--------------------------------------------------------------------------*/ 
/******************************************
 * SCILAB function : pvm_sci2f77
 ******************************************/
int intspvm_sci2f77 _PARAMS((char *fname,unsigned long fname_len))
{
  int *header;
  CheckRhs(1,1);
  CheckLhs(0,1);
  /*  checking variable func */
  header = GetData(1);
  C2F(scipvmscitof77)(&Top);
  if ( IsRef(1) ) 
    { 
      /* arg is a reference it is changed */
      LhsVar(1)=0;
    }
  else 
    {
      /* arg is not a reference we must return 
       * the changed argument 
       */
      LhsVar(1)=1;
    }
  C2F(putlhsvar)();
  return 0;
}
/*--------------------------------------------------------------------------*/ 
