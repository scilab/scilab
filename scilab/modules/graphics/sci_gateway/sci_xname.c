/*------------------------------------------------------------------------*/
/* file: sci_xname.c                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xname routine                                     */
/*------------------------------------------------------------------------*/

#include "sci_xname.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "Xcall1.h"
#include "BuildObjects.h"

/*-----------------------------------------------------------------------------------*/
int sci_xname(char *fname,unsigned long fname_len)
{
  double dv;
  integer v,m1,n1,l1;
  CheckRhs(1,1);
  CheckLhs(1,1);
  GetRhsVar(1,"c",&m1,&n1,&l1);
  SciWin();
  if ( version_flag() == 0 )
  {
    sciSetName( sciGetCurrentFigure(), cstk(l1), m1 ) ;
  }
  else
  {
    C2F(dr1)("xname",cstk(l1),&v,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,6L,bsiz);
  }

  LhsVar(1)=0;
  return 0;
}
/*-----------------------------------------------------------------------------------*/