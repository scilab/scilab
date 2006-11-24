/*------------------------------------------------------------------------*/
/* file: sci_contour.c                                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for contour routine                                   */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "sci_contour.h"
#include "sci_demo.h"
#include "GetCommandArg.h"
#include "Graphics.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "DestroyObjects.h"

/*-----------------------------------------------------------------------------------*/
int sci_contour( char * fname, unsigned long fname_len )
{
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,m4,n4,l4, flagx,nz ;
  static integer iflag_def[3]={2,2,3};
  integer *iflag = iflag_def;
  static double ebox_def[6]= { 0,1,0,1,0,1};
  double *ebox = ebox_def;
  double alpha_def = 35.0, theta_def=45.0, zlev_def=0.0;
  double *alpha=&alpha_def, *theta=&theta_def, *zlev=&zlev_def;
  int one = 1 ;
  
  static rhs_opts opts[]= { {-1,"alpha","?",0,0,0},
                            {-1,"ebox","?",0,0,0},
                            {-1,"flag","?",0,0,0},
                            {-1,"leg","?",0,0,0},
                            {-1,"theta","?",0,0,0},
                            {-1,"zlev","?",0,0,0},
                            {-1,NULL,NULL,0,0}     };

  char * labels = NULL ;

  if (Rhs <= 0) {
    sci_demo(fname,"contour(1:5,1:10,rand(5,10),5);", &one);
    return 0;
  }

  CheckRhs(4,10) ;
  CheckLhs(0,1) ;

  if ( get_optionals(fname,opts) == 0) return 0;
  if ( FirstOpt() < 5) {
    sciprint("%s: misplaced optional argument, first must be at position %d \r\n",
      fname,5);
    Error(999); 
    return(0);
  }
  GetRhsVar(1, "d", &m1, &n1, &l1);
  CheckVector(1,m1,n1);
  GetRhsVar(2, "d", &m2, &n2, &l2);
  CheckVector(2,m2,n2);
  GetRhsVar(3, "d", &m3, &n3, &l3)
    if (m3 * n3 == 0) {LhsVar(1)=0;  return 0;} 
    if (m3 == 1 || n3 == 1) {
      Scierror(999,"%s: third argument is a vector, expecting a matrix \r\n",fname,m3*n3);
      return 0;
    }

    CheckDimProp(1,3, m1 * n1 != m3); 
    CheckDimProp(2,3, m2 * n2 != n3); 

    GetRhsVar(4, "d", &m4, &n4, &l4);
    if (m4 * n4 == 0) {LhsVar(1)=0;  return 0;} 
    if (m4 * n4 == 1) {
      flagx = 0;  nz = Max(1,(integer) *stk(l4));
    } else {
      flagx = 1;  nz = m4 * n4;
    }
    GetOptionalDoubleArg(fname,5,"theta",&theta,1,opts);
    GetOptionalDoubleArg(fname,6,"alpha",&alpha,1,opts);
    GetLabels(fname,7,opts,&labels);
    GetOptionalIntArg(fname,8,"flag",&iflag,3,opts);
    GetOptionalDoubleArg(fname,9,"ebox",&ebox,6,opts);
    GetOptionalDoubleArg(fname,10,"zlev",&zlev,1,opts);
    SciWin();
    SciGerase() ;
    C2F(contour)(stk(l1), stk(l2), stk(l3), &m3, &n3, &flagx, &nz, stk(l4), theta, alpha,
      labels, iflag, ebox, zlev, bsiz);
    LhsVar(1)=0;
    return 0;
}
/*-----------------------------------------------------------------------------------*/
