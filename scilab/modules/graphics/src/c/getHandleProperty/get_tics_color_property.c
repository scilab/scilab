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
/* file: get_tics_color_property.c                                        */
/* desc : function to retrieve in Scilab the tics_color field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_tics_color_property( sciPointObj * pobj )
{
  if (sciGetEntityType (pobj) == SCI_AXES)
  {
    return sciReturnDouble( sciGetForegroundToDisplay(pobj) ) ;
  }
  else if (sciGetEntityType (pobj) == SCI_SUBWIN)
  {
    Scierror(999, _("Warning: %s use is deprecated and no more taken into account, use %s property to edit Axes color.\n"),"'tics_color'","'foreground'");
    return sciReturnDouble( pSUBWIN_FEATURE(pobj)->axes.ticscolor ) ;
  }
  else
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"tics_color");
    return -1;
  }

}
/*------------------------------------------------------------------------*/
