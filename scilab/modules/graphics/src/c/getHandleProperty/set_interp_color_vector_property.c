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
/* file: set_interp_color_vector_property.c                               */
/* desc : function to modify in Scilab the background field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "localization.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_interp_color_vector_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"interp_color_vector") ;
    return SET_PROPERTY_ERROR ;
  }

  if( sciGetEntityType(pobj) != SCI_POLYLINE )
  {
    sciprint("interp_color_vector can only be set on Polyline objects.\n");
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
    sciprint("Under interpolated color moden the column dimension of the color vector must match the number of points defining the line (which must be 3 or 4).\n") ;
    return SET_PROPERTY_ERROR ;
  }

}
/*------------------------------------------------------------------------*/
