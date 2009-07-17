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
/* file: sci_xdel.c                                                       */
/* desc : interface for xdel routine                                      */
/*------------------------------------------------------------------------*/

#include "sci_xdel.h"
#include "stack-c.h"
#include "DestroyObjects.h"
#include "GetProperty.h"
#include "CurrentObjectsManagement.h"
#include "getPropertyAssignedValue.h"
#include "WindowList.h"
#include "Scierror.h"

/*--------------------------------------------------------------------------*/
int sci_xdel(char *fname,unsigned long fname_len)
{ 
  int m1,n1,l1;
  CheckRhs(-1,1);
  if (Rhs >= 1) {
    int i;
		double * windowNumbers;
    GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);

		/* First check that all the window numbers are valid */
		windowNumbers = getDoubleMatrixFromStack(l1);
		for (i = 0; i < m1 * n1; i++)
		{
			if (!sciIsExistingFigure((int) windowNumbers[i]))
			{
				Scierror(999, "%s: Figure with figure_id %d does not exist.\n",fname, (int) windowNumbers[i]);
				return -1;
			}
		}

    for (i = 0; i < m1*n1 ; i++) 
    {
      sciDeleteWindow( (int) windowNumbers[i] ) ;
    }
  } else {
    sciDeleteWindow( sciGetNum(sciGetCurrentFigure()) ) ;
  }
  LhsVar(1)=0;
	C2F(putlhsvar)();
  return 0;
} 

/*--------------------------------------------------------------------------*/
