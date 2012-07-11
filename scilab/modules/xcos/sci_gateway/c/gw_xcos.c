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
static gw_generic_table Tab[] = {
    {NULL, ""}, //xcos
    {NULL, ""}, //warnBlockByUID
    {NULL, ""}, //closeXcos
    {NULL, ""}, //xcosDiagramToScilab
    {NULL, ""},  //"xcosPalLoad"
    {NULL, ""},  //"xcosPalCategoryAdd"
    {NULL, ""},  //"xcosPalDelete"
    {NULL, ""},  //"xcosPalMove"
    {NULL, ""},  //"xcosPalEnable"
    {NULL, ""},  //"xcosPalDisable"
    {NULL, ""},  //"xcosPalGenerateIcon"
    {NULL, ""}, //"xcosConfigureXmlFile"
    {NULL, ""}, //xcosAddToolsMenu
    {NULL, ""} //xcosUpdateBlock
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
