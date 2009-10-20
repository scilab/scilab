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
/* file: set_tics_style_property.c                                        */
/* desc : function to modify in Scilab the tics_style field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "sciprint.h"
#include "localization.h"
#include "Format.h"

/*------------------------------------------------------------------------*/
int set_tics_style_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  double * vector = NULL ;
  int N = 0 ;
 
  char xy_type ;

  if ( !isParameterStringMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "tics_style");
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_AXES )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"tics_style") ;
    return SET_PROPERTY_ERROR ;
  }

  if (    !isStringParamEqual( stackPointer, "v" )
       && !isStringParamEqual( stackPointer, "r" )
       && !isStringParamEqual( stackPointer, "i" ) )
  {
    Scierror(999, _("Wrong value for '%s' property: Must be in the set {%s}.\n"), "tics_style", "v, r, i");
    return SET_PROPERTY_ERROR ;
  }

   /* get the character 'v', 'r' or 'i' */
  xy_type = getStringFromStack( stackPointer )[0] ;

  if( pAXES_FEATURE (pobj)->str != NULL )
  {
    if( ComputeXIntervals(pobj,xy_type,&vector,&N,1) != 0 )
    {
      Scierror(999, _("Error: Bad size in %s ; you must first increase the size of the %s.\n"),"tics_coord","tics_coord");
      return SET_PROPERTY_ERROR ;
    }
    if( pAXES_FEATURE(pobj)->nb_tics_labels < N )
    {
			sciprint(_("Warning: %s has been set by user ; you must first increase the size of the %s string vector before switching to the new %s mode.\n"),"tics_labels","tics_labels","tics_style");
      return SET_PROPERTY_ERROR;
    }
  }

  pAXES_FEATURE (pobj)->tics = xy_type ;

  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
