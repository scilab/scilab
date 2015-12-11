/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
#include "EOType.hxx"
namespace org_modules_external_objects
{
types::Callable* EOType::pFunc = nullptr;
bool EOType::invoke(types::typed_list & in, types::optional_list & opt, int _iRetCount, types::typed_list & out, const ast::Exp & /*e*/)
{
    //check some flag before call invoke
    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(id);
    ScilabGatewayOptions & options = env.getGatewayOptions();
    options.setIsNew(isNew);
    in.push_back(new types::Double(static_cast<double>(id)));
    return pFunc->call(in, opt, _iRetCount, out) == types::Function::OK;
}
}
