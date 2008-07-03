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
/* file: sci_xtitle.c                                                     */
/* desc : interface for xtitle routine                                    */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "sci_xtitle.h"
#include "sci_demo.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "stack-c.h"
#include "Scierror.h"
#include "GetProperty.h"
#include "ObjectStructure.h"
#include "SetProperty.h"
#include "DrawObjects.h"
#include "MALLOC.h"
#include "CurrentObjectsManagement.h"
#include "freeArrayOfString.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int sci_xtitle( char * fname, unsigned long fname_len )
{
  int  narg          ;
  int  nbLabels      ; /* number of modified labels */
  int  box      = 0  ;
  BOOL isBoxSpecified = FALSE ;
  sciPointObj * psubwin = NULL;
  static rhs_opts opts[] = { {-1,"boxed","i" ,0,0,0},
  {-1,NULL   ,NULL,0,0,0} };

  if (Rhs <= 0)
  {
    int zero = 0;
    sci_demo(fname,"x=(1:10)';plot2d(x,x);xtitle(['Title';'Main'],'x','y');",&zero);
    return 0;
  }

  CheckRhs(1,5);


  nbLabels = Rhs ;

  /* get the given options from the name in opts */
  if ( !get_optionals(fname,opts) ) return 0;

  /* compatibility with previous version in which box was put */
  /* at the fourth position */

  if ( Rhs == 4 )
  {
    int type = GetType(4);
    if ( type == 1 || type == 8 )/* double or integer */
    {
      int n,m ;
      int boxPtr   = -1 ; /* pointer of box on the stack */
      GetRhsVar(4,MATRIX_OF_INTEGER_DATATYPE,&m,&n,&boxPtr);
      CheckScalar(4,m,n);
      box = *istk( boxPtr ) ;
      nbLabels-- ; /* it is not a label text */
      isBoxSpecified = TRUE ;
    }
  }

  if ( opts[0].position != -1 && !isBoxSpecified )
  {
    /* check if "box" is in the options */
    box = *istk(opts[0].l) ;
    if ( opts[0].m * opts[0].n != 1 )
    {       
      /* check size */
      Scierror( 999, _("%s: Wrong type for input argument: Scalar expected.\n"), fname ) ;
      return 1 ;
    } 
    nbLabels-- ; /* it is not a label text */
  }


  psubwin = sciGetCurrentSubWin();


  for ( narg = 1 ; narg <= nbLabels ; narg++)
  {
    int i,m,n;
    char **Str;
    sciPointObj * modifiedLabel = NULL ;
    char * text ;

    GetRhsVar(narg,MATRIX_OF_STRING_DATATYPE,&m,&n,&Str);
    if ( m*n == 0 ) { continue ; }
    strcpy(C2F(cha1).buf,Str[0]);
    for ( i= 1 ; i < m*n ; i++) 
    {
      strcat(C2F(cha1).buf,"@"); 
      strcat(C2F(cha1).buf,Str[i]);
    }

	freeArrayOfString(Str,m*n);

    switch(narg)
    {
    case 1:
      modifiedLabel = pSUBWIN_FEATURE(psubwin)->mon_title ;
      break;
    case 2:
      modifiedLabel = pSUBWIN_FEATURE(psubwin)->mon_x_label ;
      break;
    case 3:
      modifiedLabel = pSUBWIN_FEATURE(psubwin)->mon_y_label ;
      break;
    case 4:
      modifiedLabel = pSUBWIN_FEATURE(psubwin)->mon_z_label ;
    default:
      break;
    }


    text = C2F(cha1).buf ;
    sciSetText( modifiedLabel, &text, 1, 1 ) ;

    if ( box == 1 )
    {
      sciSetIsFilled( modifiedLabel, TRUE ) ;
    }
    else
    {
      sciSetIsFilled( modifiedLabel, FALSE ) ;
    }

  }

  sciSetCurrentObj(psubwin);
  sciRedrawFigure();

  LhsVar(1)=0;
  return 0;
}

/*--------------------------------------------------------------------------*/
