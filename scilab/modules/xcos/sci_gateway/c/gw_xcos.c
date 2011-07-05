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
#include "gw_xcos.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
#include "BOOL.h"
#include "sci_mode.h"
#include "localization.h"
#include "Scierror.h"
#include "BOOL.h"
#include "loadOnUseClassPath.h"
/*--------------------------------------------------------------------------*/
static BOOL loadedDep = FALSE;
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] = {
    {NULL, ""}, //xcos
    {NULL, ""}, //warnBlockByUID
    {NULL, ""}, //closeXcos
    {NULL, ""}, //xcosDiagramToHDF5
    {NULL, ""},  //"xcosPalLoad"
    {NULL, ""},  //"xcosPalCategoryAdd"
    {NULL, ""},  //"xcosPalDelete"
    {NULL, ""},  //"xcosPalMove"
    {NULL, ""},  //"xcosPalEnable"
    {NULL, ""},  //"xcosPalDisable"
    {NULL, ""},  //"xcosPalGenerateIcon"
    {NULL, ""} //"xcosConfigureXmlFile"
};

/*--------------------------------------------------------------------------*/
int gw_xcos(void)
{
    Rhs = Max(0, Rhs);

    if  ( getScilabMode() == SCILAB_NWNI)
    {
        Scierror(999,_("Scilab '%s' module disabled in -nogui or -nwni mode.\n"), "xcos");
        return 0;
    }

    //if (!loadedDep                                      // never reload
    //    && Tab[Fin-1].f != sci_closeXcosFromScilab)     // do not load on close
    //{
    //    loadOnUseClassPath("XCos");
    //    loadedDep = TRUE;
    //}
    callFunctionFromGateway(Tab,  SIZE_CURRENT_GENERIC_TABLE(Tab));
    return 0;
}
/*--------------------------------------------------------------------------*/
int xcosStarted(void)
{
    return loadedDep;
}
/*--------------------------------------------------------------------------*/
