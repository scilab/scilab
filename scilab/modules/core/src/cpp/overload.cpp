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
#include "localization.h"
}

#include "overload.hxx"
#include "context.hxx"

std::string Overload::buildOverloadName(std::string _stFunctionName, types::typed_list &in, int _iRetCount)
{
    switch(in.size())
    {
    case 0 :
        return "%_" + _stFunctionName;
    case 1:
        return "%" + in[0]->getShortTypeStr() + "_" + _stFunctionName;
    case 2:
        return "%" + in[0]->getShortTypeStr() + "_" + _stFunctionName + "_" + in[1]->getShortTypeStr();
    default :
        throw "Don't know how to overload " + _stFunctionName;
    }
    return _stFunctionName;
}

types::Function::ReturnValue Overload::generateNameAndCall(std::string _stFunctionName, types::typed_list &in, int _iRetCount, types::typed_list &out, ast::ConstVisitor *_execMe)
{
    return call(buildOverloadName(_stFunctionName, in, _iRetCount), in, _iRetCount, out, _execMe);
}

types::Function::ReturnValue Overload::call(std::string _stOverloadingFunctionName, types::typed_list &in, int _iRetCount, types::typed_list &out, ast::ConstVisitor *_execMe)
{
    types::InternalType *pIT = symbol::Context::getInstance()->get(_stOverloadingFunctionName);

    if(pIT == NULL || pIT->isCallable() == false)
    {
        throw _("check or define function ") + _stOverloadingFunctionName + _(" for overloading.");
    }

    return pIT->getAsCallable()->call(in, _iRetCount, out, _execMe);
}

std::string  Overload::getNameFromOper(ast::OpExp::Oper _oper)
{
    switch (_oper)
    {
        /* standard operators */
    case OpExp::plus :
        return std::string("a");
    case OpExp::minus :
        return std::string("s");
    case OpExp::times :
        return std::string("m");
    case OpExp::divide :
        return std::string("r");
    case OpExp::rdivide :
        return std::string("l");
    case OpExp::power :
        return std::string("p");
        /* dot operators */
    case OpExp::dottimes :
        return std::string("x");
    case OpExp::dotdivide :
        return std::string("d");
    case OpExp::dotrdivide :
        return std::string("q");
    case OpExp::dotpower :
        return std::string("j");
        /* Kron operators */
    case OpExp::krontimes :
        return std::string("k");
    case OpExp::krondivide :
        return std::string("y");
    case OpExp::kronrdivide :
        return std::string("z");
        /* Control Operators ??? */
    case OpExp::controltimes :
        return std::string("u");
    case OpExp::controldivide :
        return std::string("v");
    case OpExp::controlrdivide :
        return std::string("w");
    case OpExp::eq :
        return std::string("o");
    case OpExp::ne :
        return std::string("n");
    case OpExp::lt :
        return std::string("1");
    case OpExp::le :
        return std::string("3");
    case OpExp::gt :
        return std::string("2");
    case OpExp::ge :
        return std::string("4");
    case OpExp::logicalAnd :
        return std::string("h");
    case OpExp::logicalOr :
        return std::string("g");
    case OpExp::logicalShortCutAnd :
        return std::string("h");
    case OpExp::logicalShortCutOr :
        return std::string("g");
    default :
        return std::string("???");
    }
}
