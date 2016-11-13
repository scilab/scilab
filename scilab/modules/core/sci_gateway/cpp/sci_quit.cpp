/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2015 - Scilab Enterprises - Cedric Delamarre
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
