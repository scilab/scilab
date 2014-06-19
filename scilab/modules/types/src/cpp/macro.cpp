/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <sstream>
#include <cstdio>

#include "macro.hxx"
#include "list.hxx"
#include "context.hxx"
#include "symbol.hxx"
#include "scilabWrite.hxx"
#include "scilabexception.hxx"
#include "configvariable.hxx"
#include "mutevisitor.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"
#include "sci_malloc.h"
#include "os_swprintf.h"
}

namespace types
{
Macro::Macro(const std::wstring& _stName, std::list<symbol::Variable*>& _inputArgs, std::list<symbol::Variable*>& _outputArgs, ast::SeqExp &_body, const std::wstring& _stModule):
    Callable(),
    m_inputArgs(&_inputArgs), m_outputArgs(&_outputArgs), m_body(&_body),
    m_Nargin(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"nargin"))),
    m_Nargout(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"nargout"))),
    m_Varargin(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"varargin"))),
    m_Varargout(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"varargout")))
{
    setName(_stName);
    setModule(_stModule);
    bAutoAlloc = false;
    m_pDblArgIn = new Double(1);
    m_pDblArgIn->IncreaseRef(); //never delete
    m_pDblArgOut = new Double(1);
    m_pDblArgOut->IncreaseRef(); //never delete

}

Macro::~Macro()
{
    delete m_body;
}

InternalType* Macro::clone()
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
    ostr << L"FIXME : Implement Macro::toString" << std::endl;
    return true;
}

