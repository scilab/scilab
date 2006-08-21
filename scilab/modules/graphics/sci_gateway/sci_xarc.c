/*------------------------------------------------------------------------*/
/* file: sci_xarc.h                                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xarc routine                                      */
/*------------------------------------------------------------------------*/

#include "sci_xarc.h"
#include "sciCall.h"
#include "dr1Call.h"
#include "GetProperty.h"
#include "stack-c.h"
#include "BuildObjects.h"

/*-----------------------------------------------------------------------------------*/
int sci_xarc(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1,l2,l3,l4,l5,l6;
  long hdl;/* NG */

  SciWin();
  CheckRhs(6,6);
  GetRhsVar(1,"d",&m1,&n1,&l1);CheckScalar(1,m1,n1);
  GetRhsVar(2,"d",&m1,&n1,&l2);CheckScalar(2,m1,n1);
  GetRhsVar(3,"d",&m1,&n1,&l3);CheckScalar(3,m1,n1);
  GetRhsVar(4,"d",&m1,&n1,&l4);CheckScalar(4,m1,n1);
  GetRhsVar(5,"i",&m1,&n1,&l5);CheckScalar(5,m1,n1);
  GetRhsVar(6,"i",&m1,&n1,&l6);CheckScalar(6,m1,n1);
  /* NG beg */
  if ( version_flag() == 0 )
  {
    int curcolor = sciGetForeground(sciGetSelectedSubWin(sciGetCurrentFigure ())); /* current color equivalent for new graphics mode */
    if ( strcmp(fname,"xarc") == 0 )
    {
      Objarc (istk(l5),istk(l6),stk(l1),stk(l2),stk(l3),stk(l4),&curcolor,NULL,FALSE,TRUE,&hdl);
    }
    else /* xfarc case */
    {
      Objarc (istk(l5),istk(l6),stk(l1),stk(l2),stk(l3),stk(l4),NULL,&curcolor,TRUE,FALSE,&hdl);
    }
  }
  else
  {
    Xarc(fname,fname_len,istk(l5),istk(l6),stk(l1),stk(l2),stk(l3),stk(l4)) ;
  }
  /* NG end */
  LhsVar(1)=0;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
