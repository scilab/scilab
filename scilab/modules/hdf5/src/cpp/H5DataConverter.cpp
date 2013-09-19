/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "H5DataConverter.hxx"

namespace org_modules_hdf5
{

int * H5DataConverter::getHypermatrix(void * pvApiCtx, const int position, const int ndims, const hsize_t * dims)
{
    static const char * hypermat[3] = {"hm", "dims", "entries"};

    int * list = 0;
    SciErr err = createMList(pvApiCtx, position, 3, &list);
    if (err.iErr)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot create an hypermatrix on the stack"));
    }

    err = createMatrixOfStringInList(pvApiCtx, position, list, 1, 1, 3, hypermat);
    if (err.iErr)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot create an hypermatrix on the stack"));
    }

    if (sizeof(int) == sizeof(hsize_t))
    {
        err = createMatrixOfInteger32InList(pvApiCtx, position, list, 2, 1, ndims, (int *)dims);
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create an hypermatrix on the stack"));
        }
    }
    else
    {
        int * _dims = 0;
        err = allocMatrixOfInteger32InList(pvApiCtx, position, list, 2, 1, ndims, &_dims);
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create an hypermatrix on the stack"));
        }
        for (int i = 0; i < ndims; i++)
        {
            _dims[i] = (int)dims[i];
        }
    }

    return list;
}
}
