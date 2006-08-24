/*------------------------------------------------------------------------*/
/* file: sci_xfpoly.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xfpoly routine                                    */
/*------------------------------------------------------------------------*/

#include "sci_xfpoly.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "SetProperty.h"
#include "ObjectStructure.h"
#include "DrawObjects.h"
#include "dr1Call.h"
#include "sciCall.h"
#include "GetProperty.h"

/*-----------------------------------------------------------------------------------*/
int sci_xfpoly(char *fname,unsigned long fname_len)
{
  integer close=0,m1,n1,l1,m2,n2 ,l2,m3,n3,l3,mn1 ;

  long hdl; /* NG */

  SciWin();
  CheckRhs(2,3);

  GetRhsVar(1,"d",&m1,&n1,&l1);
  GetRhsVar(2,"d",&m2,&n2,&l2);
  CheckSameDims(1,2,m1,n1,m2,n2);

  if (Rhs == 3) {
    GetRhsVar(3,"d",&m3,&n3,&l3);
    CheckScalar(3,m3,n3);
    close = (integer) *stk(l3);
  } 
  mn1 = m1 * n1;
  /* NG beg */
  if ( version_flag() == 0)
  {
    sciPointObj *psubwin = (sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ());

    if(close == 0)
    {
      close = sciGetForeground(sciGetSelectedSubWin(sciGetCurrentFigure ()));
    }
    Objfpoly (stk(l1),stk(l2),mn1,&close,&hdl,0);

    if ( sciGetSubwinNbSurf( psubwin ) > 0 )
    {
      Merge3d(psubwin); /* an addtomerge function should be much more efficient */
      /*    EraseAndOrRedraw(sciGetSelectedSubWin (sciGetCurrentFigure ()));} /\* inhibit EraseAndOrRedraw for now F.Leray 20.12.04 *\/ */
      sciDrawObj(sciGetCurrentFigure ());}
    else
    {
      sciDrawObjIfRequired(sciGetCurrentObj ());
    }
    /* NG end */
  }
  else
  {
    Xfpoly(mn1,close,stk(l1),stk(l2));
  }
  LhsVar(1)=0;
  return 0;

}

/*-----------------------------------------------------------------------------------*/