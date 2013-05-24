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
#include "localization.h"
#include "scilabWrite.hxx"
#include "scilabexception.hxx"
#include "configvariable.hxx"
#include "mutevisitor.hxx"

extern "C"
{
#include "Scierror.h"
#include "MALLOC.h"
#include "os_swprintf.h"
}

namespace types
{
Macro::Macro(const std::wstring& _stName, std::list<symbol::Symbol> &_inputArgs, std::list<symbol::Symbol> &_outputArgs, ast::SeqExp &_body, const wstring& _stModule):
    Callable(), m_inputArgs(&_inputArgs), m_outputArgs(&_outputArgs), m_body(&_body),
    m_ArgInSymb(symbol::Symbol(L"nargin")), m_ArgOutSymb(symbol::Symbol(L"nargout"))
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

InternalType::RealType Macro::getType(void)
{
    return RealMacro;
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

    //add optional paramter in current scope
    optional_list::const_iterator it;
    for (it = opt.begin() ; it != opt.end() ; it++)
    {
        pContext->put(symbol::Symbol(it->first), *it->second);
    }
    //check excepted and input/output parameters numbers
    // Scilab Macro can be called with less than prototyped arguments,
    // but not more execpts with varargin

    // varargin management
    if (m_inputArgs->size() > 0 && m_inputArgs->back().name_get() == L"varargin")
    {
        int iVarPos = static_cast<int>(in.size());
        if (iVarPos > static_cast<int>(m_inputArgs->size()) - 1)
        {
            iVarPos = static_cast<int>(m_inputArgs->size()) - 1;
        }

        //add all standard variable in function context but not varargin
        std::list<symbol::Symbol>::const_iterator itName = m_inputArgs->begin();
        typed_list::const_iterator itValue = in.begin();
        while (iVarPos > 0)
        {
            pContext->put((*itName), **itValue);
            iVarPos--;
            itName++;
            itValue++;
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
            pContext->put(symbol::Symbol(L"varargin"), *pL);
        }
    }
    else if (in.size() > m_inputArgs->size())
    {
        wostringstream ostr;
        ostr << _W("Wrong number of input arguments.") << std::endl << std::endl;
        ostr << _W("Excepted: ") << m_inputArgs->size() << std::endl;

        if (m_inputArgs->size() > 0)
        {
            ostr << _W("Arguments are:") << std::endl << std::endl;
            ostr << " ";
            for (std::list<symbol::Symbol>::iterator it =  m_inputArgs->begin() ; it != m_inputArgs->end() ; ++it)
            {
                ostr << (*it).name_get() << L"    ";
            }
            ostr << std::endl;
        }

        char* pst = wide_string_to_UTF8(ostr.str().c_str());
        Scierror(58, "%s", pst);
        pContext->scope_end();
        return Callable::Error;
    }
    else
    {
        //assign value to variable in the new context
        std::list<symbol::Symbol>::const_iterator i;
        typed_list::const_iterator j;

        for (i = m_inputArgs->begin(), j = in.begin(); j != in.end (); ++j, ++i)
        {
            if (*j)
            {
                //prevent assignation of NULL value
                pContext->put((*i), **j);
            }
        }
    }

    // varargout management
    //rules :
    // varargout must be alone
    // varargout is a list
    // varargout can containt more items than caller need
    // varargout must containt at leat caller needs
    if (m_outputArgs->size() == 1 && m_outputArgs->back().name_get() == L"varargout")
    {
        bVarargout = true;
        List* pL = new List();
        pContext->put(symbol::Symbol(L"varargout"), *pL);
    }

    //common part with or without varargin/varargout

    // Declare nargin & nargout in function context.
    m_pDblArgIn->set(0, static_cast<double>(in.size()));
    m_pDblArgOut->set(0, _iRetCount);
    pContext->put(m_ArgInSymb, *m_pDblArgIn);
    pContext->put(m_ArgOutSymb, *m_pDblArgOut);

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
            InternalType* pOut = pContext->get(symbol::Symbol(L"varargout"));
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
            for (int i = 0 ; i < Min(pVarOut->getSize(), _iRetCount) ; i++)
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
            std::list<symbol::Symbol>::const_iterator i;
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
                    char* pst = wide_string_to_UTF8((*i).name_get().c_str());
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
        for (size_t j = 0; j < out.size(); ++j)
        {
            out[j]->DecreaseRef();
        }
        throw sa;
    }

    //close the current scope
    pContext->scope_end();

    for (size_t j = 0; j < out.size(); ++j)
    {
        out[j]->DecreaseRef();
    }
    return RetVal;
}

std::list<symbol::Symbol>* Macro::inputs_get()
{
    return m_inputArgs;
}

std::list<symbol::Symbol>* Macro::outputs_get()
{
    return m_outputArgs;
}
}
