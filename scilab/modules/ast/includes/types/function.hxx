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
        EntryPointOldC = 0,
        EntryPointCPP = 1,
        EntryPointMex = 2,
        EntryPointCPPOpt = 3,
        EntryPointC = 4
    };

    typedef int(*LOAD_DEPS)(const std::string&);
    typedef int (*INIT_MODULE)(void);
    typedef ReturnValue (*GW_FUNC)(typed_list &in, int _iRetCount, typed_list &out);
    typedef ReturnValue (*GW_FUNC_OPT)(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out);

    Function() : Callable() {};
    Function(const std::string& _name, GW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::string& _module);
    ~Function();

    //FIXME : Should not return NULL
    virtual Function*       clone();

    static Function*        createFunction(const std::string& _name, GW_FUNC _pFunc, const std::string& _module);
    static Function*        createFunction(const std::string& _name, GW_FUNC_OPT _pFunc, const std::string& _module);
    static Function*        createFunction(const std::string& _name, OLDGW_FUNC _pFunc, const std::string& _module);
    static Function*        createFunction(const std::string& _name, MEXGW_FUNC _pFunc, const std::string& _module);
    static Function*        createFunction(const std::string& _name, GW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::string& _module);
    static Function*        createFunction(const std::string& _name, GW_FUNC_OPT _pFunc, LOAD_DEPS _pLoadDeps, const std::string& _module);
    static Function*        createFunction(const std::string& _name, OLDGW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::string& _module);
    static Function*        createFunction(const std::string& _name, MEXGW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::string& _module);
    static Function*        createFunction(const std::string& _name, GW_C_FUNC _pFunc, const std::string& _module);
    static Function*        createFunction(const std::string& _name, GW_C_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::string& _module);

    /*dynamic gateways*/
    static Function*        createFunction(const std::string& _functionName, const std::string& _entryPointName, const std::string& _libName, FunctionType _iType, LOAD_DEPS _pLoadDeps, const std::string& _module, bool _bCloseLibAfterCall = false);
    static Function*        createFunction(const std::string& _functionName, const std::string& _entryPointName, const std::string& _libName, FunctionType _iType, const std::string& _loadDepsName, const std::string& _module, bool _bCloseLibAfterCall = false);

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

    bool                    toString(std::ostringstream& ostr) override;

    virtual ReturnValue     call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out) override;

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::string    getTypeStr()
    {
        return "fptr";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::string    getShortTypeStr()
    {
        return "fptr";
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
    OptFunction(const std::string& _name, GW_FUNC_OPT _pFunc, LOAD_DEPS _pLoadDeps, const std::string& _module);

    Callable::ReturnValue   call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out) override;
    OptFunction*            clone();

    GW_FUNC_OPT             getFunc()
    {
        return m_pFunc;
    }

private:
    GW_FUNC_OPT             m_pFunc;
};


class WrapFunction : public Function
{
private:
    WrapFunction(WrapFunction* _pWrapFunction);
public:
    WrapFunction(const std::string& _name, OLDGW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::string& _module);

    Callable::ReturnValue   call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out) override;
    WrapFunction*           clone();

    OLDGW_FUNC              getFunc()
    {
        return m_pOldFunc;
    }

private:
    OLDGW_FUNC              m_pOldFunc;
};

class WrapCFunction : public Function
{
private:
    WrapCFunction(WrapCFunction* _pWrapFunction);
public:
    WrapCFunction(const std::string& _name, GW_C_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::string& _module);

    Callable::ReturnValue   call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out) override;
    WrapCFunction*          clone();

    GW_C_FUNC               getFunc()
    {
        return m_pCFunc;
    }

private:
    GW_C_FUNC               m_pCFunc;
};

class WrapMexFunction : public Function
{
private :
    WrapMexFunction(WrapMexFunction* _pWrapFunction);
public :
    WrapMexFunction(const std::string& _name, MEXGW_FUNC _pFunc, LOAD_DEPS _pLoadDeps, const std::string& _module);

    Callable::ReturnValue call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out) override;
    WrapMexFunction*        clone();

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
    DynamicFunction(const std::string& _name, const std::string& _entryPointName, const std::string& _libName, FunctionType _iType, LOAD_DEPS _pLoadDeps, const std::string& _module, bool _bCloseLibAfterCall = false);

    virtual ~DynamicFunction();

    DynamicFunction(const std::string& _name, const std::string& _entryPointName, const std::string& _libName, FunctionType _iType, const std::string& _loadDepsName, const std::string& _module, bool _bCloseLibAfterCall = false);
    Callable::ReturnValue   call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out) override;

private :
    Callable::ReturnValue   Init();
    void                    Clear();

    std::string             m_libName;
    std::string             m_entryPoint;
    std::string             m_loadDepsName;
    bool                    m_bCloseLibAfterCall;
    FunctionType            m_iType;
    GW_FUNC                 m_pFunc;
    GW_FUNC_OPT             m_pOptFunc;
    OLDGW_FUNC              m_pOldFunc;
    GW_C_FUNC               m_pCFunc;
    MEXGW_FUNC              m_pMexFunc;
    Function*               m_pFunction;
};

}


#endif /* !__FUNCTION_HXX__ */
