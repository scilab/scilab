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
/* file: set_title_property.c                                             */
/* desc : function to modify in Scilab the title field of                 */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "HandleManagement.h"

/*------------------------------------------------------------------------*/
int set_links_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  int nblegends;
  int i;
  if ( sciGetEntityType(pobj) != SCI_LEGEND )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"links");
    return SET_PROPERTY_ERROR ;
  }
  nblegends=pLEGEND_FEATURE(pobj)->nblegends;
  if (nbRow*nbCol != nblegends) {
    Scierror(999, _("Wrong size for '%s' property: %d elements expected.\n"), "links", nblegends);
    return SET_PROPERTY_ERROR ;
  }

  for (i=0; i<nblegends; i++) {
    pLEGEND_FEATURE(pobj)->tabofhandles[i]=getHandleFromStack( stackPointer+i );
  }
  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
