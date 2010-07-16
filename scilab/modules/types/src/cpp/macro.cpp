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
#include "context.hxx"
#include "localization.h"
#include "yaspio.hxx"

extern "C"
{
    #include "Scierror.h"
}

namespace types
{

    /*--------------*/
    /*	Contructor  */
    /*--------------*/
    Macro::Macro(const wstring& _stName, list<wstring> &_inputArgs, list<wstring> &_outputArgs, ast::SeqExp &_body, const wstring& _stModule) :
    Callable(), m_inputArgs(&_inputArgs), m_outputArgs(&_outputArgs), m_body(&_body)
    {
        setName(_stName);
        setModule(_stModule);
        bAutoAlloc = false;
    }

    Macro::~Macro()
    {
        delete m_body;
    }

    Macro* Macro::clone()
    {
        IncreaseRef();
        return this;
    }
    /*--------------*/
    /*	whoIAm		*/
    /*--------------*/
    void Macro::whoAmI()
    {
        std::cout << "types::Macro";
    }

    Macro* Macro::getAsMacro(void)
    {
        return this;
    }

    InternalType::RealType Macro::getType(void)
    {
        return RealMacro;
    }

    ast::SeqExp* Macro::body_get(void)
    {
        return m_body;
    }

    wstring Macro::toString(int _iPrecision, int _iLineLen)
    {
        wostringstream ostr;

        //FIXME : Implement me.
        ostr << L"FIXME : Implement Macro::toString" << std::endl;

        return ostr.str();
    }

    Callable::ReturnValue Macro::call(typed_list &in, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc)
    {
        ReturnValue RetVal = Callable::OK;
        symbol::Context *pContext = symbol::Context::getInstance();

        //check excepted and input/output parameters numbers
        // Scilab Macro can be called with less than prototyped arguments,
        // but not more execpts with varargin
        // TODO: Manage varargin here
        if(m_inputArgs->size() > 0 && m_inputArgs->back() == L"varargin")
        {
            //open a new scope
            pContext->scope_begin();

            int iVarPos = static_cast<int>(in.size());
            if(iVarPos > static_cast<int>(m_inputArgs->size()) - 1)
            {
                iVarPos = static_cast<int>(m_inputArgs->size()) - 1;
            }

            //add all standard variable in function context but not varargin
            list<wstring>::const_iterator itName = m_inputArgs->begin();
            typed_list::const_iterator itValue = in.begin();
            while(iVarPos > 0)
            {
                pContext->put((*itName), **itValue);
                iVarPos--;
                itName++;
                itValue++;
            }

            //create varargin only if previous variable are assigned
            if(in.size() >= static_cast<int>(m_inputArgs->size()) - 1)
            {
                //create and fill varargin
                List* pL = new List();
                while(itValue != in.end())
                {
                    pL->append(*itValue);
                    itValue++;
                }
                pContext->put(L"varargin", *pL);
            }
        }
		else if(in.size() > m_inputArgs->size())
		{
            wostringstream ostr;
            ostr << _W("Wrong number of input arguments:") << std::endl << std::endl;
            ostr << _W("Excepted: ") << m_inputArgs->size() << std::endl;

            if(m_inputArgs->size() > 0)
            {
                ostr << _W("Arguments are:") << std::endl << std::endl;
                ostr << " ";
                for (list<wstring>::iterator it =  m_inputArgs->begin() ; it != m_inputArgs->end() ; ++it)
                {
                    ostr << *it << L"    ";
                }
                ostr << std::endl;
            }
            ScierrorW(58, ostr.str().c_str());
			return Callable::Error;
		}
        else
        {
            //open a new scope
            pContext->scope_begin();

            //assign value to variable in the new context
            list<wstring>::const_iterator i;
            typed_list::const_iterator j;

            for (i = m_inputArgs->begin(), j = in.begin(); j != in.end (); ++j,++i)
            {
                pContext->put((*i), **j);
            }
        }

        //common part with or without varargin

        // Declare nargin & nargout in function context.
        pContext->put(wstring(L"nargin"), *new Double(static_cast<double>(in.size())));
        pContext->put(wstring(L"nargout"), *new Double(static_cast<double>(_iRetCount)));

        try
        {
            m_body->returnable_set();
            m_body->accept(*execFunc);
            if(m_body->is_return())
            {
                m_body->returnable_set();
            }


            list<wstring>::const_iterator i;
            for (i = m_outputArgs->begin(); i != m_outputArgs->end() && _iRetCount; ++i, --_iRetCount)
            {
                InternalType *pIT = pContext->get((*i));
                if(pIT != NULL)
                {
                    out.push_back(pIT);
                    pIT->IncreaseRef();
                }
                else
                {
                    wchar_t sz[bsiz];
#ifdef _MSC_VER
                    swprintf_s(sz, bsiz, _W("Undefined variable %s.\n"), (*i).c_str());
#else
                    swprintf(sz, bsiz, _W("Undefined variable %S.\n"), (*i).c_str());
#endif
                    YaspWriteW(sz);
                }
            }
        }
        catch(wstring sz)
        {
            YaspWriteW(sz.c_str());
            YaspWriteW(L"\n");
            RetVal = Callable::Error;
        }

        //close the current scope
        pContext->scope_end();

        for (int j = 0; j < out.size(); ++j)
        {
            out[j]->DecreaseRef();
        }
        return RetVal;
    }

    std::list<wstring>* Macro::inputs_get()
    {
        return m_inputArgs;
    }

    std::list<wstring>* Macro::outputs_get()
    {
        return m_outputArgs;
    }
}
