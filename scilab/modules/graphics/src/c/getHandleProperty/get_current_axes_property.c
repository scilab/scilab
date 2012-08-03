/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_current_axes_property.c                                      */
/* desc : function to retrieve in Scilab the current_axes field of a      */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "CurrentSubwin.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"

#include "BuildObjects.h"
#include "HandleManagement.h"
#include "CurrentFigure.h"
#include "CurrentSubwin.h"

/*------------------------------------------------------------------------*/
int get_current_axes_property(void* _pvCtx, char* pobjUID)
{
    char *pFigureUID = NULL;
    char *pSubWinUID = NULL;

    if (pobjUID != NULL)
    {
        /* This property should not be called on an handle */
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "current_axes");
        return -1;
    }

    pSubWinUID = (char*)getCurrentSubWin();

    if (pSubWinUID == NULL)
    {
        pFigureUID = createNewFigureWithAxes();
        setCurrentFigure(pFigureUID);
    }


    return sciReturnHandle(_pvCtx, getHandle(getCurrentSubWin())) ;
}
/*------------------------------------------------------------------------*/
