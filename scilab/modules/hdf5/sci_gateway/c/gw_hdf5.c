/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_hdf5.h"
#include "callFunctionFromGateway.h"
#include "api_scilab.h"
#include "MALLOC.h"

/*--------------------------------------------------------------------------*/
/*  interface function */
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
    {sci_export_to_hdf5, "export_to_hdf5"},
    {sci_import_from_hdf5, "import_from_hdf5"},
    {sci_listvar_in_hdf5, "listvar_in_hdf5"},
    {sci_is_hdf5_file, "is_hdf5_file"},
    {sci_h5dump, "h5dump"},
    {sci_h5open, "h5open"},
    {sci_h5close, "h5close"},
    {sci_h5read, "h5read"},
    {sci_h5ls, "h5ls"},
    {sci_percent_H5Object_p, "%H5Object_p"},
    {sci_percent_H5Object_e, "%H5Object_e"},
    {sci_h5group, "h5group"},
    {sci_h5rm, "h5rm"},
    {sci_h5dataset, "h5dataset"},
    {sci_h5write, "h5write"},
    {sci_h5attr, "h5attr"},
    {sci_h5attr, "h5writeattr"},
    {sci_h5ln, "h5ln"},
    {sci_h5readattr, "h5readattr"},
    {sci_h5flush, "h5flush"},
    {sci_h5cp, "h5cp"},
    {sci_h5isFile, "h5isFile"},
    {sci_h5isGroup, "h5isGroup"},
    {sci_h5isSet, "h5isSet"},
    {sci_h5isAttr, "h5isAttr"},
    {sci_h5isSpace, "h5isSpace"},
    {sci_h5isType, "h5isType"},
    {sci_h5isRef, "h5isRef"},
    {sci_h5isList, "h5isList"},
    {sci_h5mount, "h5mount"},
    {sci_h5umount, "h5umount"},
    {sci_h5mv, "h5mv"},
    {sci_h5get, "h5get"},
    {sci_h5label, "h5label"},
    {sci_h5exists, "h5exists"},
    {sci_h5isCompound, "h5isCompound"},
    {sci_percent_H5Object_fieldnames, "%H5Object_fieldnames"},
    {sci_percent_H5Object_fieldnames, "%H5Object_fieldnames"},
    {sci_h5isArray, "h5isArray"},
    {sci_h5isVlen, "h5isVlen"},
};
/*--------------------------------------------------------------------------*/
int gw_hdf5(void)
{
    Rhs = Max(0, Rhs);

    if (pvApiCtx == NULL)
    {
        pvApiCtx = (StrCtx*)MALLOC(sizeof(StrCtx));
    }

    pvApiCtx->pstName = (char*)Tab[Fin - 1].name;
    callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
    return 0;
}
/*--------------------------------------------------------------------------*/

