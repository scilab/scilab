/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - Digiteo - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


/*------------------------------------------------------------------------*/
/* file: get_outside_colors_property.c                                    */
/* desc : function to retrieve in Scilab the outside_colors field of      */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_outside_colors_property( sciPointObj * pobj )
{
  int colors[2];
  if (sciGetEntityType (pobj) != SCI_FEC)
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"outside_colors") ;
    return -1 ;
  }

	sciGetOutsideColor(pobj, colors);

  return sciReturnRowVectorFromInt( colors, 2 ) ;

}
/*------------------------------------------------------------------------*/
