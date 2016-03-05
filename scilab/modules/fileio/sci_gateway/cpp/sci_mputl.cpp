/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
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
#include "filemanager.hxx"
#include "fileio_gw.hxx"
#include "string.hxx"
#include "double.hxx"
#include "bool.hxx"
#include "function.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "mputl.h"
#include "localization.h"
#include "Scierror.h"
#include "mopen.h"
#include "mclose.h"
#include "expandPathVariable.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_mputl(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iFileID     = 0;
    int iErr        = 0;
    bool bCloseFile = false;

    if (in.size() != 2)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "mputl", 2);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output argument(s): %d expected.\n"), "mputl", 1);
        return types::Function::Error;
    }

    if (in[1]->isDouble() && in[1]->getAs<types::Double>()->getSize() == 1)
    {
        iFileID = static_cast<int>(in[1]->getAs<types::Double>()->get(0));
    }
    else if (in[1]->isString() && in[1]->getAs<types::String>()->getSize() == 1)
    {
        wchar_t *expandedFileName = expandPathVariableW(in[1]->getAs<types::String>()->get(0));

        iErr = mopen(expandedFileName, L"wt", 0, &iFileID);
        FREE(expandedFileName);

        if (iErr)
        {
            char* pst = wide_string_to_UTF8(in[1]->getAs<types::String>()->get(0));
            switch (iErr)
            {
                case MOPEN_NO_MORE_LOGICAL_UNIT:
                    Scierror(66, _("%s: Too many files opened!\n"), "mputl");
                    break;
                case MOPEN_CAN_NOT_OPEN_FILE:
                    Scierror(999, _("%s: Cannot open file %s.\n"), "mputl", pst);
                    break;
                case MOPEN_NO_MORE_MEMORY:
                    Scierror(999, _("%s: No more memory.\n"), "mputl");
                    break;
                case MOPEN_INVALID_FILENAME:
                    Scierror(999, _("%s: invalid filename %s.\n"), "mputl", pst);
                    break;
                default: //MOPEN_INVALID_STATUS
                    Scierror(999, _("%s: invalid status.\n"), "mputl");
                    break;
            }
            FREE(pst);
            return types::Function::Error;
        }

        bCloseFile = true;
    }
    else
    {
        //Error
        Scierror(999, _("%s: Wrong type for input argument #%d: a String or Integer expected.\n"), "mputl", 2);
        return types::Function::Error;
    }

    //String vextor, row or col
    if (in[0]->isString() == false || (in[0]->getAs<types::String>()->getRows() != 1 && in[0]->getAs<types::String>()->getCols() != 1))
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A 1-by-n or m-by-1 array expected.\n"), "mputl", 1);
        return types::Function::Error;
    }

    types::String* pS = in[0]->getAs<types::String>();

    switch (iFileID)
    {
        case 5: // stdin
            Scierror(999, _("%s: Wrong file descriptor: %d.\n"), "mputl", iFileID);
            return types::Function::Error;
        default :
            iErr = mputl(iFileID, pS->get(), pS->getSize());
    }

    out.push_back(new types::Bool(!iErr));

    if (bCloseFile)
    {
        mclose(iFileID);
    }

    return types::Function::OK;


    //mputlErr = mputl(fileDescriptor, pStVarOne, mnOne);
    //freeArrayOfString(pStVarOne, mnOne);

    //if (bCloseFile)
    //{
    //    double dErrClose = 0.;
    //    C2F(mclose)(&fileDescriptor, &dErrClose);
    //    bCloseFile = FALSE;
    //}

    //switch (mputlErr)
    //{
    //case MPUTL_NO_ERROR:
    //    createScalarBoolean(pvApiCtx, Rhs + 1, TRUE);
    //    LhsVar(1) = Rhs + 1;
    //    PutLhsVar();
    //    break;

    //case MPUTL_INVALID_FILE_DESCRIPTOR:
    //    // commented for compatiblity
    //    // Scierror(999, _("%s: invalid file descriptor.\n"), fname);
    //    // break;
    //case MPUTL_ERROR:
    //case MPUTL_NO_WRITE_RIGHT:
    //default:
    //    createScalarBoolean(pvApiCtx, Rhs + 1, FALSE);
    //    LhsVar(1) = Rhs + 1;
    //    PutLhsVar();
    //    break;
    //}

    //return 0;
}
/*--------------------------------------------------------------------------*/
