/*------------------------------------------------------------------------*/
/* file: sci_xload.c                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for sci_xload routine                                 */
/*------------------------------------------------------------------------*/

#include "sci_xload.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "GetProperty.h"
#include "Xcall1.h"
#include "RecSaveLoad.h"

/*-----------------------------------------------------------------------------------*/
int sci_xload(char *fname,unsigned long fname_len)
{
  double dv;
  integer m1,n1,l1,m2,n2,l2,wid,v=1;

  CheckRhs(1,2);
  if (version_flag() == 0) {
    /* call a Scilab function to handle compatibility */
    C2F(callscifun)("%xload",(unsigned long) 6);
    return 0;
  }/* NG end */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  if (Rhs == 2) {
    GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2); wid = (integer) *stk(l2 );
    C2F(dr)("xset","window",&wid,&v,PI0,PI0,PI0,PI0,&dv,&dv,&dv,&dv,5L,7L);
  }
  SciWin();
  C2F(xloadplots)(cstk(l1),m1);
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------------------------------*/
