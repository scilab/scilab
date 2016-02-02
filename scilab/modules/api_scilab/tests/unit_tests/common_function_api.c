/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO
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

#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"

SciErr printf_info(void* _pvCtx, int _iVar);

int common_function(char *fname, void *pvApiCtx )
{
    SciErr sciErr;
    int i;
    int *piAddr1    = NULL;
    int iBool       = 0;

    for (i = 0 ; i < nbInputArgument(pvApiCtx) ; i++)
    {
        sciErr = printf_info(pvApiCtx, i + 1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            break;
        }
        sciprint("\n\n");
    }

    //1 for true, 0 for false
    iBool = sciErr.iErr == 0 ? 1 : 0;

    sciErr = createMatrixOfBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, 1, &iBool);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //assign allocated variables to Lhs position
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    return 0;
}

SciErr printf_info(void* _pvCtx, int _iVar)
{
    SciErr sciErr;
    int* piAddr     = NULL;
    int iType       = 0;
    int iRows       = 0;
    int iCols       = 0;
    int iItem       = 0;
    int iComplex    = 0;

    sciErr = getVarAddressFromPosition(_pvCtx, _iVar, &piAddr);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    sciprint("Variable %d information:\n", _iVar);

    sciErr = getVarType(_pvCtx, piAddr, &iType);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    sciprint("\tType: ");
    switch (iType)
    {
        case sci_matrix :
            sciprint("double\n");
            break;
        case sci_poly :
            sciprint("polynomial\n");
            break;
        case sci_boolean :
            sciprint("boolean\n");
            break;
        case sci_sparse :
            sciprint("sparse\n");
            break;
        case sci_boolean_sparse :
            sciprint("boolean_sparse\n");
            break;
        case sci_ints :
        {
            char pstSigned[]    = "signed";
            char pstUnsigned[]  = "unsigned";
            char* pstSign       = pstSigned;
            int iPrec           = 0;

            sciErr = getMatrixOfIntegerPrecision(_pvCtx, piAddr, &iPrec);
            if (sciErr.iErr)
            {
                return sciErr;
            }

            if (iPrec > 10)
            {
                pstSign = pstUnsigned;
            }

            sciprint("%s integer %d bits\n", pstSign, (iPrec % 10) * 8);
        }
        break;
        case sci_strings :
            sciprint("strings\n");
            break;
        case sci_list :
            sciprint("list\n");
            break;
        case sci_tlist :
            sciprint("tlist\n");
            break;
        case sci_mlist :
            sciprint("mlist\n");
            break;
        default :
            sciprint("Not manage by this function\n");
            return sciErr;
    }

    if (isVarComplex(_pvCtx, piAddr))
    {
        sciprint("\tComplex: Yes\n");
    }

    sciprint("\tDimensions: ");
    if (isVarMatrixType(_pvCtx, piAddr))
    {
        sciErr = getVarDimension(_pvCtx, piAddr, &iRows, &iCols);
        if (sciErr.iErr)
        {
            return sciErr;
        }

        sciprint("%d x %d", iRows, iCols);
    }
    else
    {
        sciErr = getListItemNumber(_pvCtx, piAddr, &iItem);
        if (sciErr.iErr)
        {
            return sciErr;
        }

        sciprint("%d", iItem);
    }
    return sciErr;
}
