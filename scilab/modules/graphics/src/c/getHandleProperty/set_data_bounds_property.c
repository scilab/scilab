/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
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
/* file: set_data_bounds_property.c                                       */
/* desc : function to modify in Scilab the data_bounds field of           */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "axesScale.h"
/*------------------------------------------------------------------------*/
int getdDataBoundsFromStack( size_t  stackPointer, int nbRow, int nbCol,
                             double * xMin, double * xMax,
                             double * yMin, double * yMax,
                             double * zMin, double * zMax );
/*------------------------------------------------------------------------*/
/**
 * fill bounds (xMin, xMax, yMin,... ) from the assigned value in the stack
 * beacause it might have several possible size.
 */
int getdDataBoundsFromStack( size_t  stackPointer, int nbRow, int nbCol,
                             double * xMin, double * xMax,
                             double * yMin, double * yMax,
                             double * zMin, double * zMax )
{
  double * bounds = getDoubleMatrixFromStack( stackPointer ) ;
  
  /* initialize zMin and zMax to avoid checking between 2D and 3D */
  *zMin = 1.0 ;
  *zMax = 2.0 ;

  switch ( nbRow )
  {
  case 1 : /* row vector */
    if ( nbCol == 4 )
    {
      *xMin = bounds[0] ;
      *xMax = bounds[1] ;
      *yMin = bounds[2] ;
      *yMax = bounds[3] ;
    }
    else if( nbCol == 6 )
    {
      *xMin = bounds[0] ;
      *xMax = bounds[1] ;
      *yMin = bounds[2] ;
      *yMax = bounds[3] ;
      *zMin = bounds[4] ;
      *zMax = bounds[5] ;
    }
    else
    {
      Scierror(999, _("Wrong size for argument #%d: %s, %s, %s, %s, %s or %s matrix expected.\n"),2,"2x2", "2x3", "1x4", "4x1", "1x6","6x1");
      return SET_PROPERTY_ERROR ;
    }
    break ;

  case 2 : /* 2x2 or 2x3 matrix */
    if ( nbCol == 2 )
    {
      *xMin = bounds[0] ;
      *yMin = bounds[2] ;
      *xMax = bounds[1] ;
      *yMax = bounds[3] ;
    }
    else if ( nbCol == 3 )
    {
      *xMin = bounds[0] ;
      *yMin = bounds[2] ;
      *zMin = bounds[4] ;
      *xMax = bounds[1] ;
      *yMax = bounds[3] ;
      *zMax = bounds[5] ;
    }
    else
    {
      Scierror(999, _("Wrong size for argument #%d: %s, %s, %s, %s, %s or %s matrix expected.\n"),2,"2x2", "2x3", "1x4", "4x1", "1x6","6x1");
      return SET_PROPERTY_ERROR ;
    }
    break ;

  case 4 : /* column vector for 2D */
    if ( nbCol == 1 )
    {
      *xMin = bounds[0] ;
      *xMax = bounds[1] ;
      *yMin = bounds[2] ;
      *yMax = bounds[3] ;
    }
    else
    {
      Scierror(999, _("Wrong size for argument #%d: %s, %s, %s, %s, %s or %s matrix expected.\n"),2,"2x2", "2x3", "1x4", "4x1", "1x6","6x1");
      return SET_PROPERTY_ERROR ;
    }
    break ;
  case 6 : /* column vector for 3D */
    if ( nbCol == 1 )
    {
      *xMin = bounds[0] ;
      *xMax = bounds[1] ;
      *yMin = bounds[2] ;
      *yMax = bounds[3] ;
      *zMin = bounds[4] ;
      *zMax = bounds[5] ;
    }
    else
    {
      Scierror(999, _("Wrong size for argument #%d: %s, %s, %s, %s, %s or %s matrix expected.\n"),2,"2x2", "2x3", "1x4", "4x1", "1x6","6x1");
      return SET_PROPERTY_ERROR ;
    }
    break ;
  default:
    Scierror(999, _("Wrong size for argument #%d: %s, %s, %s, %s, %s or %s matrix expected.\n"),2,"2x2", "2x3", "1x4", "4x1", "1x6","6x1");
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_SUCCEED ;
}

/*------------------------------------------------------------------------*/
int set_data_bounds_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Incompatible type for property %s.\n"),"data_bounds") ;
    return SET_PROPERTY_ERROR ;
  }

  if (sciGetEntityType (pobj) == SCI_SUBWIN)
  {

    /* JB Silvy 09/11/05 */
    sciSubWindow * ppSubWin = pSUBWIN_FEATURE (pobj) ;
    double   xMin ;
    double   xMax ;
    double   yMin ;
    double   yMax ;
    double   zMin ;
    double   zMax ;

    /* get the bounds */
    if ( getdDataBoundsFromStack( stackPointer, nbRow, nbCol, &xMin, &xMax, &yMin, &yMax, &zMin, &zMax ) == SET_PROPERTY_ERROR )
    {
      return SET_PROPERTY_ERROR ;
    }

   if (!checkDataBounds(pobj, xMin, xMax, yMin, yMax, zMin, zMax))
   {
     return SET_PROPERTY_ERROR;
   }

    /* copy the values in the axis */
    if ( nbRow * nbCol == 4 )
    { 
      /* 2D */
      double bounds[6];
      
      /* To get the Z coordinates */
      sciGetDataBounds(pobj, bounds) ;
      bounds[0] = xMin ;
      bounds[1] = xMax ;
      bounds[2] = yMin ;
      bounds[3] = yMax ;

      sciSetDataBounds(pobj, bounds) ;
    }
    else
    {
      /* 3D */
      double bounds[6] = {xMin, xMax, yMin, yMax, zMin, zMax} ;
      
      sciSetDataBounds(pobj, bounds) ;
      
    }

    ppSubWin->FirstPlot = FALSE;

    return SET_PROPERTY_SUCCEED ;
  }
  else if ( sciGetEntityType(pobj) == SCI_SURFACE )
  {
    if ( nbRow * nbCol != 6 )
    {
      Scierror(999, _("Argument #%d must have %d elements.\n"),2,6) ;
      return SET_PROPERTY_ERROR ;
    }
    sciSetDataBounds(pobj, getDoubleMatrixFromStack(stackPointer) ) ;

    return SET_PROPERTY_SUCCEED ;
  }
  else
  {
    Scierror(999, _("%s property does not exist for this handle.\n"),"data_bounds") ; 
    return SET_PROPERTY_ERROR ;
  }

  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
