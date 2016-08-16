/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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

#include <algorithm>
#include <sstream>
#include <vector>
#include "function.hxx"
#include "double.hxx"
#include "gatewaystruct.hxx"
#include "configvariable.hxx"
#include "scilabWrite.hxx"

extern "C"
{
#include "core_math.h"
#include "charEncoding.h"
#include "Scierror.h"
#include "sciprint.h"
#include "localization.h"
#include "sci_path.h"
#include "sci_malloc.h"
#include "os_string.h"
#include "lasterror.h"
#include "dynamic_module.h"
}

namespace types
{
Function* Function::createFunction(const std::wstring& _wstName, GW_FUNC _pFunc, const std::wstring& _wstModule)
{
    return new Function(_wstName, _pFunc, NULL, _wstModule);
}

Function* Function::createFunction(const std::wstring& _wstName, GW_FUNC_OPT _pFunc, const std::wstring& _wstModule)
{
    return new OptFunction(_wstName, _pFunc, NULL, _wstModule);
}

Function* Function::createFunction(const std::wstring& _wstName, OLDGW_FUNC _pFunc, const std::wstring& _wstModule)
{
    return new WrapFunction(_wstName, _pFunc, NULL, _wstModule);
}

Function* Function::createFunction(const std::wstring& _wstName, MEXGW_FUNC _pFunc, const std::wstring& _wstModule)
{
    return new WrapMexFunction(_wstName, _pFunc, NULL, _wstModule);
}

Function* Function::createFunction(const std::wstring& _wstName, GW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::wstring& _wstModule)
{
    return new Function(_wstName, _pFunc, _pLoadDeps, _wstModule);
}

Function* Function::createFunction(const std::wstring& _wstName, GW_FUNC_OPT _pFunc, LOAD_DEPS _pLoadDeps, const std::wstring& _wstModule)
{
    return new OptFunction(_wstName, _pFunc, _pLoadDeps, _wstModule);
}

Function* Function::createFunction(const std::wstring& _wstName, OLDGW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::wstring& _wstModule)
{
    return new WrapFunction(_wstName, _pFunc, _pLoadDeps, _wstModule);
}

Function* Function::createFunction(const std::wstring& _wstName, MEXGW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::wstring& _wstModule)
{
    return new WrapMexFunction(_wstName, _pFunc, _pLoadDeps, _wstModule);
}

Function* Function::createFunction(const std::wstring& _wstFunctionName, const std::wstring& _wstEntryPointName, const std::wstring& _wstLibName, FunctionType _iType, LOAD_DEPS _pLoadDeps, const std::wstring& _wstModule, bool _bCloseLibAfterCall)
{
    return new DynamicFunction(_wstFunctionName, _wstEntryPointName, _wstLibName, _iType, _pLoadDeps, _wstModule, _bCloseLibAfterCall);
}

Function* Function::createFunction(const std::wstring& _wstFunctionName, const std::wstring& _wstEntryPointName, const std::wstring& _wstLibName, FunctionType _iType, const std::wstring& _wstLoadDepsName, const std::wstring& _wstModule, bool _bCloseLibAfterCall)
{
    return new DynamicFunction(_wstFunctionName, _wstEntryPointName, _wstLibName, _iType, _wstLoadDepsName, _wstModule, _bCloseLibAfterCall);
}

Function* Function::createFunction(const std::wstring& _wstName, GW_C_FUNC _pFunc, const std::wstring& _wstModule)
{
    return new WrapCFunction(_wstName, _pFunc, NULL, _wstModule);
}

Function* Function::createFunction(const std::wstring& _wstName, GW_C_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::wstring& _wstModule)
{
    return new WrapCFunction(_wstName, _pFunc, _pLoadDeps, _wstModule);
}


Function::Function(const std::wstring& _wstName, GW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::wstring& _wstModule) : Callable(), m_pFunc(_pFunc), m_pLoadDeps(_pLoadDeps)
{
    setName(_wstName);
    char* s = wide_string_to_UTF8(m_wstName.data());
    m_stName = s;
    FREE(s);

    setModule(_wstModule);
}

Function::~Function()
{

}


Function::ReturnValue Function::call(typed_list &in, optional_list &/*opt*/, int _iRetCount, typed_list &out)
{
    int ret = 1;
    if (m_pLoadDeps != NULL)
    {
        ret = m_pLoadDeps(m_wstName);
    }

    if (ret == 0)
    {
        return Error;
    }

    return this->m_pFunc(in, _iRetCount, out);
}

void Function::whoAmI()
{
    std::cout << "types::Function";
}

bool Function::toString(std::wostringstream& ostr)
{
    // display nothing. ie : c = cos
    return true;
}

Function* Function::clone()
{
    IncreaseRef();
    return this;
}

bool Function::operator==(const InternalType& it)
{
    if (!const_cast<InternalType &>(it).isFunction())
    {
        return false;
    }
    return this->getFunc() == const_cast<InternalType &>(it).getAs<Function>()->getFunc();
}

OptFunction::OptFunction(const std::wstring& _wstName, GW_FUNC_OPT _pFunc, LOAD_DEPS _pLoadDeps, const std::wstring& _wstModule)
{
    m_wstName = _wstName;
    char* s = wide_string_to_UTF8(m_wstName.data());
    m_stName = s;
    FREE(s);
    m_pFunc = _pFunc;
    m_pLoadDeps = _pLoadDeps;
    m_wstModule = _wstModule;
}

OptFunction::OptFunction(OptFunction* _pWrapFunction)
{
    m_wstModule  = _pWrapFunction->getModule();
    m_wstName    = _pWrapFunction->getName();
    char* s = wide_string_to_UTF8(m_wstName.data());
    m_stName = s;
    FREE(s);
    m_pFunc = _pWrapFunction->getFunc();
    m_pLoadDeps = _pWrapFunction->getDeps();
}

OptFunction* OptFunction::clone()
{
    return new OptFunction(this);
}

Function::ReturnValue OptFunction::call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out)
{
    int ret = 1;
    if (m_pLoadDeps != NULL)
    {
        ret = m_pLoadDeps(m_wstName);
    }

    if (ret == 0)
    {
        return Error;
    }

    return this->m_pFunc(in, opt, _iRetCount, out);
}

