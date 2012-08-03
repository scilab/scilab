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
/* file: sci_matplot.h                                                    */
/* desc : interface for matplot routine                                   */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "GetCommandArg.h"
#include "DefaultCommandArg.h"
#include "BuildObjects.h"
#include "sciCall.h"
#include "stack-c.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_matplot(char *fname,unsigned long fname_len)
{
  int m1 = 0, n1 = 0, l1 = 0;
  int frame_def=8;
  int *frame=&frame_def;
  int axes_def=1;
  int *axes=&axes_def;
  static rhs_opts opts[]= { {-1,"axesflag","?",0,0,0},
  {-1,"frameflag","?",0,0,0},
  {-1,"nax","?",0,0,0},
  {-1,"rect","?",0,0,0},
  {-1,"strf","?",0,0,0},
  {-1,NULL,NULL,0,0}};

  char   * strf    = NULL  ;
  double * rect    = NULL  ;
  int    * nax     = NULL  ;
  BOOL     flagNax = FALSE ;

  if ( Rhs <= 0 )
  {
    sci_demo(fname, fname_len);
    return 0;
  }
  CheckRhs(1,5);

  if ( get_optionals(fname,opts) == 0 ) 
  {
	  PutLhsVar();
	  return 0 ; 
  }
  if ( FirstOpt() < 2)
  {
    Scierror(999,_("%s: Misplaced optional argument: #%d must be at position %d.\n"),
      fname,1,2);
    return(0);
  }
  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
  if (m1 * n1 == 0) 
  {  
	  LhsVar(1) = 0;
	  PutLhsVar();
	  return 0;
  }

  GetStrf(fname,2,opts,&strf);
  GetRect(fname,3,opts,&rect);
  GetNax(4,opts,&nax,&flagNax);

  getOrCreateDefaultSubwin();

  if ( isDefStrf( strf ) ) {
    char strfl[4];
   
    strcpy(strfl,DEFSTRFN) ;
   
    strf = strfl;
    if ( !isDefRect( rect ) )
    {
      strfl[1]='7';
    }

    GetOptionalIntArg(fname,5,"frameflag",&frame,1,opts);
    if(frame != &frame_def)
    {
      strfl[1] = (char)(*frame+48);
    }
    GetOptionalIntArg(fname,5,"axesflag",&axes,1,opts);
    if(axes != &axes_def)
    {
      strfl[2] = (char)(*axes+48);
    }
  }

  Objmatplot( stk(l1), &m1, &n1, strf, rect, nax, flagNax ) ;

  /* NG end */
  LhsVar(1) = 0;
  PutLhsVar();
  return 0;
}
/*--------------------------------------------------------------------------*/
