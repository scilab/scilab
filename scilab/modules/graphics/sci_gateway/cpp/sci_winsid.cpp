/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "graphics_gw.hxx"
#include "function.hxx"
#include "double.hxx"

extern "C"
{
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
#include "FigureList.h"
#include "returnProperty.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_winsid(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    void* status = NULL;

    if (in.size() != 0)
    {
        //Error
        return types::Function::Error;
    }

    int nbFigure = sciGetNbFigure();
    if (nbFigure <= 0)
    {
        /* There is no figure */
        status = sciReturnEmptyMatrix();
    }
    else
    {
        int * ids = (int*)MALLOC(nbFigure * sizeof(int));
        if (ids == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), "winsid");
            return types::Function::Error;
        }
        sciGetFiguresId(ids);

        status = sciReturnRowIntVector(ids, nbFigure);
        FREE(ids);
    }

    out.push_back((types::InternalType*)status);
    return types::Function::OK;
}