/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
#include "callable.hxx"
#include "overload.hxx"
#include "context.hxx"
#include "opexp.hxx"
#include "execvisitor.hxx"

std::string Overload::buildOverloadName(const std::string& _stFunctionName, types::typed_list &in, int /*_iRetCount*/, bool _isOperator, bool _truncated)
{
    std::string stType0 = in[0]->getShortTypeStr();

    if (_truncated)
    {
        stType0 = stType0.substr(0, 8);
    }

    switch (in.size())
    {
        case 0 :
            return "%_" + _stFunctionName;
        case 2:
            if (_isOperator)
            {
                return "%" + stType0 + "_" + _stFunctionName + "_" + in[1]->getShortTypeStr();
            }
        default:
            return "%" + stType0 + "_" + _stFunctionName;
    }
    return _stFunctionName;
}

types::Function::ReturnValue Overload::generateNameAndCall(const std::string& _stFunctionName, types::typed_list &in, int _iRetCount, types::typed_list &out, bool _isOperator)
{
    std::string stFunc = buildOverloadName(_stFunctionName, in, _iRetCount, _isOperator);
    if (symbol::Context::getInstance()->get(symbol::Symbol(stFunc)))
    {
        return call(stFunc, in, _iRetCount, out, _isOperator);
    }

    // if overload doesn't existe try with short name
    std::string stFunc2 = buildOverloadName(_stFunctionName, in, _iRetCount, _isOperator, true);
    if (symbol::Context::getInstance()->get(symbol::Symbol(stFunc)))
    {
        types::Function::ReturnValue ret = call(stFunc, in, _iRetCount, out, _isOperator);
        if (ret == types::Function::OK && ConfigVariable::getWarningMode())
        {
            sciprint(_("Warning : please rename your overloaded function\n \"%s\" to \"%s\"\n"), stFunc2.c_str(), stFunc.c_str());
        }
        return ret;
    }

    // get exeception with overloading error
    return call(stFunc, in, _iRetCount, out, _isOperator);
}

types::Function::ReturnValue Overload::call(const std::string& _stOverloadingFunctionName, types::typed_list &in, int _iRetCount, types::typed_list &out, bool _isOperator)
{
    types::InternalType *pIT = symbol::Context::getInstance()->get(symbol::Symbol(_stOverloadingFunctionName));
    types::Callable *pCall = NULL;
    try
    {
        if (pIT == NULL || pIT->isCallable() == false)
        {
            char pstError1[512];
            char pstError2[512];
            if (_isOperator)
            {
                os_sprintf(pstError2, _("check or define function %s for overloading.\n"), _stOverloadingFunctionName.c_str());
                os_sprintf(pstError1, "%s%s", _("Undefined operation for the given operands.\n"), pstError2);
            }
            else
            {
                os_sprintf(pstError2, _("  check arguments or define function %s for overloading.\n"), _stOverloadingFunctionName.c_str());
                os_sprintf(pstError1, "%s%s", _("Function not defined for given argument type(s),\n"), pstError2);
            }

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
            if (ConfigVariable::getLastErrorFunction() == "")
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

std::string Overload::getNameFromOper(const int _oper)
{
    switch (_oper)
    {
        /* standard operators */
        case ast::OpExp::plus :
            return "a";
        case ast::OpExp::unaryMinus :
        case ast::OpExp::minus :
            return "s";
        case ast::OpExp::times :
            return "m";
        case ast::OpExp::rdivide :
            return "r";
        case ast::OpExp::ldivide :
            return "l";
        case ast::OpExp::power :
            return "p";
        /* dot operators */
        case ast::OpExp::dottimes :
            return "x";
        case ast::OpExp::dotrdivide :
            return "d";
        case ast::OpExp::dotldivide :
            return "q";
        case ast::OpExp::dotpower :
            return "j";
        /* Kron operators */
        case ast::OpExp::krontimes :
            return "k";
        case ast::OpExp::kronrdivide :
            return "y";
        case ast::OpExp::kronldivide :
            return "z";
        /* Control Operators ??? */
        case ast::OpExp::controltimes :
            return "u";
        case ast::OpExp::controlrdivide :
            return "v";
        case ast::OpExp::controlldivide :
            return "w";
        case ast::OpExp::eq :
            return "o";
        case ast::OpExp::ne :
            return "n";
        case ast::OpExp::lt :
            return "1";
        case ast::OpExp::le :
            return "3";
        case ast::OpExp::gt :
            return "2";
        case ast::OpExp::ge :
            return "4";
        case ast::OpExp::logicalAnd :
            return "h";
        case ast::OpExp::logicalOr :
            return "g";
        case ast::OpExp::logicalShortCutAnd :
            return "h";
        case ast::OpExp::logicalShortCutOr :
            return "g";
        default :
            return "???";
    }
}
