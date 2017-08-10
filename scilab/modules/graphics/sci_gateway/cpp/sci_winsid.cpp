/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
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

#include "graphics_gw.hxx"
#include "function.hxx"
#include "double.hxx"

extern "C"
{
#include "sci_malloc.h"
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