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
/* file: sci_champ.c                                                      */
/* desc : interface for champ (and champ1) routine                        */
/*------------------------------------------------------------------------*/

#include "sci_champ.h"
#include "stack-c.h"
#include "sci_demo.h"
#include "GetCommandArg.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "DestroyObjects.h"
#include "DefaultCommandArg.h"
#include "Champ.h"
#include "GetProperty.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_champ (char *fname,unsigned long fname_len)
{
  return sci_champ_G(fname,C2F(champ),fname_len);
}
/*--------------------------------------------------------------------------*/
int sci_champ1 (char *fname,unsigned long fname_len)
{
  return sci_champ_G(fname,C2F(champ1),fname_len);
}
/*--------------------------------------------------------------------------*/
int sci_champ_G(char *fname,
                int (*func) (double *, double *, double *, double *, int *, int *, char *, double *, double *, int),
                unsigned long fname_len)
{
  double arfact_def=1.0;
  double * arfact=&arfact_def;
  int m1, n1, l1, m2, n2, l2, m3, n3, l3, m4, n4, l4;
  static rhs_opts opts[]= { {-1,"arfact","?",0,0,0},
  {-1,"rect","?",0,0,0},
  {-1,"strf","?",0,0,0},
  {-1,NULL,NULL,0,0}};

  char   * strf = NULL ;
  double * rect = NULL ;

  CheckRhs(-1,7) ;
  CheckLhs(0,1) ;

  if (Rhs <= 0) {
    if (strcmp(fname,"champ")==0) 
    {
      sci_demo(fname,"champ(1:10,1:10,rand(10,10),rand(10,10),1.0);", FALSE);
    }
    else if (strcmp(fname,"champ1")==0)
    {
      sci_demo(fname,"champ1(1:10,1:10,rand(10,10),rand(10,10),1.0);", FALSE);
    }
    return 0;
  } 
  else if ( Rhs < 4 ) 
  {
    Scierror(999,_("%s: Wrong number of input arguments: At least %d expected.\n"),fname,4);
    return 0;
  }
  if ( get_optionals(fname,opts) == 0) { return 0 ; }
  if ( FirstOpt() < 5 )
  {
    Scierror(999, _("%s: Misplaced optional argument: #%d must be at position %d\n"),
      fname,1, 5);
    return -1;
  }
  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);
  GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE, &m3, &n3, &l3);
  GetRhsVar(4,MATRIX_OF_DOUBLE_DATATYPE, &m4, &n4, &l4);
  CheckSameDims(3,4,m3,n3,m4,n4);
  CheckDimProp(2,3,m2 * n2 != n3);
  CheckDimProp(1,3,m1 * n1 != m3); 
  if (m3 * n3 == 0)
  { 
    LhsVar(1) = 0;
    return 0;
  } 

  GetOptionalDoubleArg(fname,5,"arfact",&arfact,1,opts);
  GetRect(fname,6,opts,&rect);
  GetStrf(fname,7,opts,&strf);

  SciWin();

  if ( isDefStrf( strf ) )
  {
    char strfl[4];
    strcpy(strfl,DEFSTRFN);
    strf = strfl;
    if ( !isDefRect( rect ) ) {strf[1]='5';} 
  }

  (*func)(stk(l1 ),stk(l2 ),stk(l3 ),stk(l4 ),&m3,&n3,strf,rect, arfact, 4L);
  LhsVar(1)=0;
  return 0;
}
/*--------------------------------------------------------------------------*/
