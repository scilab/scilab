/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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

extern "C"
{
#include "stdarg.h"
#include "os_string.h"
#include "sci_malloc.h"
#include "sciprint.h"
}

#include "configvariable.hxx"
#include "localization.hxx"
#include "callable.hxx"
#include "overload.hxx"
#include "context.hxx"
#include "opexp.hxx"
#include "execvisitor.hxx"

std::wstring Overload::buildOverloadName(const std::wstring& _stFunctionName, types::typed_list &in, int /*_iRetCount*/, bool _isOperator, bool _truncated)
{
    std::wstring stType0 = in[0]->getShortTypeStr();

    if (_truncated)
    {
        stType0 = stType0.substr(0, 8);
    }

    switch (in.size())
    {
        case 0 :
            return L"%_" + _stFunctionName;
        case 2:
            if (_isOperator)
            {
                return L"%" + stType0 + L"_" + _stFunctionName + L"_" + in[1]->getShortTypeStr();
            }
        default:
            return L"%" + stType0 + L"_" + _stFunctionName;
    }
    return _stFunctionName;
}

types::Function::ReturnValue Overload::generateNameAndCall(const std::wstring& _stFunctionName, types::typed_list &in, int _iRetCount, types::typed_list &out, bool _isOperator)
{
    std::wstring stFunc = buildOverloadName(_stFunctionName, in, _iRetCount, _isOperator);
    if (symbol::Context::getInstance()->get(symbol::Symbol(stFunc)))
    {
        return call(stFunc, in, _iRetCount, out, _isOperator);
    }

    // if overload doesn't existe try with short name
    std::wstring stFunc2 = buildOverloadName(_stFunctionName, in, _iRetCount, _isOperator, true);
    if (symbol::Context::getInstance()->get(symbol::Symbol(stFunc)))
    {
        types::Function::ReturnValue ret = call(stFunc, in, _iRetCount, out, _isOperator);
        if (ret == types::Function::OK && ConfigVariable::getWarningMode())
        {
            char* pstFunc2 = wide_string_to_UTF8(stFunc2.c_str());
            char* pstFunc = wide_string_to_UTF8(stFunc.c_str());
            sciprint(_("Warning : please rename your overloaded function\n \"%s\" to \"%s\"\n"), pstFunc2, pstFunc);
            FREE(pstFunc);
            FREE(pstFunc2);
        }
        return ret;
    }

    // get exeception with overloading error
    return call(stFunc, in, _iRetCount, out, _isOperator);
}

types::Function::ReturnValue Overload::call(const std::wstring& _stOverloadingFunctionName, types::typed_list &in, int _iRetCount, types::typed_list &out, bool _isOperator)
{
    types::InternalType *pIT = symbol::Context::getInstance()->get(symbol::Symbol(_stOverloadingFunctionName));
    types::Callable *pCall = NULL;
    try
    {
        if (pIT == NULL || pIT->isCallable() == false)
        {
            char pstError1[512];
            char pstError2[512];
            char *pstFuncName = wide_string_to_UTF8(_stOverloadingFunctionName.c_str());
            wchar_t* pwstError = NULL;
            if (_isOperator)
            {
                os_sprintf(pstError2, _("check or define function %s for overloading.\n"), pstFuncName);
                os_sprintf(pstError1, "%s%s", _("Undefined operation for the given operands.\n"), pstError2);
            }
            else
            {
                os_sprintf(pstError2, _("  check arguments or define function %s for overloading.\n"), pstFuncName);
                os_sprintf(pstError1, "%s%s", _("Function not defined for given argument type(s),\n"), pstError2);
            }

            FREE(pstFuncName);
            ast::InternalError ie(pstError1);
            ie.SetErrorType(ast::TYPE_EXCEPTION);
            throw ie;
        }

        if (ConfigVariable::increaseRecursion())
        {
            pCall = pIT->getAs<types::Callable>();

            types::optional_list opt;

            // add line and function name in where
            ConfigVariable::where_begin(0, 0, pCall);

            types::Function::ReturnValue ret;
            ret = pCall->call(in, opt, _iRetCount, out);

            // remove function name in where
            ConfigVariable::where_end();
            ConfigVariable::decreaseRecursion();
            return ret;
        }
        else
        {
            throw ast::RecursionException();
        }
    }
    catch (ast::InternalError ie)
    {
        ConfigVariable::fillWhereError(ie.GetErrorLocation().first_line);
        if (pCall)
        {
            if (ConfigVariable::getLastErrorFunction() == L"")
            {
                ConfigVariable::setLastErrorFunction(pCall->getName());
                ConfigVariable::setLastErrorLine(ie.GetErrorLocation().first_line);
            }

            // remove function name in where
            ConfigVariable::where_end();
            ConfigVariable::decreaseRecursion();
        }

        throw ie;
    }
}

std::wstring Overload::getNameFromOper(const int _oper)
{
    switch (_oper)
    {
        /* standard operators */
        case ast::OpExp::plus :
            return std::wstring(L"a");
        case ast::OpExp::unaryMinus :
        case ast::OpExp::minus :
            return std::wstring(L"s");
        case ast::OpExp::times :
            return std::wstring(L"m");
        case ast::OpExp::rdivide :
            return std::wstring(L"r");
        case ast::OpExp::ldivide :
            return std::wstring(L"l");
        case ast::OpExp::power :
            return std::wstring(L"p");
        /* dot operators */
        case ast::OpExp::dottimes :
            return std::wstring(L"x");
        case ast::OpExp::dotrdivide :
            return std::wstring(L"d");
        case ast::OpExp::dotldivide :
            return std::wstring(L"q");
        case ast::OpExp::dotpower :
            return std::wstring(L"j");
        /* Kron operators */
        case ast::OpExp::krontimes :
            return std::wstring(L"k");
        case ast::OpExp::kronrdivide :
            return std::wstring(L"y");
        case ast::OpExp::kronldivide :
            return std::wstring(L"z");
        /* Control Operators ??? */
        case ast::OpExp::controltimes :
            return std::wstring(L"u");
        case ast::OpExp::controlrdivide :
            return std::wstring(L"v");
        case ast::OpExp::controlldivide :
            return std::wstring(L"w");
        case ast::OpExp::eq :
            return std::wstring(L"o");
        case ast::OpExp::ne :
            return std::wstring(L"n");
        case ast::OpExp::lt :
            return std::wstring(L"1");
        case ast::OpExp::le :
            return std::wstring(L"3");
        case ast::OpExp::gt :
            return std::wstring(L"2");
        case ast::OpExp::ge :
            return std::wstring(L"4");
        case ast::OpExp::logicalAnd :
            return std::wstring(L"h");
        case ast::OpExp::logicalOr :
            return std::wstring(L"g");
        case ast::OpExp::logicalShortCutAnd :
            return std::wstring(L"h");
        case ast::OpExp::logicalShortCutOr :
            return std::wstring(L"g");
        default :
            return std::wstring(L"???");
    }
}
