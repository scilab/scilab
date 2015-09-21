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

#ifndef __FUNCTION_HXX__
#define __FUNCTION_HXX__


#ifdef _MSC_VER
#pragma warning(disable: 4251)
#endif

#include <string>
#include "types.hxx"
#include "callable.hxx"

extern "C"
{
#include "c_gateway_prototype.h"
#include "dynamiclibrary.h"
}

#define MAX_OUTPUT_VARIABLE		64

namespace types
{
class EXTERN_AST Function : public Callable
{
public :
    enum FunctionType
    {
        EntryPointC         = 0,
        EntryPointCPP       = 1,
        EntryPointMex       = 2,
        EntryPointCPPOpt    = 3,
    };

    typedef int(*LOAD_DEPS)(const std::wstring&);
    typedef int (*INIT_MODULE)(void);
    typedef ReturnValue (*GW_FUNC)(typed_list &in, int _iRetCount, typed_list &out);
    typedef ReturnValue (*GW_FUNC_OPT)(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out);

    Function() : Callable() {};
    Function(const std::wstring& _wstName, GW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::wstring& _wstModule);
    ~Function();

    //FIXME : Should not return NULL
    InternalType*           clone();

    static Function*        createFunction(const std::wstring& _wstName, GW_FUNC _pFunc, const std::wstring& _wstModule);
    static Function*        createFunction(const std::wstring& _wstName, GW_FUNC_OPT _pFunc, const std::wstring& _wstModule);
    static Function*        createFunction(const std::wstring& _wstName, OLDGW_FUNC _pFunc, const std::wstring& _wstModule);
    static Function*        createFunction(const std::wstring& _wstName, MEXGW_FUNC _pFunc, const std::wstring& _wstModule);
    static Function*        createFunction(const std::wstring& _wstName, GW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::wstring& _wstModule);
    static Function*        createFunction(const std::wstring& _wstName, GW_FUNC_OPT _pFunc, LOAD_DEPS _pLoadDeps, const std::wstring& _wstModule);
    static Function*        createFunction(const std::wstring& _wstName, OLDGW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::wstring& _wstModule);
    static Function*        createFunction(const std::wstring& _wstName, MEXGW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::wstring& _wstModule);

    /*dynamic gateways*/
    static Function*        createFunction(const std::wstring& _wstFunctionName, const std::wstring& _wstEntryPointName, const std::wstring& _wstLibName, FunctionType _iType, LOAD_DEPS _pLoadDeps, const std::wstring& _wstModule, bool _bCloseLibAfterCall = false);
    static Function*        createFunction(const std::wstring& _wstFunctionName, const std::wstring& _wstEntryPointName, const std::wstring& _wstLibName, FunctionType _iType, const std::wstring& _wstLoadDepsName, const std::wstring& _wstModule, bool _bCloseLibAfterCall = false);

    inline ScilabType       getType(void)
    {
        return ScilabFunction;
    }
    inline ScilabId         getId(void)
    {
        return IdFunction;
    }

    bool                    isFunction()
    {
        return true;
    }

    void                    whoAmI();

    bool                    toString(std::wostringstream& ostr);

    virtual ReturnValue     call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc);

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring    getTypeStr()
    {
        return L"fptr";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring    getShortTypeStr()
    {
        return L"fptr";
    }

    GW_FUNC                 getFunc()
    {
        return m_pFunc;
    }
    LOAD_DEPS               getDeps()
    {
        return m_pLoadDeps;
    }

private :
    GW_FUNC                 m_pFunc;

protected:
    LOAD_DEPS               m_pLoadDeps;

};

class OptFunction : public Function
{
private :
    OptFunction(OptFunction* _Function);
public :
    OptFunction(const std::wstring& _wstName, GW_FUNC_OPT _pFunc, LOAD_DEPS _pLoadDeps, const std::wstring& _wstModule);

    Callable::ReturnValue call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc);
    InternalType*           clone();

    GW_FUNC_OPT             getFunc()
    {
        return m_pFunc;
    }

private :
    GW_FUNC_OPT             m_pFunc;
};


class WrapFunction : public Function
{
private :
    WrapFunction(WrapFunction* _pWrapFunction);
public :
    WrapFunction(const std::wstring& _wstName, OLDGW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::wstring& _wstModule);

    Callable::ReturnValue call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc);
    InternalType*           clone();

    OLDGW_FUNC              getFunc()
    {
        return m_pOldFunc;
    }

private :
    OLDGW_FUNC              m_pOldFunc;
};

class WrapMexFunction : public Function
{
private :
    WrapMexFunction(WrapMexFunction* _pWrapFunction);
public :
    WrapMexFunction(const std::wstring& _wstName, MEXGW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::wstring& _wstModule);

    Callable::ReturnValue call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc);
    InternalType*           clone();

    MEXGW_FUNC              getFunc()
    {
        return m_pOldFunc;
    }

private :
    MEXGW_FUNC              m_pOldFunc;
};

class DynamicFunction : public Function
{
private :
    DynamicFunction(DynamicFunction* _pDynamicFunction);
public :
    DynamicFunction(const std::wstring& _wstName, const std::wstring& _wstEntryPointName, const std::wstring& _wstLibName, FunctionType _iType, LOAD_DEPS _pLoadDeps, const std::wstring& _wstModule, bool _bCloseLibAfterCall = false);

    virtual ~DynamicFunction();

    DynamicFunction(const std::wstring& _wstName, const std::wstring& _wstEntryPointName, const std::wstring& _wstLibName, FunctionType _iType, const std::wstring& _wstLoadDepsName, const std::wstring& _wstModule, bool _bCloseLibAfterCall = false);
    Callable::ReturnValue   call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc);
private :
    Callable::ReturnValue   Init();
    void                    Clear();

    std::wstring            m_wstLibName;
    std::wstring            m_wstEntryPoint;
    std::wstring            m_wstLoadDepsName;
    bool                    m_bCloseLibAfterCall;
    FunctionType            m_iType;
    GW_FUNC                 m_pFunc;
    GW_FUNC_OPT             m_pOptFunc;
    OLDGW_FUNC              m_pOldFunc;
    MEXGW_FUNC              m_pMexFunc;
    Function*               m_pFunction;
};

}


#endif /* !__FUNCTION_HXX__ */
