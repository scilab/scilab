/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <sstream>
#include <vector>
#include "function.hxx"
#include "double.hxx"
#include "gatewaystruct.hxx"
#include "lasterror.h"

extern "C"
{
#include "core_math.h"
#include "charEncoding.h"
#include "Scierror.h"
#include "sciprint.h"
#include "localization.h"
#include "sci_path.h"
#include "MALLOC.h"
#include "os_swprintf.h"
#include "lasterror.h"
}

namespace types
{
Function* Function::createFunction(std::wstring _wstName, GW_FUNC _pFunc, std::wstring _wstModule)
{
    return new Function(_wstName, _pFunc, NULL, _wstModule);
}

Function* Function::createFunction(std::wstring _wstName, GW_FUNC_OPT _pFunc, std::wstring _wstModule)
{
    return new OptFunction(_wstName, _pFunc, NULL, _wstModule);
}

Function* Function::createFunction(std::wstring _wstName, OLDGW_FUNC _pFunc, std::wstring _wstModule)
{
    return new WrapFunction(_wstName, _pFunc, NULL, _wstModule);
}

Function* Function::createFunction(std::wstring _wstName, MEXGW_FUNC _pFunc, std::wstring _wstModule)
{
    return new WrapMexFunction(_wstName, _pFunc, NULL, _wstModule);
}

Function* Function::createFunction(std::wstring _wstName, GW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, std::wstring _wstModule)
{
    return new Function(_wstName, _pFunc, _pLoadDeps, _wstModule);
}

Function* Function::createFunction(std::wstring _wstName, OLDGW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, std::wstring _wstModule)
{
    return new WrapFunction(_wstName, _pFunc, _pLoadDeps, _wstModule);
}

Function* Function::createFunction(std::wstring _wstName, MEXGW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, std::wstring _wstModule)
{
    return new WrapMexFunction(_wstName, _pFunc, _pLoadDeps, _wstModule);
}

Function* Function::createFunction(std::wstring _wstFunctionName, std::wstring _wstEntryPointName, std::wstring _wstLibName, FunctionType _iType, LOAD_DEPS _pLoadDeps, std::wstring _wstModule, bool _bCloseLibAfterCall)
{
    return new DynamicFunction(_wstFunctionName, _wstEntryPointName, _wstLibName, _iType, _pLoadDeps, _wstModule, _bCloseLibAfterCall);
}

Function* Function::createFunction(std::wstring _wstFunctionName, std::wstring _wstEntryPointName, std::wstring _wstLibName, FunctionType _iType, std::wstring _wstLoadDepsName, std::wstring _wstModule, bool _bCloseLibAfterCall)
{
    return new DynamicFunction(_wstFunctionName, _wstEntryPointName, _wstLibName, _iType, _wstLoadDepsName, _wstModule, _bCloseLibAfterCall);
}

Function::Function(std::wstring _wstName, GW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, std::wstring _wstModule) : Callable(), m_pFunc(_pFunc), m_pLoadDeps(_pLoadDeps)
{
    setName(_wstName);
    setModule(_wstModule);
}

Function::~Function()
{
    if (isDeletable() == true)
    {
    }
}

Function::ReturnValue Function::call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc)
{
    if (m_pLoadDeps != NULL)
    {
        m_pLoadDeps();
    }
    return this->m_pFunc(in, _iRetCount, out);
}

void Function::whoAmI()
{
    std::cout << "types::Function";
}

bool Function::toString(std::wostringstream& ostr)
{
    // FIXME : Implement me.
    ostr << L"FIXME : Implement Function::toString" << std::endl;

    return true;
}

InternalType* Function::clone()
{
    IncreaseRef();
    return this;
}

OptFunction::OptFunction(std::wstring _wstName, GW_FUNC_OPT _pFunc, LOAD_DEPS _pLoadDeps, std::wstring _wstModule)
{
    m_wstName = _wstName;
    m_pFunc = _pFunc;
    m_pLoadDeps = _pLoadDeps;
    m_wstModule = _wstModule;
}

OptFunction::OptFunction(OptFunction* _pWrapFunction)
{
    m_wstModule  = _pWrapFunction->getModule();
    m_wstName    = _pWrapFunction->getName();
    m_pFunc  = _pWrapFunction->getFunc();
    m_pLoadDeps = _pWrapFunction->getDeps();
}

InternalType* OptFunction::clone()
{
    return new OptFunction(this);
}

Function::ReturnValue OptFunction::call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc)
{
    if (m_pLoadDeps != NULL)
    {
        m_pLoadDeps();
    }
    return this->m_pFunc(in, opt, _iRetCount, out);
}

WrapFunction::WrapFunction(std::wstring _wstName, OLDGW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, std::wstring _wstModule)
{
    m_wstName = _wstName;
    m_pOldFunc = _pFunc;
    m_wstModule = _wstModule;
    m_pLoadDeps = _pLoadDeps;
}

