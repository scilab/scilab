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
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_mark_size_property.c                                         */
/* desc : function to modify in Scilab the mark_size field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "sci_malloc.h"
#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_mark_size_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
	int status = -1;
    int *tmp = NULL;

    if ( valueType != sci_matrix )
    {
        Scierror(999, _("Wrong type for '%s' property: Matrix expected.\n"), "mark_size");
        return SET_PROPERTY_ERROR;
    }

    if ( nbRow != 1 || nbCol <= 0 )
    {
        Scierror(999, _("Wrong size for '%s' property: Row vector expected.\n"), "mark_size");
        return SET_PROPERTY_ERROR;
    }

	tmp = MALLOC(nbCol * sizeof(int));
    copyDoubleVectorToIntFromStack(_pvData, tmp, nbCol);
	status = sciSetMarkSize(iObjUID, tmp, nbCol);
	FREE(tmp);

    return status;
}
/*------------------------------------------------------------------------*/
