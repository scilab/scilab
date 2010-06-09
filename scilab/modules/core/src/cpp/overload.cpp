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
