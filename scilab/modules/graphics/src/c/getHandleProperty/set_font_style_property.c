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
/* file: set_font_style_property.c                                        */
/* desc : function to modify in Scilab the font_style field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "RendererFontManager.h"

/*------------------------------------------------------------------------*/
int set_font_style_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  int value;
	/* number of fonts available */
	int nbInstalledFonts = getNbInstalledFonts();

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Integer expected.\n"), "font_style");
    return SET_PROPERTY_ERROR ;
  }

	value = (int) getDoubleFromStack( stackPointer ) ;

	/* Check that the wanted value is a correct font */
  if ( value >= nbInstalledFonts || value < 0 )
  {
    Scierror(999, _("Wrong value for '%s' property: An Integer between %d and %d expected.\n"), "font_style", 0, nbInstalledFonts - 1) ;
    return SET_PROPERTY_ERROR ;
  }
  
  return sciSetFontStyle( pobj, value ) ;
}
/*------------------------------------------------------------------------*/
