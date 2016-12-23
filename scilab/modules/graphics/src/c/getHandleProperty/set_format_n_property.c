/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_format_n_property.c                                          */
/* desc : function to modify in Scilab the format_n field of              */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "sci_malloc.h"
#include "string.h"
#include "StringMatrix.h"
#include "Format.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_format_n_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    char* format = NULL;
    char * oldFormat = NULL;
    StringMatrix * labels = NULL;

    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "format_n");
        return SET_PROPERTY_ERROR;
    }

    format = (char*)_pvData;

    getGraphicObjectProperty(iObjUID, __GO_FORMATN__, jni_string, (void **)&oldFormat);

    if (!oldFormat)
    {
        Scierror(999, _("Could not retrieve format.\n"));
        return SET_PROPERTY_ERROR;
    }

    if (strcmp(format, oldFormat) == 0)
    {
        return SET_PROPERTY_SUCCEED;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_FORMATN__, format, jni_string, 1);
    if (status == TRUE)
    {
        labels = computeDefaultTicsLabels(iObjUID);
        if (labels != NULL)
        {
            char ** data = getStrMatData(labels);
            setGraphicObjectProperty(iObjUID, __GO_TICKS_LABELS__, data, jni_string_vector, labels->nbCol * labels->nbRow);
            deleteMatrix(labels);
        }

        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "format_n");
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
