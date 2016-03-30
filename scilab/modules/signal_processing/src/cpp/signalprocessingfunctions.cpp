/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
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
#include "double.hxx"
#include "signalprocessingfunctions.hxx"
#include "configvariable.hxx"
#include "function.hxx"

extern "C"
{
#include "elem_common.h"
#include "Ex-corr.h"
#include "localization.h"
}

/*--------------------------------------------------------------------------*/

Signalprocessingfunctions* Signalprocessing::m_Signalprocessingfunctions;

void Signalprocessing::addSignalprocessingfunctions(Signalprocessingfunctions* _spFunction)
{
    m_Signalprocessingfunctions = _spFunction;
}

void Signalprocessing::removeSignalprocessingfunctions()
{
    m_Signalprocessingfunctions = NULL;
}

Signalprocessingfunctions* Signalprocessing::getSignalprocessingfunctions()
{
    return m_Signalprocessingfunctions;
}

Signalprocessingfunctions::Signalprocessingfunctions(const std::wstring& callerName)
{

    m_wstrCaller = callerName;

    m_pCallDgetx = NULL;
    m_pCallDgety = NULL;

    m_pStringDgetxDyn = NULL;
    m_pStringDgetyDyn = NULL;

    m_pStringDgetxStatic = NULL;
    m_pStringDgetyStatic = NULL;

    // init static functions
    if (callerName == L"corr")
    {
        m_staticFunctionMap[L"corexx"] = (void*)C2F(corexx);
        m_staticFunctionMap[L"corexy"] = (void*)C2F(corexy);

    }
}

void Signalprocessingfunctions::execFunctionDgetx(double* x, int* siz, int* iss)
{
    char errorMsg[256];
    if (m_pCallDgetx)
    {
        callDgetx(x, siz, iss);
    }
    else if (m_pStringDgetxDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringDgetxDyn->get(0));
        if (func == NULL)
        {
            sprintf(errorMsg, _("Undefined function '%ls'.\n"), m_pStringDgetxDyn->get(0));
            throw ast::InternalError(errorMsg);
        }
        ((dgetx_f_t)(func->functionPtr))(x, siz, iss);
    }
    else if (m_pStringDgetxStatic)// function static
    {
        ((dgetx_f_t)m_staticFunctionMap[m_pStringDgetxStatic->get(0)])(x, siz, iss);
    }
    else
    {
        sprintf(errorMsg, _("User function '%s' have not been setted.\n"), "g");
        throw ast::InternalError(errorMsg);
    }
}

void Signalprocessingfunctions::setDgetx(types::Callable* _dgetx)
{
    m_pCallDgetx = _dgetx;
}

void Signalprocessingfunctions::setDgetx(types::String* _dgetx)
{
    m_pStringDgetxStatic = _dgetx;
}
void Signalprocessingfunctions::setDgety(types::Callable* _dgety)
{
    m_pCallDgety = _dgety;
}
void Signalprocessingfunctions::setDgety(types::String* _dgety)
{
    m_pStringDgetyStatic = _dgety;
}

//param for fortran call
void dgetx_f(double* x, int* siz, int* iss)
{
    Signalprocessingfunctions* spFunction = NULL;
    spFunction = Signalprocessing::getSignalprocessingfunctions();

    if (spFunction == NULL)
    {
        throw ast::InternalError(_("An error occurred while getting Signalprocessingfunctions object.\n"));
    }

    spFunction->execFunctionDgetx(x, siz, iss);
}

void Signalprocessingfunctions::execFunctionDgety(double* y, int* siz, int* iss)
{
    char errorMsg[256];
    if (m_pCallDgety)
    {
        callDgety(y, siz, iss);

    }
    else if (m_pStringDgetxDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringDgetyDyn->get(0));
        if (func == NULL)
        {
            sprintf(errorMsg, _("Undefined function '%ls'.\n"), m_pStringDgetyDyn->get(0));
            throw ast::InternalError(errorMsg);
        }
        ((dgety_f_t)(func->functionPtr))(y, siz, iss);
    }
    else if (m_pStringDgetyStatic)// function static
    {
        ((dgety_f_t)m_staticFunctionMap[m_pStringDgetyStatic->get(0)])(y, siz, iss);
    }
    else
    {
        sprintf(errorMsg, _("User function '%s' have not been setted.\n"), "g");
        throw ast::InternalError(errorMsg);
    }
}

