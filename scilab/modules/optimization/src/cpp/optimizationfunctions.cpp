/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
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
#include "string.hxx"
#include "double.hxx"
#include "optimizationfunctions.hxx"
#include "configvariable.hxx"
#include "commentexp.hxx"

extern "C"
{
#include "elem_common.h"
#include "scioptimfunctions.h"
#include "localization.h"
}

/*
** optimization functions
** \{
*/

OptimizationFunctions* Optimization::m_OptimizationFunctions;

using namespace types;
void Optimization::addOptimizationFunctions(OptimizationFunctions* _opFunction)
{
    m_OptimizationFunctions = _opFunction;
}

void Optimization::removeOptimizationFunctions()
{
    m_OptimizationFunctions = NULL;
}

OptimizationFunctions* Optimization::getOptimizationFunctions()
{
    return m_OptimizationFunctions;
}

/*
** \}
*/


/*--------------------------------------------------------------------------*/
OptimizationFunctions::OptimizationFunctions(const std::wstring& callerName)
{
    m_iXRows = 0;
    m_iXCols = 0;

    m_wstrCaller = callerName;

    // optim
    m_pCallOptimCostfFunction           = NULL;
    m_pStringOptimCostfFunctionDyn      = NULL;
    m_pStringOptimCostfFunctionStatic   = NULL;

    // fsolve
    m_pCallFsolveFctFunction            = NULL;
    m_pStringFsolveFctFunctionDyn       = NULL;
    m_pStringFsolveFctFunctionStatic    = NULL;

    m_pCallFsolveJacFunction            = NULL;
    m_pStringFsolveJacFunctionDyn       = NULL;
    m_pStringFsolveJacFunctionStatic    = NULL;

    // init static functions
    if (callerName == L"optim")
    {
        m_staticFunctionMap[L"genros"]  = (void*) C2F(genros);
        m_staticFunctionMap[L"topt2"]   = (void*) C2F(topt2);
        m_staticFunctionMap[L"icsemc"]  = (void*) C2F(icsemc);
        m_staticFunctionMap[L"mcsec"]   = (void*) C2F(mcsec);
    }
    else if (callerName == L"fsolve")
    {
        m_staticFunctionMap[L"fsol1"]   = (void*) C2F(fsol1);
        m_staticFunctionMap[L"fsolj1"]  = (void*) C2F(fsolj1);
    }
    else if (callerName == L"lsqrsolve")
    {
        m_staticFunctionMap[L"lsqrsol1"]   = (void*) C2F(lsqrsol1);
        m_staticFunctionMap[L"lsqrsolj1"]  = (void*) C2F(lsqrsolj1);
    }
}

OptimizationFunctions::~OptimizationFunctions()
{
    m_staticFunctionMap.clear();
}

/*------------------------------- public -------------------------------------------*/
// optim
void OptimizationFunctions::execCostf(int *ind, int *n, double *x, double *f, double *g, int *ti, float *tr, double *td)
{
    char errorMsg[256];
    if (m_pCallOptimCostfFunction)
    {
        callCostfMacro(ind, n, x, f, g, ti, tr, td);
    }
    else if (m_pStringOptimCostfFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringOptimCostfFunctionDyn->get(0));
        if (func == NULL)
        {
            sprintf(errorMsg, _("Undefined function '%ls'.\n"), m_pStringOptimCostfFunctionDyn->get(0));
            throw ast::InternalError(errorMsg);
        }
        ((costf_t)(func->functionPtr))(ind, n, x, f, g, ti, tr, td);
    }
    else if (m_pStringOptimCostfFunctionStatic)
    {
        ((costf_t)m_staticFunctionMap[m_pStringOptimCostfFunctionStatic->get(0)])(ind, n, x, f, g, ti, tr, td);
    }
    else
    {
        sprintf(errorMsg, _("User function '%s' have not been setted.\n"), "costf");
        throw ast::InternalError(errorMsg);
    }
}

