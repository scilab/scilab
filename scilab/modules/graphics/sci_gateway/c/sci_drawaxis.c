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
/* file: sci_drawaxis.c                                                   */
/* desc : interface for sci_drawaxis routine                              */
/*------------------------------------------------------------------------*/

#include "sci_drawaxis.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "sciCall.h"
#include "Scierror.h"
#include "PloEch.h"
#include "localization.h"
#include "GetProperty.h"
#include "CurrentObjectsManagement.h"

/*--------------------------------------------------------------------------*/
int sci_drawaxis( char * fname, unsigned long fname_len )
{
  /** XXXXX : un point en suspens c'est le "S" ou une adresse est 
  *  stockees ds un unsigned long : est ce sufisant ? 
  */
  static rhs_opts opts[]= { 
    {-1,"dir","c",0,0,0},
    {-1,"fontsize","i",0,0,0},
    {-1,"format_n","c",0,0,0},
    {-1,"seg","i",0,0,0},
    {-1,"sub_int","i",0,0,0},
    {-1,"textcolor","i",0,0,0},
    {-1,"tics","c",0,0,0},
    {-1,"ticscolor","i",0,0,0},
    {-1,"val","S",0,0,0},
    {-1,"x","d",0,0,0},
    {-1,"y","d",0,0,0},
    {-1,NULL,NULL,0,0}
  };

  int minrhs = -1,maxrhs = 0,minlhs=0,maxlhs=1,nopt;
  char dir = 'l', *format = NULL, tics = 'v', **val = NULL;
  int fontsize = 0, sub_int=2, seg_flag = 1,textcolor = -1,ticscolor=-1;
  double *x = NULL,*y = NULL;
  int nx=0,ny=0,ntics;
  int nb_tics_labels = -1;

  nopt = NumOpt();

  CheckRhs(minrhs,maxrhs+nopt) ;
  CheckLhs(minlhs,maxlhs) ;

  if ( get_optionals(fname,opts) == 0)
	{
		/* error */
		return 0;
	}
  if ( opts[0].position != -1 ) 
  { 
    CheckLength(opts[0].position,opts[0].m,1);
    dir = *cstk(opts[0].l);
  } 
  if ( opts[1].position != -1 ) 
  {
    CheckScalar(opts[1].position,opts[1].m,opts[1].n);
    fontsize= *istk(opts[1].l);
  }
  if ( opts[2].position != -1 ) 
  { 
    /* verfier ce que l'on recoit avec "" XXX */
    format = cstk(opts[2].l);
  }

  if ( opts[3].position != -1 ) 
  { 
    CheckScalar(opts[3].position,opts[3].m,opts[3].n);
    seg_flag = *istk(opts[3].l);
  }

  if ( opts[4].position != -1 ) 
  { 
    CheckScalar(opts[4].position,opts[4].m,opts[4].n);
    sub_int= *istk(opts[4].l);
  }

  if ( opts[5].position != -1 ) 
  { 
    CheckScalar(opts[5].position,opts[5].m,opts[5].n);
    textcolor= *istk(opts[5].l);
  }

  if ( opts[6].position != -1 ) 
  { 
    CheckLength(opts[6].position,opts[6].m,1);
    tics = *cstk(opts[6].l);
  } 

  if ( opts[7].position != -1 ) 
  { 
    CheckScalar(opts[7].position,opts[7].m,opts[7].n);
    ticscolor= *istk(opts[7].l);
  }

  if ( opts[8].position != -1 ) 
  { 
    val = (char **) opts[8].l;
  } 

  if ( opts[9].position != -1 ) 
  { 
    x = stk(opts[9].l);
    nx = opts[9].m * opts[9].n ;  /* F.Leray OK here opts[9].m and opts[9].n are integers.*/
  }
  else 
  {
    static double x_def[1];
	sciPointObj * currentSubwin = sciGetCurrentSubWin();
	double bounds[6];
	sciGetDataBounds(currentSubwin, bounds);
    nx = 1;
    x = x_def ;
    if ( dir == 'l' ) 
      x_def[0] = bounds[0]; /* xMin */
    else if ( dir == 'r' ) 
      x_def[0] = bounds[1]; /* xMax */
  }

  if ( opts[10].position != -1 ) 
  { 
    y = stk(opts[10].l);
    ny = opts[10].m * opts[10].n ;
  }
  else 
  {
    static double y_def[1];
	sciPointObj * currentSubwin = sciGetCurrentSubWin();
	double bounds[6];
	sciGetDataBounds(currentSubwin, bounds);
    ny = 1;
    y = y_def ;
    if ( dir == 'd' ) 
      y_def[0] = bounds[2]; /* yMin */
    else if ( dir == 'u' ) 
      y_def[0] = bounds[3]; /* yMax */
 }

  /* compatibility test */
  switch (tics ) 
  {
  case 'r' :
    if ( check_xy(fname,dir,3,opts[9].position,opts[9].m,opts[9].n,opts[9].l,
      opts[10].position,opts[10].m,opts[10].n,opts[10].l,&ntics)==0) 
	{
	  C2F(putlhsvar)();
      return 0;
	}
    break;
  case 'i' :
    if ( check_xy(fname,dir,4,opts[9].position,opts[9].m,opts[9].n,opts[9].l,
      opts[10].position,opts[10].m,opts[10].n,opts[10].l,&ntics)==0) 
	{
	  C2F(putlhsvar)();
      return 0;
	}
    break;
  case 'v' :
    if ( check_xy(fname,dir,-1,opts[9].position,opts[9].m,opts[9].n,opts[9].l,
      opts[10].position,opts[10].m,opts[10].n,opts[10].l,&ntics)==0) 
	{
	  C2F(putlhsvar)();
      return 0;
	}
    break;
  default :
    Scierror(999,_("%: Wrong value for %s '%c': '%s', '%s' and '%s' expected.\n"), fname,"tics",dir,"r","v","i");
    return 0;
  }

  if ( val != 0) 
  {
    CheckLength( opts[8].position, opts[8].m*opts[8].n,ntics);
    nb_tics_labels = opts[8].m*opts[8].n;
  }

  
  Objdrawaxis(dir,tics,x,&nx,y,&ny,val,sub_int,format,fontsize,textcolor,ticscolor,'n',seg_flag,nb_tics_labels);


  LhsVar(1) = 0;
  C2F(putlhsvar)();
  return 0;
}