WrapFunction::WrapFunction(const std::wstring& _wstName, OLDGW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::wstring& _wstModule)
{
    m_wstName = _wstName;
    m_pOldFunc = _pFunc;
    m_wstModule = _wstModule;
    char* s = wide_string_to_UTF8(m_wstName.data());
    m_stName = s;
    FREE(s);
    m_pLoadDeps = _pLoadDeps;
}

WrapFunction::WrapFunction(WrapFunction* _pWrapFunction)
{
    m_wstModule = _pWrapFunction->getModule();
    m_wstName = _pWrapFunction->getName();
    char* s = wide_string_to_UTF8(m_wstName.data());
    m_stName = s;
    FREE(s);
    m_pOldFunc  = _pWrapFunction->getFunc();
    m_pLoadDeps = _pWrapFunction->getDeps();
}

WrapFunction* WrapFunction::clone()
{
    return new WrapFunction(this);
}

Function::ReturnValue WrapFunction::call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out)
{
    int ret = 1;
    int inSize = (int)in.size();
    int optSize = (int)opt.size();
    bool isRef = checkReferenceModule(m_wstModule.c_str());

    if (m_pLoadDeps != NULL)
    {
        ret = m_pLoadDeps(m_wstName);
    }

    if (ret == 0)
    {
        return Error;
    }

    ReturnValue retVal = Callable::OK;
    GatewayStruct gStr;
    _iRetCount = std::max(1, _iRetCount);
    gStr.m_iIn = inSize + optSize;
    gStr.m_iOut = _iRetCount;

    //copy input parameter to prevent calling gateway modifies input data
    typed_list inCopy;

    if (isRef == false)
    {
        for (int i = 0; i < inSize; i++)
        {
            inCopy.push_back(in[i]->clone());
        }
    }
    else
    {
        for (int i = 0; i < inSize; i++)
        {
            inCopy.push_back(in[i]);
        }
    }
    gStr.m_pIn = &inCopy;
    gStr.m_pOpt = &opt;
    typed_list::value_type tmpOut[MAX_OUTPUT_VARIABLE];
    std::fill_n(tmpOut, MAX_OUTPUT_VARIABLE, static_cast<typed_list::value_type>(0));
    gStr.m_pOut = tmpOut;
    gStr.m_piRetCount = &_iRetCount;
    gStr.m_pstName = m_stName.data();
    // we should use a stack array of the max size to avoid dynamic alloc.
    std::vector<int> outOrder(_iRetCount < 1 ? 1 : _iRetCount, -1);
    gStr.m_pOutOrder = outOrder.data();

    //call gateway
    m_pOldFunc(const_cast<char*>(m_stName.data()), reinterpret_cast<int*>(&gStr));
    if (ConfigVariable::isError())
    {
        retVal = Callable::Error;
        ConfigVariable::resetError();
    }
    else
    {
        for (std::size_t i(0); i != (size_t)_iRetCount && outOrder[i] != -1 && outOrder[i] != 0; ++i)
        {
            if (outOrder[i] - 1 < gStr.m_iIn)
            {
                std::size_t const iPos(outOrder[i] - 1);
                //protect variable to deletion
                inCopy[iPos]->IncreaseRef();
                if (inCopy[iPos]->isDouble() && ((types::Double*)inCopy[iPos])->isViewAsInteger())
                {
                    types::Double* pD = inCopy[iPos]->getAs<types::Double>();
                    pD->convertFromInteger();
                }

                if (inCopy[iPos]->isDouble() && ((types::Double*)inCopy[iPos])->isViewAsZComplex())
                {
                    types::Double* pD = inCopy[iPos]->getAs<types::Double>();
                    pD->convertFromZComplex();
                }

                out.push_back(inCopy[iPos]);
            }
            else
            {
                std::size_t const iPos(outOrder[i] - gStr.m_iIn - 1);
                if (tmpOut[iPos]->isDouble() && ((types::Double*)tmpOut[iPos])->isViewAsInteger())
                {
                    types::Double* pD = tmpOut[iPos]->getAs<types::Double>();
                    pD->convertFromInteger();
                }

                if (tmpOut[iPos]->isDouble() && ((types::Double*)tmpOut[iPos])->isViewAsZComplex())
                {
                    types::Double* pD = tmpOut[iPos]->getAs<types::Double>();
                    pD->convertFromZComplex();
                }

                out.push_back(tmpOut[iPos]);
                tmpOut[iPos] = 0;
            }
        }
    }

    for (std::size_t i(0); i != MAX_OUTPUT_VARIABLE; ++i)
    {
        if (tmpOut[i])
        {
            tmpOut[i]->killMe();
        }
    }

    //clean input copy
    if (isRef == false)
    {
        //protect outputs
        int size = (int)out.size();
        for (int i = 0; i < size; i++)
        {
            out[i]->IncreaseRef();
        }

        for (int i = 0; i < inSize; i++)
        {
            inCopy[i]->killMe();
        }

        //unprotect outputs
        for (int i = 0; i < size; i++)
        {
            out[i]->DecreaseRef();
        }
    }

    return retVal;
}

