/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
