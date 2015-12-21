/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "sparse_gw.hxx"
#include "function.hxx"
#include "pointer.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "lu.h"
}

types::Function::ReturnValue sci_ludel(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int ierr        = 0;
    int *fmatindex  = NULL;
    const void *pData = NULL;

    //check input parameters
    if (in.size() != 1 )
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d  expected.\n"), "ludel", 1);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output argument(s): %d expected.\n"), "ludel", 1);
        return types::Function::Error;
    }

    if (in[0]->isPointer() == false)
    {
        Scierror(999, _("%s: Wrong type for argument %d:  Handle to sparse lu factors expected.\n"), "ludel", 1);
        return types::Function::Error;
    }

    types::Pointer *pPointerIn = in[0]->getAs<types::Pointer>();
    pData = pPointerIn->get();
    fmatindex = (int*)pData;

    C2F(ludel1)(fmatindex, &ierr);
    if (ierr != 0)
    {
        Scierror(999, _("Wrong value for argument #%d: the lu handle is no more valid.\n"), 1);
        return types::Function::Error;
    }

    return types::Function::OK;
}
