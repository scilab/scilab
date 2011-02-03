/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_box_property.c                                               */
/* desc : function to retrieve in Scilab the box field of                 */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_box_property( sciPointObj * pobj )
{
    int iBoxType = 0;
    int* piBoxType = &iBoxType;

    getGraphicObjectProperty(pobj->UID, __GO_BOX_TYPE__, jni_int, &piBoxType);

    if (piBoxType == NULL) {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"box");
        return -1;
    }

    if (iBoxType == 0)
    {
        return sciReturnString("off");
    }
    else if (iBoxType == 1)
    {
        return sciReturnString("on");
    }
    else if (iBoxType == 2)
    {
        return sciReturnString("hidden_axes");
    }
    else if (iBoxType == 3)
    {
        return sciReturnString("back_half");
    }

#if 0
  /*
   * To be implemented using the MVC framework, since the Text object's "Box" property is
   * internally represented as a Boolean and its constant is __GO_BOX__ instead of __GO_BOX_TYPE__
   */
  else if (sciGetEntityType( pobj ) == SCI_TEXT)
  {
    int* box;
    box = (int*) getGraphicObjectProperty(pobj->UID, __GO_BOX__, jni_bool);

    if (*box)
    {
      return sciReturnString( "on" );
    }
    else
    {
      return sciReturnString( "off" );
    }
  }
#endif

}
/*------------------------------------------------------------------------*/
