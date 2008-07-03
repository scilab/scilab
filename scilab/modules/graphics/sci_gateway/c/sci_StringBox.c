/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
/* file: sci_StringBox.c                                                  */
/* desc : interface for StringBox routine                                 */
/*------------------------------------------------------------------------*/

#include "sci_StringBox.h"
#include "Scierror.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "DrawObjects.h"
#include "StringBox.h"
#include "localization.h"
#include "DrawingBridge.h"
#include "axesScale.h"
/*--------------------------------------------------------------------------*/
int sci_StringBox( char * fname, unsigned long fname_len )
{
  integer    two   = 2     ;
  integer    four  = 4     ;
  integer    m,n           ;
  integer    stackPointer  ;
  double     corners[4][2] ; /* the four edges of the boundingRect */
  sciPointObj * pText = NULL ;

  /* The function should be called with stringbox( handle ) */
  CheckRhs( 1, 1 ) ;
  CheckLhs( 0, 1 ) ;

  if ( VarType(1) != sci_handles )
  {
    Scierror(999,_("%s: Wrong type for input argument #%d: Only one text handle expected.\n"),fname, 1);
    return 0 ;
  }

  /* get the handle */
  GetRhsVar( 1,GRAPHICAL_HANDLE_DATATYPE, &m, &n, &stackPointer );

  if ( m * n != 1 )
  {
	  Scierror(999,_("%s: Wrong type for input argument #%d: Only one text handle expected.\n"),fname, 1);
    return 0 ;
  }

  pText = sciGetPointerFromHandle( (unsigned long) *hstk( stackPointer ) ) ;

  if ( pText == NULL )
  {
    Scierror(999,_("%s: The handle is not valid.\n"),fname);
    return 0 ;
  }

  if ( sciGetEntityType( pText ) == SCI_LABEL )
  {
    pText = pLABEL_FEATURE( pText )->text ;
  }
  else if ( sciGetEntityType( pText ) != SCI_TEXT )
  {
	  Scierror(999,_("%s: Wrong type for input argument #%d: Only one text handle expected.\n"),fname, 1);
    return 0 ;
  }

  /* create a window if needed to initialize the X11 graphic context  */

  /* update stringbox */
  updateTextBounds(pText);

  /* get the string box */
  sciGet2dViewBoundingBox( pText, corners[0], corners[1], corners[2], corners[3]) ;

  /* copy everything into the lhs */
  CreateVar( Rhs + 1,MATRIX_OF_DOUBLE_DATATYPE, &two, &four, &stackPointer );
  *stk( stackPointer     )  = corners[1][0] ; 
  *stk( stackPointer + 1 )  = corners[1][1] ;
  *stk( stackPointer + 2 )  = corners[0][0] ;
  *stk( stackPointer + 3 )  = corners[0][1] ;
  *stk( stackPointer + 4 )  = corners[3][0] ;
  *stk( stackPointer + 5 )  = corners[3][1] ;
  *stk( stackPointer + 6 )  = corners[2][0] ;
  *stk( stackPointer + 7 )  = corners[2][1] ;

  LhsVar( 1 ) = Rhs + 1 ;
  return 0;
}

/*--------------------------------------------------------------------------*/
