/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*------------------------------------------------------------------------*/
/* file: set_fractional_fonts_property.c                                  */
/* desc : function to modify in Scilab the fractional_font field of       */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int set_fractional_font_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{

  if (   sciGetEntityType(pobj) != SCI_SUBWIN
      && sciGetEntityType(pobj) != SCI_TEXT
      && sciGetEntityType(pobj) != SCI_LABEL
      && sciGetEntityType(pobj) != SCI_AXES
      && sciGetEntityType(pobj) != SCI_LEGEND)
  {
    sciprint(_("Incompatible type for property %s.\n"),"fractional_font") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( isStringParamEqual( stackPointer, "on" ) )
  {
    sciSetIsUsingFractionalMetrics(pobj, TRUE);
  }
  else if ( isStringParamEqual( stackPointer, "off" ) )
  {
    sciSetIsUsingFractionalMetrics(pobj, FALSE);
  }
  else
  {
    sciprint("Nothing to do (value must be 'on' or 'off').\n") ;
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
