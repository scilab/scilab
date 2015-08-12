/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 * Copyright (C) 2012 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "Xcos.hxx"
#include "GiwsException.hxx"

extern "C" {
#include "gw_xcos.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "api_scilab.h"
#include "localization.h"
#include "getFullFilename.h"
#include "Scierror.h"
#include "getScilabJavaVM.h"
}

using namespace org_scilab_modules_xcos;

static int callXcos(char* fname, char* file, char* var);

/*--------------------------------------------------------------------------*/
int sci_Xcos(char *fname, unsigned long fname_len)
{
    CheckRhs(0, 2);
    CheckLhs(0, 1);

    int *piAddressVar = NULL;
    int iType = 0;
    SciErr sciErr;

    /*
     * xcos() call
     */
    if (Rhs == 0)
    {
        LhsVar(1) = 0;
        PutLhsVar();

        return callXcos(fname, NULL, NULL);
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVar);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddressVar, &iType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    /*
     * xcos("file.zcos") or xcos(["file.zcos" "foo.zcos"]) call
     */
    if (Rhs == 1 && iType == sci_strings)
    {
        int i;
        int m = 0, n = 0;
        int* len = NULL;
        char **var = NULL;

        sciErr = getMatrixOfString(pvApiCtx, piAddressVar, &m, &n, NULL, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname,
                     1);
            return 0;
        }

        len = (int*) MALLOC(sizeof(int) * (m * n));
        if (len == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        sciErr = getMatrixOfString(pvApiCtx, piAddressVar, &m, &n, len, NULL);
        if (sciErr.iErr)
        {
            FREE(len);

            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname,
                     1);
            return 0;
        }

        var = (char**) MALLOC(sizeof(char*) * (m * n));
        if (var == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        for (i = 0; i < m * n; i++)
        {
            var[i] = (char*) MALLOC(sizeof(char) * (len[i] + 1));
            if (var[i] == NULL)
            {
                FREE(len);
                freeArrayOfString(var, i);

                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }
        }

        sciErr = getMatrixOfString(pvApiCtx, piAddressVar, &m, &n, len,
                                   var);
        if (sciErr.iErr)
        {
            FREE(len);
            freeArrayOfString(var, m * n);

            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname,
                     1);
            return 0;
        }

        for (i = m * n - 1; i >= 0; i--)
        {
            char* file = getFullFilename(var[i]);
            if (file == NULL)
            {
                FREE(len);
                freeArrayOfString(var, m * n);
                return 1;
            }
            if (callXcos(fname, file, NULL))
            {
                FREE(len);
                freeArrayOfString(var, m * n);
                return 1;
            }
        }

        FREE(len);
        freeArrayOfString(var, m * n);

        LhsVar(1) = 0;
        PutLhsVar();
        return 0;
    }

    /*
     * xcos(scs_m) call
     */
    if (Rhs == 1 && iType == sci_mlist)
    {
        int lw = 1;

        // overloaded by %diagram_xcos.sci
        C2F(overload)(&lw, fname, fname_len);

        LhsVar(1) = 0;
        PutLhsVar();
        return 0;
    }

    /*
     * xcos(scs_m, "scs_m") call (usually from the overload macro)
     */
    if (Rhs == 2 && iType == sci_mlist)
    {
        char* variable = NULL;

        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVar);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname,
                     2);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddressVar, &variable))
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        LhsVar(1) = 0;
        PutLhsVar();
        return callXcos(fname, NULL, variable);
    }

    /*
     * if not returned yet, disp the error message.
     */
    Scierror(999,
             _("%s: Wrong type for input argument #%d: A string expected.\n"),
             fname, 1);
    return 0;
}
/*--------------------------------------------------------------------------*/

static int callXcos(char *fname, char* file, char* var)
{
    try
    {
        Xcos::xcos(getScilabJavaVM(), file, var);
    }
    catch (GiwsException::JniCallMethodException &exception)
    {
        Scierror(999, "%s: %s\n", fname,
                 exception.getJavaDescription().c_str());

        if (file)
        {
            FREE(file);
        }
        if (var)
        {
            FREE(var);
        }
        return 1;
    }
    catch (GiwsException::JniException &exception)
    {
        Scierror(999, "%s: %s\n", fname, exception.whatStr().c_str());

        if (file)
        {
            FREE(file);
        }
        if (var)
        {
            FREE(var);
        }
        return 1;
    }

    if (file)
    {
        FREE(file);
    }
    if (var)
    {
        FREE(var);
    }
    return 0;
}
