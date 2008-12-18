/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - ENPC - Jean-Philipe Chancelier
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
/* file: sci_zoom_rect.c                                                  */
/* desc : interface for zoom_rect routine                                 */
/*------------------------------------------------------------------------*/

#include "sci_zoom_rect.h"
#include "stack-c.h"
#include "getPropertyAssignedValue.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "axesScale.h"
#include "DrawingBridge.h"
#include "CurrentObjectsManagement.h"
#include "GraphicSynchronizerInterface.h"
#include "Interaction.h"
#include "localization.h"
#include "Scierror.h"
#include "getPropertyAssignedValue.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"

/*--------------------------------------------------------------------------*/
static sciPointObj * getZoomedObject(const char * fname);
static BOOL getZoomRect(const char * fname, int attribPos, double rect[4]);
/*--------------------------------------------------------------------------*/
/**
 * Get the [xmin, ymin, xmax, ymax] vector specified as input argument
 * @param fname name of the calling function for error messages
 * @param attribPos position of the argument within the rhs (1 or 2)
 * @param[out] rect retrieved rectangle
 * @return TRUE if the rect could be retrieved, false otherwise
 */
static BOOL getZoomRect(const char * fname, int attribPos, double rect[4])
{
  int nbRow;
  int nbCol;
  size_t stackPointer = 0;
  int i;
  double * rectVect;
  GetRhsVar(attribPos, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &stackPointer);

  if (nbRow * nbCol != 4)
  {
    if (attribPos == 1)
    {
      Scierror(999, _("%s: Wrong size for input argument #%d: Vector of size %d expected.\n"), fname, 1, 4);
    }
    else
    {
      Scierror(999, _("%s: Wrong size for input argument #%d: Vector of size %d expected.\n"), fname, 2, 4);
    }
    return FALSE;
  }

  rectVect = getDoubleMatrixFromStack(stackPointer);
  for (i = 0; i < 4; i++)
  {
    rect[i] = rectVect[i];
  }

  return TRUE;

}
/*--------------------------------------------------------------------------*/
/**
 * Return the handle passed as input argument if one exists
 * @param fname of the function for errors
 * @return NULL if the input argument is not correct,
 *              the object to zoom otherwise
 */
static sciPointObj * getZoomedObject(const char * fname)
{
  int nbRow;
  int nbCol;
  size_t stackPointer = 0;
  sciPointObj * res = NULL;
  /* if a handle is specified it must be the first input argument */
  GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stackPointer);

  /* check that there is only a single Figre or subwin */
  if (nbRow * nbCol != 1)
  {
    Scierror(999, _("%s: Wrong size for input argument #%d: Single handle expected.\n"), fname, 1);
    return NULL;
  }

  res = sciGetPointerFromHandle(getHandleFromStack(stackPointer));

  if (sciGetEntityType(res) != SCI_SUBWIN && sciGetEntityType(res) != SCI_FIGURE)
  {
    Scierror(999, _("%s: Wrong type for input argument #%d: Figure or Axes handle expected.\n"), fname, 1);
    return NULL;
  }

  /* chack bounds */


  return res;


}
/*--------------------------------------------------------------------------*/
int sci_zoom_rect(char *fname,unsigned long fname_len)
{
  CheckRhs(0,2) ;
  CheckLhs(0,1) ;
  if (Rhs == 0) 
  {
    /* zoom_rect() */
    sciDefaultInteractiveZoom();
  }
  else if (Rhs == 1)
  {
    /* zoom_rect([xmin,ymin,xmax,ymax]) or zoom_rect(handle) */
    /* with handle a figure or subwindow */
    if (GetType(1) == sci_handles)
    {
      sciPointObj * zoomedObject = getZoomedObject(fname);
      if (zoomedObject == NULL)
      {
        LhsVar(1)=0; 
        return -1;
      }
      sciInteractiveZoom(zoomedObject);
    }
    else if (GetType(1) == sci_matrix)
    {
      double rect[4];
      if (getZoomRect(fname, 1, rect))
      {
        /* rectangle found */
        int status = sciDefaultZoom2D(rect);
        if (status == SET_PROPERTY_ERROR)
        {
          /* error on rectangle bounds */
          Scierror(999, _("%s: Wrong value for input argument #%d: Specified bounds are not correct.\n"), fname, 1);
          LhsVar(1)=0; 
          return -1;
        }
      }
      else
      {
        /* error on rectagle definition */
        LhsVar(1)=0; 
        return -1;
      }
    }
    else
    {
      Scierror(999, _("%s: Wrong type for input argument #%d: Handle or vector of double expected.\n"), fname, 1);
      LhsVar(1)=0; 
      return 0;
    }
  }
  else if (Rhs == 2)
  {
    /* zoom_rect(handle, [xmin,ymin,xmax,ymax]) */

    double rect[4];
    sciPointObj * zoomedObject = NULL;

    if (GetType(1) != sci_handles || GetType(2) != sci_matrix)
    {
      Scierror(999, _("%s: Wrong type for input arguments: Handle or vector of double expected.\n"), fname);
      LhsVar(1)=0; 
      return -1;
    }

    zoomedObject = getZoomedObject(fname);
    if (zoomedObject == NULL || !getZoomRect(fname, 2, rect))
    {
      LhsVar(1)=0; 
      return -1;
    }

    if (sciZoomRect(zoomedObject, rect) == SET_PROPERTY_ERROR)
    {
      /* error on rectangle bounds */
      Scierror(999, _("%s: Error on input argument #%d: Specified bounds are not correct.\n"), fname, 1);
      LhsVar(1)=0; 
      return -1;
    }
  }

  LhsVar(1)=0; 
  return 0;
} 
/*--------------------------------------------------------------------------*/
