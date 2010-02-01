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
/* file: set_font_color_property.c                                        */
/* desc : function to modify in Scilab the font_color field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_font_color_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{

  int value ;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Integer expected.\n"), "font_color");
    return SET_PROPERTY_ERROR ;
  }

	value = (int) getDoubleFromStack( stackPointer ) ;

  if (   sciGetEntityType(pobj) == SCI_SUBWIN
           || sciGetEntityType(pobj) == SCI_FIGURE
           || sciGetEntityType(pobj) == SCI_LEGEND
           || sciGetEntityType(pobj) == SCI_AXES)
  {
    return sciSetFontForeground( pobj,value ) ;
  } /* F.Leray 08.04.04 */
  else
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"font_color") ;
  }
	return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
