/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007 - INRIA - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#include "gw_core.h"
#include "stack-c.h"
#include "api_scilab.h"
#include "api_oldstack.h"
#include "libraryinfo.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int sci_libraryinfo(char *fname, int *_piKey)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;

    CheckRhs(1,1);
    CheckLhs(1,2);

    sciErr = getVarAddressFromPosition(_piKey, 1, &piAddressVarOne);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (isStringType(_piKey, piAddressVarOne))
    {
        char *libraryname = NULL;

        if (!isScalar(_piKey, piAddressVarOne))
        {
            Scierror(999,_("%s: Wrong size for input argument #%d: String expected.\n"), fname, 1);
            return 0;
        }

        if (getAllocatedSingleString(_piKey, piAddressVarOne, &libraryname) == 0)
        {
            if (libraryname)
            {
                char *pathlibrary = getlibrarypath(libraryname);
                if (pathlibrary)
                {
                    int sizemacrosarray = 0;
                    char **macros = getlistmacrosfromlibrary(libraryname, &sizemacrosarray);
                    if (macros)
                    {
                        int m = sizemacrosarray;
                        int n = 1;

                        sciErr = createMatrixOfString(_piKey, Rhs + 1, m, n, macros);
                        if(sciErr.iErr)
                        {
                            freeArrayOfString(macros, sizemacrosarray);
                            if (pathlibrary)
                            {
                                FREE(pathlibrary);
                                pathlibrary = NULL;
                            }

                            if (libraryname)
                            {
                                freeAllocatedSingleString(libraryname);
                                libraryname = NULL;
                            }
                            printError(&sciErr, 0);
                            return 0;
                        }
                    }
                    else
                    {
                        createEmptyMatrix(_piKey, Rhs + 1);
                    }
                    LhsVar(1) = Rhs+1;

                    freeArrayOfString(macros, sizemacrosarray);

                    if (Lhs == 2)
                    {
                        createSingleString(_piKey, Rhs + 2, pathlibrary);
                        LhsVar(2) = Rhs+2;
                    }

                    if (pathlibrary) {FREE(pathlibrary);pathlibrary=NULL;}

                    PutLhsVar();
                }
                else
                {
                    Scierror(999,_("%s: Invalid library %s.\n"),fname, libraryname);
                }

                if (libraryname)
                {
                    freeAllocatedSingleString(libraryname);
                    libraryname = NULL;
                }
            }
            else
            {
                Scierror(999,_("%s: Memory allocation error.\n"), fname);
            }
        }
        else
        {
            Scierror(999,_("%s: Memory allocation error.\n"), fname);
        }
    }
    else
    {
        Scierror(999,_("%s: Wrong type of input argument #%d: String expected.\n"),fname,1);
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
