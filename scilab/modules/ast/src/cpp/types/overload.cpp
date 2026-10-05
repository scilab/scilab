/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
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
#include "Sciwarning.h"
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
        if (in[0]->isObject())
        {
            stType0 = L"object";
        }
        else
        {
            stType0 = stType0.substr(0, 8);
        }
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

types::Function::ReturnValue Overload::generateNameAndCall(const std::wstring& _stFunctionName, types::typed_list& in, int _iRetCount, types::typed_list& out, bool _isOperator, bool errorOnUndefined, const Location& _Location)
{
    std::wstring stFunc = buildOverloadName(_stFunctionName, in, _iRetCount, _isOperator);
    if (symbol::Context::getInstance()->get(symbol::Symbol(stFunc)))
    {
        return call(stFunc, in, _iRetCount, out, _isOperator, errorOnUndefined, _Location);
    }

    // if overload doesn't existe try with short name
    std::wstring stFunc2 = buildOverloadName(_stFunctionName, in, _iRetCount, _isOperator, true);
    if (symbol::Context::getInstance()->get(symbol::Symbol(stFunc2)))
    {
        types::Function::ReturnValue ret = call(stFunc2, in, _iRetCount, out, _isOperator, errorOnUndefined, _Location);
        if (ret == types::Function::OK && ConfigVariable::getWarningMode() && in[0]->isObject() == false)
        {
            char* pstFunc2 = wide_string_to_UTF8(stFunc2.c_str());
            char* pstFunc = wide_string_to_UTF8(stFunc.c_str());
            Sciwarning(_("Warning : please rename your overloaded function\n \"%s\" to \"%s\"\n"), pstFunc2, pstFunc);
            FREE(pstFunc);
            FREE(pstFunc2);
        }
        return ret;
    }

    // get exeception with overloading error
    return call(stFunc, in, _iRetCount, out, _isOperator, errorOnUndefined, _Location);
}

types::Function::ReturnValue Overload::call(const std::wstring& _stOverloadingFunctionName, types::typed_list& in, int _iRetCount, types::typed_list& out, bool _isOperator, bool errorOnUndefined, const Location& _location)
{
    if (in.size() > 0 && in[0]->isObject())
    {
        types::Object* obj = in[0]->getAs<types::Object>();
        types::typed_list in2(in.begin() + 1, in.end());
        types::optional_list opt;
        if (obj->callMethod(_stOverloadingFunctionName, in2, opt, _iRetCount, out, ast::CommentExp(_location, new std::wstring(L""))) == types::Function::OK)
        {
            return types::Function::OK;
        }
    }

    types::InternalType *pIT = symbol::Context::getInstance()->get(symbol::Symbol(_stOverloadingFunctionName));
    types::Callable* pCall = NULL;
    try
    {
        if (pIT == NULL || pIT->isCallable() == false)
        {
            if (!errorOnUndefined)
            {
                // don't report an error if requested
                return types::Function::ReturnValue::OK_NoResult;
            }

            char pstError1[512];
            char pstError2[512];
            char *pstFuncName = wide_string_to_UTF8(_stOverloadingFunctionName.c_str());
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

            wchar_t* pwstError = to_wide_string(pstError1);
            ast::InternalError ie(pwstError, 999, _location);
            FREE(pwstError);

            ie.SetErrorType(ast::TYPE_EXCEPTION);
            throw ie;
        }

        if (ConfigVariable::increaseRecursion())
        {
            pCall = pIT->getAs<types::Callable>();

            types::optional_list opt;

            // add line and function name in where
            int iMacroLine = 0;
            if(_location.first_line)
            {
                iMacroLine =  _location.first_line + 1 - ConfigVariable::getMacroFirstLines();
            }
            ConfigVariable::where_begin(iMacroLine, pCall, _location);

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
    catch (const ast::InternalError& ie)
    {
        ConfigVariable::fillWhereError(ie.GetErrorLocation());
        if (pCall)
        {
            // remove function name in where
            ConfigVariable::where_end();
            ConfigVariable::decreaseRecursion();
        }

        throw ie;
    }
    catch (const ast::InternalAbort& ia)
    {
        ConfigVariable::where_end();
        ConfigVariable::decreaseRecursion();
        throw ia;
    }
}

std::wstring Overload::getNameFromOper(const int _oper)
{
    switch (_oper)
    {
        /* standard operators */
        case ast::OpExp::plus :
        case ast::OpExp::unaryPlus :
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

std::pair<std::wstring, int> Overload::getMethodFromOper(const int _oper)
{
    switch (_oper)
    {
        /* standard operators */
        case ast::OpExp::plus:
            return {std::wstring(L"plus"), 2};
        case ast::OpExp::unaryPlus:
            return {std::wstring(L"uplus"), 1};
        case ast::OpExp::unaryMinus:
            return {std::wstring(L"uminus"), 1};
        case ast::OpExp::minus:
            return {std::wstring(L"minus"), 2};
        case ast::OpExp::times:
            return {std::wstring(L"mtimes"), 2};
        case ast::OpExp::rdivide:
            return {std::wstring(L"mrdivide"), 2};
        case ast::OpExp::ldivide:
            return {std::wstring(L"mldivide"), 2};
        case ast::OpExp::power:
            return {std::wstring(L"mpower"), 2};
        /* dot operators */
        case ast::OpExp::dottimes:
            return {std::wstring(L"times"), 2};
        case ast::OpExp::dotrdivide:
            return {std::wstring(L"rdivide"), 2};
        case ast::OpExp::dotldivide:
            return {std::wstring(L"ldivide"), 2};
        case ast::OpExp::dotpower:
            return {std::wstring(L"power"), 2};
        /* Kron operators */
        case ast::OpExp::krontimes:
            return {std::wstring(L"kron"), 2};
        case ast::OpExp::kronrdivide:
            return {std::wstring(L"rkron"), 2};
        case ast::OpExp::kronldivide:
            return {std::wstring(L"lkron"), 2};
        /* Control Operators ??? */
        case ast::OpExp::controltimes:
            return {std::wstring(L"controltimes"), 2};
        case ast::OpExp::controlrdivide:
            return {std::wstring(L"controlrdivide"), 2};
        case ast::OpExp::controlldivide:
            return {std::wstring(L"controlldivide"), 2};
        /* comparison */
        case ast::OpExp::eq:
            return {std::wstring(L"eq"), 2};
        case ast::OpExp::ne:
            return {std::wstring(L"ne"), 2};
        case ast::OpExp::lt:
            return {std::wstring(L"lt"), 2};
        case ast::OpExp::le:
            return {std::wstring(L"le"), 2};
        case ast::OpExp::gt:
            return {std::wstring(L"gt"), 2};
        case ast::OpExp::ge:
            return {std::wstring(L"ge"), 2};
        case ast::OpExp::logicalAnd:
            return {std::wstring(L"and"), 2};
        case ast::OpExp::logicalOr:
            return {std::wstring(L"or"), 2};
        case ast::OpExp::logicalShortCutAnd:
            return {std::wstring(L"shortand"), 2};
        case ast::OpExp::logicalShortCutOr:
            return {std::wstring(L"shortor"), 2};
        default:
            return {std::wstring(L"???"), 2};
    }
}
