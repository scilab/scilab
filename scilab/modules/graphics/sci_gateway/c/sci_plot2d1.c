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
/* file: sci_plot2d1.c                                                    */
/* desc : interface for plot2d1, plot2d2, plot2d3 and plot2d4 routines    */
/*------------------------------------------------------------------------*/

#include "sci_plot2d1.h"
#include "stack-c.h"
#include "sci_demo.h"
#include "GetCommandArg.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "DestroyObjects.h"
#include "DefaultCommandArg.h"
#include "GetProperty.h"
#include "sciCall.h"
#include "GraphicSynchronizerInterface.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_plot2d1_1 (char *fname,unsigned long fname_len)
{
  return sci_plot2d1_G("plot2d1",1,fname_len);/* NG */
}
/*--------------------------------------------------------------------------*/
int sci_plot2d1_2 (char *fname,unsigned long fname_len)
{
  return sci_plot2d1_G("plot2d2",2,fname_len); /* NG */
}
/*--------------------------------------------------------------------------*/
int sci_plot2d1_3 (char *fname,unsigned long fname_len)
{
  return sci_plot2d1_G("plot2d3",3,fname_len);/* NG */
}
/*--------------------------------------------------------------------------*/
int sci_plot2d1_4 (char *fname,unsigned long fname_len)
{
  return sci_plot2d1_G("plot2d4",4,fname_len);/* NG */
}
/*--------------------------------------------------------------------------*/
int sci_plot2d1_G( char * fname, int ptype, unsigned long fname_len )
{
  int frame_def=8;
  int *frame=&frame_def;
  int axes_def=1;
  int *axes=&axes_def;
  int iskip,test;
  int m1 = 0,n1 = 0,l1 = 0, m2 = 0, n2 = 0, l2 = 0, lt, i, j ;

  static rhs_opts opts[]= { {-1,"axesflag","?",0,0,0},
                            {-1,"frameflag","?",0,0,0},
                            {-1,"leg","?",0,0,0},
                            {-1,"logflag","?",0,0,0},
                            {-1,"nax","?",0,0,0},
                            {-1,"rect","?",0,0,0},
                            {-1,"strf","?",0,0,0},
                            {-1,"style","?",0,0,0},
                            {-1,NULL,NULL,0,0}       };

  int    * style    = NULL  ;
  double * rect     = NULL  ;
  int    * nax      = NULL  ;
  BOOL     flagNax  = FALSE ;
  char   * strf     = NULL  ;
  char   * legend   = NULL  ;
  char   * logFlags = NULL  ;

  if (Rhs <= 0) {
    /* lauch the default routines depending on the name of the calling funtion */
    sci_demo(fname, fname_len);
    return 0;
  }
  CheckRhs(1,9); /* to allow plot2dxx(y) */


  iskip=0;
  if ( get_optionals(fname,opts) == 0) 
  {
	  C2F(putlhsvar)();
	  return 0;
  }

  if (GetType(1)==sci_strings) {
    /* logflags */
    GetLogflags(fname,1,opts,&logFlags);
    iskip=1;
  }

  /* added to support plot2dxx([logflags],y) */
  if ( Rhs == 1 + iskip )
  {
    if ( FirstOpt() <= Rhs)
    {
      Scierror(999,_("%s: Misplaced optional argument: #%d must be at position %d.\n"),fname,1, 3+iskip);
      return(0);
    }

    GetRhsVar(1+iskip,MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);
    CreateVar(2+iskip,MATRIX_OF_DOUBLE_DATATYPE,  &m2, &n2, &l1);
    if (m2 == 1 && n2 > 1) { m2 = n2; n2 = 1;}
    m1 = m2;  n1 = n2;
    for (i = 0; i < m2 ; ++i) 
      for (j = 0 ; j < n2 ;  ++j)
        *stk( l1 + i + m2*j) = (double) i+1;
  }


  if (Rhs >= 2+iskip)
  {
    if ( FirstOpt() < 3+iskip) 
    {
      Scierror(999,_("%s: Misplaced optional argument: #%d must be at position %d.\n"),
        fname,1, 3+iskip);
      return(0);
    }


    /* x */
    GetRhsVar(1+iskip,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
    if (iskip==1) 
    {
      if (logFlags[0]=='e')
      {
        m1 = 0 ;
        n1 = 0 ;
      }
    }

    /* y */
    GetRhsVar(2+iskip,MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);
    /* if (m2 * n2 == 0) { m1 = 0; n1 = 0;}  */

    test = (m1*n1 == 0) /* x = [] */
      /* x,y vectors of same length */  
      || ((m1 == 1 || n1 == 1) && (m2 == 1 || n2 ==1) && (m1*n1 == m2*n2))
      || ((m1 == m2) && (n1 == n2)) /* size(x) == size(y) */
      /* x vector size(y)==[size(x),.] */
      || ((m1 == 1 && n1 == m2) || (n1 == 1 && m1 == m2)); 

    CheckDimProp(1+iskip,2+iskip,!test);

    if (m1*n1 == 0) 
    { /* default x=1:n */
      CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,  &m2, &n2, &lt);
      if (m2 == 1 && n2 > 1) { m2 = n2; n2 = 1;}
      for (i = 0; i < m2 ; ++i)
      {
        for (j = 0 ; j < n2 ;  ++j)
        {
          *stk( lt + i + m2*j) = (double) i+1;
        }
      }
      m1 = m2;
      n1 = n2;
      l1 = lt;
    }
    else if ((m1 == 1 || n1 == 1) && (m2 != 1 && n2 != 1) ) {
      /* a single x vector for mutiple columns for y */
      CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,  &m2, &n2, &lt);
      for (i = 0; i < m2 ; ++i)
      {
        for (j = 0 ; j < n2 ;  ++j)
        {
          *stk( lt + i + m2*j) = *stk(l1 +i);
        }
      }
      m1 = m2;
      n1 = n2;
      l1 = lt;
    }
    else if ((m1 == 1 && n1 == 1) && (n2 != 1) ) {
      /* a single y row vector  for a single x */
      CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,  &m1, &n2, &lt);
      for (j = 0 ; j < n2 ;  ++j)
      {
        *stk( lt + j ) = *stk(l1);
      }
      n1 = n2;
      l1 = lt;
    }
    else {
      if (m2 == 1 && n2 > 1) { m2 = n2; n2 = 1;}
      if (m1 == 1 && n1 > 1) { m1 = n1; n1 = 1;}
    }
  }

  sciGetStyle(fname,3+iskip,n1,opts,&style);
  GetStrf(fname,4+iskip,opts,&strf);
  GetLegend(fname,5+iskip,opts,&legend);
  GetRect(fname,6+iskip,opts,&rect);
  GetNax(7+iskip,opts,&nax,&flagNax);
  if (iskip==0) GetLogflags(fname,8,opts,&logFlags);

  if ( isDefStrf( strf ) ) {
    char strfl[4];
    strcpy(strfl,DEFSTRFN);

    strf = strfl;
    if ( !isDefRect( rect ) )
    {
      strfl[1]='7';
    }
    if ( !isDefLegend( legend ) )
    {
      strfl[0]='1';
    }
    GetOptionalIntArg(fname,9,"frameflag",&frame,1,opts);
    if(frame != &frame_def)
    {
      strfl[1] = (char)(*frame+48);
    }
    GetOptionalIntArg(fname,9,"axesflag",&axes,1,opts);
    if(axes != &axes_def)
    {
      strfl[2] = (char)(*axes+48);
    }
  }

  if(ptype == 0) { ptype = 1 ; }

  Objplot2d (ptype,logFlags,stk(l1), stk(l2), &n1, &m1, style, strf,legend,rect, nax, flagNax);


  LhsVar(1) = 0;
  C2F(putlhsvar)();
  return 0;
}
/*--------------------------------------------------------------------------*/
