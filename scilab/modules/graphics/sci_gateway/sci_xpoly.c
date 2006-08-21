/*------------------------------------------------------------------------*/
/* file: sci_xpoly.h                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xpoly routine                                     */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "sci_xpoly.h"
#include "stack-c.h"
#include "Graphics.h"
#include "sciCall.h"
#include "dr1Call.h"
#include "DrawObjects.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "BuildObjects.h"

/*-----------------------------------------------------------------------------------*/
int sci_xpoly( char * fname, unsigned long fname_len )
{
  integer m1,n1,l1 ,m2 ,n2 ,l2,m3,n3,l3,m4,n4,l4,close=0,mn2;

  long hdl;/* NG */
  int mark;/* NG */

  SciWin();

  CheckRhs(2,4);
  GetRhsVar(1,"d",&m1,&n1,&l1);
  GetRhsVar(2,"d",&m2,&n2,&l2);
  CheckSameDims(1,2,m1,n1,m2,n2);
  mn2 = m2 * n2;
  if ( mn2 == 0 ) {  LhsVar(1)=0; return 0; } 
  if (Rhs >= 3) {
    GetRhsVar(3,"c",&m3,&n3,&l3);
    if ( strcmp(cstk(l3),"lines") == 0) {
      strcpy(C2F(cha1).buf,"xlines"); 
      mark=1; /* NG */
    } else if (strcmp(cstk(l3),"marks") == 0) {
      strcpy(C2F(cha1).buf,"xmarks"); 
      mark=0; /* NG */
    } else {
      Scierror(999,"%s:  dtype must be \"lines\" or \"marks\"\r\n",fname);
      return 0;
    } 
  }
  else {
    strcpy(C2F(cha1).buf,"xlines");
    mark=1; /* NG */
  }

  if (Rhs >= 4) { GetRhsVar(4,"d",&m4,&n4,&l4); CheckScalar(4,m4,n4); close = (integer) *stk(l4);} 
  /* NG beg */
  if (version_flag() == 0){
    sciPointObj *pobj = NULL;
    sciPointObj *psubwin = (sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ());

    Objpoly (stk(l1),stk(l2),mn2,close,mark,&hdl);

    pobj = sciGetCurrentObj(); /* the polyline newly created */
    if(mark == 0){ 
      /* marks are enabled but markstyle & foreground 
      is determined by parents' markstyle & foreground */
      sciSetIsMark(pobj, TRUE);
      sciSetIsLine(pobj,FALSE);
      sciSetMarkStyle (pobj,sciGetMarkStyle(psubwin));
      sciSetForeground (pobj, sciGetForeground (psubwin));
    }
    else{
      sciSetIsMark(pobj, FALSE);
      sciSetIsLine(pobj, TRUE);
      sciSetLineStyle(pobj, sciGetLineStyle (psubwin));
      sciSetForeground (pobj, sciGetForeground (psubwin));
    }

    if (pSUBWIN_FEATURE(psubwin)->surfcounter>0){
      Merge3d(psubwin); /* an addtomerge function should be much more efficient */
      sciDrawObj(sciGetCurrentFigure ());}
    else
      sciDrawObjIfRequired(pobj);
  }
  else
    Xpoly(C2F(cha1).buf,bsiz,mn2,close,stk(l1),stk(l2));
  /* NG end */
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------------------------------*/
