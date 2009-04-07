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
/* file: sci_param3d1.h                                                   */
/* desc : interface for param3d1 routine                                  */
/*------------------------------------------------------------------------*/

#include "sci_param3d1.h"
#include "sci_demo.h"
#include "GetCommandArg.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "DestroyObjects.h"
#include "GetProperty.h"
#include "sciCall.h"
#include "MALLOC.h"
#include "stack-c.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_param3d1( char *fname, unsigned long fname_len )
{
  int izcol, isfac;
  double *zcol=NULL;
  static double  ebox_def [6]= { 0,1,0,1,0,1};
  double *ebox = ebox_def ;
  static int iflag_def[3]={1,2,4};
  int iflag[3] , *ifl;
  double  alpha_def=35.0 , theta_def=45.0 ;
  double *alpha=&alpha_def, *theta=&theta_def;
  int m1, n1, l1, m2, n2, l2, m3, n3, l3;
  int m3n = 0, n3n = 0, l3n, m3l, n3l, l3l;
  static rhs_opts opts[]= { {-1,"alpha","?",0,0,0},
                            {-1,"ebox","?",0,0,0},
                            {-1,"flag","?",0,0,0},
                            {-1,"leg","?",0,0,0},
                            {-1,"theta","?",0,0,0},
                            {-1,NULL,NULL,0,0}     };

  char * labels = NULL ;

  if (Rhs <= 0)
  {
    sci_demo(fname, fname_len);
    return 0;
  }

  CheckRhs(3,8);

  if ( get_optionals(fname,opts) == 0) 
  {
	  C2F(putlhsvar)();
	  return 0;
  }

  if ( FirstOpt() < 4) 
  {
    Scierror(999,_("%s: Misplaced optional argument: #%d must be at position %d.\n"),fname,1,4);
    return(0);
  }

  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);/* x */
  if (m1 == 1 && n1 > 1) {m1 = n1;n1 = 1;}

  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);/* y */
  if (m2 == 1 && n2 > 1) {m2 = n2;n2 = 1;}

  if (m1 * n1 == 0) 
  { 
	  LhsVar(1) = 0;
	  C2F(putlhsvar)();
	  return 0;
  }
  CheckSameDims(1,2,m1,n1,m2,n2); 

  switch ( VarType(3) ) 
  {
  case 1 : 
    izcol = 0;
    GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE, &m3, &n3, &l3); /* z */
    break;
  case 15 : 
    izcol = 1;
    /* z = list(z,colors) */
    GetRhsVar(3,LIST_DATATYPE,&m3l,&n3l,&l3l);
    if ( m3l != 2 ) 
    {
      Scierror(999,_("%s: Wrong size for input argument #%d: List of size %d expected.\n"),
        fname,2,m3l,2);
      return 0;
    }
    GetListRhsVar(3,1,MATRIX_OF_DOUBLE_DATATYPE,&m3,&n3,&l3); /* z*/ 
    GetListRhsVar(3,2,MATRIX_OF_DOUBLE_DATATYPE,&m3n,&n3n,&l3n); /* colors */
    zcol  = stk(l3n);
    if (m3n * n3n != n3) 
    {
      Scierror(999,_("%s: Wrong size for input argument #%d: %d expected.\n"),fname,3,n3);
      return 0;
    }
    break ;
  default : 
    OverLoad(3);
    return 0;
  }
  if (m3 == 1 && n3 > 1) {m3 = n3;n3 = 1;}
  CheckSameDims(1,3,m1,n1,m3,n3); 

  GetOptionalDoubleArg(fname,4,"theta",&theta,1,opts);
  GetOptionalDoubleArg(fname,5,"alpha",&alpha,1,opts);
  GetLabels(fname,6,opts,&labels);
  iflag_def[1]=8;
  ifl=&(iflag_def[1]);
  GetOptionalIntArg(fname,7,"flag",&ifl,2,opts);
  iflag[0]=iflag_def[0];iflag[1]=ifl[0];iflag[2]=ifl[1];

  GetOptionalDoubleArg(fname,8,"ebox",&ebox,6,opts);

  if (m1 == 1 && n1 > 1) { m1 = n1;    n1 = 1; }
  SciWin();
  /* NG beg */
  isfac=-1;

  Objplot3d (fname,&isfac,&izcol,stk(l1),stk(l2),stk(l3),zcol,&m1,&n1,theta,alpha,labels,iflag,ebox,&m1,&n1,&m2,&n2,&m3,&n3,&m3n,&n3n); /*Adding F.Leray 12.03.04*/
  
  LhsVar(1) = 0;
  C2F(putlhsvar)();
  return 0;
}
/*--------------------------------------------------------------------------*/
