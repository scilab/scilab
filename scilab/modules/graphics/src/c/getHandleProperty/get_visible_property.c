/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_visible_property.c                                           */
/* desc : function to retrieve in Scilab the visible field of a handle    */
/*------------------------------------------------------------------------*/

#include <stdlib.h>

#include "ObjectStructure.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/

int get_visible_property( sciPointObj * pobj )
{
    int visible = 0;
    int* piVisible = &visible;
    getGraphicObjectProperty(pobj->UID, __GO_VISIBLE__, jni_bool, &piVisible);

#ifdef __OLD_IMPLEMENTATION__
    // Uicontrol should follow MVC implementation
    if ( (sciGetEntityType(pobj) == SCI_UIMENU) || (sciGetEntityType(pobj) == SCI_UICONTROL) )
    {
        return GetUiobjectVisible(pobj);
    }
#endif

    if ( piVisible == NULL )
	{
		Scierror(999, _("'%s' property does not exist for this handle.\n"),"visible");
		return -1;
	}


    if (visible)
    {
        return sciReturnString( "on" ) ;
    }
    else
    {
        return sciReturnString( "off" ) ;
    }
}

int get_UID(sciPointObj * pobj)
{
    return sciReturnString(pobj->UID);
}
/*------------------------------------------------------------------------*/
