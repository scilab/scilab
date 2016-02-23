/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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
