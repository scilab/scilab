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

#include "function.hxx"
#include "configvariable.hxx"
#include "types.hxx"
#include "double.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}

types::Function::ReturnValue sci_exit(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    // exit() or exit
    if (in.size() == 0)
    {
        ConfigVariable::setForceQuit(true);
        return types::Function::OK;
    }
    
    // More than one input argument => ERROR
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected."), "exit", 0, 1);
        return types::Function::Error;
    }
    
    // in[0] Should be a scalar double value.
    if (!in.front()->isDouble())
    {
        Scierror(999,_("%s: Wrong type for input argument #%d: A scalar expected.\n"), "exit", 1);
        return types::Function::Error;
    }
    
    if (in.front()->getAsDouble()->size_get() != 1)
    {
        Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"), "exit", 1);
        return types::Function::Error;
    }
    
    double dExit = in.front()->getAsDouble()->real_get(0,0);
    int iExit = (int) in.front()->getAsDouble()->real_get(0,0);

    if (dExit != (double) iExit)
    {
      Scierror(999,_("%s: Wrong value for input argument #%d: A integer expected.\n"), "exit", 1);
      return types::Function::Error;
    }

    ConfigVariable::setExitStatus(iExit);
    ConfigVariable::setForceQuit(true);
    return types::Function::OK;
}
