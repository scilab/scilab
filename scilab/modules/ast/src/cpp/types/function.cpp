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
Function* Function::createFunction(const std::string& _stName, GW_FUNC _pFunc, const std::string& _stModule)
{
    return new Function(_stName, _pFunc, NULL, _stModule);
}

Function* Function::createFunction(const std::string& _stName, GW_FUNC_OPT _pFunc, const std::string& _stModule)
{
    return new OptFunction(_stName, _pFunc, NULL, _stModule);
}

Function* Function::createFunction(const std::string& _stName, OLDGW_FUNC _pFunc, const std::string& _stModule)
{
    return new WrapFunction(_stName, _pFunc, NULL, _stModule);
}

Function* Function::createFunction(const std::string& _stName, MEXGW_FUNC _pFunc, const std::string& _stModule)
{
    return new WrapMexFunction(_stName, _pFunc, NULL, _stModule);
}

Function* Function::createFunction(const std::string& _stName, GW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::string& _stModule)
{
    return new Function(_stName, _pFunc, _pLoadDeps, _stModule);
}

Function* Function::createFunction(const std::string& _stName, GW_FUNC_OPT _pFunc, LOAD_DEPS _pLoadDeps, const std::string& _stModule)
{
    return new OptFunction(_stName, _pFunc, _pLoadDeps, _stModule);
}

Function* Function::createFunction(const std::string& _stName, OLDGW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::string& _stModule)
{
    return new WrapFunction(_stName, _pFunc, _pLoadDeps, _stModule);
}

Function* Function::createFunction(const std::string& _stName, MEXGW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::string& _stModule)
{
    return new WrapMexFunction(_stName, _pFunc, _pLoadDeps, _stModule);
}

Function* Function::createFunction(const std::string& _stFunctionName, const std::string& _stEntryPointName, const std::string& _stLibName, FunctionType _iType, LOAD_DEPS _pLoadDeps, const std::string& _stModule, bool _bCloseLibAfterCall)
{
    return new DynamicFunction(_stFunctionName, _stEntryPointName, _stLibName, _iType, _pLoadDeps, _stModule, _bCloseLibAfterCall);
}

Function* Function::createFunction(const std::string& _stFunctionName, const std::string& _stEntryPointName, const std::string& _stLibName, FunctionType _iType, const std::string& _stLoadDepsName, const std::string& _stModule, bool _bCloseLibAfterCall)
{
    return new DynamicFunction(_stFunctionName, _stEntryPointName, _stLibName, _iType, _stLoadDepsName, _stModule, _bCloseLibAfterCall);
}

Function* Function::createFunction(const std::string& _stName, GW_C_FUNC _pFunc, const std::string& _stModule)
{
    return new WrapCFunction(_stName, _pFunc, NULL, _stModule);
}

Function* Function::createFunction(const std::string& _stName, GW_C_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::string& _stModule)
{
    return new WrapCFunction(_stName, _pFunc, _pLoadDeps, _stModule);
}


Function::Function(const std::string& _stName, GW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::string& _stModule) : Callable(), m_pFunc(_pFunc), m_pLoadDeps(_pLoadDeps)
{
    setName(_stName);
    setModule(_stModule);
}

Function::~Function()
{

}


