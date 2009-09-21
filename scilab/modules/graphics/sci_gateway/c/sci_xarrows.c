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
/* file: sci_xarrows.h                                                    */
/* desc : interface for xarrows routine                                   */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "sciCall.h"
#include "stack-c.h"
#include "localization.h"
#include "Scierror.h"
#include "CurrentObjectsManagement.h"
#include "DrawingBridge.h"
/*--------------------------------------------------------------------------*/
int sci_xarrows(char *fname,unsigned long fname_len)
{
  int dstyle = -1,m1,n1,l1,m2,n2,l2,m3=1,n3=1,l3;
  int *style,flag;
  int m4,n4,l4,mn2;
  double arsize=-1.0;

  CheckRhs(2,4);

  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2);
  mn2 = m2 * n2;
  CheckSameDims(1,2,m1,n1,m2,n2);
  if (mn2 == 0)
	{
		LhsVar(1)=0;
		C2F(putlhsvar)();
		return 0;
	} 

  if (Rhs >= 3) { GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE,&m3,&n3,&l3); CheckScalar(3,m3,n3); arsize = *stk(l3); } 

  if (Rhs >= 4) { 
    GetRhsVar(4,MATRIX_OF_INTEGER_DATATYPE,&m4,&n4,&l4); CheckVector(4,m4,n4);
    if (m4 * n4 == 1) dstyle = *istk(l4);
    if (m4 * n4 != 1 && m2 * n2 / 2 != m4 * n4) {
      Scierror(999,_("%s: Wrong size for input argument #%d: %d expected.\n"),fname, 4, m2 * n2 / 2 );
      return 0;
    }
  }
  /* NG beg */
  if (Rhs == 4 && m4 * n4 != 1)
  {
    style=istk(l4) ;
    flag = 1 ;
  } 
  else {
    style = &dstyle ;
    flag = 0;
  }
  
  Objsegs (style,flag,mn2,stk(l1),stk(l2),NULL,arsize);
  
  sciDrawObj(sciGetCurrentObj());

  LhsVar(1)=0;
	C2F(putlhsvar)();
  return 0;
} 


/*--------------------------------------------------------------------------*/
