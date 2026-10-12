/*
* Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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
#include "EOType.hxx"
#include "configvariable.hxx"
#include "exp.hxx"

namespace org_modules_external_objects
{
bool EOType::invoke(types::typed_list& in, types::optional_list& opt, int _iRetCount, types::typed_list& out, const ast::Exp& e)
{
    //check some flag before call invoke
    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(id);
    ScilabGatewayOptions & options = env.getGatewayOptions();
    options.setIsNew(isNew);
    in.push_back(new types::Double(static_cast<double>(id)));

    /* resolve "!!_invoke_" on each call: a cached pointer would dangle
       after a TerminateScilab and Call_ScilabOpen cycle */
    symbol::Context* ctx = symbol::Context::getInstance();
    types::InternalType* pIT = ctx->get(symbol::Symbol(ScilabObjects::_INVOKE_));
    if (pIT == nullptr || pIT->isCallable() == false)
    {
        throw ast::InternalError(std::wstring(L"\"") + ScilabObjects::_INVOKE_ + L"\" is not defined.", 999, e.getLocation());
    }

    auto Ret = pIT->getAs<types::Callable>()->call(in, opt, _iRetCount, out);
    if (Ret == types::Callable::Error)
    {
        throw ast::InternalError(ConfigVariable::getLastErrorMessage(), ConfigVariable::getLastErrorNumber(), e.getLocation());
    }

    return true;
}
}
