/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2017 - Scilab Enterprises - Clément DAVID
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
/* file: get_tag_property.cpp                                               */
/* desc : function to retrieve in Scilab the tag field of                 */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

extern "C"
{
#include "sci_types.h"
#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

/*------------------------------------------------------------------------*/
void* get_tag_property(void* _pvCtx, int iObjUID)
{
    char* tag = NULL;
    void* status = NULL;

	// the tag pointer is allocated using new[]
    getGraphicObjectProperty(iObjUID,  __GO_TAG__, jni_string, (void**) &tag);

    if (tag == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "Tag");
        return nullptr;
    }
    else
    {
        status = sciReturnString(tag);
        delete[] tag;
        return status;
    }
}
/*------------------------------------------------------------------------*/
