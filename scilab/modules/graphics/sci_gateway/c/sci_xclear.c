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
/* file: sci_xclear.c                                                     */
/* desc : interface for xclear routine                                    */
/*------------------------------------------------------------------------*/

#include "sci_xclear.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "GetProperty.h"
#include "DrawObjects.h"
#include "WindowList.h"


/*--------------------------------------------------------------------------*/
int sci_xclear( char * fname, unsigned long fname_len )
{
  integer wid;
  integer ix,m1,n1,l1/*,v=0*/;

  CheckRhs(0,2); /* NG  */ /*CheckRhs(-1,1) */ 
  CheckLhs(0,1);
  if (Rhs == 1) 
  {
    GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
    for (ix = 0 ; ix < m1*n1 ; ++ix) 
    {
      wid = (integer) *stk(l1 +ix );
      sciClearFigure(getFigureFromIndex(wid));
    }
  }
  else 
  {
    sciXclear() ;
  } 

  LhsVar(1)=0;
  return 0;
}
/*--------------------------------------------------------------------------*/