Function::ReturnValue Function::call(typed_list &in, optional_list &/*opt*/, int _iRetCount, typed_list &out)
{
    int ret = 1;
    if (m_pLoadDeps != NULL)
    {
        ret = m_pLoadDeps(m_stName);
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

bool Function::toString(std::ostringstream& ostr)
{
    // display nothing. ie : c = cos
    return true;
}

Function* Function::clone()
{
    IncreaseRef();
    return this;
}

OptFunction::OptFunction(const std::string& _stName, GW_FUNC_OPT _pFunc, LOAD_DEPS _pLoadDeps, const std::string& _stModule)
{
    m_stName = _stName;
    m_pFunc = _pFunc;
    m_pLoadDeps = _pLoadDeps;
    m_stModule = _stModule;
}

OptFunction::OptFunction(OptFunction* _pWrapFunction)
{
    m_stModule  = _pWrapFunction->getModule();
    m_stName    = _pWrapFunction->getName();
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
        ret = m_pLoadDeps(m_stName);
    }

    if (ret == 0)
    {
        return Error;
    }

    return this->m_pFunc(in, opt, _iRetCount, out);
}

WrapFunction::WrapFunction(const std::string& _stName, OLDGW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::string& _stModule)
{
    m_stName = _stName;
    m_pOldFunc = _pFunc;
    m_stModule = _stModule;
    m_pLoadDeps = _pLoadDeps;
}

WrapFunction::WrapFunction(WrapFunction* _pWrapFunction)
{
    m_stModule = _pWrapFunction->getModule();
    m_stName = _pWrapFunction->getName();
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
    bool isRef = checkReferenceModule(m_stModule.c_str());

    if (m_pLoadDeps != NULL)
    {
        ret = m_pLoadDeps(m_stName);
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

WrapMexFunction::WrapMexFunction(const std::string& _stName, MEXGW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::string& _stModule)
{
    m_stName = _stName;
    m_pOldFunc = _pFunc;
    m_stModule = _stModule;
    m_pLoadDeps = _pLoadDeps;
}

WrapMexFunction::WrapMexFunction(WrapMexFunction* _pWrapFunction)
{
    m_stModule = _pWrapFunction->getModule();
    m_stName = _pWrapFunction->getName();
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
        ret = m_pLoadDeps(m_stName);
    }

    if (ret == 0)
    {
        return Error;
    }

    ReturnValue retVal = Callable::OK;

    ConfigVariable::setMexFunctionName(getName().c_str());

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

WrapCFunction::WrapCFunction(const std::string& _stName, GW_C_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::string& _stModule)
{
    m_stName = _stName;
    m_pCFunc = _pFunc;
    m_stModule = _stModule;
    m_pLoadDeps = _pLoadDeps;
}

WrapCFunction::WrapCFunction(WrapCFunction* _pWrapFunction)
{
    m_stModule = _pWrapFunction->getModule();
    m_stName = _pWrapFunction->getName();
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
        if (m_pLoadDeps(m_stName) == 0)
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

DynamicFunction::DynamicFunction(const std::string& _stName, const std::string& _stEntryPointName, const std::string& _stLibName, FunctionType _iType, const std::string& _stLoadDepsName, const std::string& _stModule, bool _bCloseLibAfterCall)
{
    m_stName = _stName;
    m_libName = _stLibName;
    m_stModule = _stModule;
    m_entryPoint = _stEntryPointName;
    m_loadDepsName = _stLoadDepsName;
    m_pLoadDeps = NULL;
    m_bCloseLibAfterCall = _bCloseLibAfterCall;
    m_iType = _iType;
    m_pFunc = NULL;
    m_pOptFunc = NULL;
    m_pOldFunc = NULL;
    m_pMexFunc = NULL;
    m_pFunction = NULL;
    m_pCFunc = NULL;
}

DynamicFunction::DynamicFunction(const std::string& _stName, const std::string& _stEntryPointName, const std::string& _stLibName, FunctionType _iType, LOAD_DEPS _pLoadDeps, const std::string& _stModule, bool _bCloseLibAfterCall)
{
    m_stName = _stName;
    m_libName = _stLibName;
    m_stModule = _stModule;
    m_entryPoint = _stEntryPointName;
    m_pLoadDeps = _pLoadDeps;
    m_loadDepsName = "";
    m_bCloseLibAfterCall = _bCloseLibAfterCall;
    m_iType = _iType;
    m_pFunc = NULL;
    m_pOptFunc = NULL;
    m_pOldFunc = NULL;
    m_pMexFunc = NULL;
    m_pFunction = NULL;
    m_pCFunc = NULL;
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
    if (m_libName.empty())
    {
        Scierror(999, _("%s: Library name must not be empty\n."), m_stName.c_str());
        return Error;
    }

    DynLibHandle hLib = getDynModule(m_libName.c_str());
    if (hLib == 0)
    {
        hLib = LoadDynLibrary(m_libName.c_str());

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
            char* pstScilabPath = getSCI();
            char pstModulesPath[] = "/modules/";
            char pstLTDir[] =  ".libs/";

            /* Build the full path to the library */
            int iPathToLibLen = (strlen(pstScilabPath) + strlen(pstModulesPath) + strlen(m_stModule.c_str()) + strlen("/") + strlen(pstLTDir) + strlen(m_libName.c_str()) + 1);
            char* pstPathToLib = (char*)MALLOC(iPathToLibLen * sizeof(char));
#ifdef _MSC_VER
            os_sprintf(pstPathToLib, iPathToLibLen, "%s%s%s/%s%s", pstScilabPath, pstModulesPath, m_stModule.c_str(), pstLTDir, m_libName.c_str());
#else
            os_sprintf(pstPathToLib, "%s%s%s/%s%s", pstScilabPath, pstModulesPath, m_stModule.c_str(), pstLTDir, m_libName.c_str());
#endif
            FREE(pstScilabPath);
            hLib = LoadDynLibrary(pstPathToLib);

            if (hLib == 0)
            {
                Scierror(999, _("An error has been detected while loading %s: %s\n"), m_libName.c_str(), pstError);
                FREE(pstError);

                pstError = GetLastDynLibError();
                Scierror(999, _("An error has been detected while loading %s: %s\n"), pstPathToLib, pstError);

                FREE(pstPathToLib);
                return Error;
            }
            FREE(pstPathToLib);
            FREE(pstError);
#else
            Scierror(999, _("Impossible to load %s library\n"), m_libName.c_str());
            return Error;
#endif
        }
        addDynModule(m_libName.c_str(), hLib);

        /*Load deps*/
        if (m_loadDepsName.empty() == false && m_pLoadDeps == NULL)
        {
            m_pLoadDeps = (LOAD_DEPS)GetDynLibFuncPtr(hLib, m_loadDepsName.c_str());
        }

    }

    /*Load gateway*/
    if (m_stName != "")
    {
        const char* pstEntryPoint = m_entryPoint.c_str();
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
    }

    if (m_pFunc == NULL && m_pOldFunc == NULL && m_pMexFunc == NULL && m_pOptFunc == NULL && m_pCFunc == NULL)
    {
        const char* pstEntry = m_entryPoint.c_str();
        const char* pstLib = m_libName.c_str();
        Scierror(999, _("Impossible to load %s function in %s library: %s\n"), pstEntry, pstLib, GetLastDynLibError());
        return Error;
    }

    switch (m_iType)
    {
        case EntryPointCPPOpt :
            m_pFunction = new OptFunction(m_stName, m_pOptFunc, m_pLoadDeps, m_stModule);
            break;
        case EntryPointCPP :
            m_pFunction = new Function(m_stName, m_pFunc, m_pLoadDeps, m_stModule);
            break;
        case EntryPointOldC :
            m_pFunction = new WrapFunction(m_stName, m_pOldFunc, m_pLoadDeps, m_stModule);
            break;
        case EntryPointMex:
            m_pFunction = new WrapMexFunction(m_stName, m_pMexFunc, m_pLoadDeps, m_stModule);
            break;
        case EntryPointC:
            m_pFunction = new WrapCFunction(m_stName, m_pCFunc, m_pLoadDeps, m_stModule);
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
