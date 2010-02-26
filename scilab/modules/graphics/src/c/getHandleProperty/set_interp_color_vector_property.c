/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_interp_color_vector_property.c                               */
/* desc : function to modify in Scilab the background field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_interp_color_vector_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  if( sciGetEntityType(pobj) != SCI_POLYLINE )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"interp_color_vector");
    return SET_PROPERTY_ERROR ;
  }

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "interp_color_vector");
    return SET_PROPERTY_ERROR ;
  }



  if( ( nbCol == 3 && sciGetNbPoints(pobj) == 3 ) || 
      ( nbCol == 4 && sciGetNbPoints(pobj) == 4 ) )
  {
    int tmp[4] ;
    getDoubleMatrixFromStack( stackPointer ) ;

    copyDoubleVectorToIntFromStack( stackPointer, tmp, nbCol ) ;

    return sciSetInterpVector( pobj, nbCol, tmp ) ;
  }
  else
  {
    Scierror(999, _("Under interpolated color mode the column dimension of the color vector must match the number of points defining the line (which must be %d or %d).\n"),3,4);
    return SET_PROPERTY_ERROR ;
  }

}
/*------------------------------------------------------------------------*/