//param for fortran call
void dgety_f(double* y, int* siz, int* iss)
{
    Signalprocessingfunctions* spFunction = NULL;
    spFunction = Signalprocessing::getSignalprocessingfunctions();

    if (spFunction == NULL)
    {
        throw ast::InternalError(_("An error occurred while getting Signalprocessingfunctions object.\n"));
    }

    spFunction->execFunctionDgety(y, siz, iss);
}

void Signalprocessingfunctions::callDgety(double* y, int* siz, int* iss)
{
    char errorMsg[256];
    int one         = 1;
    int iRetCount   = 1;

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;

    types::Double* pDblY    = new types::Double(*siz);

    // create input args
    types::Double* pDblT = new types::Double(*iss);
    pDblT->IncreaseRef();
    pDblY->IncreaseRef();

    // push_back
    in.push_back(pDblY);
    in.push_back(pDblT);



    for (int i = 0; i < (int)m_FArgs.size(); i++)
    {
        m_FArgs[i]->IncreaseRef();
        in.push_back(m_FArgs[i]);
    }

    bool bOk = m_pCallDgety->call(in, opt, iRetCount, out) == types::Function::OK;

    for (int i = 0; i < (int)m_FArgs.size(); i++)
    {
        m_FArgs[i]->DecreaseRef();
    }

    if (bOk == false)
    {
        sprintf(errorMsg, _("%ls: error while calling user function.\n"), m_pCallDgety->getName().c_str());
        throw ast::InternalError(errorMsg);
    }

    if (out.size() != iRetCount)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallDgety->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of input argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[0]->IncreaseRef();

    pDblT->DecreaseRef();
    if (pDblT->isDeletable())
    {
        delete pDblT;
    }


    pDblY->DecreaseRef();
    if (pDblY->isDeletable())
    {
        delete pDblY;
    }


    out[0]->DecreaseRef();

    if (out[0]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallDgety->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }
    types::Double* pDblOut = out[0]->getAs<types::Double>();
    if (pDblOut->isComplex())
    {
        char* pstrName = wide_string_to_UTF8(m_pCallDgety->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    C2F(dcopy)(siz, pDblOut->get(), &one, y, &one);


    if (out[0]->isDeletable())
    {
        delete out[0];
    }
}
//call external
void Signalprocessingfunctions::callDgetx(double* x, int* siz, int* iss)
{
    char errorMsg[256];
    int one         = 1;
    int iRetCount   = 1;

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;

    types::Double* pDblX    = new types::Double(*siz);

    // create input args
    types::Double* pDblT = new types::Double(*iss);
    pDblT->IncreaseRef();
    pDblX->IncreaseRef();

    // push_back
    in.push_back(pDblX);
    in.push_back(pDblT);



    for (int i = 0; i < (int)m_FArgs.size(); i++)
    {
        m_FArgs[i]->IncreaseRef();
        in.push_back(m_FArgs[i]);
    }

    bool bOk = m_pCallDgetx->call(in, opt, iRetCount, out) == types::Function::OK;

    for (int i = 0; i < (int)m_FArgs.size(); i++)
    {
        m_FArgs[i]->DecreaseRef();
    }

    if (bOk == false)
    {
        sprintf(errorMsg, _("%ls: error while calling user function.\n"), m_pCallDgetx->getName().c_str());
        throw ast::InternalError(errorMsg);
    }

    if (out.size() != iRetCount)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallDgetx->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of input argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[0]->IncreaseRef();

    pDblT->DecreaseRef();
    if (pDblT->isDeletable())
    {
        delete pDblT;
    }


    pDblX->DecreaseRef();
    if (pDblX->isDeletable())
    {
        delete pDblX;
    }


    out[0]->DecreaseRef();

    if (out[0]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallDgetx->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }
    types::Double* pDblOut = out[0]->getAs<types::Double>();
    if (pDblOut->isComplex())
    {
        char* pstrName = wide_string_to_UTF8(m_pCallDgetx->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    C2F(dcopy)(siz, pDblOut->get(), &one, x, &one);


    if (out[0]->isDeletable())
    {
        delete out[0];
    }
}
