/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2005-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
* Copyright (C) 2005-2008 - INRIA - Pierrick MODE
* Copyright (C) 2007-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
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

#include "spreadsheet_gw.hxx"
#include "function.hxx"
#include "double.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "xls.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_xls_read(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    // input
    types::Double* pDblId       = NULL;
    types::Double* pDblSheetpos = NULL;

    int iId         = 0;
    int iSheetpos   = 0;
    int rows        = 0;
    int cols        = 0;
    int iErr        = 0;

    int* ind     = NULL;
    double* data = NULL;

    // *** check the minimal number of input args. ***
    if (in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "xls_read", 2);
        return types::Function::Error;
    }

    // *** check number of output args according the methode. ***
    if (_iRetCount != 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "xls_read", 2);
        return types::Function::Error;
    }

    // *** check type of input args and get it. ***
    // file id
    if (in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "xls_read", 1);
        return types::Function::Error;
    }

    pDblId = in[0]->getAs<types::Double>();

    if (pDblId->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "xls_read", 1);
        return types::Function::Error;
    }

    iId = static_cast<int>(pDblId->get(0));

    // sheetpos
    if (in[1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "xls_read", 2);
        return types::Function::Error;
    }

    pDblSheetpos = in[1]->getAs<types::Double>();

    if (pDblSheetpos->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "xls_read", 2);
        return types::Function::Error;
    }

    iSheetpos = static_cast<int>(pDblSheetpos->get(0));

    xls_read(&iId, &iSheetpos, &data, &ind, &rows, &cols, &iErr);
    switch (iErr)
    {
        case 1 :
            Scierror(999, _("%s: No more memory.\n"), "xls_read");
            return types::Function::Error;
        case 2 :
            Scierror(999, _("%s: Failed to read expected data, may be invalid xls file.\n"), "xls_read");
            return types::Function::Error;
        case 3 :
            Scierror(999, _("%s: End of file.\n"), "xls_read");
            return types::Function::Error;
        default :
            /* no error */
            break;
    }

    if (rows * cols)
    {
        types::Double* pDblData = new types::Double(rows, cols);
        pDblData->set(data);
        types::Double* pDblInd = new types::Double(rows, cols);

        for (int i = 0; i < cols; i++)
        {
            for (int j = 0; j < rows; j++)
            {
                pDblInd->set(j, i, static_cast<int>(ind[i * rows + j]));
            }
        }

        out.push_back(pDblData);
        out.push_back(pDblInd);

        free(data);
        free(ind);
    }
    else
    {
        out.push_back(types::Double::Empty());
        out.push_back(types::Double::Empty());
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

