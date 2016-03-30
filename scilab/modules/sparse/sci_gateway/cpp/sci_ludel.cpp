/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
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
