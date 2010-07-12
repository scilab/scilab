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

        //check excepted and input/output parameters numbers
        // Scilab Macro can be called with less than prototyped arguments,
        // but not more execpts with varargin
        // TODO: Manage varargin here
		if(in.size() > m_inputArgs->size())
		{
            wostringstream ostr;
            ostr << _W("Wrong number of input arguments:") << std::endl << std::endl;
            ostr << _W("Arguments are:") << std::endl << std::endl;
            ostr << " ";
            for (list<wstring>::iterator it =  m_inputArgs->begin() ; it != m_inputArgs->end() ; ++it)
            {
                ostr << *it << L"    ";
            }
            ostr << std::endl << std::endl;
            ScierrorW(58, ostr.str().c_str());
			return Callable::Error;
		}

        list<wstring>::const_iterator i;
        typed_list::const_iterator j;
        //ast::ExecVisitor execFunc;

        //open a new scope
        symbol::Context *pContext = symbol::Context::getInstance();
        pContext->scope_begin();

        // Declare nargin & nargout in function context.
        pContext->put(wstring(L"nargin"), *new Double(static_cast<double>(in.size())));
        pContext->put(wstring(L"nargout"), *new Double(static_cast<double>(_iRetCount)));

        //assign value to variable in the new context
        for (i = m_inputArgs->begin(), j = in.begin(); j != in.end (); ++j,++i)
        {
            pContext->put((*i), **j);
        }

        try
        {
            m_body->returnable_set();
            m_body->accept(*execFunc);
            if(m_body->is_return())
            {
                m_body->returnable_set();
            }


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