WrapFunction::WrapFunction(WrapFunction* _pWrapFunction)
{
    m_wstModule  = _pWrapFunction->getModule();
    m_wstName    = _pWrapFunction->getName();
    m_pOldFunc  = _pWrapFunction->getFunc();
    m_pLoadDeps = _pWrapFunction->getDeps();
}

InternalType* WrapFunction::clone()
{
    return new WrapFunction(this);
}

Function::ReturnValue WrapFunction::call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc)
{
    if (m_pLoadDeps != NULL)
    {
        m_pLoadDeps();
    }

    ReturnValue retVal = Callable::OK;
    int iRet ;
    GatewayStruct gStr;
    _iRetCount = Max(1, _iRetCount);
    gStr.m_iIn = (int)in.size() + (int)opt.size();
    gStr.m_iOut = _iRetCount;

    //copy input parameter to prevent calling gateway modifies input data
    typed_list inCopy;
    for (int i = 0 ; i < in.size() ; i++)
    {
        inCopy.push_back(in[i]->clone());
    }

    gStr.m_pIn = &inCopy;
    gStr.m_pOpt = &opt;
    typed_list::value_type tmpOut[MAX_OUTPUT_VARIABLE];
    std::fill_n(tmpOut, MAX_OUTPUT_VARIABLE, static_cast<typed_list::value_type>(0));
    gStr.m_pOut = tmpOut;
    gStr.m_piRetCount = &_iRetCount;
    gStr.m_pstName = wide_string_to_UTF8(m_wstName.c_str());
    gStr.m_pVisitor = execFunc;
    // we should use a stack array of the max size to avoid dynamic alloc.
    std::vector<int> outOrder(_iRetCount < 1 ? 1 : _iRetCount, -1);
    gStr.m_pOutOrder = &outOrder[0];

    char* pFunctionName = wide_string_to_UTF8(m_wstName.c_str());

    //call gateway
    iRet = m_pOldFunc(pFunctionName, reinterpret_cast<int*>(&gStr));
    FREE(pFunctionName);
    if (isError())
    {
        retVal = Callable::Error;
        resetError();
    }
    else
    {
        for (std::size_t i(0); i != _iRetCount && outOrder[i] != -1 && outOrder[i] != 0 ; ++i)
        {
            if (outOrder[i] - 1 < in.size())
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
                std::size_t const iPos(outOrder[i] - in.size() - 1);
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
        delete tmpOut[i];// delete 0 is safe cf.5.3.5/2
    }

    //protect outputs
    for (int i = 0 ; i < out.size() ; i++)
    {
        out[i]->IncreaseRef();
    }

    //clean input copy
    for (int i = 0 ; i < in.size() ; i++)
    {
        if (inCopy[i]->isDeletable())
        {
            delete inCopy[i];
        }
    }

    //unprotect outputs
    for (int i = 0 ; i < out.size() ; i++)
    {
        out[i]->DecreaseRef();
    }

    FREE(gStr.m_pstName);
    return retVal;
}

WrapMexFunction::WrapMexFunction(std::wstring _wstName, MEXGW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, std::wstring _wstModule)
{
    m_wstName = _wstName;
    m_pOldFunc = _pFunc;
    m_wstModule = _wstModule;
    m_pLoadDeps = _pLoadDeps;
}

WrapMexFunction::WrapMexFunction(WrapMexFunction* _pWrapFunction)
{
    m_wstModule  = _pWrapFunction->getModule();
    m_wstName    = _pWrapFunction->getName();
    m_pOldFunc  = _pWrapFunction->getFunc();
    m_pLoadDeps = _pWrapFunction->getDeps();
}

InternalType* WrapMexFunction::clone()
{
    return new WrapMexFunction(this);
}

Function::ReturnValue WrapMexFunction::call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc)
{
    if (m_pLoadDeps != NULL)
    {
        m_pLoadDeps();
    }

    ReturnValue retVal = Callable::OK;

    int nlhs = _iRetCount;
    int** plhs = new int*[nlhs];
    memset(plhs, 0x00, sizeof(int*) * nlhs);

    int nrhs = (int)in.size();
    int** prhs = new int*[nrhs];
    for (int i = 0 ; i < nrhs ; i++)
    {
        prhs[i] = (int*)(in[i]);
    }

    m_pOldFunc(nlhs, plhs, nrhs, prhs);

    if (_iRetCount == 1 && plhs[0] == NULL)
    {
        //dont copy empty values, juste return "no value"
        return retVal;
    }

    for (int i = 0 ; i < nlhs ; i++)
    {
        out.push_back((types::InternalType*)plhs[i]);
    }

    return retVal;
}

DynamicFunction::DynamicFunction(std::wstring _wstName, std::wstring _wstEntryPointName, std::wstring _wstLibName, FunctionType _iType, std::wstring _wstLoadDepsName, std::wstring _wstModule, bool _bCloseLibAfterCall)
{
    m_wstName               = _wstName;
    m_wstLibName            = _wstLibName;
    m_wstModule             = _wstModule;
    m_wstEntryPoint         = _wstEntryPointName;
    m_wstLoadDepsName       = _wstLoadDepsName;
    m_pLoadDeps             = NULL;
    m_bCloseLibAfterCall    = _bCloseLibAfterCall;
    m_bLoaded               = false;
    m_iType                 = _iType;
    m_pFunc                 = NULL;
    m_pOldFunc              = NULL;
    m_pMexFunc              = NULL;
    m_hLib                  = NULL;
}