WrapMexFunction::WrapMexFunction(const std::wstring& _wstName, MEXGW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::wstring& _wstModule)
{
    m_wstName = _wstName;
    char* s = wide_string_to_UTF8(m_wstName.data());
    m_stName = s;
    FREE(s);
    m_pOldFunc = _pFunc;
    m_wstModule = _wstModule;
    m_pLoadDeps = _pLoadDeps;
}

WrapMexFunction::WrapMexFunction(WrapMexFunction* _pWrapFunction)
{
    m_wstModule = _pWrapFunction->getModule();
    char* s = wide_string_to_UTF8(m_wstName.data());
    m_stName = s;
    FREE(s);
    m_wstName = _pWrapFunction->getName();
    m_pOldFunc = _pWrapFunction->getFunc();
    m_pLoadDeps = _pWrapFunction->getDeps();
}

WrapMexFunction* WrapMexFunction::clone()
{
    return new WrapMexFunction(this);
}

Function::ReturnValue WrapMexFunction::call(typed_list &in, optional_list &/*opt*/, int _iRetCount, typed_list &out)
{
    int ret = 1;
    if (m_pLoadDeps != NULL)
    {
        ret = m_pLoadDeps(m_wstName);
    }

    if (ret == 0)
    {
        return Error;
    }

    ReturnValue retVal = Callable::OK;

    char* name = wide_string_to_UTF8(getName().c_str());
    ConfigVariable::setMexFunctionName(name);
    FREE(name);

    int nlhs = _iRetCount;
    int** plhs = new int*[nlhs];
    memset(plhs, 0x00, sizeof(int*) * nlhs);

    int nrhs = (int)in.size();
    int** prhs = new int*[nrhs];
    for (int i = 0; i < nrhs; i++)
    {
        prhs[i] = (int*)(in[i]);
    }

    try
    {
        m_pOldFunc(nlhs, plhs, nrhs, prhs);
    }
    catch (const ast::InternalError& ie)
    {
        delete[] plhs;
        delete[] prhs;
        throw ie;
    }

    if (_iRetCount == 1 && plhs[0] == NULL)
    {
        //dont copy empty values, juste return "no value"
        return retVal;
    }

    for (int i = 0; i < nlhs; i++)
    {
        out.push_back((types::InternalType*)plhs[i]);
    }

    delete[] plhs;
    delete[] prhs;
    return retVal;
}

