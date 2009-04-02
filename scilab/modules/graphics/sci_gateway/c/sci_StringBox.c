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
#include "StringBox.h"
#include "localization.h"
#include "axesScale.h"
#include "getPropertyAssignedValue.h"
#include "CurrentObjectsManagement.h"

#define DEFAULT_ANGLE 0.0

/*--------------------------------------------------------------------------*/
static int getScalarFromStack(int paramIndex, char * funcName, double * res);
/*--------------------------------------------------------------------------*/
static int getScalarFromStack(int paramIndex, char * funcName, double * res)
{
  int m;
  int n;
  size_t stackPointer = 0;
  if ( VarType(paramIndex) != sci_matrix )
  {
    Scierror(999,_("%s: Wrong type for input argument #%d: Real scalar expected.\n"), funcName, paramIndex);
    return -1 ;
  }

  /* get the handle */
  GetRhsVar( paramIndex, MATRIX_OF_DOUBLE_DATATYPE, &m, &n, &stackPointer );

  if ( m * n != 1 )
  {
    Scierror(999,_("%s: Wrong size for input argument #%d: Real scalar expected.\n"), funcName, paramIndex);
    return -1 ;
  }

  *res = getDoubleFromStack(stackPointer);
  return 0;
}
/*--------------------------------------------------------------------------*/
int sci_stringbox( char * fname, unsigned long fname_len )
{
  int two   = 2;
  int four  = 4;
  size_t stackPointer = 0;
  double corners[4][2]; /* the four edges of the boundingRect */
  

  /* The function should be called with stringbox( handle ) */
  CheckRhs( 1, 6 ) ;
  CheckLhs( 0, 1 ) ;

  if (Rhs == 1)
  {
    int m;
    int n;
    /* A text handle should be specified */
    sciPointObj * pText = NULL ;
    if ( VarType(1) != sci_handles )
    {
      Scierror(999,_("%s: Wrong type for input argument #%d: A 'Text' handle expected.\n"), fname, 1);
      return 0 ;
    }

    /* get the handle */
    GetRhsVar( 1,GRAPHICAL_HANDLE_DATATYPE, &m, &n, &stackPointer );
    if ( m * n != 1 )
    {
      Scierror(999,_("%s: Wrong size for input argument #%d: A 'Text' handle expected.\n"), fname, 1);
      return 0 ;
    }

    /* Get the handle and check that this is a text handle */
    pText = sciGetPointerFromHandle( getHandleFromStack(stackPointer) ) ;

    if ( pText == NULL )
    {
      Scierror(999,_("%s: The handle is not valid.\n"),fname);
      return 0 ;
    }

    if ( sciGetEntityType( pText ) == SCI_LABEL )
    {
      // a label, get the real text
      pText = pLABEL_FEATURE( pText )->text ;
    }
    else if ( sciGetEntityType( pText ) != SCI_TEXT )
    {
      Scierror(999,_("%s: Wrong type for input argument #%d: A 'Text' handle expected.\n"), fname, 1);
      return 0 ;
    }

    /* update stringbox */
    updateTextBounds(pText);

    /* get the string box */
    sciGet2dViewBoundingBox( pText, corners[0], corners[1], corners[2], corners[3]) ;
    
  }
  else if (Rhs == 2)
  {
     Scierror(999,_("%s: Wrong number of input arguments: %d or %d to %d expected.\n"),fname, 1, 3, 6);
     return 0 ;
  }
  else
  {
    sciPointObj * parentSubwin = sciGetCurrentSubWin();
    char ** text = NULL;
    int textNbRow;
    int textNbCol;
    double xPos;
    double yPos;
    double angle = DEFAULT_ANGLE;
    int fontId = sciGetFontStyle(parentSubwin);
    double fontSize = sciGetFontSize(parentSubwin);

    /* Check that first argument is a string */
    if ( VarType(1) != sci_strings )
    {
      Scierror(999,_("%s: Wrong type for input argument #%d: 2D array of strings expected.\n"), fname, 1);
      return 0 ;
    }
    GetRhsVar( 1, MATRIX_OF_STRING_DATATYPE, &textNbRow, &textNbCol, &stackPointer );
    /* retrieve it */
    text = getStringMatrixFromStack(stackPointer);

    /* Second and third argument should be scalars */
    if (getScalarFromStack(2, fname, &xPos) < 0) { return 0; }
    if (getScalarFromStack(3, fname, &yPos) < 0) { return 0; }

    if (Rhs >= 4)
    {
      /* angle is defined */
      if (getScalarFromStack(4, fname, &angle) < 0) { return 0; }
    }

    if (Rhs >= 5)
    {
      double fontIdD;
      /* font style is defined */
      if (getScalarFromStack(5, fname, &fontIdD) < 0) { return 0; }
      fontId = (int) fontIdD;
    }

    if (Rhs >= 6)
    {
      /* font size is defined */
      if (getScalarFromStack(6, fname, &fontSize) < 0) { return 0; }
    }

    /* compute the box */
    getTextBoundingBox(text, textNbRow, textNbCol, xPos, yPos, angle, fontId, fontSize, corners);
  }
  

  /* copy everything into the lhs */
  stackPointer = 0; /* Fix for 64 bits: MSB of stackPointer has been set by GetRhsVar but are not reset by CreateVar */
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
	C2F(putlhsvar)();
  return 0;
}

/*--------------------------------------------------------------------------*/
#undef DEFAULT_ANGLE
