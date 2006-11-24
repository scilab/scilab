/*------------------------------------------------------------------------*/
/* file: sci_xsegs.c                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xsegs routine                                     */
/*------------------------------------------------------------------------*/

#include "sci_xsegs.h"
#include "sciCall.h"
#include "dr1Call.h"
#include "GetProperty.h"
#include "DrawObjects.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "gw_graphics.h"

/*-----------------------------------------------------------------------------------*/
int sci_xsegs(char *fname,unsigned long fname_len)
{
  integer dstyle = -1, *style,flag;
  integer mn2;
  integer m1,n1,l1,m2,n2,l2,m3=1,n3=1,l3; 
  double arsize = 0.0 ;

  SciWin();

  CheckRhs(2,3);

  GetRhsVar(1,"d",&m1,&n1,&l1);
  GetRhsVar(2,"d",&m2,&n2,&l2);
  CheckSameDims(1,2,m1,n1,m2,n2);
  if (m2*n2 == 0) { LhsVar(1)=0; return 0;} 

  if (Rhs == 3)
  {
    GetRhsVar(3,"i",&m3,&n3,&l3); CheckVector(3,m3,n3);
    if (m3 * n3 == 1) dstyle = *istk(l3 );
    if (m3 * n3 != 1 && m2 * n2 / 2 != m3 * n3) {
      Scierror(999,"%s: style has a wrong size (%d), expecting (%d)\r\n",fname,m3 * n3, m2 * n2 / 2);
      return 0;
    }
  }
  mn2 = m2 * n2; 

  /* NG beg */
  if (version_flag() == 0)
  {
    sciPointObj * psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());

    if (Rhs == 3 && m3 * n3 != 1)
    {
      style = istk(l3);
      flag = 1 ;
    }
    else if (Rhs == 3 && m3 * n3 == 1) {
      style = istk(l3);
      flag = 0 ;
    }
    else
    { /* Rhs < 3 => no color specified, use current color (taken from axes parent) */
      int col = sciGetForeground(sciGetSelectedSubWin(sciGetCurrentFigure ()));
      style = &col;
      flag= 0 ;
    }

    Objsegs (style,flag,mn2,stk(l1),stk(l2),arsize);

    if ( pSUBWIN_FEATURE(psubwin)->surfcounter > 0 )
    {
      Merge3d(psubwin); /* an addtomerge function should be much more efficient */
      /*     EraseAndOrRedraw(sciGetSelectedSubWin (sciGetCurrentFigure ()));} /\* inhibit EraseAndOrRedraw for now F.Leray 20.12.04 *\/ */
      sciDrawObj(sciGetCurrentFigure ());}
    else
    {
      sciDrawObjIfRequired(sciGetCurrentObj ());
    }
  }
  else{
    if (Rhs == 3 && m3 * n3 != 1) {
      style = istk(l3);
      flag = 1 ;
    } 
    else {
      style = &dstyle ;
      flag = 0 ;
    }

    Xsegs (style,flag,mn2,stk(l1),stk(l2),arsize);
  }
  /* NG end */
  LhsVar(1)=0;
  return 0;
} 
/*-----------------------------------------------------------------------------------*/