WrapCFunction::WrapCFunction(const std::wstring& _wstName, GW_C_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::wstring& _wstModule)
{
    m_wstName = _wstName;
    char* s = wide_string_to_UTF8(m_wstName.data());
    m_stName = s;
    FREE(s);
    m_pCFunc = _pFunc;
    m_wstModule = _wstModule;
    m_pLoadDeps = _pLoadDeps;
}

WrapCFunction::WrapCFunction(WrapCFunction* _pWrapFunction)
{
    m_wstModule = _pWrapFunction->getModule();
    char* s = wide_string_to_UTF8(m_wstName.data());
    m_stName = s;
    FREE(s);
    m_wstName = _pWrapFunction->getName();
    m_pCFunc = _pWrapFunction->getFunc();
    m_pLoadDeps = _pWrapFunction->getDeps();
}

WrapCFunction* WrapCFunction::clone()
{
    return new WrapCFunction(this);
}

Function::ReturnValue WrapCFunction::call(typed_list& in, optional_list& opt, int _iRetCount, typed_list& out)
{
    if (m_pLoadDeps != NULL)
    {
        if (m_pLoadDeps(m_wstName) == 0)
        {
            return Error;
        }
    }

    ReturnValue retVal = OK;

    try
    {
        GatewayCStruct gcs;
        gcs.name = m_stName;
        out.resize(_iRetCount, NULL);
        if (m_pCFunc((void*)&gcs, (int)in.size(), (scilabVar*)(in.data()), (int)opt.size(), (scilabOpt)&opt, _iRetCount, (scilabVar*)(out.data())))
        {
            retVal = Error;
        }
    }
    catch (const ast::InternalError& ie)
    {
        throw ie;
    }

    if (retVal == OK)
    {
        if (_iRetCount == 1 && out[0] == NULL)
        {
            //dont copy empty values, juste return "no value"
            out.clear();
            return retVal;
        }
    }

    return retVal;
}

