/*------------------------------------------------------------------------*/
/* file: sci_geom3d.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for geom3d routine                                    */
/*------------------------------------------------------------------------*/

#include "sci_geom3d.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "sci_demo.h"
#include "Plo3d.h"
#include "PloEch.h"
#include "GetProperty.h"
#include "axesScale.h"
#include "CurrentObjectsManagement.h"
#include "sciprint.h"

int geom3d(double *x, double *y, double *z, integer *n) ;

/*-----------------------------------------------------------------------------------*/
int geom3d(double *x, double *y, double *z, integer *n)
{
  integer j;

  sciPointObj * psubwin = sciGetCurrentSubWin();
  sciSubWindow * ppsubwin = pSUBWIN_FEATURE(psubwin);

  if(ppsubwin->logflags[0] =='l')
  {
    for ( j = 0 ; j < (*n) ; j++ ) {
      if(x[j] <= 0.){
        sciprint("geom3d error : Operation can not be performed because X axis is in logscale mode and the specified x vector has a negative value\n");
        return -1;
      }
      x[j] = log10(x[j]);
    }
  }

  if(ppsubwin->logflags[1] =='l')
  {
    for ( j =0 ; j < (*n) ; j++) {
      if(y[j] <= 0.){
        sciprint("geom3d error : Operation can not be performed because Y axis is in logscale mode and the specified y vector has a negative value\n");
        return -1;
      }
      y[j] = log10(y[j]);
    }
  }

  if(ppsubwin->logflags[2] =='l')
  {
    for ( j =0 ; j < (*n) ; j++) {
      if(z[j] <= 0.){
        sciprint("geom3d error : Operation can not be performed because Z axis is in logscale mode and the specified z vector has a negative value\n");
        return -1;
      }
      z[j] = log10(z[j]);
    }
  }

  if( ppsubwin->axes.reverse[0] )
  {
    for ( j =0 ; j < (*n) ; j++) x[j] = InvAxis(ppsubwin->FRect[0],ppsubwin->FRect[2],x[j]);
  }

  if( ppsubwin->axes.reverse[1] )
  {
    for ( j =0 ; j < (*n) ; j++) y[j] = InvAxis(ppsubwin->FRect[1],ppsubwin->FRect[3],y[j]);
  }

  if( ppsubwin->axes.reverse[2] )
  {
    for ( j =0 ; j < (*n) ; j++) z[j] = InvAxis(ppsubwin->FRect[4],ppsubwin->FRect[5],z[j]);
  }

  for ( j =0 ; j < (*n) ; j++)
  {
    double x1,y1_;
    x1=TRX(x[j],y[j],z[j]);
    y1_=TRY(x[j],y[j],z[j]);
    z[j]=TRZ(x[j],y[j],z[j]);
    x[j]=x1;
    y[j]=y1_;
  }
  return(0);
}

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

  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);
  GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE, &m3, &n3, &l3);
  CheckSameDims(1,2,m1,n1,m2,n2);
  CheckSameDims(2,3,m2,n2,m3,n3);
  if (m1 * n1 == 0)  { LhsVar(1) = 0; return 0;}
  SciWin();
  ix1 = m1 * n1;
  geom3d(stk(l1), stk(l2), stk(l3), &ix1);
  LhsVar(1)=1;
  LhsVar(2)=2;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
