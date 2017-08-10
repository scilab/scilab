/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
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

#include <memory>
#include <sstream>
#include <cstdio>

#include "macro.hxx"
#include "list.hxx"
#include "context.hxx"
#include "symbol.hxx"
#include "scilabWrite.hxx"
#include "configvariable.hxx"
#include "serializervisitor.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"
#include "sci_malloc.h"
#include "os_string.h"
}

namespace types
{
Macro::Macro(const std::wstring& _stName, std::list<symbol::Variable*>& _inputArgs, std::list<symbol::Variable*>& _outputArgs, ast::SeqExp &_body, const std::wstring& _stModule):
    Callable(),
    m_inputArgs(&_inputArgs), m_outputArgs(&_outputArgs), m_body(_body.clone()),
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

    m_body->setReturnable();
    m_stPath = L"";
}

Macro::~Macro()
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

    for (const auto & sub : m_submacro)
    {
        sub.second->DecreaseRef();
        sub.second->killMe();
    }

    m_submacro.clear();
}

void Macro::cleanCall(symbol::Context * pContext, int oldPromptMode)
{
    //restore previous prompt mode
    ConfigVariable::setPromptMode(oldPromptMode);
    //close the current scope
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
    ostr << L"[";

    // output arguments [a,b,c] = ....
    if (m_outputArgs->empty() == false)
    {
        std::list<symbol::Variable*>::iterator OutArg = m_outputArgs->begin();
        std::list<symbol::Variable*>::iterator OutArgfter = OutArg;
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
    ostr << L"=" << wcsVarName << L"(";

    // input arguments function(a,b,c)
    if (m_inputArgs->empty() == false)
    {
        std::list<symbol::Variable*>::iterator inArg = m_inputArgs->begin();
        std::list<symbol::Variable*>::iterator inRagAfter = inArg;
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

Callable::ReturnValue Macro::call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out)
{
    bool bVarargout = false;
    ReturnValue RetVal = Callable::OK;
    symbol::Context *pContext = symbol::Context::getInstance();

    //open a new scope
    pContext->scope_begin();
    //store the line number where is stored this macro in file.
    ConfigVariable::macroFirstLine_begin(getFirstLine());

    //check excepted and input/output parameters numbers
    // Scilab Macro can be called with less than prototyped arguments,
    // but not more execpts with varargin

    // varargin management
    if (m_inputArgs->size() > 0 && m_inputArgs->back()->getSymbol().getName() == L"varargin")
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
        optional_list::const_iterator it = opt.begin();
        if (in.size() >= m_inputArgs->size() - 1)
        {
            //create and fill varargin
            List* pL = new List();
            while (itValue != in.end())
            {
                if (*itValue != NULL)
                {
                    pL->append(*itValue);
                }
                else
                {
                    pL->append(it->second);
                    it++;
                }

                itValue++;
            }
            pContext->put(m_Varargin, pL);
        }
    }
    else if (in.size() > m_inputArgs->size())
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
        ConfigVariable::macroFirstLine_end();
        return Callable::Error;
    }
    else
    {
        //assign value to variable in the new context
        std::list<symbol::Variable*>::iterator i;
        typed_list::const_iterator j;

        for (i = m_inputArgs->begin(), j = in.begin(); j != in.end(); ++j, ++i)
        {
            if (*j)
            {
                //prevent assignation of NULL value
                pContext->put(*i, *j);
            }
        }

        //add optional paramter in current scope
        optional_list::const_iterator it;
        for (it = opt.begin() ; it != opt.end() ; it++)
        {
            pContext->put(symbol::Symbol(it->first), it->second);
        }


    }

    // varargout management
    //rules :
    // varargout must be alone
    // varargout is a list
    // varargout can containt more items than caller need
    // varargout must containt at leat caller needs
    if (m_outputArgs->size() == 1 && m_outputArgs->back()->getSymbol().getName() == L"varargout")
    {
        bVarargout = true;
        List* pL = new List();
        pContext->put(m_Varargout, pL);
    }

    //common part with or without varargin/varargout

    // Declare nargin & nargout in function context.
    if (m_pDblArgIn->getRef() > 1)
    {
        m_pDblArgIn->DecreaseRef();
        m_pDblArgIn = (Double*)m_pDblArgIn->clone();
        m_pDblArgIn->IncreaseRef();
    }
    m_pDblArgIn->set(0, static_cast<double>(in.size()));

    if (m_pDblArgOut->getRef() > 1)
    {
        m_pDblArgOut->DecreaseRef();
        m_pDblArgOut = (Double*)m_pDblArgOut->clone();
        m_pDblArgOut->IncreaseRef();
    }
    m_pDblArgOut->set(0, _iRetCount);

    pContext->put(m_Nargin, m_pDblArgIn);
    pContext->put(m_Nargout, m_pDblArgOut);


    //add sub macro in current context
    for (const auto & sub : m_submacro)
    {
        pContext->put(sub.first, sub.second);
    }

    //save current prompt mode
    int oldVal = ConfigVariable::getPromptMode();
    std::unique_ptr<ast::ConstVisitor> exec (ConfigVariable::getDefaultVisitor());
    try
    {
        ConfigVariable::setPromptMode(-1);
        m_body->accept(*exec);
        //restore previous prompt mode
        ConfigVariable::setPromptMode(oldVal);
    }
    catch (const ast::InternalError& ie)
    {
        cleanCall(pContext, oldVal);
        throw ie;
    }
    catch (const ast::InternalAbort& ia)
    {
        cleanCall(pContext, oldVal);
        throw ia;
    }
    // Normally, seqexp throws only SM so no need to catch SErr

    //varargout management
    if (bVarargout)
    {
        InternalType* pOut = pContext->get(m_Varargout);
        if (pOut == NULL)
        {
            cleanCall(pContext, oldVal);
            Scierror(999, _("Invalid index.\n"));
            return Callable::Error;
        }

        if (pOut->isList() == false || pOut->getAs<List>()->getSize() == 0)
        {
            cleanCall(pContext, oldVal);
            Scierror(999, _("Invalid index.\n"));
            return Callable::Error;
        }

        List* pVarOut = pOut->getAs<List>();
        const int size = std::min(pVarOut->getSize(), _iRetCount);
        for (int i = 0 ; i < size ; ++i)
        {
            InternalType* pIT = pVarOut->get(i);
            if (pIT->isListUndefined())
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
    else
    {
        //normal output management
        for (std::list<symbol::Variable*>::iterator i = m_outputArgs->begin(); i != m_outputArgs->end() && _iRetCount; ++i, --_iRetCount)
        {
            InternalType * pIT = pContext->get(*i);
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

                char* pstArgName = wide_string_to_UTF8((*i)->getSymbol().getName().c_str());
                char* pstMacroName = wide_string_to_UTF8(getName().c_str());
                Scierror(999, _("Undefined variable '%s' in function '%s'.\n"), pstArgName, pstMacroName);
                FREE(pstArgName);
                FREE(pstMacroName);
                return Callable::Error;
            }
        }
    }

    //close the current scope
    cleanCall(pContext, oldVal);

    for (typed_list::iterator i = out.begin(), end = out.end(); i != end; ++i)
    {
        (*i)->DecreaseRef();
    }

    return RetVal;
}

std::list<symbol::Variable*>* Macro::getInputs()
{
    return m_inputArgs;
}

std::list<symbol::Variable*>* Macro::getOutputs()
{
    return m_outputArgs;
}

int Macro::getNbInputArgument(void)
{
    return (int)m_inputArgs->size();
}

int Macro::getNbOutputArgument(void)
{
    if (m_outputArgs->size() == 1 && m_outputArgs->back()->getSymbol().getName() == L"varargout")
    {
        return -1;
    }

    return (int)m_outputArgs->size();
}

bool Macro::operator==(const InternalType& it)
{
    if (const_cast<InternalType &>(it).isMacro() == false)
    {
        return false;
    }

    std::list<symbol::Variable*>* pInput = NULL;
    std::list<symbol::Variable*>* pOutput = NULL;
    types::Macro* pRight = const_cast<InternalType &>(it).getAs<types::Macro>();

    //check inputs
    pInput = pRight->getInputs();
    if (pInput->size() != m_inputArgs->size())
    {
        return false;
    }

    std::list<symbol::Variable*>::iterator itOld = pInput->begin();
    std::list<symbol::Variable*>::iterator itEndOld = pInput->end();
    std::list<symbol::Variable*>::iterator itMacro = m_inputArgs->begin();

    for (; itOld != itEndOld ; ++itOld, ++itMacro)
    {
        if ((*itOld)->getSymbol() != (*itMacro)->getSymbol())
        {
            return false;
        }
    }

    //check outputs
    pOutput = pRight->getOutputs();
    if (pOutput->size() != m_outputArgs->size())
    {
        return false;
    }

    itOld = pOutput->begin();
    itEndOld = pOutput->end();
    itMacro = m_outputArgs->begin();

    for (; itOld != itEndOld ; ++itOld, ++itMacro)
    {
        if ((*itOld)->getSymbol() != (*itMacro)->getSymbol())
        {
            return false;
        }
    }

    ast::Exp* pExp = pRight->getBody();
    ast::SerializeVisitor serialOld(pExp);
    unsigned char* oldSerial = serialOld.serialize(false, false);
    ast::SerializeVisitor serialMacro(m_body);
    unsigned char* macroSerial = serialMacro.serialize(false, false);

    //check buffer length
    unsigned int oldSize = *((unsigned int*)oldSerial);
    unsigned int macroSize = *((unsigned int*)macroSerial);
    if (oldSize != macroSize)
    {
        free(oldSerial);
        free(macroSerial);
        return false;
    }

    bool ret = (memcmp(oldSerial, macroSerial, oldSize) == 0);

    free(oldSerial);
    free(macroSerial);

    return ret;
}

void Macro::add_submacro(const symbol::Symbol& s, Macro* macro)
{
    macro->IncreaseRef();
    symbol::Context* ctx = symbol::Context::getInstance();
    symbol::Variable* var = ctx->getOrCreate(s);
    m_submacro[var] = macro;
}
}