DynamicFunction::DynamicFunction(const std::wstring& _wstName, const std::wstring& _wstEntryPointName, const std::wstring& _wstLibName, FunctionType _iType, const std::wstring& _wstLoadDepsName, const std::wstring& _wstModule, bool _bCloseLibAfterCall)
{
    m_wstName               = _wstName;
    char* s = wide_string_to_UTF8(m_wstName.data());
    m_stName = s;
    FREE(s);
    m_wstLibName = _wstLibName;
    m_wstModule             = _wstModule;
    m_wstEntryPoint         = _wstEntryPointName;
    m_wstLoadDepsName       = _wstLoadDepsName;
    m_pLoadDeps             = NULL;
    m_bCloseLibAfterCall    = _bCloseLibAfterCall;
    m_iType                 = _iType;
    m_pFunc                 = NULL;
    m_pOptFunc              = NULL;
    m_pOldFunc              = NULL;
    m_pMexFunc              = NULL;
    m_pFunction             = NULL;
    m_pCFunc                = NULL;
}

DynamicFunction::DynamicFunction(const std::wstring& _wstName, const std::wstring& _wstEntryPointName, const std::wstring& _wstLibName, FunctionType _iType, LOAD_DEPS _pLoadDeps, const std::wstring& _wstModule, bool _bCloseLibAfterCall)
{
    m_wstName               = _wstName;
    char* s = wide_string_to_UTF8(m_wstName.data());
    m_stName = s;
    FREE(s);
    m_wstLibName = _wstLibName;
    m_wstModule             = _wstModule;
    m_wstEntryPoint         = _wstEntryPointName;
    m_pLoadDeps             = _pLoadDeps;
    m_wstLoadDepsName       = L"";
    m_bCloseLibAfterCall    = _bCloseLibAfterCall;
    m_iType                 = _iType;
    m_pFunc                 = NULL;
    m_pOptFunc              = NULL;
    m_pOldFunc              = NULL;
    m_pMexFunc              = NULL;
    m_pFunction             = NULL;
    m_pCFunc                = NULL;
}

Function::ReturnValue DynamicFunction::call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out)
{
    if (m_pFunction == NULL)
    {
        if (Init() != OK)
        {
            return Error;
        }
    }

    /*call function*/
    if (m_pFunction->call(in, opt, _iRetCount, out) != OK)
    {
        return Error;
    }

    /*Close lib is mandatory*/
    if (m_bCloseLibAfterCall)
    {
        Clear();
    }

    return OK;
}

DynamicFunction::~DynamicFunction()
{
    if (m_pFunction)
    {
        delete m_pFunction;
    }
}

