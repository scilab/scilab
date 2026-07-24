//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2021 - UTC - Stéphane MOTTELET
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// For more information, see the COPYING file which you should have received
//
//--------------------------------------------------------------------------

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

#include "OdeManager.hxx"

extern "C"
{
#include "localization.h"
}

template<typename ODEManagerType>
types::Function::ReturnValue sci_sundialsode(types::typed_list &in, types::optional_list &opt, int _iRetCount, types::typed_list &out)
{
    ODEManagerType *manager = NULL;
    char errorMsg[256];
    int iStart = 0;
    bool bIsExtension = false;

    manager = new ODEManagerType();

    if (in.size() != 2 && in.size() != manager->getMaxNargin() && in.size() != manager->getMinNargin())
    {
        if (manager->getMaxNargin() == manager->getMinNargin())
        {
            sprintf(errorMsg, _("%s: Wrong number of input argument(s): %d or %d expected.\n"), manager->getSolverName().c_str(), 2, manager->getMaxNargin());
        }
        else
        {
            sprintf(errorMsg, _("%s: Wrong number of input argument(s): %d, %d or %d expected.\n"), manager->getSolverName().c_str(), 2, manager->getMinNargin(), manager->getMaxNargin());
        }
        delete manager;
        throw ast::InternalError(errorMsg);
    }
    if (_iRetCount > 3)
    {
        sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), manager->getSolverName().c_str(), 1, 3);
        delete manager;
        throw ast::InternalError(errorMsg);
    }

    if (in[0]->isMList() && in[0]->getAs<types::MList>()->getTypeStr() == L"_odeSolution")
    {
        if (in.size() != 2)
        {
            sprintf(errorMsg, _("%s: Wrong number of input argument(s): %d expected.\n"), manager->getSolverName().c_str(), 2);
            delete manager;
            throw ast::InternalError(errorMsg);
        }
        if (_iRetCount > 1)
        {
            sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d expected.\n"), manager->getSolverName().c_str(), 1);
            delete manager;
            throw ast::InternalError(errorMsg);
        }
        types::InternalType *pI;
        types::MList *pObj = in[0]->getAs<types::MList>();
        if (pObj->extract(L"manager", pI) && pI->isPointer())
        {
            OdeManager *p = static_cast<OdeManager *>(pI->getAs<types::Pointer>()->get());
            if (p->getSolverName() != manager->getSolverName())
            {
                sprintf(errorMsg, _("%s: wrong solver \"%s\" in solution to be extended.\n"), manager->getSolverName().c_str(), p->getSolverName().c_str());
                delete manager;
                throw ast::InternalError(errorMsg);
            }
            manager->setPreviousManager(pI->getAs<types::Pointer>()->get());
            bIsExtension = true;
        }
        else
        {
            sprintf(errorMsg, _("%s: missing manager field in argument 1.\n"), manager->getSolverName().c_str());
            delete manager;
            throw ast::InternalError(errorMsg);
        }
    }
    else if (in.size() < manager->getMinNargin() || in.size() > manager->getMaxNargin())
    {
        if (manager->getMaxNargin() == manager->getMinNargin())
        {
            sprintf(errorMsg, _("%s: Wrong number of input argument(s): %d expected.\n"), manager->getSolverName().c_str(), manager->getMaxNargin());
        }
        else
        {
            sprintf(errorMsg, _("%s: Wrong number of input argument(s): %d or %d expected.\n"), manager->getSolverName().c_str(), manager->getMinNargin(), manager->getMaxNargin());
        }
        delete manager;
        throw ast::InternalError(errorMsg);
    }

    try
    {
        manager->setIretCount(_iRetCount);
        manager->parseMatrices(in);
        if (bIsExtension == false)
        {
            manager->parseFunction(in[iStart], iStart);
            try
            {
                // some steppers require a secondary rhs
                manager->parseFunction(in[iStart+1], OdeManager::SRHS, iStart+1);
            }
            catch (ast::InternalError& ie)
            {
                manager->setLastErrorMessage(ie.GetErrorMessage());
            }
        }
        manager->parseOptions(opt);
        manager->init();
        manager->solve();
    }
    catch (ast::InternalError& ie)
    {
        delete manager;
        return types::Function::Error;
    }
    catch (ast::InternalAbort& ia)
    {
        delete manager;
        throw (ia);
    }

    if (_iRetCount > 1)
    {
        out.push_back(manager->getTOut());
        out.push_back(manager->getYOut());
        if (_iRetCount > 2)
        {
            manager->createSolutionOutput(out);
        }
        delete manager;        
    }
    else if (_iRetCount == 1) 
    {
        manager->createSolutionOutput(out);
    }
    else // _iRetCount == 0
    {
        delete manager;
    }
    return types::Function::OK;
}