// fsolve
void OptimizationFunctions::execFsolveFct(int* n, double* x, double* v, int* iflag)
{
    char errorMsg[256];
    if (m_pCallFsolveFctFunction)
    {
        callFsolveFctMacro(n, x, v, iflag);
    }
    else if (m_pStringFsolveFctFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringFsolveFctFunctionDyn->get(0));
        if (func == NULL)
        {
            sprintf(errorMsg, _("Undefined function '%ls'.\n"), m_pStringFsolveFctFunctionDyn->get(0));
            throw ast::InternalError(errorMsg);
        }
        ((fct_t)(func->functionPtr))(n, x, v, iflag);
    }
    else if (m_pStringFsolveFctFunctionStatic)
    {
        ((fct_t)m_staticFunctionMap[m_pStringFsolveFctFunctionStatic->get(0)])(n, x, v, iflag);
    }
    else
    {
        sprintf(errorMsg, _("User function '%s' have not been setted.\n"), "costf");
        throw ast::InternalError(errorMsg);
    }
}
void OptimizationFunctions::execFsolveJac(int* n, double* x, double* v, double* jac, int* ldjac, int* iflag)
{
    char errorMsg[256];
    if (m_pCallFsolveJacFunction)
    {
        callFsolveJacMacro(n, x, v, jac, ldjac, iflag);
    }
    else if (m_pStringFsolveJacFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringFsolveJacFunctionDyn->get(0));
        if (func == NULL)
        {
            sprintf(errorMsg, _("Undefined function '%ls'.\n"), m_pStringFsolveJacFunctionDyn->get(0));
            throw ast::InternalError(errorMsg);
        }
        // c or fortran jac fuction are the same proto as fct
        ((fct_t)(func->functionPtr))(n, x, jac, iflag);
    }
    else if (m_pStringFsolveJacFunctionStatic)
    {
        // c or fortran jac fuction are the same proto as fct
        ((fct_t)m_staticFunctionMap[m_pStringFsolveJacFunctionStatic->get(0)])(n, x, jac, iflag);
    }
    else
    {
        sprintf(errorMsg, _("User function '%s' have not been setted.\n"), "costf");
        throw ast::InternalError(errorMsg);
    }
}

// lsqrsolve
void OptimizationFunctions::execLsqrsolveFct(int* m, int* n, double* x, double* v, int* iflag)
{
    char errorMsg[256];
    if (m_pCallFsolveFctFunction)
    {
        callLsqrsolveFctMacro(m, n, x, v, iflag);
    }
    else if (m_pStringFsolveFctFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringFsolveFctFunctionDyn->get(0));
        if (func == NULL)
        {
            sprintf(errorMsg, _("Undefined function '%ls'.\n"), m_pStringFsolveFctFunctionDyn->get(0));
            throw ast::InternalError(errorMsg);
        }
        ((lsqrfct_t)(func->functionPtr))(m, n, x, v, iflag);
    }
    else if (m_pStringFsolveFctFunctionStatic)
    {
        ((lsqrfct_t)m_staticFunctionMap[m_pStringFsolveFctFunctionStatic->get(0)])(m, n, x, v, iflag);
    }
    else
    {
        sprintf(errorMsg, _("User function '%s' have not been setted.\n"), "costf");
        throw ast::InternalError(errorMsg);
    }
}
void OptimizationFunctions::execLsqrsolveJac(int* m, int* n, double* x, double* v, double* jac, int* ldjac, int* iflag)
{
    char errorMsg[256];
    if (m_pCallFsolveJacFunction)
    {
        callLsqrsolveJacMacro(m, n, x, v, jac, ldjac, iflag);
    }
    else if (m_pStringFsolveJacFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringFsolveJacFunctionDyn->get(0));
        if (func == NULL)
        {
            sprintf(errorMsg, _("Undefined function '%ls'.\n"), m_pStringFsolveJacFunctionDyn->get(0));
            throw ast::InternalError(errorMsg);
        }
        // c or fortran jac fuction are the same proto as fct
        ((lsqrjac_ext_t)(func->functionPtr))(m, n, x, jac, ldjac, iflag);
    }
    else if (m_pStringFsolveJacFunctionStatic)
    {
        // c or fortran jac fuction are the same proto as fct
        ((lsqrjac_ext_t)m_staticFunctionMap[m_pStringFsolveJacFunctionStatic->get(0)])(m, n, x, jac, ldjac, iflag);
    }
    else
    {
        sprintf(errorMsg, _("User function '%s' have not been setted.\n"), "costf");
        throw ast::InternalError(errorMsg);
    }
}

//*** setter ***
void OptimizationFunctions::setXRows(int _iRows)
{
    m_iXRows = _iRows;
}
void OptimizationFunctions::setXCols(int _iCols)
{
    m_iXCols = _iCols;
}

// optim
void OptimizationFunctions::setCostfArgs(types::InternalType* _Args)
{
    m_OptimArgs.push_back(_Args);
}

void OptimizationFunctions::setOptimCostfFunction(types::Callable* _func)
{
    m_pCallOptimCostfFunction = _func;
}

