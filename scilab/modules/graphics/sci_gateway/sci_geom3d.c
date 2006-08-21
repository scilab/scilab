/*------------------------------------------------------------------------*/
/* file: sci_geom3d.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for geom3d routine                                    */
/*------------------------------------------------------------------------*/

#include "sci_geom3d.h"
#include "BuildObjects.h"
#include "sci_demo.h"
#include "Plo3d.h"

/*-----------------------------------------------------------------------------------*/
int sci_geom3d( char * fname, unsigned long fname_len )
{
  integer ix1, m1, n1, l1, m2, n2, l2, m3, n3, l3;

  if ( Rhs <= 0 )
  {
    int one = 1 ;
    sci_demo( fname, "t=0:0.1:5*%pi,[x,y]=geom3d(sin(t),cos(t),t/10);", &one ) ;
    return 0 ;
  }

  CheckRhs(3,3);
  CheckLhs(2,3);

  GetRhsVar(1, "d", &m1, &n1, &l1);
  GetRhsVar(2, "d", &m2, &n2, &l2);
  GetRhsVar(3, "d", &m3, &n3, &l3);
  CheckSameDims(1,2,m1,n1,m2,n2);
  CheckSameDims(2,3,m2,n2,m3,n3);
  if (m1 * n1 == 0)  { LhsVar(1) = 0; return 0;}
  SciWin();
  ix1 = m1 * n1;
  C2F(geom3d)(stk(l1), stk(l2), stk(l3), &ix1);
  LhsVar(1)=1;
  LhsVar(2)=2;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
