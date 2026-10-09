/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2023 - Dassault Systèmes S.E. - Antoine ELIAS
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
#include <iostream>

#include "argumentvisitor.hxx"
#include "configvariable.hxx"
#include "debugmanager.hxx"
#include "context.hxx"
#include "listinsert.hxx"
#include "macro.hxx"
#include "macrofile.hxx"
#include "macrovarvisitor.hxx"
#include "mlist.hxx"
#include "parser.hxx"
#include "runvisitor.hxx"
#include "scilabWrite.hxx"
#include "serializervisitor.hxx"
#include "string.hxx"

extern "C"
{
#include "Scierror.h"
#include "Sciwarning.h"
#include "localization.h"
#include "sci_malloc.h"
#include "sciprint.h"
}

namespace types
{
Macro::Macro(std::vector<symbol::Variable*>& _inputArgs, ast::SeqExp& _body, const std::wstring& _stModule, std::unordered_map<std::wstring, types::InternalType*> captured) : Callable(),
    m_inputArgs(&_inputArgs), m_outputArgs(nullptr), m_body(_body.clone()),
    m_Nargin(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"nargin"))),
    m_Nargout(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"nargout"))),
    m_Varargin(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"varargin"))),
    m_Varargout(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"varargout"))),
    m_isLambda(true),
    m_captured(captured),
    parent(nullptr)
{
    setName(L"anonymous");
    setModule(_stModule);
    m_pDblArgIn = new Double(1);
    m_pDblArgIn->IncreaseRef(); // never delete
    m_pDblArgOut = new Double(1);
    m_pDblArgOut->IncreaseRef(); // never delete

    m_body->setReturnable();
    m_stPath = L"";

    try
    {
        updateArguments();
    }
    catch (const ast::InternalError& ie)
    {
        cleanup();
        throw ie;
    }

    // check variables/macros in body
    ast::MacrovarVisitor visit;
    getBody()->accept(visit);

    for (auto&& c : m_captured)
    {
        c.second->IncreaseRef(); // protect loaded variable
    }

    // external variables
    auto externals = visit.getExternal();
    for (auto&& e : externals)
    {
        symbol::Symbol var = symbol::Symbol(e);
        if (std::find_if(m_inputArgs->begin(), m_inputArgs->end(), [var](symbol::Variable* v)
                         { return v->getSymbol() == var; }) != m_inputArgs->end())
        {
            // input parameter
            continue;
        }

        types::InternalType* pIT = symbol::Context::getInstance()->get(var);
        if (pIT == nullptr && m_captured.find(e) == m_captured.end())
        {
            throw ast::InternalError(formatError(_("%s: variable `'%ls\' must exist.\n"), "lambda", e.data()));
        }

        if (pIT)
        {
            m_captured[e] = pIT->clone();
            m_captured[e]->IncreaseRef();
        }
    }

    // called functions
    auto called = visit.getCalled();
    for (auto&& c : called)
    {
        symbol::Symbol var = symbol::Symbol(c);
        if (std::find_if(m_inputArgs->begin(), m_inputArgs->end(), [var](symbol::Variable* v)
                         { return v->getSymbol() == var; }) != m_inputArgs->end())
        {
            // input parameter
            continue;
        }

        types::InternalType* pIT = symbol::Context::getInstance()->get(var);
        if (pIT == nullptr && m_captured.find(c) == m_captured.end())
        {
            throw ast::InternalError(formatError(_("%s: variable `%ls` must exist.\n"), "lambda", c.data()));
        }

        if (pIT)
        {
            symbol::Variable* v = symbol::Context::getInstance()->getOrCreate(var);
            if (v->empty())
            {
                types::InternalType* p = symbol::Context::getInstance()->get(var);
                if (p)
                {
                    m_captured[c] = p->clone();
                    m_captured[c]->IncreaseRef();
                }
            }
            else if (v->top()->m_iLevel > SCOPE_GATEWAY)
            {
                // sciprint("level: %ls(%d)\n", c.data(), v->top()->m_iLevel);
                //  not a original function of Scilab
                m_captured[c] = pIT->clone();
                m_captured[c]->IncreaseRef();
            }
        }
    }
}

Macro::Macro(const std::wstring& _stName, std::vector<symbol::Variable*>& _inputArgs, std::vector<symbol::Variable*>& _outputArgs, ast::SeqExp& _body, const std::wstring& _stModule) : Callable(),
    m_inputArgs(&_inputArgs), m_outputArgs(&_outputArgs), m_body(_body.clone()),
    m_Nargin(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"nargin"))),
    m_Nargout(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"nargout"))),
    m_Varargin(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"varargin"))),
    m_Varargout(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"varargout"))),
    m_isLambda(false),
    parent(nullptr)
{
    setName(_stName);
    setModule(_stModule);
    m_pDblArgIn = new Double(1);
    m_pDblArgIn->IncreaseRef(); // never delete
    m_pDblArgOut = new Double(1);
    m_pDblArgOut->IncreaseRef(); // never delete

    m_body->setReturnable();
    m_stPath = L"";

    // Do not debug the macros called when checking arguments (calling sci2exp)
    debugger::ExecutionModeGuard normalMode(false);
    try
    {
        updateArguments();
    }
    catch (const ast::InternalError& ie)
    {
        cleanup();
        throw ie;
    }
}

