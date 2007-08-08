/*------------------------------------------------------------------------*/
/* file: sci_unzoom.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy, Jean-Philipe Chancelier  */
/* desc : interface for unzoom routine                                    */
/*------------------------------------------------------------------------*/

#include "sci_unzoom.h"
#include "stack-c.h"
#include "PloEch.h"
#include "GetProperty.h"
#include "BuildObjects.h"
#include "gw_graphics.h"

/*-----------------------------------------------------------------------------------*/
int sci_unzoom(char *fname,unsigned long fname_len)
{
  CheckRhs(0,1) ;
  CheckLhs(0,1) ;
  SciWin() ;
  if ( Rhs == 0 )
  {
    unzoom();
  }
  else
  {
    int m,n,l,i;
    GetRhsVar(1,GRAPHICAL_HANDLE_DATATYPE,&m,&n,&l); 
    for (i=0;i<m*n;i++)
    {
      unzoom_one_axes((sciPointObj*)sciGetPointerFromHandle((long) *hstk(l+i))); /** Correction Bug 1476 + Warning Windows **/
    }
  }
  LhsVar(1)=0; 
  return 0;
}
/*-----------------------------------------------------------------------------------*/
