/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises
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

#include <api_scilab.h>
#include "localization.h"

int hypermatIntExample(char *fname, void* pvApiCtx)
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
                case SCI_INT64:
                    sciErr = getHypermatOfInteger64(pvApiCtx, piAddr, &dims, &ndims, (long long*)&data);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return sciErr.iErr;
                    }

                    sciErr = createHypermatOfInteger64(pvApiCtx, nbInputArgument(pvApiCtx) + 1, dims, ndims, (const long long*)data);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return sciErr.iErr;
                    }
                    break;
                case SCI_UINT64:
                    sciErr = getHypermatOfUnsignedInteger64(pvApiCtx, piAddr, &dims, &ndims, (unsigned long long*)&data);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return sciErr.iErr;
                    }

                    sciErr = createHypermatOfUnsignedInteger64(pvApiCtx, nbInputArgument(pvApiCtx) + 1, dims, ndims, (const unsigned long long*)data);
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
