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

std::wstring Overload::buildOverloadName(std::wstring _stFunctionName, types::typed_list &in, int _iRetCount)
{
    switch(in.size())
    {
    case 0 :
        return L"%_" + _stFunctionName;
    case 1:
        return L"%" + in[0]->getShortTypeStr() + L"_" + _stFunctionName;
    case 2:
        return L"%" + in[0]->getShortTypeStr() + L"_" + _stFunctionName + L"_" + in[1]->getShortTypeStr();
    default :
        throw L"Don't know how to overload " + _stFunctionName;
    }
    return _stFunctionName;
}

types::Function::ReturnValue Overload::generateNameAndCall(std::wstring _stFunctionName, types::typed_list &in, int _iRetCount, types::typed_list &out, ast::ConstVisitor *_execMe)
{
    return call(buildOverloadName(_stFunctionName, in, _iRetCount), in, _iRetCount, out, _execMe);
}

types::Function::ReturnValue Overload::call(std::wstring _stOverloadingFunctionName, types::typed_list &in, int _iRetCount, types::typed_list &out, ast::ConstVisitor *_execMe)
{
    types::InternalType *pIT = symbol::Context::getInstance()->get(_stOverloadingFunctionName);

    if(pIT == NULL || pIT->isCallable() == false)
    {
        throw _W("check or define function ") + _stOverloadingFunctionName + _W(" for overloading.");
    }

    return pIT->getAsCallable()->call(in, _iRetCount, out, _execMe);
}

std::wstring Overload::getNameFromOper(ast::OpExp::Oper _oper)
{
    switch (_oper)
    {
        /* standard operators */
    case OpExp::plus :
        return std::wstring(L"a");
    case OpExp::minus :
        return std::wstring(L"s");
    case OpExp::times :
        return std::wstring(L"m");
    case OpExp::divide :
        return std::wstring(L"r");
    case OpExp::rdivide :
        return std::wstring(L"l");
    case OpExp::power :
        return std::wstring(L"p");
        /* dot operators */
    case OpExp::dottimes :
        return std::wstring(L"x");
    case OpExp::dotdivide :
        return std::wstring(L"d");
    case OpExp::dotrdivide :
        return std::wstring(L"q");
    case OpExp::dotpower :
        return std::wstring(L"j");
        /* Kron operators */
    case OpExp::krontimes :
        return std::wstring(L"k");
    case OpExp::krondivide :
        return std::wstring(L"y");
    case OpExp::kronrdivide :
        return std::wstring(L"z");
        /* Control Operators ??? */
    case OpExp::controltimes :
        return std::wstring(L"u");
    case OpExp::controldivide :
        return std::wstring(L"v");
    case OpExp::controlrdivide :
        return std::wstring(L"w");
    case OpExp::eq :
        return std::wstring(L"o");
    case OpExp::ne :
        return std::wstring(L"n");
    case OpExp::lt :
        return std::wstring(L"1");
    case OpExp::le :
        return std::wstring(L"3");
    case OpExp::gt :
        return std::wstring(L"2");
    case OpExp::ge :
        return std::wstring(L"4");
    case OpExp::logicalAnd :
        return std::wstring(L"h");
    case OpExp::logicalOr :
        return std::wstring(L"g");
    case OpExp::logicalShortCutAnd :
        return std::wstring(L"h");
    case OpExp::logicalShortCutOr :
        return std::wstring(L"g");
    default :
        return std::wstring(L"???");
    }
}
