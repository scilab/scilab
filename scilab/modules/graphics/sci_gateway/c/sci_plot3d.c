/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRU (nicer default plot3d)
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_plot3d.c                                                     */
/* desc : interface for plot3d (and plot3d1) routine                      */
/*------------------------------------------------------------------------*/

#include <stdio.h>

#include "sci_plot3d.h"
#include "stack-c.h"
#include "Plo3d.h"
#include "sci_demo.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "GetProperty.h"
#include "GetCommandArg.h"
#include "DestroyObjects.h"
#include "MALLOC.h"
#include "sciCall.h"
#include "sciprint.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_plot3d( char * fname, unsigned long fname_len )
{
  static double  ebox_def [6]= { 0,1,0,1,0,1};
  double *ebox = ebox_def ;
  static integer iflag_def[3]={2,2,4};
  integer *iflag = iflag_def;
  double  alpha_def=35.0 , theta_def=45.0 ;
  double *alpha=&alpha_def, *theta=&theta_def;
  integer m1 = 0, n1 = 0, l1 = 0, m2 = 0, n2 = 0, l2 = 0, m3 = 0, n3 = 0, l3 = 0;
  integer m3n = 0, n3n = 0, l3n = 0, m3l = 0, n3l = 0, l3l = 0;

  integer izcol,  isfac;
  double *zcol=NULL;

  static rhs_opts opts[]= { {-1,"alpha","?",0,0,0},
  {-1,"ebox","?",0,0,0},
  {-1,"flag","?",0,0,0},
  {-1,"leg","?",0,0,0},
  {-1,"theta","?",0,0,0},
  {-1,NULL,NULL,0,0}};

  char * legend = NULL ;

  if (Rhs <= 0)
  {
    int zero = 0 ;
    sprintf(C2F(cha1).buf,"x = %%pi * [-1:0.05:1]';z = sin(x)*cos(x)';f = gcf();f.color_map = jetcolormap(32);%s(x, x, z, 70, 70);e=gce();e.color_flag = 1;",fname);
    sci_demo(fname,C2F(cha1).buf, &zero);
    return 0;
  }

  CheckRhs(3,8);

  if ( get_optionals(fname,opts) == 0) return 0;
  if ( FirstOpt() < 4) {
    sciprint(_("%s: Misplaced optional argument: #%d must be at position %d.\n"),
      fname,1, 4);
    Error(999); 
    return(0);
  }

  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);
  if (m1 * n1 == 0) 	{ LhsVar(1) = 0; return 0;}

  if (Rhs >= 3) {
    /*     third argument can be a matrix z or a list list(z,zcol) */
    switch ( VarType(3) ) 
    {
    case 1 : 
      GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE, &m3, &n3, &l3);
      izcol = 0;
      break;
    case 15 : 
      izcol = 1;
      /* z = list(z,colors) */
      GetRhsVar(3,LIST_DATATYPE,&m3l,&n3l,&l3l);
      if ( m3l != 2 ) 
      {
        Scierror(999,_("%s: Wrong size for input argument #%d: List of size %d expected.\n"),
          fname, 3, 2);
        return 0;
      }
      GetListRhsVar(3,1,MATRIX_OF_DOUBLE_DATATYPE,&m3,&n3,&l3);
      GetListRhsVar(3,2,MATRIX_OF_DOUBLE_DATATYPE,&m3n,&n3n,&l3n);
      zcol  = stk(l3n);
      if (m3n * n3n != n3 &&  m3n*n3n != m3*n3)
      {
        Scierror(999,_("%s: Wrong size for input argument #%d: %d or %d expected.\n"),fname,3, n3,m3*n3);
        return 0;
      }
      /* 
      *   Added by E Segre 4/5/4000. In the case where zcol is a 
      *   matrix of the same size as z, we set izcol to 2. This
      *   value is later transmitted to the C2F(fac3dg) routine,
      *   which has been modified to do the interpolated shading 
      *    (see the file SCI/modules/graphics/src/c/Plo3d.c
      */
      if (   m3n*n3n == m3*n3 ) { izcol=2  ; }
      break ;
    default : 
      OverLoad(3);
      return 0;
    }
  }
  iflag_def[1]=8;

  GetOptionalDoubleArg(fname,4,"theta",&theta,1,opts);
  GetOptionalDoubleArg(fname,5,"alpha",&alpha,1,opts);
  GetLabels(fname,6,opts,&legend);
  GetOptionalIntArg(fname,7,"flag",&iflag,3,opts);
  GetOptionalDoubleArg(fname,8,"ebox",&ebox,6,opts);

  if (m1 * n1 == m3 * n3 && m1 * n1 == m2 * n2 && m1 * n1 != 1) {
    if (! (m1 == m2 && m2 == m3 && n1 == n2 && n2 == n3)) {
      Scierror(999,_("%s: Wrong value for input arguments #%d, #%d and #%d: Incompatible length.\n"),fname, 1, 2, 3);
      return 0;
    }
  } else {
    if (m2 * n2 != n3) {
      Scierror(999,_("%s: Wrong value for input arguments #%d and #%d: Incompatible length.\n"),fname, 2, 3);
      return 0;
    }
    if (m1 * n1 != m3) {
      Scierror(999,_("%s: Wrong value for input arguments #%d and #%d: Incompatible length.\n"),fname, 1, 3);
      return 0;
    }
    if ( m1*n1 <= 1 || m2*n2 <= 1 ) 
    {
		Scierror(999,_("%s: Wrong size for input arguments #%d and #%d: %s expected.\n"),fname, 2, 3, ">= 2");
      return 0;
    }
  }

  if (m1 * n1 == 0 || m2 * n2 == 0 || m3 * n3 == 0) { LhsVar(1)=0; return 0;} 
  SciWin() ;

  /******************** 24/015/2002 ********************/
  if (m1 * n1 == m3 * n3 && m1 * n1 == m2 * n2 && m1 * n1 != 1) /* NG beg */
  {
    isfac=1;
  }
  else
  {
    isfac=0;
  }


  Objplot3d (fname,&isfac,&izcol,stk(l1),stk(l2),stk(l3),zcol,&m3,&n3,theta,alpha,legend,iflag,ebox,&m1,&n1,&m2,&n2,&m3,&n3,&m3n,&n3n);/*Adding F.Leray 12.03.04 and 19.03.04*/

  LhsVar(1)=0;
  return 0;

}
/*--------------------------------------------------------------------------*/