Macro::Macro(const std::wstring& _stName, Classdef* def, std::vector<symbol::Variable*>& _inputArgs, std::vector<symbol::Variable*>& _outputArgs, ast::SeqExp& _body, const std::wstring& _stModule) : Callable(),
      m_inputArgs(&_inputArgs), m_outputArgs(&_outputArgs), m_body(_body.clone()),
      m_Nargin(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"nargin"))),
      m_Nargout(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"nargout"))),
      m_Varargin(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"varargin"))),
      m_Varargout(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"varargout"))),
      m_isLambda(false),
      parent(nullptr)
{
    setName(_stName);
    setModule(_stModule);
    m_pDblArgIn = new Double(1);
    m_pDblArgIn->IncreaseRef(); // never delete
    m_pDblArgOut = new Double(1);
    m_pDblArgOut->IncreaseRef(); // never delete

    m_body->setReturnable();
    m_stPath = L"";
    parent = def;

    // Do not debug the macros called when checking arguments (calling sci2exp)
    debugger::ExecutionModeGuard normalMode(false);
    updateArguments();
}

Macro::~Macro()
{
    cleanup();
}

MacroParentScope::MacroParentScope(Callable* call, InternalType* parent) : macro(nullptr), previous(nullptr)
{
    if (call->isMacro())
    {
        macro = call->getAs<Macro>();
    }
    else if (call->isMacroFile())
    {
        macro = call->getAs<MacroFile>()->getMacro();
    }

    if (macro)
    {
        previous = macro->getParent();
        macro->setParent(parent);
    }
}

MacroParentScope::~MacroParentScope()
{
    if (macro)
    {
        macro->setParent(previous);
    }
}

void Macro::cleanup()
{
    delete m_body;
    m_pDblArgIn->DecreaseRef();
    m_pDblArgIn->killMe();
    m_pDblArgOut->DecreaseRef();
    m_pDblArgOut->killMe();

    if (m_inputArgs)
    {
        delete m_inputArgs;
    }

    if (m_outputArgs)
    {
        delete m_outputArgs;
    }

    for (auto&& sub : m_submacro)
    {
        sub.second->DecreaseRef();
        sub.second->killMe();
    }

    if (isLambda())
    {
        for (auto&& c : m_captured)
        {
            c.second->DecreaseRef();
            c.second->killMe();
        }
    }
    m_submacro.clear();

    for(auto& a : m_arguments)
    {
        for(auto& v : a.second.validators)
        {
            for(auto& i : v.inputs)
            {
                types::InternalType* val = std::get<1>(i);
                if(val)
                {
                    val->DecreaseRef();
                    val->killMe();
                }
            }
        }
    }
    m_arguments.clear();
}

void Macro::cleanCall(symbol::Context* pContext, int oldPromptMode)
{
    // restore previous prompt mode
    ConfigVariable::setPromptMode(oldPromptMode);

    // close the current scope
    pContext->scope_end();

    ConfigVariable::macroFirstLine_end();
}

Macro* Macro::clone()
{
    IncreaseRef();
    return this;
}

void Macro::whoAmI()
{
    std::cout << "types::Macro";
}

ast::SeqExp* Macro::getBody(void)
{
    return m_body;
}

bool Macro::toString(std::wostringstream& ostr)
{
    // get macro name
    wchar_t* wcsVarName = NULL;
    if (ostr.str() == SPACES_LIST)
    {
        wcsVarName = os_wcsdup(getName().c_str());
    }
    else
    {
        wcsVarName = os_wcsdup(ostr.str().c_str());
    }

    ostr.str(L"");

    if (isLambda())
    {
        ostr << wcsVarName << L": ";
    }
    else
    {
        ostr << L"[";
        // output arguments [a,b,c] = ....
        if (m_outputArgs->empty() == false)
        {
            std::vector<symbol::Variable*>::iterator OutArg = m_outputArgs->begin();
            std::vector<symbol::Variable*>::iterator OutArgfter = OutArg;
            OutArgfter++;

            for (; OutArgfter != m_outputArgs->end(); OutArgfter++)
            {
                ostr << (*OutArg)->getSymbol().getName();
                ostr << ",";
                OutArg++;
            }

            ostr << (*OutArg)->getSymbol().getName();
        }

        ostr << L"]";
        // function name
        ostr << L"=" << wcsVarName;
    }

    ostr << L"(";
    // input arguments function(a,b,c)
    if (m_inputArgs->empty() == false)
    {
        std::vector<symbol::Variable*>::iterator inArg = m_inputArgs->begin();
        std::vector<symbol::Variable*>::iterator inRagAfter = inArg;
        inRagAfter++;

        for (; inRagAfter != m_inputArgs->end(); inRagAfter++)
        {
            ostr << (*inArg)->getSymbol().getName();
            ostr << ",";
            inArg++;
        }

        ostr << (*inArg)->getSymbol().getName();
    }

    ostr << L")" << std::endl;

    FREE(wcsVarName);
    return true;
}

