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


