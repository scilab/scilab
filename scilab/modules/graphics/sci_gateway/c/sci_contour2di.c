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
/* file: sci_contour2di.h                                                 */
/* desc : interface for contour2di routine                                */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "sci_contour2di.h"
#include "GetCommandArg.h"
#include "Scierror.h"
#include "Contour.h"
#include "localization.h"

/*--------------------------------------------------------------------------*/
int sci_contour2di( char * fname, unsigned long fname_len )
{
  int flagx=0,nz=10; /* default number of level curves : 10 */
  int m1,n1,l1,m2,n2,l2,m3,n3,l3,m4,n4,l4,l5;
  double  *hl1, *hl2;
  double *znz= NULL;
  int ix4, i, un = 1;

  CheckRhs(3,4);
  CheckLhs(2,2);

  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
  CheckVector(1,m1,n1);
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);
  CheckVector(2,m2,n2);
  GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE, &m3, &n3, &l3);
  if (m3 * n3 == 0) {
		LhsVar(1) = 0;
		C2F(putlhsvar)();
		return 0;
	}
  if (m3 == 1 || n3 == 1) {
    Scierror(999,_("%s: Wrong type for input argument #%d: Matrix expected.\n"),fname,3);
    return 0;
  }

  CheckDimProp(1,3, m1 * n1 != m3); 
  CheckDimProp(2,3, m2 * n2 != n3); 

  /*     number of level curves */
  if ( Rhs == 4 ) 
  {
    GetRhsVar(4,MATRIX_OF_DOUBLE_DATATYPE, &m4, &n4, &l4);
    if (m4 * n4 == 1) {
      flagx = 0;  nz = Max(1,(int)  *stk(l4)),znz= stk(l4);
    } else {
      flagx = 1;  nz = m4 * n4; znz=stk(l4);
    }
  }

  ix4 = Max(nz,2);
  CreateVar(Rhs+1,MATRIX_OF_INTEGER_DATATYPE,&un,&ix4,&l5);
  for (i =0 ; i < ix4 ; ++i) *istk(l5 + i ) = i+1;
  if (nz == 1) *istk(l5 +1) = 1;

  if (C2F(contourif)(stk(l1),stk(l2),stk(l3),&m3,&n3,&flagx,&nz,znz,istk(l5)) != 0)
	{
		/* Something wrong happened */
		return -1;	
	}
  C2F(getconts)(&hl1, &hl2, &m1, &n1);
  if (n1 == 0)
  {
    CreateVar(6,MATRIX_OF_DOUBLE_DATATYPE, &n1, &n1, &l1);
    CreateVar(7,MATRIX_OF_DOUBLE_DATATYPE, &n1, &n1, &l2);
  }
  else 
  {
    CreateVarFromPtr(6,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &hl1);
    CreateVarFromPtr(7,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &hl2);
  }
  LhsVar(1)=Rhs+2;
  LhsVar(2)=Rhs+3;
  C2F(putlhsvar)();
  return 0;
}
/*--------------------------------------------------------------------------*/
