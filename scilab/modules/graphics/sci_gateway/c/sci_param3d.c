/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_param3d.c                                                    */
/* desc : interface for param3d routine                                   */
/*------------------------------------------------------------------------*/

#include "sci_param3d.h"
#include "sci_demo.h"
#include "GetCommandArg.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "DestroyObjects.h"
#include "GetProperty.h"
#include "sciCall.h"
#include "sciprint.h"
#include "stack-c.h"
#include "localization.h"
#include "Scierror.h"
/*------------------------------------------------------------------------*/
int sci_param3d( char * fname, unsigned long fname_len )
{
  integer izcol, isfac;
  static double  ebox_def[6]= { 0,1,0,1,0,1};
  double *ebox = ebox_def ;
  static integer iflag_def[3]= {1,2,4};
  integer iflag[3], *ifl, ix1, one=1, zero=0;
  double  alpha_def=35.0 , theta_def=45.0 ;
  double *alpha=&alpha_def, *theta=&theta_def;
  integer m1, n1, l1, m2, n2, l2, m3, n3, l3;
  integer m3n = 0, n3n = 0; /* F.Leray 19.03.04*/

  static rhs_opts opts[]= { {-1,"alpha","?",0,0,0},
                            {-1,"ebox","?",0,0,0},
                            {-1,"flag","?",0,0,0},
                            {-1,"leg","?",0,0,0},
                            {-1,"theta","?",0,0,0},
                            {-1,NULL,NULL,0,0}     };

  char * labels = NULL ;

  if (Rhs <= 0) {
    sci_demo(fname,"t=0:0.1:5*%pi;param3d(sin(t),cos(t),t/10,35,45,'X@Y@Z',[2,4]);", &zero);
    return 0;
  }

  CheckRhs(3,8);

  if ( get_optionals(fname,opts) == 0) return 0;
  if ( FirstOpt() < 4) {
    Scierror(999,_("%s: Misplaced optional argument: #%d must be at position %d.\n"),
      fname,1, 4);
    return(0);
  }

  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
  if (m1 * n1 == 0) { LhsVar(1) = 0; return 0;} 
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);
  GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE, &m3, &n3, &l3);
  CheckSameDims(1,2,m1,n1,m2,n2);
  CheckSameDims(2,3,m2,n2,m3,n3);

  GetOptionalDoubleArg(fname,4,"theta",&theta,1,opts);
  GetOptionalDoubleArg(fname,5,"alpha",&alpha,1,opts);
  GetLabels(fname,6,opts,&labels);

  iflag_def[1]=8;
  ifl=&(iflag_def[1]);
  GetOptionalIntArg(fname,7,"flag",&ifl,2,opts);
  iflag[0]=iflag_def[0];iflag[1]=ifl[0];iflag[2]=ifl[1];
  GetOptionalDoubleArg(fname,8,"ebox",&ebox,6,opts);

  SciWin();
  ix1 = m1 * n1;

  /* NG beg */
  isfac=-1;
  izcol=0;
  
  Objplot3d (fname,&isfac,&izcol,stk(l1),stk(l2),stk(l3),(double *) NULL,&ix1,&one,theta,alpha,labels,iflag,ebox,&m1,&n1,&m2,&n2,&m3,&n3,&m3n,&n3n);/*Adding F.Leray 12.03.04 */
  

  /* NG end */
  LhsVar(1)=0;
  return 0;
}
/*------------------------------------------------------------------------*/
