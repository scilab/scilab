/*------------------------------------------------------------------------*/
/* file: sci_xpolys.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xpolys routine                                    */
/*------------------------------------------------------------------------*/

#include "sci_xpoly.h"
#include "stack-c.h"
#include "Graphics.h"
#include "sciCall.h"
#include "dr1Call.h"
#include "DrawObjects.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "BuildObjects.h"
#include "gw_graphics.h"


/*-----------------------------------------------------------------------------------*/
int sci_xpolys(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3 ;
  int i;
  long hdl;

  SciWin();
  CheckRhs(2,3);

  GetRhsVar(1,"d",&m1,&n1,&l1);
  GetRhsVar(2,"d",&m2,&n2,&l2);
  CheckSameDims(1,2,m1,n1,m2,n2);

  if ( m1 * n1 == 0 || m2 * n2 == 0 )
  {
    /* dimension 0, 0 polyline to draw */
    LhsVar(1)=0 ;
    return 0 ;
  }

  if (Rhs == 3) 
  {
    GetRhsVar(3,"i",&m3,&n3,&l3); CheckVector (3,m3,n3); CheckDimProp(1,3,m3 * n3 < n1);
  }
  else
  {
    int un=1;
    CreateVar(3,"i",&un,&n1,&l3);
    for (i = 0 ; i < n1 ; ++i) *istk(l3+i) = 1;
  } 
  if (version_flag() == 0)
  {
    sciPointObj *psubwin = (sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ());
    for (i = 0; i < n1; ++i) 
      Objpoly (stk(l1+(i*m1)),stk(l2+(i*m2)),m1,0,*istk(l3+i),&hdl);

    /** construct Compound and make it current object**/
    sciSetCurrentObj (ConstructCompoundSeq (n1));

    if (pSUBWIN_FEATURE(psubwin)->surfcounter>0)
    {
      Merge3d(psubwin); /* an addtomerge function should be much more efficient */
      /*    EraseAndOrRedraw(sciGetSelectedSubWin (sciGetCurrentFigure ()));} */ /* inhibit EraseAndOrRedraw for now F.Leray 20.12.04 */
      sciDrawObj(sciGetCurrentFigure ());
    }
    else
    {
      sciDrawObjIfRequired(sciGetCurrentObj ());
    }
  }
  else
  {
    Xpolys(istk(l3),n2,m2,stk(l1),stk(l2));
  }
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------------------------------*/
