/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
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
/* file: set_callback_property.c                                          */
/* desc : function to modify in Scilab the callback field of              */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/
#include <string.h>
#include "setHandleProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "graphicObjectProperties.h"
#include "setGraphicObjectProperty.h"
#include "api_scilab.h"
#include "sci_malloc.h"
/*------------------------------------------------------------------------*/
int set_callback_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    // Callback must be only one character string

    BOOL status = FALSE;
    char * cbString = NULL;
    int cbType = 0;

    int strNbRow = 0, strNbCol = 0;
    int iRows = 0, iCols = 0;
    double* pdblData = NULL;

    if (valueType == sci_strings)
    {
        if (nbCol != 1)
        {
            Scierror(999, _("Wrong size for '%s' property: string expected.\n"), "Callback");
            return SET_PROPERTY_ERROR;
        }
        cbString = (char*)_pvData;
    }
    else if (valueType == sci_list)
    {
        int iLen = 0;
        if (nbRow * nbCol != 2)
        {
            Scierror(999, _("Wrong size for '%s' property: a 2-item list expected.\n"), "Callback");
            return SET_PROPERTY_ERROR;
        }

        getMatrixOfDoubleInList(_pvCtx, (int*)_pvData, 1, &iRows, &iCols, &pdblData);
        if (iRows * iCols != 1)
        {
            Scierror(999, _("Wrong size for '%s' property: A real expected.\n"), "callback_type");
            return SET_PROPERTY_ERROR;
        }
        else
        {
            cbType = (int)pdblData[0];
        }


        getMatrixOfStringInList(_pvCtx, (int*)_pvData, 2, &iRows, &iCols, NULL, NULL);
        if (iRows * iCols != 1)
        {
            Scierror(999, _("Wrong size for '%s' property: string expected.\n"), "Callback");
            return SET_PROPERTY_ERROR;
        }

        getMatrixOfStringInList(_pvCtx, (int*)_pvData, 2, &iRows, &iCols, &iLen, NULL);
        cbString = (char*)MALLOC(sizeof(char) * (iLen + 1));
        getMatrixOfStringInList(_pvCtx, (int*)_pvData, 2, &iRows, &iCols, &iLen, &cbString);
    }
    else
    {

        Scierror(999, _("Wrong type for '%s' property: string or 2-item list expected.\n"), "Callback");
        return SET_PROPERTY_ERROR;
    }

    if (strcmp(cbString, "") == 0)
    {
        cbType = -1; /* Disabled */
    }

    status = setGraphicObjectProperty(iObjUID, __GO_CALLBACK__, cbString, jni_string, 1);

    if (status != TRUE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "Callback");
        return SET_PROPERTY_ERROR;
    }

    if (setGraphicObjectProperty(iObjUID, __GO_CALLBACKTYPE__, &cbType, jni_int, 1) == FALSE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "callback_type");
        return SET_PROPERTY_ERROR;
    }

    return SET_PROPERTY_SUCCEED;
}
/*------------------------------------------------------------------------*/
