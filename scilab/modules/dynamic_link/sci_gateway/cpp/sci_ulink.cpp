/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - 2011 - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
    if(getenv("PROFILE_SCILAB_DYNAMIC_LINK") != NULL)
    {
        return types::Function::OK;
    }

    if(in.size() > 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "ulink", 0, 1);
        return types::Function::Error;
    }

    if(in.size() == 0)
    {
        unLinkAll();
    }
    else if(in.size() == 1)
    {
        types::Double* pDIds = in[0]->getAs<types::Double>();
        if(pDIds == NULL)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Matrix expected.\n"), "ulink", 1);
            return types::Function::Error;
        }

        for(int i = 0 ; i < pDIds->getSize() ; i++)
        {
            unLink(pDIds->get(i));
        }
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
void unLinkAll()
{
    std::vector<ConfigVariable::DynamicLibraryStr*>* pDLLIst =  ConfigVariable::getDynamicLibraryList();
    for(int i = 0 ; i < pDLLIst->size() ; i++)
    {
        unLink(i);
    }
}
/*--------------------------------------------------------------------------*/
void unLink(int _iLib)
{
    unsigned long long iLib = ConfigVariable::getDynamicLibrary(_iLib)->hLib;
    ConfigVariable::removeDynamicLibrary(_iLib);
    Sci_dlclose(iLib);
}
/*--------------------------------------------------------------------------*/
