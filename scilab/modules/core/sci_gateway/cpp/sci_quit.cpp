/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2015 - Scilab Enterprises - Cedric Delamarre
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "core_gw.hxx"
#include "function.hxx"
#include "configvariable.hxx"
#include "scilabexception.hxx"

types::Function::ReturnValue sci_quit(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    //Terminates Scilab or decreases the pause level
    if (ConfigVariable::getPauseLevel())
    {
        ConfigVariable::DecreasePauseLevel();
    }
    else
    {
        ConfigVariable::setForceQuit(true);
        throw ast::InternalAbort();
    }

    return types::Function::OK;
}