DynamicFunction::DynamicFunction(std::wstring _wstName, std::wstring _wstEntryPointName, std::wstring _wstLibName, FunctionType _iType, LOAD_DEPS _pLoadDeps, std::wstring _wstModule, bool _bCloseLibAfterCall)
{
    m_wstName               = _wstName;
    m_wstLibName            = _wstLibName;
    m_wstModule             = _wstModule;
    m_wstEntryPoint         = _wstEntryPointName;
    m_pLoadDeps             = _pLoadDeps;
    m_wstLoadDepsName       = L"";
    m_bCloseLibAfterCall    = _bCloseLibAfterCall;
    m_bLoaded               = false;
    m_iType                 = _iType;
    m_pFunc                 = NULL;
    m_pOldFunc              = NULL;
    m_pMexFunc              = NULL;
    m_hLib                  = NULL;
}

Function::ReturnValue DynamicFunction::call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc)
{
    ReturnValue ret = OK;
    if (m_bLoaded == false)
    {
        if (Init() != OK)
        {
            return Error;
        }
        /*Create function object*/
        m_bLoaded = true;
    }
    /*call function*/

    if (m_pFunction->call(in, opt, _iRetCount, out, execFunc) != OK)
    {
        return Error;
    }

    /*Close lib is mandatory*/
    if (m_bCloseLibAfterCall)
    {
        Clear();
        m_bLoaded = false;
    }

    return OK;
}

Callable::ReturnValue DynamicFunction::Init()
{
    DynLibFuncPtr pFunc = 0;
    /*Load library*/
    if (m_wstLibName.empty())
    {
        Scierror(999, _("%s: Library name must not be empty\n."), m_wstName.c_str());
        return Error;
    }

    char* pstLibName = wide_string_to_UTF8(m_wstLibName.c_str());
    m_hLib = LoadDynLibrary(pstLibName);
    if (m_hLib == NULL)
    {
        //2nd chance for linux !
#ifndef _MSC_VER
        char* pstError = GetLastDynLibError();

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
        char* pstPathToLib = wide_string_to_UTF8(pwstPathToLib);
        m_hLib = LoadDynLibrary(pstPathToLib);

        FREE(pwstPathToLib);

        if (m_hLib == NULL)
        {
            if (pstError != NULL)
            {
                Scierror(999, _("A error has been detected while loading %s: %s\n"), pstLibName, pstError);
            }
            return Error;
        }
#else
        char* pstError = wide_string_to_UTF8(m_wstLibName.c_str());
        Scierror(999, _("Impossible to load %s library\n"), pstError);
        FREE(pstError);
        return Error;
#endif
    }

    /*Load deps*/
    if (m_wstLoadDepsName.empty() == false && m_pLoadDeps == NULL)
    {
        char* pstLoadDepsName = wide_string_to_UTF8(m_wstLoadDepsName.c_str());
        m_pLoadDeps = (LOAD_DEPS)GetDynLibFuncPtr(m_hLib, pstLoadDepsName);
    }

    /*Load gateway*/
    if (m_wstName != L"")
    {
        char* _pstEntryPoint = wide_string_to_UTF8(m_wstEntryPoint.c_str());
        switch (m_iType)
        {
            case EntryPointCPPOpt :
                m_pOptFunc = (GW_FUNC_OPT)GetDynLibFuncPtr(m_hLib, _pstEntryPoint);
                break;
            case EntryPointCPP :
                m_pFunc = (GW_FUNC)GetDynLibFuncPtr(m_hLib, _pstEntryPoint);
                break;
            case EntryPointC :
                m_pOldFunc = (OLDGW_FUNC)GetDynLibFuncPtr(m_hLib, _pstEntryPoint);
                break;
            case EntryPointMex :
                m_pMexFunc = (MEXGW_FUNC)GetDynLibFuncPtr(m_hLib, _pstEntryPoint);
                break;
        }
    }

    if (m_pFunc == NULL && m_pOldFunc == NULL && m_pMexFunc == NULL && m_pOptFunc == NULL)
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
        case EntryPointC :
            m_pFunction = new WrapFunction(m_wstName, m_pOldFunc, m_pLoadDeps, m_wstModule);
            break;
        case EntryPointMex :
            m_pFunction = new WrapMexFunction(m_wstName, m_pMexFunc, m_pLoadDeps, m_wstModule);
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
    if (m_hLib)
    {
        FreeDynLibrary(m_hLib);
        m_hLib = NULL;
    }

    m_pFunc     = NULL;
    m_pOldFunc  = NULL;
    m_pMexFunc  = NULL;
}

}
