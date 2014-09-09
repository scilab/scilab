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

#include "core_gw.hxx"
#include "function.hxx"
#include "configvariable.hxx"
#include "double.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"
#include "sci_mode.h"
#include "createMainScilabObject.h"
}

using namespace types;

Function::ReturnValue sci_exit(typed_list &in, int _iRetCount, typed_list &out)
{
    // exit() or exit
    if (in.size() == 0)
    {
        ConfigVariable::setForceQuit(true);
        return Function::OK;
    }

    // More than one input argument => ERROR
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected."), "exit", 0, 1);
        return Function::Error;
    }

    // in[0] Should be a scalar double value.
    InternalType* pIT = in[0];
    if (pIT->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "exit", 1);
        return Function::Error;
    }

    Double* pD = pIT->getAs<Double>();
    if (pD->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "exit", 1);
        return Function::Error;
    }

    double dExit = pD->get(0);
    if (dExit != (int) dExit)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), "exit", 1);
        return Function::Error;
    }

    if (ConfigVariable::getScilabMode() != SCILAB_NWNI)
    {
        if (in.size() == 0)
        {
            //shouldExit = canCloseMainScilabObject();
        }
        else
        {
            //forceCloseMainScilabObject();
        }
    }

    ConfigVariable::setExitStatus((int)dExit);
    ConfigVariable::setForceQuit(true);
    return Function::OK;
}
