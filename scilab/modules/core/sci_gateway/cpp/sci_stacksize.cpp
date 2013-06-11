/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
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
#include "double.hxx"
#include "scilabWrite.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}

using namespace types;

Function::ReturnValue sci_stacksize(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() > 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "stacksize", 0, 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "stacksize", 0, 1);
        return types::Function::Error;
    }

    scilabWriteW(_W("[WARNING]: stakcsize: No more need to change Scilab stack size. Function will be remove in version 6.1.\n"));

    if (in.size() == 0)
    {
        Double *pDbl = new Double(1, 2);
        double dbl1 = 1.0;
        double dbl0 = dbl1 - dbl1;
        pDbl->set(0, dbl1 / dbl0);
        pDbl->set(1, dbl1 / dbl0);
        out.push_back(pDbl);
    }

    return Function::OK;
}
