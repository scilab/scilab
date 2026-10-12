/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
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

#include <sstream>
#include <vector>
#include "callable.hxx"
#include "configvariable.hxx"
#include "exp.hxx"
#include "scilabexception.hxx"

namespace types
{

bool Callable::invoke(typed_list & in, optional_list & opt, int _iRetCount, typed_list & out, const ast::Exp & e)
{
    invokeCall(e, [&]() { return call(in, opt, _iRetCount, out); });
    return true;
}

Callable::ReturnValue Callable::invokeCall(const ast::Exp & e, const std::function<ReturnValue()>& doCall)
{
    //check recursion before try catch, to make difference with errors
    if (ConfigVariable::increaseRecursion())
    {
        //reset previous error before call function
        ConfigVariable::resetError();
        //update verbose";" flag
        ConfigVariable::setVerbose(e.isVerbose());
        // add line and function name in where
        int iFirstLine = e.getLocation().first_line;
        ConfigVariable::where_begin(iFirstLine + 1 - ConfigVariable::getMacroFirstLines(), this, e.getLocation());
        Callable::ReturnValue ret = Callable::OK;

        try
        {
            ret = doCall();
            ConfigVariable::where_end();
            ConfigVariable::decreaseRecursion();
        }
        catch (ast::InternalError & ie)
        {
            ConfigVariable::where_end();
            ConfigVariable::decreaseRecursion();
            throw ie;
        }
        catch (ast::InternalAbort & ia)
        {
            ConfigVariable::where_end();
            ConfigVariable::decreaseRecursion();
            throw ia;
        }

        if (ret == Callable::Error)
        {
            throw ast::InternalError(ConfigVariable::getLastErrorMessage(), ConfigVariable::getLastErrorNumber(), e.getLocation());
        }

        return ret;
    }
    else
    {
        throw ast::RecursionException();
    }
}
}
