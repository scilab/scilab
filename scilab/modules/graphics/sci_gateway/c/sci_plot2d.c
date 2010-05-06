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
/* file: sci_plot2d.c                                                     */
/* desc : interface for plot2d routine                                    */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "GetCommandArg.h"
#include "stack-c.h"
#include "BasicAlgos.h"
#include "sciCall.h"
#include "DefaultCommandArg.h"
#include "Scierror.h"
#include "localization.h"
#include "CurrentObjectsManagement.h"

/*------------------------------------------------------------------------*/
int sci_plot2d( char * fname, unsigned long fname_len )
{

  int m1, n1, l1, m2, n2, l2, lt;
  int test,i,j,iskip;
  int frame_def=8;
  int *frame=&frame_def;
  int axes_def=1;
  int *axes=&axes_def;

  /* F.Leray 18.05.04 : log. case test*/
  int size_x,size_y;
  double xd[2];
  double *x1;
  char dataflag;

  char   * logFlags = NULL  ;
  int    * style    = NULL  ;
  double * rect     = NULL  ;
  char   * strf     = NULL  ;
  char   * legend   = NULL  ;
  int    * nax      = NULL  ;
  BOOL     flagNax  = FALSE ;

  static rhs_opts opts[]= { {-1,"axesflag","?",0,0,0},
			                      {-1,"frameflag","?",0,0,0},
		                        {-1,"leg","?",0,0,0},
		                        {-1,"logflag","?",0,0,0},
		                        {-1,"nax","?",0,0,0},
		                        {-1,"rect","?",0,0,0},
		                        {-1,"strf","?",0,0,0},
		                        {-1,"style","?",0,0,0},
                            {-1,NULL,NULL,0,0}};
  if (Rhs == 0) 
  {
    sci_demo(fname, fname_len);
    return 0;
  }

  CheckRhs(1,9);

  iskip=0;
  if ( get_optionals(fname,opts) == 0) 
  { 
	  C2F(putlhsvar)();
	  return 0 ; 
  }

  if (GetType(1)==sci_strings)
  {
    /* logflags */
    GetLogflags( fname, 1, opts, &logFlags ) ;
    iskip=1;
  }

  if (GetType(1+iskip) != sci_matrix) {
	  Scierror(999, _("%s: Wrong type for input argument #%d: Real or Complex matrix expected.\n"), fname, 1+iskip);
    return 0;
  }

  if (FirstOpt() == 2+iskip)       				/** plot2d([loglags,] y, <opt_args>); **/
  {
    GetRhsVar(1+iskip,MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);

    if (m2 == 1 && n2 > 1) { m2 = n2; n2 = 1;}
    m1 = m2;  n1 = n2;
    CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,  &m1, &n1, &l1);

    for (i = 0; i < m2 ; ++i) 
    {
	    for (j = 0 ; j < n2 ;  ++j)
      {
	      *stk( l1 + i + m2*j) = (double) i+1;
      }
    }
  }
  else if (FirstOpt() >= 3 + iskip)     /** plot2d([loglags,] x, y[, style [,...]]); **/
  {

    /* x */
    GetRhsVar(1+iskip,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);

    /* y */
    if (GetType(2+iskip) != sci_matrix) {
	    Scierror(999, _("%s: Wrong type for input argument #%d: Real or Complex matrix expected.\n"), fname, 2+iskip);
      return 0;
    }
    GetRhsVar(2+iskip,MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);

    test = (m1*n1 == 0)||
      ((m1 == 1 || n1 == 1) && (m2 == 1 || n2 ==1) && (m1*n1 == m2*n2))  ||
      ((m1 == m2) && (n1 == n2)) ||
      ((m1 == 1 && n1 == m2) || (n1 == 1 && m1 == m2));
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
    else if ((m1 == 1 || n1 == 1) && (m2 != 1 && n2 != 1) )
    {
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
    else if ((m1 == 1 && n1 == 1) && (n2 != 1) )
    {
      /* a single y row vector  for a single x */
      CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,  &m1, &n2, &lt);
      for (j = 0 ; j < n2 ;  ++j)
      {
	      *stk( lt + j ) = *stk(l1);
      }
      n1 = n2;
      l1 = lt;
    }
    else
    {
      if (m2 == 1 && n2 > 1) { m2 = n2; n2 = 1;}
      if (m1 == 1 && n1 > 1) { m1 = n1; n1 = 1;}
    }
  }
  else
  {
	  Scierror(999, _("%s: Wrong number of mandatory input arguments. At least %d expected.\n"), fname, 1);
	  return 0;    
  }

  if(n1 == -1 || n2 == -1 || m1 == -1 || m2 == -1)
  {
	  Scierror(999, _("%s: Wrong size for input arguments #%d and #%d.\n"), fname, 1, 2); /* @TODO : detail error */
	  return 0;
  }

  sciGetStyle( fname, 3+iskip, n1, opts, &style ) ;
  GetStrf( fname, 4+iskip, opts, &strf ) ;
  GetLegend( fname, 5+iskip, opts, &legend );
  GetRect( fname, 6+iskip, opts, &rect );
  GetNax( 7+iskip, opts, &nax, &flagNax ) ;
  if (iskip==0) { GetLogflags( fname, 8, opts, &logFlags ) ; }

  if ( isDefStrf( strf ) )
  {
    char strfl[4];
    strcpy(strfl,DEFSTRFN);

    strf = strfl;
    if ( !isDefRect( rect ) )
    {
      strfl[1] = '7';
    }
    if ( !isDefLegend( legend ) )
    {
      strfl[0] = '1';
    }

    GetOptionalIntArg(fname,9,"frameflag",&frame,1,opts);
    if( frame != &frame_def )
    {
      strfl[1] = (char)(*frame+48);
    }
    GetOptionalIntArg(fname, 9,"axesflag",&axes,1,opts);
    if(axes != &axes_def)
    {
      strfl[2] = (char)(*axes+48);
    }
  }

  /* Make a test on log. mode : available or not depending on the bounds set by Rect arg. or xmin/xmax :
  Rect case :
  - if the min bound is strictly posivite, we can use log. mode
  - if not, send error message 
  x/y min/max case:
  - we find the first strictly positive min bound in Plo2dn.c ?? */

  switch (strf[1])
  {
  case '0': 
    /* no computation, the plot use the previous (or default) scale */
    break;
  case '1' : case '3' : case '5' : case '7':
    /* based on Rect arg */ 
    if( rect[0] > rect[2] || rect[1] > rect[3])
    {
      Scierror(999, _("%s: Impossible status min > max in x or y rect data.\n"),fname);
      return -1;
    }

    if( rect[0] <= 0. && logFlags[1] =='l') /* xmin */
    {
      Scierror(999, _("%s: Bounds on x axis must be strictly positive to use logarithmic mode.\n"),fname);
      return -1 ;
    }

    if( rect[1] <= 0. && logFlags[2] =='l') /* ymin */
    {
      Scierror(999, _("%s: Bounds on y axis must be strictly positive to use logarithmic mode.\n"),fname);
      return -1 ;
    }

    break;
  case '2' : case '4' : case '6' : case '8': case '9':
    /* computed from the x/y min/max */
    if ( (int)strlen(logFlags) < 1)
    {
      dataflag='g' ;
    }
    else
    {
      dataflag=logFlags[0];
    }

    switch ( dataflag )
    {
    case 'e' : 
      xd[0] = 1.0; xd[1] = (double)m1;
      x1 = xd;size_x = (m1 != 0) ? 2 : 0 ;
      break; 
    case 'o' : 
      x1 = stk(l1);size_x = m1;
      break;
    case 'g' : 
    default  : 
      x1 = stk(l1);size_x = (n1*m1) ;
      break; 
    }

    if (size_x != 0)
    {
      if(logFlags[1] == 'l' && sciFindStPosMin(stk(l1),size_x) <= 0.0 )
      {
        Scierror(999, _("%s: At least one x data must be strictly positive to compute the bounds and use logarithmic mode.\n"),fname);
        return -1 ;
      }
    }

    size_y = (n1*m1) ;

    if (size_y != 0)
    {
      if( logFlags[2] == 'l' && sciFindStPosMin(stk(l2),size_y) <= 0.0 )
      {
        Scierror(999, _("%s: At least one y data must be strictly positive to compute the bounds and use logarithmic mode\n"),fname);
        return -1 ;
      }
    }

    break;
  }

  // open a figure if none already exists
  sciGetCurrentFigure();
  Objplot2d (1,logFlags,stk(l1), stk(l2), &n1, &m1, style, strf,legend, rect,nax,flagNax);

  LhsVar(1) = 0;
  C2F(putlhsvar)();
  return 0;
}
/*------------------------------------------------------------------------*/