Callable::ReturnValue DynamicFunction::Init()
{
    /*Load library*/
    if (m_wstLibName.empty())
    {
        Scierror(999, _("%s: Library name must not be empty\n."), m_wstName.c_str());
        return Error;
    }

    DynLibHandle hLib = getDynModule(m_wstLibName.c_str());
    if (hLib == 0)
    {
        char* pstLibName = wide_string_to_UTF8(m_wstLibName.c_str());
        hLib = LoadDynLibrary(pstLibName);

        if (hLib == 0)
        {
            //2nd chance for linux !
#ifndef _MSC_VER
            char* pstError = strdup(GetLastDynLibError());

            /* Haven't been able to find the lib with dlopen...
            * This can happen for two reasons:
            * - the lib must be dynamically linked
            * - Some silly issues under Suse (see bug #2875)
            * Note that we are handling only the "source tree build"
            * because libraries are split (they are in the same directory
            * in the binary)
            */
            wchar_t* pwstScilabPath = getSCIW();
            wchar_t pwstModulesPath[] = L"/modules/";
            wchar_t pwstLTDir[] =  L".libs/";

            /* Build the full path to the library */
            int iPathToLibLen = (wcslen(pwstScilabPath) + wcslen(pwstModulesPath) + wcslen(m_wstModule.c_str()) + wcslen(L"/") + wcslen(pwstLTDir) + wcslen(m_wstLibName.c_str()) + 1);
            wchar_t* pwstPathToLib = (wchar_t*)MALLOC(iPathToLibLen * sizeof(wchar_t));
            os_swprintf(pwstPathToLib, iPathToLibLen, L"%ls%ls%ls/%ls%ls", pwstScilabPath, pwstModulesPath, m_wstModule.c_str(), pwstLTDir, m_wstLibName.c_str());
            FREE(pwstScilabPath);
            char* pstPathToLib = wide_string_to_UTF8(pwstPathToLib);
            FREE(pwstPathToLib);
            hLib = LoadDynLibrary(pstPathToLib);

            if (hLib == 0)
            {
                Scierror(999, _("An error has been detected while loading %s: %s\n"), pstLibName, pstError);
                FREE(pstError);

                pstError = GetLastDynLibError();
                Scierror(999, _("An error has been detected while loading %s: %s\n"), pstPathToLib, pstError);

                FREE(pstLibName);
                FREE(pstPathToLib);
                return Error;
            }
            FREE(pstPathToLib);
            FREE(pstError);
#else
            char* pstError = wide_string_to_UTF8(m_wstLibName.c_str());
            Scierror(999, _("Impossible to load %s library\n"), pstError);
            FREE(pstError);
            FREE(pstLibName);
            return Error;
#endif
        }
        FREE(pstLibName);
        addDynModule(m_wstLibName.c_str(), hLib);

        /*Load deps*/
        if (m_wstLoadDepsName.empty() == false && m_pLoadDeps == NULL)
        {
            char* pstLoadDepsName = wide_string_to_UTF8(m_wstLoadDepsName.c_str());
            m_pLoadDeps = (LOAD_DEPS)GetDynLibFuncPtr(hLib, pstLoadDepsName);
            FREE(pstLoadDepsName);
        }

    }

    /*Load gateway*/
    if (m_wstName != L"")
    {
        char* pstEntryPoint = wide_string_to_UTF8(m_wstEntryPoint.c_str());
        switch (m_iType)
        {
            case EntryPointCPPOpt :
                m_pOptFunc = (GW_FUNC_OPT)GetDynLibFuncPtr(hLib, pstEntryPoint);
                break;
            case EntryPointCPP :
                m_pFunc = (GW_FUNC)GetDynLibFuncPtr(hLib, pstEntryPoint);
                break;
            case EntryPointOldC :
                m_pOldFunc = (OLDGW_FUNC)GetDynLibFuncPtr(hLib, pstEntryPoint);
                break;
            case EntryPointMex:
                m_pMexFunc = (MEXGW_FUNC)GetDynLibFuncPtr(hLib, pstEntryPoint);
                break;
            case EntryPointC:
                m_pCFunc = (GW_C_FUNC)GetDynLibFuncPtr(hLib, pstEntryPoint);
                break;
        }

        FREE(pstEntryPoint);
    }

    if (m_pFunc == NULL && m_pOldFunc == NULL && m_pMexFunc == NULL && m_pOptFunc == NULL && m_pCFunc == NULL)
    {
        char* pstEntry = wide_string_to_UTF8(m_wstEntryPoint.c_str());
        char* pstLib = wide_string_to_UTF8(m_wstLibName.c_str());
        Scierror(999, _("Impossible to load %s function in %s library: %s\n"), pstEntry, pstLib, GetLastDynLibError());
        FREE(pstEntry);
        FREE(pstLib);
        return Error;
    }

    switch (m_iType)
    {
        case EntryPointCPPOpt :
            m_pFunction = new OptFunction(m_wstName, m_pOptFunc, m_pLoadDeps, m_wstModule);
            break;
        case EntryPointCPP :
            m_pFunction = new Function(m_wstName, m_pFunc, m_pLoadDeps, m_wstModule);
            break;
        case EntryPointOldC :
            m_pFunction = new WrapFunction(m_wstName, m_pOldFunc, m_pLoadDeps, m_wstModule);
            break;
        case EntryPointMex:
            m_pFunction = new WrapMexFunction(m_wstName, m_pMexFunc, m_pLoadDeps, m_wstModule);
            break;
        case EntryPointC:
            m_pFunction = new WrapCFunction(m_wstName, m_pCFunc, m_pLoadDeps, m_wstModule);
            break;
    }

    if (m_pFunction == NULL)
    {
        return Error;
    }
    return OK;
}

void DynamicFunction::Clear()
{
    m_pFunc     = NULL;
    m_pOldFunc  = NULL;
    m_pMexFunc  = NULL;
}

}