bool OptimizationFunctions::setOptimCostfFunction(types::String* _func)
{
    if (ConfigVariable::getEntryPoint(_func->get(0)))
    {
        m_pStringOptimCostfFunctionDyn = _func;
        return true;
    }
    else
    {
        if (m_staticFunctionMap.find(_func->get(0)) != m_staticFunctionMap.end())
        {
            m_pStringOptimCostfFunctionStatic = _func;
            return true;
        }
        return false;
    }
}

// fsolve
void OptimizationFunctions::setFsolveFctArgs(types::InternalType* _Args)
{
    m_fsolveFctArgs.push_back(_Args);
}

void OptimizationFunctions::setFsolveFctFunction(types::Callable* _func)
{
    m_pCallFsolveFctFunction = _func;
}

bool OptimizationFunctions::setFsolveFctFunction(types::String* _func)
{
    if (ConfigVariable::getEntryPoint(_func->get(0)))
    {
        m_pStringFsolveFctFunctionDyn = _func;
        return true;
    }
    else
    {
        if (m_staticFunctionMap.find(_func->get(0)) != m_staticFunctionMap.end())
        {
            m_pStringFsolveFctFunctionStatic = _func;
            return true;
        }
        return false;
    }
}

void OptimizationFunctions::setFsolveJacArgs(types::InternalType* _Args)
{
    m_fsolveJacArgs.push_back(_Args);
}

void OptimizationFunctions::setFsolveJacFunction(types::Callable* _func)
{
    m_pCallFsolveJacFunction = _func;
}

bool OptimizationFunctions::setFsolveJacFunction(types::String* _func)
{
    if (ConfigVariable::getEntryPoint(_func->get(0)))
    {
        m_pStringFsolveJacFunctionDyn = _func;
        return true;
    }
    else
    {
        if (m_staticFunctionMap.find(_func->get(0)) != m_staticFunctionMap.end())
        {
            m_pStringFsolveJacFunctionStatic = _func;
            return true;
        }
        return false;
    }
}

//*** getter ***
int OptimizationFunctions::getXRows()
{
    return m_iXRows;
}
int OptimizationFunctions::getXCols()
{
    return m_iXCols;
}