Callable::ReturnValue Macro::call(typed_list& in, optional_list& opt, int _iRetCount, typed_list& out)
{
    int rhs = (int)in.size();
    bool bVarargout = false;

    int iRetCount = std::max(0, _iRetCount);

    ReturnValue RetVal = Callable::OK;
    symbol::Context* pContext = symbol::Context::getInstance();

    // open a new scope
    pContext->scope_begin(parent, getName());
    if (parent != nullptr && parent->isObject())
    {
        pContext->put(symbol::Symbol(L"this"), parent);
    }

    // store the line number where is stored this macro in file.
    ConfigVariable::macroFirstLine_begin(getFirstLine());

    // check excepted and input/output parameters numbers
    //  Scilab Macro can be called with less than prototyped arguments,
    //  but not more execpts with varargin

    bool bVarargin = false;
    // varargin management
    if (m_inputArgs->size() > 0 && m_inputArgs->back()->getSymbol().getName() == L"varargin")
    {
        bVarargin = true;
        List* pL = new List();
        int iVarPos = rhs;
        if (iVarPos > static_cast<int>(m_inputArgs->size()) - 1)
        {
            iVarPos = static_cast<int>(m_inputArgs->size()) - 1;
        }

        // add variables in context or varargin list
        std::vector<symbol::Variable*>::iterator itName = m_inputArgs->begin();
        for (int i = 0; i < rhs; ++i)
        {
            if (in[i]->isListInsert())
            {
                // named
                std::wstring var(in[i]->getAs<ListInsert>()->getInsert()->getAs<String>()->get()[0]);
                if (i < iVarPos)
                {
                    pContext->put(symbol::Symbol(var), opt[var]);
                    ++itName;
                }
                else
                {
                    // varargin
                    pL->append(opt[var]);
                }
            }
            else
            {
                // context
                if (i < iVarPos)
                {
                    pContext->put(*itName, in[i]);
                    ++itName;
                }
                else
                {
                    // varargin
                    pL->append(in[i]);
                }
            }
        }

        // add varargin to macro scope
        pContext->put(m_Varargin, pL);
    }
    else if (rhs > m_inputArgs->size())
    {
        if (m_inputArgs->size() == 0)
        {
            Scierror(999, _("Wrong number of input arguments: This function has no input argument.\n"));
        }
        else
        {
            Scierror(999, _("Wrong number of input arguments.\n"));
        }

        pContext->scope_end();
        ConfigVariable::fillWhereError(getBody()->getLocation());
        ConfigVariable::macroFirstLine_end();
        return Callable::Error;
    }
    else
    {
        // assign value to variable in the new context
        std::vector<symbol::Variable*>::iterator i;
        typed_list::const_iterator j;

        for (i = m_inputArgs->begin(), j = in.begin(); j != in.end(); ++j, ++i)
        {
            if (*j && (*j)->isListInsert() == false)
            {
                // prevent assignation of NULL value
                pContext->put(*i, *j);
            }
        }

        if (m_arguments.size() != 0 && opt.size() != 0)
        {
            Scierror(999, _("%s: Named argument are not compatible with arguments block.\n"), scilab::UTF8::toUTF8(m_wstName).data());

            pContext->scope_end();
            ConfigVariable::fillWhereError(getBody()->getLocation());
            ConfigVariable::macroFirstLine_end();
            return Callable::Error;
        }

        // add optional parameters in current scope
        for (auto&& it : opt)
        {
            if (it.second)
            {
                pContext->put(symbol::Symbol(it.first), it.second);
            }
        }
    }
    /*argument checker*/
    if (m_arguments.size() != 0)
    {
        try
        {
            // look only in variables: a missing symbol makes Context::get scan all libraries
            static const symbol::Symbol skipArgsSymbol(L"%skipArgs");
            types::InternalType* skipArgs = symbol::Context::getInstance()->getOrCreate(skipArgsSymbol)->get();
            if (skipArgs == nullptr)
            {
                int expectedmin = 0;
                int expectedmax = 0;
                for (auto&& a : m_arguments)
                {
                    expectedmin += a.second.default_value == nullptr ? 1 : 0;
                    expectedmax += 1;
                }

                if (in.size() < expectedmin || (bVarargin == false && in.size() > m_arguments.size()))
                {
                    std::wstring msg;
                    if (expectedmin != expectedmax)
                    {
                        msg = formatError(_("%s: Wrong number of input arguments: %d to %d expected.\n"), scilab::UTF8::toUTF8(m_wstName).data(), expectedmin, expectedmax);
                    }
                    else
                    {
                        if (bVarargin)
                        {
                            msg = formatError(_("%s: Wrong number of input argument(s): at least %d expected.\n"), scilab::UTF8::toUTF8(m_wstName).data(), (int)m_arguments.size());
                        }
                        else
                        {
                            msg = formatError(_("%s: Wrong number of input argument(s): %d expected.\n"), scilab::UTF8::toUTF8(m_wstName).data(), (int)m_arguments.size());
                        }
                    }

                    throw ast::InternalError(msg, 999, getBody()->getLocation());
                }
            }

            // manage default_value of all inputs before everything else
            for (int i = 0; i < m_inputArgs->size(); ++i)
            {
                auto it = m_arguments.find((*m_inputArgs)[i]->getSymbol().getName());
                if (it == m_arguments.end())
                {
                    continue;
                }

                const ARG& arg = it->second;
                if (i >= in.size())
                {
                    if (arg.default_value)
                    {
                        std::unique_ptr<ast::ConstVisitor> exec(ConfigVariable::getDefaultVisitor());
                        arg.default_value->accept(*exec);
                        InternalType* pIT = ((ast::RunVisitor*)exec.get())->getResult();
                        if (pIT == nullptr || pIT->isAssignable() == false)
                        {
                            throw ast::InternalError(formatError(_("%s: Unable to evaluate default value.\n"), scilab::UTF8::toUTF8(m_wstName).data()), 999, arg.default_value->getLocation());
                        }

                        pIT->IncreaseRef();
                        pContext->put((*m_inputArgs)[i], pIT);
                        in.push_back(pIT);
                    }
                }
            }

            // current value of each input: validators and error messages must see
            // what the function body will receive, not the raw inputs
            types::typed_list values(in.begin(), in.end());

            // 1. size then type conversion, one value per input
            for (int i = 0; i < m_inputArgs->size() && i < values.size(); ++i)
            {
                auto it = m_arguments.find((*m_inputArgs)[i]->getSymbol().getName());
                if (it == m_arguments.end())
                {
                    continue;
                }

                const ARG& arg = it->second;
                types::InternalType* cur = values[i];
                if (arg.dimsConvertor)
                {
                    // check size + expand + transpose
                    types::InternalType* p = arg.dimsConvertor(cur);
                    if (p == nullptr)
                    {
                        if (skipArgs == nullptr)
                        {
                            throw ast::InternalError(formatError(_("%s: Wrong size of input argument #%d: %ls expected.\n"), scilab::UTF8::toUTF8(m_wstName).data(), i + 1, arg.dimsStr().c_str()));
                        }

                        // no error and send "bad formatted var to function, following 'skipArguments' status"
                    }
                    else
                    {
                        cur = p;
                    }
                }

                for (auto&& convertor : arg.convertors)
                {
                    types::InternalType* p = nullptr;
                    try
                    {
                        p = convertor.convertor(cur);
                    }
                    catch (const ast::InternalError&)
                    {
                        if (cur != values[i])
                        {
                            cur->killMe();
                        }

                        throw;
                    }

                    if (p && p != cur)
                    {
                        // intermediate value (expanded or transposed) is not used anymore
                        if (cur != values[i])
                        {
                            cur->killMe();
                        }

                        cur = p;
                    }
                }

                if (cur != values[i])
                {
                    pContext->put((*m_inputArgs)[i], cur);
                    values[i] = cur;
                }
            }

            // 2. validators, once all inputs are converted (they can refer to other inputs)
            if (skipArgs == nullptr)
            {
                for (int i = 0; i < m_inputArgs->size() && i < values.size(); ++i)
                {
                    auto it = m_arguments.find((*m_inputArgs)[i]->getSymbol().getName());
                    if (it == m_arguments.end())
                    {
                        continue;
                    }

                    const ARG& arg = it->second;
                    for (auto&& validator : arg.validators)
                    {
                        types::typed_list args;
                        for (auto&& input : validator.inputs)
                        {
                            int index = std::get<0>(input);
                            args.push_back(index != -1 ? values[index] : std::get<1>(input));
                        }

                        if (validator.validator(args) == false)
                        {
                            // messages take at most 3 extra strings, unused ones are ignored by the format
                            std::string strs[3];
                            for (int k = 0; k < validator.errorArgs.size() && k < 3; ++k)
                            {
                                int pos = std::get<0>(validator.errorArgs[k]);
                                strs[k] = pos != -1 ? scilab::UTF8::toUTF8(var2str(values[pos])) : std::get<1>(validator.errorArgs[k]);
                            }

                            throw ast::InternalError(formatError(_(std::get<0>(validator.error).data()), scilab::UTF8::toUTF8(m_wstName).data(), i + 1, strs[0].data(), strs[1].data(), strs[2].data()), 999, arg.loc);
                        }
                    }
                }
            }
        }
        catch (const ast::InternalError& ie)
        {
            pContext->scope_end();
            ConfigVariable::fillWhereError(ie.GetErrorLocation());
            ConfigVariable::macroFirstLine_end();
            // return types::Function::Error;
            throw ie;
        }
    }

    // varargout management
    // rules :
    // varargout is a list
    // varargout can containt more items than caller need
    // varargout must containt at leat caller needs

    if (isLambda() == false)
    {
        if (m_outputArgs->size() >= 1 && m_outputArgs->back()->getSymbol().getName() == L"varargout")
        {
            bVarargout = true;
            List* pL = new List();
            pContext->put(m_Varargout, pL);
        }

        // iRetCount = 0 is granted to the macro (as argn(0))
        // when there is no formal output argument
        // or if varargout is the only formal output argument.
        if (m_outputArgs->size() - (bVarargout ? 1 : 0) >= 1)
        {
            iRetCount = std::max(1, iRetCount);
        }
    }

    // common part with or without varargin/varargout

    // Declare nargin & nargout in function context.
    if (m_pDblArgIn->getRef() > 1)
    {
        m_pDblArgIn->DecreaseRef();
        m_pDblArgIn = m_pDblArgIn->clone();
        m_pDblArgIn->IncreaseRef();
    }
    m_pDblArgIn->set(0, static_cast<double>(rhs));

    if (m_pDblArgOut->getRef() > 1)
    {
        m_pDblArgOut->DecreaseRef();
        m_pDblArgOut = m_pDblArgOut->clone();
        m_pDblArgOut->IncreaseRef();
    }

    m_pDblArgOut->set(0, iRetCount);

    pContext->put(m_Nargin, m_pDblArgIn);
    pContext->put(m_Nargout, m_pDblArgOut);

    // add sub macro in current context
    for (auto&& sub : m_submacro)
    {
        pContext->put(sub.first, sub.second);
    }

    if (isLambda())
    {
        // add varargout in new context
        // List* pL = new List();
        // pContext->put(m_Varargout, pL);

        for (auto&& c : m_captured)
        {
            pContext->put(symbol::Symbol(c.first), c.second);
        }
    }

    // save current prompt mode
    int oldVal = ConfigVariable::getPromptMode();
    std::wstring iExecFile = ConfigVariable::getExecutedFile();
    std::unique_ptr<ast::ConstVisitor> exec(ConfigVariable::getDefaultVisitor());
    ((ast::RunVisitor*)exec.get())->setLambda(isLambda());

    try
    {
        ConfigVariable::setExecutedFile(m_stPath);
        ConfigVariable::setPromptMode(-1);
        m_body->accept(*exec);
        // restore previous prompt mode
        ConfigVariable::setPromptMode(oldVal);
        ConfigVariable::setExecutedFile(iExecFile);
    }
    catch (const ast::InternalError& ie)
    {
        if (m_arguments.size() != 0)
        {
            types::InternalType* pIT = symbol::Context::getInstance()->get(symbol::Symbol(L"%skipArgs"));
            if (pIT)
            {
                wchar_t* func = pIT->getAs<types::String>()->get()[0];
                Sciwarning("WARNING: \"skipArguments\" was called in \"%ls\".\n", func);
            }
        }

        ConfigVariable::setExecutedFile(iExecFile);
        cleanCall(pContext, oldVal);
        throw ie;
    }
    catch (const ast::InternalAbort& ia)
    {
        ConfigVariable::setExecutedFile(iExecFile);
        cleanCall(pContext, oldVal);
        throw ia;
    }

    // nb excepted output without varargout
    int iRet = iRetCount;

    if (isLambda() == false)
    {
        iRet = std::min((int)m_outputArgs->size() - (bVarargout ? 1 : 0), std::max(1, iRetCount));

        // normal output management
        for (auto arg : *m_outputArgs)
        {
            iRet--;
            if (iRet < 0)
            {
                break;
            }

            InternalType* pIT = pContext->get(arg);
            if (pIT)
            {
                out.push_back(pIT);
                pIT->IncreaseRef();
            }
            else
            {
                const int size = (const int)out.size();
                for (int j = 0; j < size; ++j)
                {
                    out[j]->DecreaseRef();
                    out[j]->killMe();
                }
                out.clear();
                cleanCall(pContext, oldVal);

                char* pstArgName = wide_string_to_UTF8(arg->getSymbol().getName().c_str());
                char* pstMacroName = wide_string_to_UTF8(getName().c_str());
                Scierror(999, _("Undefined variable '%s' in function '%s'.\n"), pstArgName, pstMacroName);
                FREE(pstArgName);
                FREE(pstMacroName);
                return Callable::Error;
            }
        }

        // varargout management
        if (bVarargout)
        {
            InternalType* pOut = pContext->get(m_Varargout);
            if (pOut == NULL)
            {
                cleanCall(pContext, oldVal);
                Scierror(999, _("Invalid index.\n"));
                return Callable::Error;
            }

            if (pOut->isList() == false)
            {
                cleanCall(pContext, oldVal);
                char* pstMacroName = wide_string_to_UTF8(getName().c_str());
                Scierror(999, _("%s: Wrong type for %s: A list expected.\n"), pstMacroName, "Varargout");
                FREE(pstMacroName);
                return Callable::Error;
            }

            List* pVarOut = pOut->getAs<List>();
            const int size = std::min(pVarOut->getSize(), std::max(1, iRetCount) - (int)out.size());
            for (int i = 0; i < size; ++i)
            {
                InternalType* pIT = pVarOut->get(i);
                if (pIT->isVoid())
                {
                    for (int j = 0; j < i; ++j)
                    {
                        out[j]->DecreaseRef();
                        out[j]->killMe();
                    }
                    out.clear();
                    cleanCall(pContext, oldVal);

                    Scierror(999, _("List element number %d is Undefined.\n"), i + 1);
                    return Callable::Error;
                }

                pIT->IncreaseRef();
                out.push_back(pIT);
            }
        }
    }
    else
    {
        InternalType* pOut = pContext->getCurrentLevel(m_Varargout);
        if (pOut == NULL)
        {
            types::InternalType* result = ((ast::RunVisitor*)exec.get())->getLambdaResult();
            if (result)
            {
                types::InternalType* p = result;
                p->IncreaseRef();
                out.push_back(p);
                ((ast::RunVisitor*)exec.get())->clearLambdaResult();
            }
        }
        else
        {
            if (pOut->isList() == false)
            {
                cleanCall(pContext, oldVal);
                char* pstMacroName = wide_string_to_UTF8(getName().c_str());
                Scierror(999, _("%s: Wrong type for %s: A list expected.\n"), pstMacroName, "Varargout");
                FREE(pstMacroName);
                return Callable::Error;
            }

            List* pVarOut = pOut->getAs<List>();
            const int size = std::min(pVarOut->getSize(), iRetCount);
            for (int i = 0; i < size; ++i)
            {
                types::InternalType* p = pVarOut->get(i);
                p->IncreaseRef();
                out.push_back(p);
            }
        }
    }

    // close the current scope
    cleanCall(pContext, oldVal);

    // reduce ref of outputs to case of in and out have same symbol
    for (typed_list::iterator i = out.begin(), end = out.end(); i != end; ++i)
    {
        (*i)->DecreaseRef();
    }

    return RetVal;
}

