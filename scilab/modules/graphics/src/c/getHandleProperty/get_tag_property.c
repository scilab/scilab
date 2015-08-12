/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
/* file: get_tag_property.c                                               */
/* desc : function to retrieve in Scilab the tag field of                 */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "sci_types.h"
#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_tag_property(void* _pvCtx, int iObjUID)
{
    char* tag = NULL;
    int status = 0;

    getGraphicObjectProperty(iObjUID,  __GO_TAG__, jni_string, (void**) &tag);

    if (tag == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "Tag");
        return FALSE;
    }
    else
    {
        status = sciReturnString(_pvCtx, tag);
        free(tag);
        return status;
    }
}
/*------------------------------------------------------------------------*/