/*------------------------------- private -------------------------------------------*/
// optim
void OptimizationFunctions::callCostfMacro(int *ind, int *n, double *x, double *f, double *g, int *ti, float *tr, double *td)
{
    char errorMsg[256];
    int iRetCount   = 3;
    int one         = 1;

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;

    // create input args
    types::Double* pDblX = new types::Double(m_iXRows, m_iXCols);
    C2F(dcopy)(n, x, &one, pDblX->get(), &one);
    pDblX->IncreaseRef();

    types::Double* pDblInd = new types::Double((double)(*ind));
    pDblInd->IncreaseRef();

    // push_back
    in.push_back(pDblX);
    in.push_back(pDblInd);

    for (int i = 0; i < (int)m_OptimArgs.size(); i++)
    {
        m_OptimArgs[i]->IncreaseRef();
        in.push_back(m_OptimArgs[i]);
    }

    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        m_pCallOptimCostfFunction->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalError& ie)
    {
        for (int i = 0; i < (int)m_OptimArgs.size(); i++)
        {
            m_OptimArgs[i]->DecreaseRef();
        }

        throw ie;
    }

    for (int i = 0; i < (int)m_OptimArgs.size(); i++)
    {
        m_OptimArgs[i]->DecreaseRef();
    }

    if (out.size() != iRetCount)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallOptimCostfFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of input argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[0]->IncreaseRef();
    out[1]->IncreaseRef();
    out[2]->IncreaseRef();

    pDblX->DecreaseRef();
    if (pDblX->isDeletable())
    {
        delete pDblX;
    }

    pDblInd->DecreaseRef();
    if (pDblInd->isDeletable())
    {
        delete pDblInd;
    }

    types::Double* pDblOut = NULL;

    // get f
    if (out[0]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallOptimCostfFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real scalar expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    pDblOut = out[0]->getAs<types::Double>();
    if (pDblOut->isComplex() || pDblOut->isScalar() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallOptimCostfFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real scalar expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    *f = pDblOut->get(0);

    out[0]->DecreaseRef();
    if (out[0]->isDeletable())
    {
        delete out[0];
    }

    // get g
    if (out[1]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallOptimCostfFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 2);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    pDblOut = out[1]->getAs<types::Double>();
    if (pDblOut->isComplex())
    {
        char* pstrName = wide_string_to_UTF8(m_pCallOptimCostfFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 2);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    C2F(dcopy)(n, pDblOut->get(), &one, g, &one);

    out[1]->DecreaseRef();
    if (out[1]->isDeletable())
    {
        delete out[1];
    }

    // get ind
    if (out[2]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallOptimCostfFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real scalar expected.\n"), pstrName, 3);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    pDblOut = out[2]->getAs<types::Double>();
    if (pDblOut->isComplex() || pDblOut->isScalar() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallOptimCostfFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real scalar expected.\n"), pstrName, 3);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    *ind = (int)pDblOut->get(0);

    out[2]->DecreaseRef();
    if (out[2]->isDeletable())
    {
        delete out[2];
    }
}

// fsolve
void OptimizationFunctions::callFsolveFctMacro(int *n, double *x, double *v, int *iflag)
{
    char errorMsg[256];
    int iRetCount   = 1;
    int one         = 1;

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;

    // create input args
    types::Double* pDblX = new types::Double(m_iXRows, m_iXCols);
    C2F(dcopy)(n, x, &one, pDblX->get(), &one);
    pDblX->IncreaseRef();

    // push_back
    in.push_back(pDblX);

    for (int i = 0; i < (int)m_fsolveFctArgs.size(); i++)
    {
        m_fsolveFctArgs[i]->IncreaseRef();
        in.push_back(m_fsolveFctArgs[i]);
    }

    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        m_pCallFsolveFctFunction->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalError& ie)
    {
        for (int i = 0; i < (int)m_fsolveFctArgs.size(); i++)
        {
            m_fsolveFctArgs[i]->DecreaseRef();
        }

        throw ie;
    }

    for (int i = 0; i < (int)m_fsolveFctArgs.size(); i++)
    {
        m_fsolveFctArgs[i]->DecreaseRef();
    }

    if (out.size() != iRetCount)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFsolveFctFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of input argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[0]->IncreaseRef();

    pDblX->DecreaseRef();
    if (pDblX->isDeletable())
    {
        delete pDblX;
    }

    types::Double* pDblOut = NULL;

    // get v
    if (out[0]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFsolveFctFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real scalar expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    pDblOut = out[0]->getAs<types::Double>();
    if (pDblOut->getRows() != m_iXRows || pDblOut->getCols() != m_iXCols)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFsolveFctFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong size for output argument #%d: A matrix of size %d x %d expected.\n"), pstrName, 1, m_iXRows, m_iXCols);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    C2F(dcopy)(n, pDblOut->get(), &one, v, &one);

    out[0]->DecreaseRef();
    if (out[0]->isDeletable())
    {
        delete out[0];
    }
}

void OptimizationFunctions::callFsolveJacMacro(int *n, double *x, double *v, double* jac, int* ldjac, int *iflag)
{
    char errorMsg[256];
    int iRetCount   = 1;
    int one         = 1;

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;

    // create input args
    types::Double* pDblX = new types::Double(m_iXRows, m_iXCols);
    C2F(dcopy)(n, x, &one, pDblX->get(), &one);
    pDblX->IncreaseRef();

    // push_back
    in.push_back(pDblX);

    for (int i = 0; i < (int)m_fsolveJacArgs.size(); i++)
    {
        m_fsolveJacArgs[i]->IncreaseRef();
        in.push_back(m_fsolveJacArgs[i]);
    }

    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        m_pCallFsolveJacFunction->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalError& ie)
    {
        for (int i = 0; i < (int)m_fsolveJacArgs.size(); i++)
        {
            m_fsolveJacArgs[i]->DecreaseRef();
        }

        throw ie;
    }

    for (int i = 0; i < (int)m_fsolveJacArgs.size(); i++)
    {
        m_fsolveJacArgs[i]->DecreaseRef();
    }

    if (out.size() != iRetCount)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFsolveJacFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of input argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[0]->IncreaseRef();

    pDblX->DecreaseRef();
    if (pDblX->isDeletable())
    {
        delete pDblX;
    }

    types::Double* pDblOut = NULL;

    // get jac
    if (out[0]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFsolveJacFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real scalar expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    pDblOut = out[0]->getAs<types::Double>();
    if (pDblOut->getRows() != *ldjac || pDblOut->getCols() != *n)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFsolveJacFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong size for output argument #%d: A matrix of size %d x %d expected.\n"), pstrName, 1, *ldjac, *n);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    int iSize = (*ldjac) * (*n);
    C2F(dcopy)(&iSize, pDblOut->get(), &one, jac, &one);

    out[0]->DecreaseRef();
    if (out[0]->isDeletable())
    {
        delete out[0];
    }
}

// lsqrsolve
void OptimizationFunctions::callLsqrsolveFctMacro(int *m, int *n, double *x, double *v, int *iflag)
{
    char errorMsg[256];
    int iRetCount   = 1;
    int one         = 1;

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;

    // create input args
    types::Double* pDblX = new types::Double(m_iXRows, m_iXCols);
    C2F(dcopy)(n, x, &one, pDblX->get(), &one);
    pDblX->IncreaseRef();
    types::Double* pDblM = new types::Double((double)*m);
    pDblM->IncreaseRef();

    // push_back
    in.push_back(pDblX);
    in.push_back(pDblM);

    for (int i = 0; i < (int)m_fsolveFctArgs.size(); i++)
    {
        m_fsolveFctArgs[i]->IncreaseRef();
        in.push_back(m_fsolveFctArgs[i]);
    }

    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        m_pCallFsolveFctFunction->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalError& ie)
    {
        for (int i = 0; i < (int)m_fsolveFctArgs.size(); i++)
        {
            m_fsolveFctArgs[i]->DecreaseRef();
        }

        throw ie;
    }

    for (int i = 0; i < (int)m_fsolveFctArgs.size(); i++)
    {
        m_fsolveFctArgs[i]->DecreaseRef();
    }

    if (out.size() != iRetCount)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFsolveFctFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of input argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[0]->IncreaseRef();

    pDblX->DecreaseRef();
    if (pDblX->isDeletable())
    {
        delete pDblX;
    }

    pDblM->DecreaseRef();
    if (pDblM->isDeletable())
    {
        delete pDblM;
    }

    types::Double* pDblOut = NULL;

    // get v
    if (out[0]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFsolveFctFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real scalar expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    pDblOut = out[0]->getAs<types::Double>();
    if (pDblOut->getSize() != *m)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFsolveFctFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong size for output argument #%d: A vector of %d expected.\n"), pstrName, 1, *m);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    C2F(dcopy)(m, pDblOut->get(), &one, v, &one);

    out[0]->DecreaseRef();
    if (out[0]->isDeletable())
    {
        delete out[0];
    }
}
void OptimizationFunctions::callLsqrsolveJacMacro(int *m, int *n, double *x, double *v, double *jac, int *ldjac, int *iflag)
{
    char errorMsg[256];
    int iRetCount   = 1;
    int one         = 1;

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;

    // create input args
    types::Double* pDblX = new types::Double(m_iXRows, m_iXCols);
    C2F(dcopy)(n, x, &one, pDblX->get(), &one);
    pDblX->IncreaseRef();
    types::Double* pDblM = new types::Double((double)*m);
    pDblM->IncreaseRef();

    // push_back
    in.push_back(pDblX);
    in.push_back(pDblM);

    for (int i = 0; i < (int)m_fsolveJacArgs.size(); i++)
    {
        m_fsolveJacArgs[i]->IncreaseRef();
        in.push_back(m_fsolveJacArgs[i]);
    }

    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        m_pCallFsolveJacFunction->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalError& ie)
    {
        for (int i = 0; i < (int)m_fsolveJacArgs.size(); i++)
        {
            m_fsolveJacArgs[i]->DecreaseRef();
        }

        throw ie;
    }

    for (int i = 0; i < (int)m_fsolveJacArgs.size(); i++)
    {
        m_fsolveJacArgs[i]->DecreaseRef();
    }

    if (out.size() != iRetCount)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFsolveJacFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of input argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[0]->IncreaseRef();

    pDblX->DecreaseRef();
    if (pDblX->isDeletable())
    {
        delete pDblX;
    }

    pDblM->DecreaseRef();
    if (pDblM->isDeletable())
    {
        delete pDblM;
    }

    types::Double* pDblOut = NULL;

    // get jac
    if (out[0]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFsolveJacFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real scalar expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    pDblOut = out[0]->getAs<types::Double>();
    if (pDblOut->getSize() != *m **n)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFsolveJacFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong size for output argument #%d: A vector of %d expected.\n"), pstrName, 1, *m);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    int iSize = *m **n;
    C2F(dcopy)(&iSize, pDblOut->get(), &one, jac, &one);

    out[0]->DecreaseRef();
    if (out[0]->isDeletable())
    {
        delete out[0];
    }
}

