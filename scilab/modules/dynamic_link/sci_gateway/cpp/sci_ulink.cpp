/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - 2011 - Antoine ELIAS
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
#include "dynamic_link_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "dynamic_link.h"
}

void unLinkAll();
void unLink(int _iLib);
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_ulink(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    /* environment variable used (linux) to detect a PROFILING tools */
    /* in this case, we do not ulink external dynamic libraries */
    if (getenv("PROFILE_SCILAB_DYNAMIC_LINK") != NULL)
    {
        return types::Function::OK;
    }

    if (in.size() > 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "ulink", 0, 1);
        return types::Function::Error;
    }

    if (in.size() == 0)
    {
        unLinkAll();
    }
    else if (in.size() == 1 && in[0]->isDouble())
    {
        types::Double* pDIds = in[0]->getAs<types::Double>();

        for (int i = 0 ; i < pDIds->getSize() ; i++)
        {
            unLink((int)pDIds->get(i));
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: real scalar expected.\n"), "ulink", 1);
        return types::Function::Error;
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
void unLinkAll()
{
    std::vector<ConfigVariable::DynamicLibraryStr*>* pDLLIst =  ConfigVariable::getDynamicLibraryList();
    for (int i = 0 ; i < pDLLIst->size() ; i++)
    {
        unLink(i);
    }
}
/*--------------------------------------------------------------------------*/
void unLink(int _iLib)
{
    ConfigVariable::DynamicLibraryStr* pStr = ConfigVariable::getDynamicLibrary(_iLib);
    if (pStr)
    {
        DynLibHandle iLib = pStr->hLib;
        ConfigVariable::removeDynamicLibrary(_iLib);
        Sci_dlclose(iLib);
    }
}
/*--------------------------------------------------------------------------*/
