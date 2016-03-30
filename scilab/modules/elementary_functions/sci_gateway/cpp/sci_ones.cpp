/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 * Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
 * Copyright (C) 2014 - Scilab Enterprises - Anais Aubert
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
/*--------------------------------------------------------------------------*/
#include "elem_func_gw.hxx"
#include "function.hxx"
#include "overload.hxx"
#include "double.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_ones(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pOut = NULL;

    int iDims = 0;
    int* piDims = NULL;
    bool alloc = false;

    bool ret = getDimsFromArguments(in, "ones", &iDims, &piDims, &alloc);
    if (ret == false)
    {
        switch (iDims)
        {
            case -1 :
                Scierror(21, _("Invalid index.\n"));
                break;
            case 1:
            {
                //call overload
                return Overload::generateNameAndCall(L"ones", in, _iRetCount, out);
            }
        }

        return types::Function::Error;
    }

    pOut = new types::Double(iDims, piDims);
    if (alloc)
    {
        delete[] piDims;
    }

    pOut->setOnes();

    out.push_back(pOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
