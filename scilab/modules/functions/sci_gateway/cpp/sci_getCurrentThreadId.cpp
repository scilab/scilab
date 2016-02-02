/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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

#include "functions_gw.hxx"
#include "configvariable.hxx"
#include "function.hxx"
#include "threadId.hxx"

extern "C"
{
#define NOMINMAX
#include "Thread_Wrapper.h"
#include "Scierror.h"
#include "localization.h"
}

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
