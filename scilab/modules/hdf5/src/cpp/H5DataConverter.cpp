/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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
