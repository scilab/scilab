/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_user_data_property.c                                         */
/* desc : function to modify in Scilab the user_data field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/
#include "setHandleProperty.h"
#include "SetPropertyStatus.h"
#include "Scierror.h"
#include "localization.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "api_scilab.h"
/*------------------------------------------------------------------------*/
int set_user_data_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    /*NOT COMPATIBLE WITH SCILAB 6*/
    int iRhs = getRhsFromAddress(pvApiCtx, (int*)_pvData);
    int iUserDataSize = GetDataSize(iRhs) * 2; /* GetDataSize returns the size of the variable in double words */
    int *piUserData = (int*)GetData(iRhs);

    BOOL status = FALSE;

    if (setGraphicObjectProperty(iObjUID, __GO_USER_DATA__, piUserData, jni_int_vector, iUserDataSize) == FALSE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "user_data");
        return SET_PROPERTY_ERROR;
    }

    return SET_PROPERTY_SUCCEED;
}

/*------------------------------------------------------------------------*/
