/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_callback_property.c                                          */
/* desc : function to modify in Scilab the callback field of              */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/
#include "sci_types.h"
#include "setHandleProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "graphicObjectProperties.h"
#include "setGraphicObjectProperty.h"
#include "stack-c.h"
/*------------------------------------------------------------------------*/
int set_callback_property(void* _pvCtx, char* pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
    // Callback must be only one character string

    BOOL status = FALSE;
    char * cbString = NULL;
    int cbType = 0;

    int strNbRow = 0, strNbCol = 0;
    int typeNbRow = 0, typeNbCol = 0;
    int typeStackPointer = 0, stringStackPointer = 0;

    if (valueType == sci_strings)
    {
        if (nbCol != 1) {
            Scierror(999, _("Wrong size for '%s' property: A string expected.\n"), "Callback");
            return SET_PROPERTY_ERROR;
        }
        cbString = getStringFromStack(stackPointer);
    }
    else if (valueType == sci_list)
    {
        if (nbRow * nbCol != 2)
        {
            Scierror(999, _("Wrong size for '%s' property: a 2-item list expected.\n"), "Callback");
            return SET_PROPERTY_ERROR;
        }

        GetListRhsVar((int)stackPointer, 1, MATRIX_OF_DOUBLE_DATATYPE, &typeNbRow, &typeNbCol, &typeStackPointer);
        if (typeNbRow * typeNbCol !=1)
        {
            Scierror(999, _("Wrong size for '%s' property: A real expected.\n"), "callback_type");
            return SET_PROPERTY_ERROR;
        }
        else
        {
            cbType = (int) (*stk(typeStackPointer));
        }

        GetListRhsVar((int)stackPointer, 2, STRING_DATATYPE, &strNbRow, &strNbCol, &stringStackPointer);
        if (strNbCol !=1)
        {
            Scierror(999, _("Wrong size for '%s' property: A string expected.\n"), "Callback");
            return SET_PROPERTY_ERROR;
        }
        else
        {
            cbString = cstk(stringStackPointer);
        }
    }
    else
    {

        Scierror(999, _("Wrong type for '%s' property: A string or a 2-item list expected.\n"), "Callback");
        return SET_PROPERTY_ERROR;
    }

    if (strcmp(cbString, "") == 0)
    {
        cbType = -1; /* Disabled */
    }

    status = setGraphicObjectProperty(pobjUID, __GO_CALLBACK__, cbString, jni_string, 1);

    if (status != TRUE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "Callback");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(pobjUID, __GO_CALLBACKTYPE__, &cbType, jni_int, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "callback_type");
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
