/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
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
/* file: get_figures_id_property.c                                        */
/* desc : function to retrieve in Scilab the figures_id field of a handle */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "sci_malloc.h"
#include "Scierror.h"
#include "localization.h"
#include "FigureList.h"

/*------------------------------------------------------------------------*/
void* get_figures_id_property(void* _pvCtx, int iObjUID)
{
    int nbFig = 0;
    int* ids = NULL;
    void* status = NULL;

    if (iObjUID != 0)
    {
        /* This property should not be called on an handle */
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "figures_id");
        return NULL;
    }

    nbFig = sciGetNbFigure() ; /* get the number of opened windows */

    ids = (int*)MALLOC(nbFig * sizeof(int));
    if (ids == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), "get_figures_id_property");
        return NULL;
    }

    sciGetFiguresId(ids);

    status = sciReturnRowIntVector(ids, nbFig);

    FREE(ids);

    return status;
}
/*------------------------------------------------------------------------*/

