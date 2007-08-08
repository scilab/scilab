/*------------------------------------------------------------------------*/
/* file: sci_xclear.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xclear routine                                    */
/*------------------------------------------------------------------------*/

#include "sci_xclear.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "Xcall1.h"
#include "GetProperty.h"
#include "DrawObjects.h"


/*-----------------------------------------------------------------------------------*/
int sci_xclear( char * fname, unsigned long fname_len )
{
  integer verb=0,wid,cur,win,na;
  integer ix,m1,n1,l1/*,v=0*/;
  double dv;

  CheckRhs(0,2); /* NG  */ /*CheckRhs(-1,1) */ 
  CheckLhs(0,1);
  SciWin();
  if (Rhs == 1) 
  {
    GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
    C2F(dr1)("xget","window",&verb,&cur,&na,PI0,PI0,PI0,&dv,&dv,&dv,&dv,5L,7L);
    for (ix = 0 ; ix < m1*n1 ; ++ix) 
    {
      wid = (integer) *stk(l1 +ix );
      C2F(dr1)("xset","window",&wid,PI0,PI0,PI0,PI0,PI0,&dv,&dv,&dv,&dv,5L,7L);
      C2F(dr1)("xclear","v",PI0,PI0,PI0,PI0,PI0,PI0,&dv,&dv,&dv,&dv,7L,2L);
    }
    C2F(dr1)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,&dv,&dv,&dv,&dv,5L,7L);
  }
  else 
  {
    C2F(dr1)("xget","window",&verb,&win,&na,PI0,PI0,PI0,&dv,&dv,&dv,&dv,5L,7L);
    C2F(dr1)("xset","window",&win,PI0,PI0,PI0,PI0,PI0,&dv,&dv,&dv,&dv,5L,7L);
    C2F(dr1)("xclear","v",PI0,PI0,PI0,PI0,PI0,PI0,&dv,&dv,&dv,&dv,7L,2L); 
  } 

  sciXclear();   /* NG */

  LhsVar(1)=0;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
