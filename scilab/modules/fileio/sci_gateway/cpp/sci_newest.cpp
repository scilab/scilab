/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Digiteo - Cedric DELAMARRE
 *
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "fileio_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "double.hxx"
#include "filemanager.hxx"

extern "C"
{
#include <stdio.h>
#include <string.h>
#include "sci_malloc.h"
#include "expandPathVariable.h"
#include "Scierror.h"
#include "localization.h"
#include "newest.h"
#include "freeArrayOfString.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_newest(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iRet                    = 0;
    int iNbrString              = 0;
    char** pStringInput         = NULL;

    if (in.size() == 0)
    {
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }

    if (in.size() == 1)
    {
        if (in[0]->isString() == FALSE)
        {
            if (in[0]->getAs<types::GenericType>()->getSize() == 0)
            {
                out.push_back(types::Double::Empty());
                return types::Function::OK;
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A String(s) expected.\n"), "newest", 1);
                return types::Function::Error;
            }
        }

        if (in[0]->getAs<types::String>()->isScalar())
        {
            out.push_back(new types::Double(1));
            return types::Function::OK;
        }
        else
        {
            int size = in[0]->getAs<types::String>()->getSize();
            pStringInput = (char**)MALLOC(size * sizeof(char*));
            for (iNbrString = 0; iNbrString < size; iNbrString++)
            {
                pStringInput[iNbrString] = in[0]->getAs<types::String>()->get(iNbrString);
            }

            iRet = newest(pStringInput, iNbrString);
            FREE(pStringInput);
            out.push_back(new types::Double(iRet));
        }
    }
    else
    {
        int size = (int)in.size();
        pStringInput = (char**)MALLOC(size * sizeof(char*));
        for (iNbrString = 0; iNbrString < size; iNbrString++)
        {
            if (in[iNbrString]->isString() == FALSE)
            {
                FREE(pStringInput);
                Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "newest", iNbrString + 1);
                return types::Function::Error;
            }
            pStringInput[iNbrString] = in[iNbrString]->getAs<types::String>()->get(0);
        }

        if (in[1]->getAs<types::String>()->isScalar() == false)
        {
            FREE(pStringInput);
            Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), "newest", 2);
            return types::Function::Error;
        }

        iRet = newest(pStringInput, iNbrString);
        FREE(pStringInput);
        out.push_back(new types::Double((double)iRet));
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
