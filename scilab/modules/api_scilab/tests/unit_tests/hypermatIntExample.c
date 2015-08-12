/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "MALLOC.h"

int hypermatIntExample(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int* piAddr = NULL;
    int iType   = 0;
    int iRet    = 0;

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 0, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (isEmptyMatrix(pvApiCtx, piAddr))
    {
        iRet = createEmptyMatrix(pvApiCtx, nbInputArgument(pvApiCtx) + 1);
        if (iRet)
        {
            return iRet;
        }

        AssignOutputVariable(pvApiCtx, 1) = 0;
    }
    else if (isHypermatType(pvApiCtx, piAddr))
    {
        int * dims = NULL;
        int ndims;
        void * data = NULL;
        int htype = 0;
        int precision;

        sciErr = getHypermatType(pvApiCtx, piAddr, &htype);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return sciErr.iErr;
        }

        if (htype == sci_ints)
        {
            sciErr = getHypermatOfIntegerPrecision(pvApiCtx, piAddr, &precision);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return sciErr.iErr;
            }

            switch (precision)
            {
                case SCI_INT8:
                    sciErr = getHypermatOfInteger8(pvApiCtx, piAddr, &dims, &ndims, (char*)&data);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return sciErr.iErr;
                    }

                    sciErr = createHypermatOfInteger8(pvApiCtx, nbInputArgument(pvApiCtx) + 1, dims, ndims, (const char*)data);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return sciErr.iErr;
                    }
                    break;
                case SCI_UINT8:
                    sciErr = getHypermatOfUnsignedInteger8(pvApiCtx, piAddr, &dims, &ndims, (unsigned char*)&data);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return sciErr.iErr;
                    }

                    sciErr = createHypermatOfUnsignedInteger8(pvApiCtx, nbInputArgument(pvApiCtx) + 1, dims, ndims, (const unsigned char*)data);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return sciErr.iErr;
                    }
                    break;
                case SCI_INT16:
                    sciErr = getHypermatOfInteger16(pvApiCtx, piAddr, &dims, &ndims, (short*)&data);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return sciErr.iErr;
                    }

                    sciErr = createHypermatOfInteger16(pvApiCtx, nbInputArgument(pvApiCtx) + 1, dims, ndims, (const short*)data);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return sciErr.iErr;
                    }
                    break;
                case SCI_UINT16:
                    sciErr = getHypermatOfUnsignedInteger16(pvApiCtx, piAddr, &dims, &ndims, (unsigned short*)&data);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return sciErr.iErr;
                    }

                    sciErr = createHypermatOfUnsignedInteger16(pvApiCtx, nbInputArgument(pvApiCtx) + 1, dims, ndims, (const unsigned short*)data);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return sciErr.iErr;
                    }
                    break;
                case SCI_INT32:
                    sciErr = getHypermatOfInteger32(pvApiCtx, piAddr, &dims, &ndims, (int*)&data);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return sciErr.iErr;
                    }

                    sciErr = createHypermatOfInteger32(pvApiCtx, nbInputArgument(pvApiCtx) + 1, dims, ndims, (const int*)data);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return sciErr.iErr;
                    }
                    break;
                case SCI_UINT32:
                    sciErr = getHypermatOfUnsignedInteger32(pvApiCtx, piAddr, &dims, &ndims, (unsigned int*)&data);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return sciErr.iErr;
                    }

                    sciErr = createHypermatOfUnsignedInteger32(pvApiCtx, nbInputArgument(pvApiCtx) + 1, dims, ndims, (const unsigned int*)data);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return sciErr.iErr;
                    }
                    break;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: An integer expected.\n"), fname, 1);
            return 1;
        }

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    }

    return 0;
}