/*--------------------------------------------------------------------------*/
int check_xy(char *fname, char dir, int mn, int xpos, int xm, int xn, 
             long unsigned int xl, int ypos, int yRow, int yCol, long unsigned int yl, 
             int *ntics)
{
  switch ( dir ) 
  {
  case 'l': case 'r' : 
    /* x must be scalar */
    if ( xpos != -1 ) CheckScalar(xpos,xm,xn);
    /* y must be of size mn */
    if ( mn != -1 ) CheckDims(ypos,yRow,yCol,1,mn);
    switch (mn) 
    {
    case 3: 
      *ntics = (int) *stk(yl+2)+1;break;
    case 4: 
      *ntics = (int) *stk(yl+3)+1;break;
    case -1: 
      *ntics =  yRow*yCol;break;
    }
    break;
  case 'u' : case 'd' : 
    /* y must be scalar */
    if ( ypos  != -1 ) CheckScalar(ypos,yRow,yCol);
    /* x must be of size mn */
    if (mn != -1 ) CheckDims(xpos,xm,xn,1,mn);
    switch (mn) 
    {
    case 3: 
      *ntics =  (int) *stk(xl+2)+1;break;
    case 4: 
      *ntics =  (int) *stk(xl+3)+1;break;
    case -1: 
      *ntics =  xm*xn;break;
    }
    break;
  default :
    Scierror(999,"%s: Wrong value for %s '%c': '%s','%s','%s' and '%s' expected.\n", 
      fname,"dir",dir,"u","d","r","l");
    return 0;
  }
  return 1;
}
/*--------------------------------------------------------------------------*/