Callable::ReturnValue Macro::call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc)
{
    bool bVarargout = false;
    ReturnValue RetVal = Callable::OK;
    symbol::Context *pContext = symbol::Context::getInstance();

    //open a new scope
    pContext->scope_begin();
    //store the line number where is stored this macro in file.
    ConfigVariable::macroFirstLine_begin(getFirstLine());

    //add optional paramter in current scope
    optional_list::const_iterator it;
    for (it = opt.begin() ; it != opt.end() ; it++)
    {
        pContext->put(symbol::Symbol(it->first), it->second);
    }
    //check excepted and input/output parameters numbers
    // Scilab Macro can be called with less than prototyped arguments,
    // but not more execpts with varargin

    // varargin management
    if (m_inputArgs->size() > 0 && m_inputArgs->back()->name_get().name_get() == L"varargin")
    {
        int iVarPos = static_cast<int>(in.size());
        if (iVarPos > static_cast<int>(m_inputArgs->size()) - 1)
        {
            iVarPos = static_cast<int>(m_inputArgs->size()) - 1;
        }

        //add all standard variable in function context but not varargin
        std::list<symbol::Variable*>::iterator itName = m_inputArgs->begin();
        typed_list::const_iterator itValue = in.begin();
        while (iVarPos > 0)
        {
            pContext->put(*itName, *itValue);
            iVarPos--;
            ++itName;
            ++itValue;
        }

        //create varargin only if previous variable are assigned
        if (in.size() >= m_inputArgs->size() - 1)
        {
            //create and fill varargin
            List* pL = new List();
            while (itValue != in.end())
            {
                pL->append(*itValue);
                itValue++;
            }
            pContext->put(m_Varargin, pL);
        }
    }
    else if (in.size() > m_inputArgs->size())
    {
       

        Scierror(999, _("Wrong number of input arguments: %d expected.\n"), m_inputArgs->size());
        pContext->scope_end();
        ConfigVariable::macroFirstLine_end();
        return Callable::Error;
    }
    else
    {
        //assign value to variable in the new context
        std::list<symbol::Variable*>::iterator i;
        typed_list::const_iterator j;

        for (i = m_inputArgs->begin(), j = in.begin(); j != in.end (); ++j, ++i)
        {
            if (*j)
            {
                //prevent assignation of NULL value
                pContext->put(*i, *j);
            }
        }
    }

    // varargout management
    //rules :
    // varargout must be alone
    // varargout is a list
    // varargout can containt more items than caller need
    // varargout must containt at leat caller needs
    if (m_outputArgs->size() == 1 && m_outputArgs->back()->name_get().name_get() == L"varargout")
    {
        bVarargout = true;
        List* pL = new List();
        pContext->put(m_Varargout, pL);
    }

    //common part with or without varargin/varargout

    // Declare nargin & nargout in function context.
    m_pDblArgIn->set(0, static_cast<double>(in.size()));
    m_pDblArgOut->set(0, _iRetCount);
    pContext->put(m_Nargin, m_pDblArgIn);
    pContext->put(m_Nargout, m_pDblArgOut);

    //save current prompt mode
    int oldVal = ConfigVariable::getPromptMode();
    try
    {
        //m_body->mute();
        //MuteVisitor mute;
        //m_body->accept(mute);

        ConfigVariable::setPromptMode(-1);
        m_body->returnable_set();
        m_body->accept(*execFunc);
        //restore previous prompt mode
        ConfigVariable::setPromptMode(oldVal);
        if (m_body->is_return())
        {
            m_body->returnable_set();
        }

        //varargout management
        if (bVarargout)
        {
            InternalType* pOut = pContext->get(m_Varargout);
            if (pOut == NULL)
            {
                Scierror(999, _("Invalid index.\n"));
                return Callable::Error;
            }

            if (pOut->isList() == false || pOut->getAs<List>()->getSize() == 0)
            {
                Scierror(999, _("Invalid index.\n"));
                return Callable::Error;
            }

            List* pVarOut = pOut->getAs<List>();
            for (int i = 0 ; i < Max(pVarOut->getSize(), _iRetCount) ; i++)
            {
                InternalType* pIT = pVarOut->get(i)->clone();
                if (pIT->isListUndefined())
                {
                    Scierror(999, _("List element number %d is Undefined.\n"), i + 1);
                    return Callable::Error;
                }

                out.push_back(pIT);
            }
        }
        else
        {
            //normal output management
            std::list<symbol::Variable*>::iterator i;
            for (i = m_outputArgs->begin(); i != m_outputArgs->end() && _iRetCount; ++i, --_iRetCount)
            {
                InternalType *pIT = pContext->get((*i));
                if (pIT != NULL)
                {
                    out.push_back(pIT);
                    pIT->IncreaseRef();
                }
                else
                {
                    char* pst = wide_string_to_UTF8((*i)->name_get().name_get().c_str());
                    Scierror(999, _("Undefined variable %s.\n"), pst);
                    FREE(pst);
                    return Callable::Error;
                }
            }
        }
    }
    catch (ast::ScilabMessage se)
    {
        //restore previous prompt mode
        ConfigVariable::setPromptMode(oldVal);
        //close the current scope
        pContext->scope_end();
        ConfigVariable::macroFirstLine_end();
        for (size_t j = 0; j < out.size(); ++j)
        {
            out[j]->DecreaseRef();
        }
        throw se;
    }
    catch (ast::InternalAbort sa)
    {
        //restore previous prompt mode
        ConfigVariable::setPromptMode(oldVal);
        //close the current scope
        pContext->scope_end();
        ConfigVariable::macroFirstLine_end();
        for (size_t j = 0; j < out.size(); ++j)
        {
            out[j]->DecreaseRef();
        }
        throw sa;
    }

    //close the current scope
    pContext->scope_end();
    ConfigVariable::macroFirstLine_end();

    for (size_t j = 0; j < out.size(); ++j)
    {
        out[j]->DecreaseRef();
    }
    return RetVal;
}

std::list<symbol::Variable*>* Macro::inputs_get()
{
    return m_inputArgs;
}

std::list<symbol::Variable*>* Macro::outputs_get()
{
    return m_outputArgs;
}

int Macro::getNbInputArgument(void)
{
    return (int)m_inputArgs->size();
}

int Macro::getNbOutputArgument(void)
{
    if (m_outputArgs->size() == 1 && m_outputArgs->back()->name_get().name_get() == L"varargout")
    {
        return -1;
    }

    return (int)m_outputArgs->size();
}
}
