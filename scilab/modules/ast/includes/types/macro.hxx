/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2010 - DIGITEO - Bruno JOFRET
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

#ifndef __MACRO_HXX__
#define __MACRO_HXX__

#include "callable.hxx"
#include "context.hxx"
#include "double.hxx"
#include "object.hxx"
#include "seqexp.hxx"
#include "types.hxx"
#include "arguments.hxx"
#include <functional>
#include <string>
#include <vector>

namespace types
{
class EXTERN_AST Macro : public Callable
{
  public:
    Macro() : Callable(),
              m_Nargin(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"nargin"))),
              m_Nargout(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"nargout"))),
              m_Varargin(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"varargin"))),
              m_Varargout(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"varargout"))) /*,
              m_firstCall(true)*/
    {
    }
    //standard
    Macro(const std::wstring& _stName, std::vector<symbol::Variable*>& _inputArgs, std::vector<symbol::Variable*>& _outputArgs, ast::SeqExp& _body, const std::wstring& _stModule);
    //lambda
    Macro(std::vector<symbol::Variable*>& _inputArgs, ast::SeqExp& _body, const std::wstring& _stModule, std::unordered_map<std::wstring, types::InternalType*> captured = {});
    //static for classdef
    Macro(const std::wstring& _stName, Classdef* def, std::vector<symbol::Variable*>& _inputArgs, std::vector<symbol::Variable*>& _outputArgs, ast::SeqExp& _body, const std::wstring& _stModule);

    virtual ~Macro();

    Macro* clone() override;

    inline ScilabType getType(void) override
    {
        return ScilabMacro;
    }
    inline ScilabId getId(void) override
    {
        return IdMacro;
    }
    bool isMacro() override
    {
        return true;
    }

    bool isA(const std::wstring& type)
    {
        return type == L"function";
    }

    bool isLambda()
    {
        return m_isLambda;
    }

    void whoAmI() override;

    bool toString(std::wostringstream& ostr) override;

    Callable::ReturnValue call(typed_list& in, optional_list& opt, int _iRetCount, typed_list& out) override;

    inline void cleanCall(symbol::Context* pContext, int oldPromptMode);

    ast::SeqExp* getBody();

    bool getMemory(long long* _piSize, long long* _piSizePlusType) override;

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring getTypeStr() const override
    {
        return L"function";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring getShortTypeStr() const override
    {
        return L"function";
    }

    const std::wstring& getFileName() const
    {
        return m_stPath;
    }

    void setFileName(const std::wstring& _fileName)
    {
        m_stPath = _fileName;
    }

    std::vector<symbol::Variable*>* getInputs();
    std::vector<symbol::Variable*>* getOutputs();

    virtual int getNbInputArgument(void) override;
    virtual int getNbOutputArgument(void) override;

    bool operator==(const InternalType& it) override;

    void add_submacro(const symbol::Symbol& s, Macro* macro);

    inline const std::map<symbol::Variable*, Macro*>& getSubMacros() const
    {
        return m_submacro;
    }

    inline const std::map<symbol::Variable*, Macro*>& getSubMacros()
    {
        return m_submacro;
    }

    void updateArguments();
    bool checkArgument(ast::Exp* exp);
    bool checkStaticDims(const std::vector<std::tuple<std::vector<int>, symbol::Variable*>>& dims);
    inline const std::unordered_map<std::wstring, types::InternalType*>& getCaptured()
    {
        return m_captured;
    }

    void setParent(InternalType* p)
    {
        parent = p;
        if (p)
        {
            //mustAsParent = true;
        }
    }

    InternalType* getParent()
    {
        return parent;
    }

  private:
    std::vector<symbol::Variable*>* m_inputArgs;
    std::vector<symbol::Variable*>* m_outputArgs;
    ast::SeqExp* m_body;
    symbol::Variable* m_Nargin;
    symbol::Variable* m_Nargout;
    symbol::Variable* m_Varargin;
    symbol::Variable* m_Varargout;
    Double* m_pDblArgIn;
    Double* m_pDblArgOut;
    std::map<symbol::Variable*, Macro*> m_submacro;
    std::wstring m_stPath;
    std::map<std::wstring, ARG> m_arguments;

    /*lambda*/
    bool m_isLambda;
    std::unordered_map<std::wstring, types::InternalType*> m_captured;
    InternalType* parent;
    //bool mustAsParent;

    void cleanup();
};

// a macro runs in the context of its parent (object or classdef): set it for the duration
// of a call only, the macro can be shared (inherited method, function assigned as method, ...)
class EXTERN_AST MacroParentScope
{
public:
    MacroParentScope(Callable* call, InternalType* parent);
    ~MacroParentScope();

private:
    Macro* macro;
    InternalType* previous;
};
}

#endif /* !__MACRO_HXX__ */
