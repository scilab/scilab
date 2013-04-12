/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
	{NULL, "export_to_hdf5"},
	{NULL, "import_from_hdf5"},
	{NULL, "listvar_in_hdf5"},
	{NULL, "is_hdf5_file"},
    {NULL, "h5dump"},
    {NULL, "h5open"},
    {NULL, "h5close"},
    {NULL, "h5read"},
    {NULL, "h5ls"},
    {NULL, "%H5Object_p"},
    {NULL, "%H5Object_e"},
    {NULL, "h5group"},
    {NULL, "h5rm"},
    {NULL, "h5dataset"},
    {NULL, "h5write"},
    {NULL, "h5attr"},
    {NULL, "h5writeattr"},
    {NULL, "h5ln"},
    {NULL, "h5readattr"},
    {NULL, "h5flush"},
    {NULL, "h5cp"},
    {NULL, "h5isFile"},
    {NULL, "h5isGroup"},
    {NULL, "h5isSet"},
    {NULL, "h5isAttr"},
    {NULL, "h5isSpace"},
    {NULL, "h5isType"},
    {NULL, "h5isRef"},
    {NULL, "h5isList"},
    {NULL, "h5mount"},
    {NULL, "h5umount"},
    {NULL, "h5mv"},
    {NULL, "h5get"},
    {NULL, "h5label"},
    {NULL, "h5exists"},
    {NULL, "h5isCompound"},
    {NULL, "%H5Object_fieldnames"},
    {NULL, "%H5Object_fieldnames"},
    {NULL, "h5isArray"},
    {NULL, "h5isVlen"}
};
/*--------------------------------------------------------------------------*/
int gw_hdf5(void)
{
	return 0;
}
/*--------------------------------------------------------------------------*/

