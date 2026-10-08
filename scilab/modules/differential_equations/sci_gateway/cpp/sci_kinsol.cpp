//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2021-2023 - UTC - Stéphane MOTTELET
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// For more information, see the COPYING file which you should have received
//
//--------------------------------------------------------------------------

#include "differential_equations_gw.hxx"

#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "list.hxx"
#include "mlist.hxx"
#include "pointer.hxx"
#include "struct.hxx"
#include "callable.hxx"
#include "runvisitor.hxx"
#include "context.hxx"

#include "KINSOLManager.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"
#include "charEncoding.h"
#include "sci_malloc.h"
}

types::Function::ReturnValue sci_kinsol(types::typed_list &in, types::optional_list &opt, int _iRetCount, types::typed_list &out)
{
    KINSOLManager *manager = NULL;
    char errorMsg[256];

    manager = new KINSOLManager();

    if (in.size() != 2)
    {
        sprintf(errorMsg, _("%s: Wrong number of input argument(s): %d expected.\n"), manager->getSolverName().c_str(), 2);
        delete manager;
        throw ast::InternalError(errorMsg);
    }
    if (_iRetCount > 4)
    {
        sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), manager->getSolverName().c_str(), 1, 4);
        delete manager;
        throw ast::InternalError(errorMsg);
    }

    manager->setIretCount(_iRetCount);

    try
    {
        manager->parseMatrices(in);
        manager->parseFunction(in[0]);
        manager->parseOptions(opt);
        manager->init();
        manager->solve();
    }
    catch (ast::InternalError& ie)
    {
        if (manager->getUserStop() == false)
        {
            delete manager;
            return types::Function::Error;            
        }
        else if (manager->getDisplay() != L"none")
        {
            char* pstMsg = wide_string_to_UTF8(ie.GetErrorMessage().c_str());
            sciprint("\n%s: %s", "kinsol", pstMsg);
            FREE(pstMsg);
        }
    }
    catch (ast::InternalAbort& ia)
    {
        delete manager;
        throw (ia);
    }

    out.push_back(manager->getYOut());
    
    if (_iRetCount > 1)
    {
        out.push_back(manager->getFOut());
    }
    if (_iRetCount > 2)
    {
        out.push_back(manager->getExitCode());
    }
    if (_iRetCount > 3)
    {
        manager->createSolutionOutput(out);
    }

    delete manager;
    return types::Function::OK;
}
