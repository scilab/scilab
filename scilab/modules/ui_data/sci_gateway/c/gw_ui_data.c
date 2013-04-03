/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Allan CORNET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_ui_data.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "callFunctionFromGateway.h"
#include "BOOL.h"
#include "scilabmode.h"
#include "localization.h"
#include "Scierror.h"
#include "loadOnUseClassPath.h"
/*--------------------------------------------------------------------------*/
/*static BOOL loadedDep = FALSE;*/
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
    {sci_editvar, "editvar"},
    {sci_browsevar, "browsevar"},
    {sci_filebrowser, "filebrowser"},
    {sci_updatebrowsevar, "updatebrowsevar"},
    {sci_closeEditvar, "closeEditvar"}
};
/*--------------------------------------------------------------------------*/
int gw_ui_data(void)
{
    Rhs = Max(0, Rhs);

    if ( getScilabMode() == SCILAB_NWNI)
    {
        Scierror(999,_("Scilab '%s' module disabled in -nogui or -nwni mode.\n"), "ui_data");
        return 0;
    }

/*      if (!loadedDep)
        {
        loadOnUseClassPath("ui_data");
        loadedDep = TRUE;
        }
*/

    if(pvApiCtx == NULL)
    {
        pvApiCtx = (StrCtx*)MALLOC(sizeof(StrCtx));
    }

    pvApiCtx->pstName = (char*)Tab[Fin-1].name;
    callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
    return 0;
}
/*--------------------------------------------------------------------------*/