std::vector<symbol::Variable*>* Macro::getInputs()
{
    return m_inputArgs;
}

std::vector<symbol::Variable*>* Macro::getOutputs()
{
    return m_outputArgs;
}

int Macro::getNbInputArgument(void)
{
    return (int)m_inputArgs->size();
}

int Macro::getNbOutputArgument(void)
{
    if (isLambda())
    {
        return -1; // will be manage later in call()
    }

    if (m_outputArgs->size() >= 1 && m_outputArgs->back()->getSymbol().getName() == L"varargout")
    {
        return -1;
    }

    return (int)m_outputArgs->size();
}

bool Macro::getMemory(long long* _piSize, long long* _piSizePlusType)
{
    ast::SerializeVisitor serialMacro(m_body);
    unsigned char* macroSerial = serialMacro.serialize(false, false);
    unsigned int macroSize = *((unsigned int*)macroSerial);

    *_piSize = macroSize;
    *_piSizePlusType = *_piSize + sizeof(Macro);
    return true;
}

bool Macro::operator==(const InternalType& it)
{
    if (const_cast<InternalType&>(it).isMacro() == false)
    {
        return false;
    }

    std::vector<symbol::Variable*>* pInput = NULL;
    std::vector<symbol::Variable*>* pOutput = NULL;
    types::Macro* pRight = const_cast<InternalType&>(it).getAs<types::Macro>();

    if (pRight->isLambda() != isLambda())
    {
        return false;
    }

    // check inputs
    pInput = pRight->getInputs();
    if (pInput->size() != m_inputArgs->size())
    {
        return false;
    }

    std::vector<symbol::Variable*>::iterator itOld = pInput->begin();
    std::vector<symbol::Variable*>::iterator itEndOld = pInput->end();
    std::vector<symbol::Variable*>::iterator itMacro = m_inputArgs->begin();

    for (; itOld != itEndOld; ++itOld, ++itMacro)
    {
        if ((*itOld)->getSymbol() != (*itMacro)->getSymbol())
        {
            return false;
        }
    }

    if (isLambda() == false)
    {
        // check outputs
        pOutput = pRight->getOutputs();
        if (pOutput->size() != m_outputArgs->size())
        {
            return false;
        }

        itOld = pOutput->begin();
        itEndOld = pOutput->end();
        itMacro = m_outputArgs->begin();

        for (; itOld != itEndOld; ++itOld, ++itMacro)
        {
            if ((*itOld)->getSymbol() != (*itMacro)->getSymbol())
            {
                return false;
            }
        }
    }

    ast::Exp* pExp = pRight->getBody();
    ast::SerializeVisitor serialOld(pExp);
    unsigned char* oldSerial = serialOld.serialize(false, false);
    ast::SerializeVisitor serialMacro(m_body);
    unsigned char* macroSerial = serialMacro.serialize(false, false);

    // check buffer length
    unsigned int oldSize = *((unsigned int*)oldSerial);
    unsigned int macroSize = *((unsigned int*)macroSerial);
    if (oldSize != macroSize)
    {
        return false;
    }

    bool ret = (memcmp(oldSerial, macroSerial, oldSize) == 0);
    return ret;
}

