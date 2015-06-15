/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <sstream>
#include <vector>
#include "callable.hxx"
#include "scilabexception.hxx"
#include "configvariable.hxx"


namespace types
{

bool Callable::invoke(typed_list & in, optional_list & opt, int _iRetCount, typed_list & out, ast::ConstVisitor & execFunc, const ast::Exp & e)
{
    //reset previous error before call function
    ConfigVariable::resetError();
    //update verbose";" flag
    ConfigVariable::setVerbose(e.isVerbose());
    // add line and function name in where
    int iFirstLine = e.getLocation().first_line;
    ConfigVariable::where_begin(iFirstLine + 1 - ConfigVariable::getMacroFirstLines(), iFirstLine, this);
    Callable::ReturnValue Ret;

    try
    {
        Ret = call(in, opt, _iRetCount, out, &execFunc);
        ConfigVariable::where_end();
    }
    catch (ast::ScilabMessage & sm)
    {
        ConfigVariable::where_end();
        ConfigVariable::setLastErrorFunction(getName());

        throw sm;
    }
    catch (ast::InternalAbort & ia)
    {
        ConfigVariable::where_end();
        ConfigVariable::setLastErrorFunction(getName());

        throw ia;
    }
    catch (ast::ScilabError & se)
    {
        ConfigVariable::where_end();
        ConfigVariable::setLastErrorFunction(getName());

        throw se;
    }

    if (Ret == Callable::Error)
    {
        ConfigVariable::setLastErrorFunction(getName());
        ConfigVariable::setLastErrorLine(e.getLocation().first_line);
        throw ast::ScilabError(ConfigVariable::getLastErrorMessage(), ConfigVariable::getLastErrorNumber(), e.getLocation());
    }

    return true;
}
}
