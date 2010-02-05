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
/*--------------------------------------------------------------------------*/ 
/******************************************
 * SCILAB function : pvm_sci2f77
 ******************************************/
int sci_pvm_sci2f77(char *fname,unsigned long fname_len)
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
