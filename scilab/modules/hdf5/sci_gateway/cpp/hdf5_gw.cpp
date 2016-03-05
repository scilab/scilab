/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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
/*--------------------------------------------------------------------------*/

#include "H5Type.hxx"
#include "H5VariableScope.hxx"
#include "H5Object.hxx"

extern "C"
{
#include "gw_hdf5.h"
#include "api_scilab.h"
#include "sci_malloc.h"
#include "h5_fileManagement.h"
}
/*--------------------------------------------------------------------------*/
int Initialize(void)
{
    org_modules_hdf5::H5VariableScope::initScope();
    org_modules_hdf5::H5Object::initRoot();
    return 0;
}
/*--------------------------------------------------------------------------*/
int Finalize(void)
{
    org_modules_hdf5::H5Type::cleanMap();
    //org_modules_hdf5::H5VariableScope::clearScope();
    org_modules_hdf5::H5Object::clearRoot();
    /* cleanup HDF5 */
    HDF5cleanup();
    return 0;
}
/*--------------------------------------------------------------------------*/


