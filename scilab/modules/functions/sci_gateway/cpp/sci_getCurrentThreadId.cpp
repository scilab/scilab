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
#define NOMINMAX
#include "Thread_Wrapper.h"
#include "Scierror.h"
#include "localization.h"
}

#include "functions_gw.hxx"
#include "configvariable.hxx"
#include "function.hxx"

types::Function::ReturnValue sci_getCurrentThreadId(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 0)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "getCurrentThreadId" , 0);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), "getCurrentThreadId" , 1);
        return types::Function::Error;
    }

    out.push_back(ConfigVariable::getLastRunningThread());

    return types::Function::OK;
}
