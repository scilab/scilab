/*------------------------------------------------------------------------*/
/* file: sci_xrect.c                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy, Jean-Philipe Chancelier  */
/* desc : interface for xrect routine                                     */
/*------------------------------------------------------------------------*/

#include "sci_xrect.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "sciCall.h"
#include "DrawObjects.h"
#include "GetProperty.h"
#include "CurrentObjectsManagement.h"

/*-----------------------------------------------------------------------------------*/
int sci_xrect( char *fname, unsigned long fname_len )
{
  long hdl;
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,m4,n4,l4;
  sciPointObj * psubwin = NULL ;
  
  SciWin();
  CheckRhs(1,4);

  psubwin = sciGetCurrentSubWin() ;

  switch ( Rhs ) 
  {
  case 1 :
    GetRhsVar(1,"d",&m1,&n1,&l1); 
    CheckLength(1,m1*n1,4);
    if (strcmp(fname,"xrect")==0)
    {
      int foreground = sciGetForeground(psubwin);
      Objrect (stk(l1),stk(l1+1),stk(l1+2),stk(l1+3),
        &foreground,NULL,FALSE,TRUE,0,&hdl,FALSE);
    }
    else
    { /* xfrect case */
      int foreground = sciGetForeground(psubwin);
      Objrect (stk(l1),stk(l1+1),stk(l1+2),stk(l1+3),
        NULL,&foreground,TRUE,FALSE,0,&hdl,FALSE);
    }

    if ( hdl < 0 )
    {
      return -1 ;
    }

    if (pSUBWIN_FEATURE(psubwin)->surfcounter>0)
    {
      Merge3d(psubwin); /* an addtomerge function should be much more efficient */
      sciDrawObj(sciGetCurrentFigure ());
    }
    else
    {
      sciDrawObjIfRequired(sciGetCurrentObj ());
    }

    break;
  case 4 :
    GetRhsVar(1,"d",&m1,&n1,&l1); CheckScalar(1,m1,n1);
    GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2);
    GetRhsVar(3,"d",&m3,&n3,&l3); CheckScalar(3,m3,n3);
    GetRhsVar(4,"d",&m4,&n4,&l4); CheckScalar(4,m4,n4);
    if (strcmp(fname,"xrect")==0)
    {	
      int foreground = sciGetForeground(psubwin);
      Objrect (stk(l1),stk(l2),stk(l3),stk(l4),
        &foreground,NULL,FALSE,TRUE,0,&hdl,FALSE) ;
    }
    else
    {
      int foreground = sciGetForeground(psubwin);
      Objrect (stk(l1),stk(l2),stk(l3),stk(l4),
        NULL,&foreground,TRUE,FALSE,0,&hdl,FALSE);
    }

    if ( hdl < 0 )
    {
      return -1 ;
    }

    if (pSUBWIN_FEATURE(psubwin)->surfcounter>0)
    {
      Merge3d(psubwin); /* an addtomerge function should be much more efficient */
      sciDrawObj(sciGetCurrentFigure ());}
    else
    {
      sciDrawObjIfRequired(sciGetCurrentObj ());
    }

    break;
  default :
    Scierror(999,"%s: wrong number of rhs argumens (%d), rhs must be 1 or 4\r\n",fname,Rhs);
    break ;
  }
  LhsVar(1)=0;
  return 0;
} 

/*-----------------------------------------------------------------------------------*/