void Macro::add_submacro(const symbol::Symbol& s, Macro* macro)
{
    macro->IncreaseRef();
    symbol::Context* ctx = symbol::Context::getInstance();
    symbol::Variable* var = ctx->getOrCreate(s);
    m_submacro[var] = macro;
}

void Macro::updateArguments()
{
    // build a map of inputs argument name and position
    std::vector<std::wstring> inputNames;
    for (auto&& in : *m_inputArgs)
    {
        inputNames.push_back(in->getSymbol().getName());
    }

    bool needDefaultValue = false;
    bool bvarargin = false;
    for (auto&& e : m_body->getExps())
    {
        if (e->isCommentExp())
            continue;

        if (e->isArgumentsExp())
        {
            for (int i = 0; i < e->getExps().size(); ++i)
            {
                ast::Exp* d = e->getExps()[i];
                if (d->isCommentExp())
                    continue;

                ast::ArgumentDec* dec = d->getAs<ast::ArgumentDec>();
                std::wstring name;
                if (dec->getArgumentName()->isSimpleVar())
                {
                    std::vector<std::wstring> allowedVar = {L"%eps", L"%i", L"%inf", L"%nan"};
                    name = dec->getArgumentName()->getAs<ast::SimpleVar>()->getSymbol().getName();
                    if (std::find(allowedVar.begin(), allowedVar.end(), name) == allowedVar.end())
                    {
                        if (m_arguments.size() >= inputNames.size() || inputNames[m_arguments.size()] != name)
                        {
                            std::wstring msg;
                            if (std::find(inputNames.begin(), inputNames.end(), name) == inputNames.end())
                            {
                                msg = formatError(_("%s: Identifier '%s' must be an input argument.\n"), scilab::UTF8::toUTF8(m_wstName).data(), scilab::UTF8::toUTF8(name).data());
                            }
                            else
                            {
                                msg = formatError(_("%s: Identifier must be define in same order that parameters.\n"), scilab::UTF8::toUTF8(m_wstName).data());
                            }

                            throw ast::InternalError(msg, 999, dec->getArgumentType()->getLocation());
                        }
                    }
                }
                else // FieldExp
                {
                    /*
                    const ast::FieldExp* f = dec->getArgumentName()->getAs<ast::FieldExp>();
                    name = f->getHead()->getAs<ast::SimpleVar>()->getSymbol().getName();
                    if (m_arguments.size() >= inputNames.size() || inputNames[m_arguments.size()] != name)
                    {
                        char msg[128];
                        if (std::find(inputNames.begin(), inputNames.end(), name) == inputNames.end())
                        {
                            os_sprintf(msg, _("%s: Identifier '%s' must be an input argument.\n"), scilab::UTF8::toUTF8(m_wstName).data(), scilab::UTF8::toUTF8(name).data());
                        }
                        else
                        {
                            os_sprintf(msg, _("%s: Identifier must be define in same order that parameters.\n"), scilab::UTF8::toUTF8(m_wstName).data());
                        }

                        throw ast::InternalError(scilab::UTF8::toWide(msg), 999, dec->getArgumentType()->getLocation());
                    }

                    name += L".";
                    name += f->getTail()->getAs<ast::SimpleVar>()->getSymbol().getName();
                    */

                    throw ast::InternalError(formatError(_("%s: Expression with field are not managed.\n"), "arguments"), 999, dec->getArgumentType()->getLocation());
                }

                if (name == L"varargin")
                {
                    // check that there is no information !
                    if (dec->getArgumentDims()->getExps().size() != 0 ||
                        dec->getArgumentDefaultValue()->getExps().size() != 0 ||
                        dec->getArgumentType()->getExps().size() != 0 ||
                        dec->getArgumentValidators()->getExps().size() != 0)
                    {
                        throw ast::InternalError(formatError(_("%s: varargin must be declared without parameter.\n"), "arguments"), 999, dec->getArgumentType()->getLocation());
                    }

                    bvarargin = true;
                    continue;
                }

                ARG arg;
                arg.loc = d->getLocation();

                // dims
                std::vector<std::tuple<std::vector<int>, symbol::Variable*>> dims = {};
                for (auto&& dim : dec->getArgumentDims()->getExps())
                {
                    // TODO
                    if (dim->isSimpleVar())
                    {
                        std::wstring name = dim->getAs<ast::SimpleVar>()->getSymbol().getName();
                        if (std::find(inputNames.begin(), inputNames.end(), name) == inputNames.end())
                        {
                            throw ast::InternalError(formatError(_("%s: Dimension must be an input parameter.\n"), scilab::UTF8::toUTF8(m_wstName).data()), 999, dec->getArgumentType()->getLocation());
                        }

                        symbol::Variable* var = symbol::Context::getInstance()->getOrCreate(dim->getAs<ast::SimpleVar>()->getSymbol());
                        dims.push_back({{-1}, var});
                    }
                    else if (dim->isColonVar())
                    {
                        dims.push_back({{-1}, nullptr});
                    }
                    else if (dim->isDoubleExp())
                    {
                        dims.push_back({{static_cast<int>(dim->getAs<ast::DoubleExp>()->getValue())}, nullptr});
                    }
                    else if (dim->isMatrixExp())
                    {
                        std::vector<int> d;
                        // allow only one line matrix
                        ast::MatrixExp* m = dim->getAs<ast::MatrixExp>();
                        if (m->getLines().size() != 1)
                        {
                            throw ast::InternalError(formatError(_("%s: Dimension must be a number, row vector or ':'.\n"), scilab::UTF8::toUTF8(m_wstName).data()), 999, dec->getArgumentType()->getLocation());
                        }

                        ast::MatrixLineExp* ml = m->getLines()[0]->getAs<ast::MatrixLineExp>();
                        for (auto&& c : ml->getColumns())
                        {
                            if (c->isDoubleExp() == false)
                            {
                                throw ast::InternalError(formatError(_("%s: Dimension must be a number, row vector or ':'.\n"), scilab::UTF8::toUTF8(m_wstName).data()), 999, dec->getArgumentType()->getLocation());
                            }

                            d.push_back(static_cast<int>(c->getAs<ast::DoubleExp>()->getValue()));
                        }

                        dims.push_back({d, nullptr});
                    }
                    else
                    {
                        throw ast::InternalError(formatError(_("%s: Dimension must be a number, row vector or ':'.\n"), scilab::UTF8::toUTF8(m_wstName).data()), 999, dec->getArgumentType()->getLocation());
                    }
                }

                arg.dimsConvertor = nullptr;
                if (dims.size() != 0)
                {
                    bool isStatic = checkStaticDims(dims);

                    arg.dimsConvertor = [dims, isStatic](types::InternalType* x)
                    { return checksize(x, dims, isStatic); };
                    arg.dimsStr = [dims]()
                    { return dims2str(dims); };
                }

                // conversion
                if (dec->getArgumentType()->isSimpleVar())
                {
                    ARG_CONVERTOR argConv;
                    std::wstring name = dec->getArgumentType()->getAs<ast::SimpleVar>()->getSymbol().getName();
                    auto f = getTypeConvertor(name);
                    if (f == nullptr)
                    {
                        throw ast::InternalError(formatError(_("%s: Unknown conversion function '%s'\n"), scilab::UTF8::toUTF8(m_wstName).data(), scilab::UTF8::toUTF8(name).data()), 999, dec->getArgumentType()->getLocation());
                    }

                    argConv.convertor = [f, this](types::InternalType* x)
                    { return f(x, m_wstName); };
                    arg.convertors.push_back(argConv);
                }

                // default value
                if (dec->getArgumentDefaultValue()->isNilExp() == false)
                {
                    needDefaultValue = true;
                    arg.default_value = dec->getArgumentDefaultValue();
                }
                else if (needDefaultValue)
                {
                    throw ast::InternalError(formatError(_("%s: Identifier '%s' needs a default value.\n"), scilab::UTF8::toUTF8(m_wstName).data(), scilab::UTF8::toUTF8(name).data()), 999, dec->getArgumentType()->getLocation());
                }

                // validators
                for (auto&& v : dec->getArgumentValidators()->getExps())
                {
                    if (v->isSimpleVar())
                    {
                        std::vector<int> rhs;
                        std::wstring name = v->getAs<ast::SimpleVar>()->getSymbol().getName();
                        std::function<int(typed_list&)> f;
                        std::tie(f, rhs) = getFunctionValidator(name);

                        if (f == nullptr)
                        {
                            throw ast::InternalError(formatError(_("%s: Unknown validation function '%s'\n"), scilab::UTF8::toUTF8(m_wstName).data(), scilab::UTF8::toUTF8(name).data()), 999, dec->getArgumentType()->getLocation());
                        }

                        if (rhs[0] != 1)
                        {
                            throw ast::InternalError(formatError(_("%s: Wrong number of input argument(s): %d expected.\n"), scilab::UTF8::toUTF8(m_wstName).data(), 1), 999, dec->getArgumentType()->getLocation());
                        }

                        ARG_VALIDATOR argValidator;
                        argValidator.validator = f;
                        argValidator.inputs.push_back({i, nullptr});

                        argValidator.error = getErrorValidator(name);
                        auto args = getErrorArgs(name);
                        for (int k = 0; k < args.size(); ++k)
                        {
                            if (std::get<0>(args[k]) != -1)
                            {
                                // never occur
                                sciprint("arg != -1");
                                argValidator.errorArgs.push_back({-1, "arg != -1"});
                            }
                            else
                            {
                                int pos = std::get<0>(argValidator.inputs[std::get<0>(args[k])]) + 1;
                                argValidator.errorArgs.push_back({pos, ""});
                            }
                        }

                        arg.validators.push_back(argValidator);
                    }
                    else // CallExp
                    {
                        ast::CallExp* c = v->getAs<ast::CallExp>();
                        std::vector<int> rhs;
                        std::wstring name = (&c->getName())->getAs<ast::SimpleVar>()->getSymbol().getName();
                        std::function<int(typed_list&)> f;
                        std::tie(f, rhs) = getFunctionValidator(name);
                        int size = static_cast<int>(c->getArgs().size());

                        if (f == nullptr)
                        {
                            throw ast::InternalError(formatError(_("%s: \"%ls\" is not a validation function.\n"), scilab::UTF8::toUTF8(m_wstName).data(), name.c_str()), 999, dec->getArgumentType()->getLocation());
                        }

                        if (rhs.size() == 1)
                        {
                            if (rhs[0] != size && rhs[0] != -1)
                            {
                                throw ast::InternalError(formatError(_("%s: Wrong number of input argument(s): %d expected.\n"), scilab::UTF8::toUTF8(m_wstName).data(), rhs[0]), 999, dec->getArgumentType()->getLocation());
                            }
                        }
                        else
                        {
                            if (size < rhs[0] || size > rhs[1])
                            {
                                throw ast::InternalError(formatError(_("%s: Wrong number of input argument(s): between %d and %d expected.\n"), scilab::UTF8::toUTF8(m_wstName).data(), rhs[0], rhs[1]), 999, dec->getArgumentType()->getLocation());
                            }
                        }

                        ARG_VALIDATOR argValidator;
                        argValidator.validator = f;
                        ast::exps_t inputs = c->getArgs();
                        for (int i = 0; i < size; ++i)
                        {
                            if (inputs[i]->isSimpleVar())
                            {
                                std::vector<std::wstring> allowedVar = {L"%eps", L"%i", L"%inf", L"%nan", L"this"};
                                std::wstring name = inputs[i]->getAs<ast::SimpleVar>()->getSymbol().getName();
                                if (std::find(allowedVar.begin(), allowedVar.end(), name) == allowedVar.end())
                                {
                                    if (std::find(inputNames.begin(), inputNames.end(), name) == inputNames.end())
                                    {
                                        throw ast::InternalError(formatError(_("%s: Identifier '%s' must be an input argument.\n"), scilab::UTF8::toUTF8(m_wstName).data(), scilab::UTF8::toUTF8(name).data()), 999, dec->getArgumentType()->getLocation());
                                    }

                                    int pos = static_cast<int>(std::find(inputNames.begin(), inputNames.end(), inputs[i]->getAs<ast::SimpleVar>()->getSymbol().getName()) - inputNames.begin());
                                    argValidator.inputs.push_back({pos, nullptr});
                                }
                                else
                                {
                                    types::InternalType* pIT = symbol::Context::getInstance()->get(symbol::Symbol(name));
                                    if (pIT == nullptr)
                                    {
                                        argValidator.inputs.push_back({-1, nullptr});
                                    }
                                    else
                                    {
                                        pIT->IncreaseRef();
                                        argValidator.inputs.push_back({-1, pIT});
                                    }
                                }
                            }
                            else // constant
                            {
                                if (checkArgument(inputs[i]) == false)
                                {
                                    throw ast::InternalError(formatError(_("%s: argument must be constant expression.\n"), scilab::UTF8::toUTF8(m_wstName).data()), 999, dec->getArgumentType()->getLocation());
                                }

                                std::unique_ptr<ast::ConstVisitor> exec(ConfigVariable::getDefaultVisitor());
                                try
                                {
                                    inputs[i]->accept(*exec);
                                }
                                catch (const ast::InternalError& /*ie*/)
                                {
                                    throw ast::InternalError(formatError(_("%s: Unable to evaluate argument.\n"), scilab::UTF8::toUTF8(m_wstName).data()), 999, inputs[i]->getLocation());
                                }

                                types::InternalType* pIT = ((ast::RunVisitor*)exec.get())->getResult();
                                if (pIT == nullptr || pIT->isAssignable() == false)
                                {
                                    throw ast::InternalError(formatError(_("%s: Unable to evaluate argument.\n"), scilab::UTF8::toUTF8(m_wstName).data()), 999, inputs[i]->getLocation());
                                }

                                pIT->IncreaseRef();
                                argValidator.inputs.push_back({-1, pIT});
                            }
                        }

                        argValidator.error = getErrorValidator(name);
                        auto args = getErrorArgs(name);
                        for (int k = 0; k < args.size(); ++k)
                        {
                            int idx = std::get<0>(args[k]);
                            if (idx > 0) // #num of variable
                            {
                                int pos = std::get<0>(argValidator.inputs[idx]) + 1;
                                argValidator.errorArgs.push_back({-1, std::to_string(pos)});
                            }
                            else // content of variable
                            {
                                types::InternalType* pIT = std::get<1>(argValidator.inputs[std::abs(idx)]);
                                if (pIT) // data from variable
                                {
                                    std::wstring output = var2str(pIT);
                                    argValidator.errorArgs.push_back({-1, scilab::UTF8::toUTF8(output)});
                                }
                                else // use var content of variable as message information
                                {
                                    int pos = std::get<0>(argValidator.inputs[std::abs(idx)]);
                                    argValidator.errorArgs.push_back({pos, ""});
                                }
                            }
                        }

                        arg.validators.push_back(argValidator);
                    }
                }

                m_arguments[name] = arg;
            } // for

            if (m_arguments.size() + (bvarargin ? 1 : 0) != m_inputArgs->size())
            {
                throw ast::InternalError(formatError(_("%s: All parameters must be specified in arguments block.\n"), scilab::UTF8::toUTF8(m_wstName).data()), 999, e->getLocation());
            }
        }
    }
}

bool Macro::checkArgument(ast::Exp* exp)
{
    ast::ArgumentVisitor v;
    exp->accept(v);
    return v.getStatus();
}

bool Macro::checkStaticDims(const std::vector<std::tuple<std::vector<int>, symbol::Variable*>>& dims)
{
    bool res = true;
    for (auto&& d : dims)
    {
        if (std::get<1>(d) == nullptr)
        {
            return false;
        }

        if (std::get<0>(d).size() != 1)
        {
            return false;
        }
    }

    return res;
}
} // namespace types
