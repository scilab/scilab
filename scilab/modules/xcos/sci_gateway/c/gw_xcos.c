/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2009-2010 - Allan CORNET <allan.cornet@scilab.org>
 * Copyright (C) DIGITEO - 2010-2010 - Clément DAVID <clement.david@scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_xcos.h"
#include "callFunctionFromGateway.h"
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
static BOOL loadedDep = FALSE;

/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
    {sci_Xcos, ""}, //xcos"},
    {sci_warnBlockByUID, ""}, //warnBlockByUID"},
    {sci_closeXcosFromScilab, ""}, //closeXcos"},
    {sci_xcosDiagramToScilab, ""}, //xcosDiagramToScilab"},
    {sci_xcosPalLoad, ""}, //xcosPalLoad"},
    {sci_xcosPalCategoryAdd, ""}, //xcosPalCategoryAdd"},
    {sci_xcosPalDelete, ""}, //xcosPalDelete"},
    {sci_xcosPalMove, ""}, //xcosPalMove"},
    {sci_xcosPalEnable, ""}, //xcosPalEnable"},
    {sci_xcosPalDisable, ""}, //xcosPalDisable"},
    {sci_xcosPalGenerateIcon, ""}, //xcosPalGenerateIcon"},
    {sci_xcosConfigureXmlFile, ""}, //xcosConfigureXmlFile"},
    {sci_xcosAddToolsMenu, ""}, //xcosAddToolsMenu"},
    {sci_xcosUpdateBlock, ""}, //xcosUpdateBlock"},
    {sci_loadXcos, ""}, //loadXcos"}
    {sci_xcosSimulationStarted, ""} //xcosSimulationStarted"}
};

/*--------------------------------------------------------------------------*/
int gw_xcos(void)
{
    return 0;
}

/*--------------------------------------------------------------------------*/
int xcosStarted(void)
{
    return loadedDep;
}

/*--------------------------------------------------------------------------*/
