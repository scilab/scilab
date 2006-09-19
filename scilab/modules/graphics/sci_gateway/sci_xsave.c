/*------------------------------------------------------------------------*/
/* file: sci_xsave.c                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for sci_xsave routine                                 */
/*------------------------------------------------------------------------*/

#include "sci_xsave.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "Xcall1.h"
#include "GetProperty.h"
#include "RecSaveLoad.h"

/*-----------------------------------------------------------------------------------*/
int sci_xsave( char * fname, unsigned long fname_len )
{
  integer m1,n1,l1,m2,n2,l2,v,wid;
  double dv;

  CheckRhs(1,2);
  /* NG beg */
  if (version_flag() == 0) {
    /* call a Scilab function to handle compatibility */
    C2F(callscifun)("%xsave",(unsigned long) 6);
    return 0;
  }/* NG end */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  if (Rhs == 2)
  {
    GetRhsVar(2,"d",&m2,&n2,&l2);
    CheckScalar(2,m2,n2);
    wid = (integer) *stk(l2 );
  } 
  else {
    integer verb=0,na;
    C2F(dr)("xget","window",&verb,&wid,&na,&v,&v,&v,&dv,&dv,&dv,&dv,5L,7L);
  }
  SciWin();

  C2F(xsaveplots)(&wid,cstk(l1),m1);
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------------------------------*/
