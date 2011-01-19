/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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
/* file: get_position_property.c                                          */
/* desc : function to retrieve in Scilab the position field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_position_property( sciPointObj * pobj )
{
    char* type;

  /* Deactivated for now */
#if 0
    if (sciGetEntityType(pobj) == SCI_UIMENU )
    {
        return sciReturnDouble( pUIMENU_FEATURE(pobj)->MenuPosition ) ;
    }
    else if (sciGetEntityType(pobj) == SCI_UICONTROL)
    {
        return  GetUicontrolPosition(pobj);
    }
#endif

    getGraphicObjectProperty(pobj->UID, __GO_TYPE__, jni_string, &type);

    if (strcmp(type, __GO_FIGURE__) == 0)
    {
        int* figurePosition;
        int* figureSize;
        double position[4];

        getGraphicObjectProperty(pobj->UID, __GO_POSITION__, jni_int_vector, &figurePosition);

        getGraphicObjectProperty(pobj->UID, __GO_SIZE__, jni_int_vector, &figureSize);

        if (figurePosition == NULL || figureSize == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "position");
            return -1;
        }

        position[0] = (double) figurePosition[0];
        position[1] = (double) figurePosition[1];
        position[2] = (double) figureSize[0];
        position[3] = (double) figureSize[1];

        return sciReturnRowVector(position, 4);
    }
    else
    {
        double* position;

        getGraphicObjectProperty(pobj->UID, __GO_POSITION__, jni_double_vector, &position);

        if (position == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "position");
            return -1;
        }

        return sciReturnRowVector(position, 2);
    }

}
/*------------------------------------------------------------------------*/
