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
/* file: sci_draw.h                                                       */
/* desc : interface for draw routine                                      */
/*------------------------------------------------------------------------*/
#include "stack-c.h"
#include "gw_graphics.h"
#include "HandleManagement.h"
#include "CurrentObjectsManagement.h"
#include "DrawingBridge.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
int sci_draw( char * fname, unsigned long fname_len )
{ 

  CheckRhs(0,1) ;
  CheckLhs(0,1) ;

  if (Rhs == 0)
  {
    sciDrawSingleObj(sciGetCurrentObj()) ;
  }
  else
  {
    // Rhs = 1
    sciPointObj ** drawnObjects = NULL;
    int nbObjects = 0;
    int nbRow;
    int nbCol;
    size_t stackPointer = 0;
    int i;

    GetRhsVar( 1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stackPointer ); 

    nbObjects = nbRow * nbCol;

    /* allocate array of objects */
    drawnObjects = MALLOC(nbObjects * sizeof(sciPointObj *));

    /* fill it */
    for (i = 0; i < nbObjects; i++)
    {
      /* Convert handle to sciPointObj */
      unsigned long hdl = (unsigned long) hstk(stackPointer)[i];
      drawnObjects[i] = sciGetPointerFromHandle(hdl);

      if (drawnObjects[i] == NULL) {
        FREE(drawnObjects);
        Scierror(999,_("%s: The handle is not or no more valid.\n"),fname);
        return 0;
      }
    }
      
    sciDrawSetOfObj(drawnObjects, nbObjects);

    FREE(drawnObjects);
  }

  LhsVar(1) = 0;
  C2F(putlhsvar)();
  return 0;
}
/*--------------------------------------------------------------------------*/
