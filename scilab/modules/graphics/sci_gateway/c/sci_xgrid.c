/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_xgrid.c                                                      */
/* desc : interface for xgrid routine                                     */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "stack-c.h"
#include "SetProperty.h"
#include "CurrentObjectsManagement.h"
#include "DrawingBridge.h"

/*--------------------------------------------------------------------------*/
int sci_xgrid(char *fname,unsigned long fname_len)
{
  int style = 0,m1,n1,l1;
  int status = 0 ;
  CheckRhs(-1,1);
  if (Rhs == 1) {
    GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
    CheckScalar(1,m1,n1);
    style = (int)  *stk(l1);
  }
  LhsVar(1)=0;
	C2F(putlhsvar)();

  status = sciSetGridStyle( sciGetCurrentSubWin(), style, style, style ) ;
  if ( status == 0 )
  {
    sciDrawObj( sciGetCurrentSubWin() ) ;
  }

  return status ;
} 
/*--------------------------------------------------------------------------*/
