/*------------------------------------------------------------------------*/
/* file: sci_xclear.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
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
