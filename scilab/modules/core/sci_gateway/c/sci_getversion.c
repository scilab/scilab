/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Bruno JOFRET
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "api_scilab.h"
#include "gw_core.h"
#include "getversion.h"
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
#include "with_module.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
#define VERSION_STRING "string_info"
/*--------------------------------------------------------------------------*/
static int getversion_no_rhs(char *fname);
static int getversion_one_rhs(char *fname);
static int getversion_two_rhs(char *fname);
/*--------------------------------------------------------------------------*/
int sci_getversion(char *fname, unsigned long fname_len)
{
    Rhs = Max(0, Rhs);

    CheckRhs(0, 2);

    if (Rhs == 0)
    {
        CheckLhs(1, 2);
        getversion_no_rhs(fname);
    }
    else if (Rhs == 1)
    {
        CheckLhs(1, 1);
        getversion_one_rhs(fname);
    }
    else /* Rhs == 2 */
    {
        CheckLhs(1, 1);
        getversion_two_rhs(fname);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
int getversion_no_rhs(char *fname)
{
    char *version = getScilabVersionAsString();
    if (version)
    {
        createSingleString(pvApiCtx, Rhs + 1, version);
        LhsVar(1) = Rhs + 1;
        FREE(version);
        version = NULL;
    }
    else
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    if (Lhs == 2)
    {
        int sizeOptions = 0;
        char **ScilabOptions = getScilabVersionOptions(&sizeOptions);

        if (ScilabOptions)
        {
            SciErr sciErr;
            int m = 1;
            int n = sizeOptions;
            sciErr = createMatrixOfString(pvApiCtx, Rhs + 2, m, n, ScilabOptions);
            freeArrayOfString(ScilabOptions, sizeOptions);

            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            LhsVar(2) = Rhs + 2;
        }
        else
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }
    }

    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
int getversion_one_rhs(char *fname)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddressVarOne))
    {
        char *modulename = NULL;

        if (!isScalar(pvApiCtx, piAddressVarOne))
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: String expected.\n"), fname, 1);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddressVarOne, &modulename) == 0)
        {
            if (modulename)
            {
                if ( with_module(modulename) || (strcmp(modulename, "scilab") == 0) )
                {
                    int versionSize = 0;
                    int *version = getModuleVersion(modulename, &versionSize);

                    if (version)
                    {
                        int m = 1;
                        int n = versionSize;
                        double *versionAsDouble = (double*)MALLOC(sizeof(double) * versionSize);
                        if (versionAsDouble)
                        {
                            int i = 0;
                            for (i = 0; i < versionSize; i++)
                            {
                                versionAsDouble[i] = (double)version[i];
                            }
                            FREE(version);
                            version = NULL;

                            freeAllocatedSingleString(modulename);
                            modulename = NULL;

                            sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, m, n, versionAsDouble);
                            FREE(versionAsDouble);
                            versionAsDouble = NULL;

                            if (sciErr.iErr)
                            {
                                printError(&sciErr, 0);
                                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                                return 0;
                            }

                            LhsVar(1) = Rhs + 1;
                            PutLhsVar();
                        }
                        else
                        {
                            Scierror(999, _("%s: Memory allocation error.\n"), fname);
                            return 0;
                        }
                    }
                    else
                    {
                        Scierror(999, _("%s: Wrong file version.xml %s.\n"), fname, modulename);

                        freeAllocatedSingleString(modulename);
                        modulename = NULL;

                        return 0;
                    }
                }
                else
                {
                    Scierror(999, _("%s: Wrong module name %s.\n"), fname , modulename);

                    freeAllocatedSingleString(modulename);
                    modulename = NULL;

                    return 0;
                }
                freeAllocatedSingleString(modulename);
                modulename = NULL;
            }
            else
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }
        }
        else
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
        return 0;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
int getversion_two_rhs(char *fname)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int *piAddressVarTwo = NULL;

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddressVarOne) && isStringType(pvApiCtx, piAddressVarTwo))
    {
        char *modulename = NULL;
        char *optionname = NULL;

        if (!isScalar(pvApiCtx, piAddressVarOne))
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: String expected.\n"), fname, 1);
            return 0;
        }

        if (!isScalar(pvApiCtx, piAddressVarTwo))
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: String expected.\n"), fname, 2);
            return 0;
        }

        if ( (getAllocatedSingleString(pvApiCtx, piAddressVarOne, &modulename) == 0) &&
                (getAllocatedSingleString(pvApiCtx, piAddressVarTwo, &optionname) == 0) )
        {
            if ( (modulename) && (optionname) )
            {
                if ( with_module(modulename) || (strcmp(modulename, "scilab") == 0) )
                {
                    if ( strcmp(optionname, VERSION_STRING) == 0)
                    {
                        char *versionInfo = getModuleVersionInfoAsString(modulename);

                        if (versionInfo)
                        {
                            createSingleString(pvApiCtx, Rhs + 1, versionInfo);

                            FREE(versionInfo);
                            versionInfo = NULL;

                            LhsVar(1) = Rhs + 1;
                            PutLhsVar();
                        }
                        else
                        {
                            Scierror(999, _("%s: Wrong file version.xml %s.\n"), fname, modulename);

                            freeAllocatedSingleString(modulename);
                            modulename = NULL;
                            freeAllocatedSingleString(optionname);
                            optionname = NULL;

                            return 0;
                        }
                    }
                    else
                    {
                        freeAllocatedSingleString(modulename);
                        modulename = NULL;
                        freeAllocatedSingleString(optionname);
                        optionname = NULL;

                        Scierror(999, _("%s: Wrong value for input argument #%d: '%s' expected.\n"), fname, 2, VERSION_STRING);
                        return 0;
                    }
                }
                else
                {
                    Scierror(999, _("%s: Wrong module name %s.\n"), fname , modulename);

                    freeAllocatedSingleString(modulename);
                    modulename = NULL;
                    freeAllocatedSingleString(optionname);
                    optionname = NULL;

                    return 0;
                }
            }
            else
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }
        }
        else
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input arguments #%d and #%d: Strings expected.\n"), fname, 1, 2);
        return 0;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
