/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
/* file: graphicModuleClose.c                                             */
/* desc : function to be called each time the graphic module is unloaded  */
/*------------------------------------------------------------------------*/

#include "GetHashTable.h"
#include "SetHashTable.h"
#include "DestroyObjects.h"
#include "graphicModuleLoad.h"
#include "InitObjects.h"
#include "FigureList.h"

static BOOL isGraphicModuleLoaded = FALSE;

/*------------------------------------------------------------------------*/
void loadGraphicModule(void)
{
    if (isGraphicModuleLoaded)
    {
        return;
    }

    /* Create hastable for get and set functions */
    createScilabGetHashTable();
    createScilabSetHashTable();

    /* Create hastable for handle storing */
    //getScilabHandleTab();

    /* Create data for synchronization */
    //createGraphicSynchronizer();

    C2F(graphicsmodels)();

    /* Register Scilab as a dedicated View */
    registerToController();

    isGraphicModuleLoaded = TRUE;
}
/*------------------------------------------------------------------------*/
void closeGraphicModule(void)
{
    if (!isGraphicModuleLoaded)
    {
        return;
    }

    /* destroy all graphic windows */
    AllGraphWinDelete();

    /* Unegister Scilab as a dedicated View */
    unregisterToController();

    isGraphicModuleLoaded = FALSE;

}
/*------------------------------------------------------------------------*/
