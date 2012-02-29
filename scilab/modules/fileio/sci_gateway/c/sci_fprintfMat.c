/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA
* Copyright (C) DIGITEO - 2010 - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#include <string.h>
#include "api_scilab.h"
#include "MALLOC.h"
#include "gw_fileio.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "expandPathVariable.h"

#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "fprintfMat.h"
/*--------------------------------------------------------------------------*/
int sci_fprintfMat(char *fname,unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int m1 = 0, n1 = 0;
    int iType1 = 0;

    int *piAddressVarTwo = NULL;
    int m2 = 0, n2 = 0;
    int iType2 = 0;

    fprintfMatError ierr = FPRINTFMAT_ERROR;

    char *filename = NULL;
    char *expandedFilename = NULL;
    char **textAdded = NULL;
    char *Format = NULL;
    double *dValues = NULL;
    char *separator = NULL;
    int m4n4 = 0;
    int i = 0;

    Nbvars = 0;
    CheckRhs(1,5);
    CheckLhs(1,1);

    if (Rhs >= 3)
    {
        int *piAddressVarThree = NULL;
        int iType3	= 0;
        int m3 = 0, n3 = 0;

        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
            return 0;
        }

        sciErr = getVarType(pvApiCtx, piAddressVarThree, &iType3);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
            return 0;
        }

        if (iType3 != sci_strings)
        {
            Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 3);
            return 0;
        }

        sciErr = getVarDimension(pvApiCtx, piAddressVarThree, &m3, &n3);

        if ( (m3 != n3) && (n3 != 1) ) 
        {
            Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 3);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddressVarThree, &Format))
        {
            Scierror(999,_("%s: Memory allocation error.\n"), fname);
            return 0;
        }
    }
    else
    {
        Format = strdup(DEFAULT_FPRINTFMAT_FORMAT);
    }

    if ( Rhs >= 4 )
    {
        int *piAddressVarFour = NULL;
        int *lengthStrings = NULL;
        int iType4	= 0;
        int m4 = 0, n4 = 0;

        sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddressVarFour);
        if(sciErr.iErr)
        {
            if (Format) {FREE(Format); Format = NULL;}
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
            return 0;
        }

        sciErr = getVarType(pvApiCtx, piAddressVarFour, &iType4);
        if(sciErr.iErr)
        {
            if (Format) {FREE(Format); Format = NULL;}
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
            return 0;
        }

        if (iType4 != sci_strings)
        {
            if (Format) {FREE(Format); Format = NULL;}
            Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 4);
            return 0;
        }

        sciErr = getVarDimension(pvApiCtx, piAddressVarFour, &m4, &n4);
        if(sciErr.iErr)
        {
            if (Format) {FREE(Format); Format = NULL;}
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
            return 0;
        }

        if  (! ((m4 == 1) || (n4 == 1)))
        {
            if (Format) {FREE(Format); Format = NULL;}
            Scierror(999,_("%s: Wrong size for input argument #%d.\n"), fname, 4);
            return 0;
        }

        lengthStrings = (int*)MALLOC(sizeof(int) * (m4 * n4));
        if (lengthStrings == NULL)
        {
            if (Format) {FREE(Format); Format = NULL;}
            Scierror(999,_("%s: Memory allocation error.\n"),fname);
            return 0;
        }

        // get lengthStrings value
        sciErr = getMatrixOfString(pvApiCtx, piAddressVarFour, &m4, &n4, lengthStrings, NULL);
        if(sciErr.iErr)
        {
            if (Format) {FREE(Format); Format = NULL;}
            if (lengthStrings) {FREE(lengthStrings); lengthStrings = NULL;}
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
            return 0;
        }

        textAdded = (char**)MALLOC(sizeof(char*) * (m4 * n4));
        if (textAdded == NULL)
        {
            if (Format) {FREE(Format); Format = NULL;}
            if (lengthStrings) {FREE(lengthStrings); lengthStrings = NULL;}
            Scierror(999,_("%s: Memory allocation error.\n"),fname);
            return 0;
        }

        for (i = 0; i < (m4 * n4); i++)
        {
            textAdded[i] = (char*)MALLOC(sizeof(char) * (lengthStrings[i] + 1));
            if (textAdded[i] == NULL)
            {
                freeArrayOfString(textAdded, m4 * n4);
                if (Format) {FREE(Format); Format = NULL;}
                if (lengthStrings) {FREE(lengthStrings); lengthStrings = NULL;}
                Scierror(999,_("%s: Memory allocation error.\n"),fname);
                return 0;
            }
        }

        // get textAdded
        sciErr = getMatrixOfString(pvApiCtx, piAddressVarFour, &m4, &n4, lengthStrings, textAdded);
        if (lengthStrings) {FREE(lengthStrings); lengthStrings = NULL;}
        if(sciErr.iErr)
        {
            freeArrayOfString(textAdded, m4 * n4);
            if (Format) {FREE(Format); Format = NULL;}
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
            return 0;
        }

        m4n4 = m4 * n4;
    }

    if (Rhs > 4)
    {
        int *piAddressVarFive = NULL;
        int iType5	= 0;
        int m5 = 0, n5 = 0;

        sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddressVarFive);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 5);
            return 0;
        }

        sciErr = getVarType(pvApiCtx, piAddressVarFive, &iType5);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 5);
            return 0;
        }

        if (iType5 != sci_strings)
        {
            Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 5);
            return 0;
        }

        sciErr = getVarDimension(pvApiCtx, piAddressVarFive, &m5, &n5);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 5);
            return 0;
        }

        if ( (m5 != n5) && (n5 != 1) ) 
        {
            Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 5);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddressVarFive, &separator))
        {
            Scierror(999,_("%s: Memory allocation error.\n"), fname);
            return 0;
        }
    }
    else
    {
        separator = strdup(DEFAULT_FPRINTFMAT_SEPARATOR);
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if(sciErr.iErr)
    {
        if (textAdded) freeArrayOfString(textAdded, m4n4);
        if (Format) {FREE(Format); Format = NULL;}
        if (separator){FREE(separator); separator = NULL;}
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarTwo, &iType2);
    if(sciErr.iErr)
    {
        if (textAdded) freeArrayOfString(textAdded, m4n4);
        if (Format) {FREE(Format); Format = NULL;}
        if (separator){FREE(separator); separator = NULL;}
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (iType2 != sci_matrix)
    {
        if (textAdded) freeArrayOfString(textAdded, m4n4);
        if (Format) {FREE(Format); Format = NULL;}
        if (separator){FREE(separator); separator = NULL;}
        Scierror(999,_("%s: Wrong type for input argument #%d: Matrix of floating point numbers expected.\n"), fname, 2);
        return 0;
    }

    if (isVarComplex(pvApiCtx, piAddressVarTwo))
    {
        if (textAdded) freeArrayOfString(textAdded, m4n4);
        if (Format) {FREE(Format); Format = NULL;}
        if (separator){FREE(separator); separator = NULL;}
        Scierror(999,_("%s: Wrong type for input argument #%d: Real values expected.\n"), fname, 2);
        return 0;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarTwo, &m2, &n2, &dValues);
    if(sciErr.iErr)
    {
        if (textAdded) freeArrayOfString(textAdded, m4n4);
        if (Format) {FREE(Format); Format = NULL;}
        if (separator){FREE(separator); separator = NULL;}
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if(sciErr.iErr)
    {
        if (textAdded) freeArrayOfString(textAdded, m4n4);
        if (Format) {FREE(Format); Format = NULL;}
        if (separator){FREE(separator); separator = NULL;}
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType1);
    if(sciErr.iErr)
    {
        if (textAdded) freeArrayOfString(textAdded, m4n4);
        if (Format) {FREE(Format); Format = NULL;}
        if (separator){FREE(separator); separator = NULL;}
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (iType1 != sci_strings)
    {
        if (textAdded) freeArrayOfString(textAdded, m4n4);
        if (Format) {FREE(Format); Format = NULL;}
        if (separator){FREE(separator); separator = NULL;}
        Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarDimension(pvApiCtx, piAddressVarOne, &m1, &n1);

    if ( (m1 != n1) && (n1 != 1) ) 
    {
        if (textAdded) freeArrayOfString(textAdded, m4n4);
        if (Format) {FREE(Format); Format = NULL;}
        if (separator){FREE(separator); separator = NULL;}
        Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddressVarOne, &filename))
    {
        if (textAdded) freeArrayOfString(textAdded, m4n4);
        if (Format) {FREE(Format); Format = NULL;}
        if (separator){FREE(separator); separator = NULL;}
        Scierror(999,_("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    expandedFilename = expandPathVariable(filename);
    ierr = fprintfMat(expandedFilename, Format, separator, dValues, m2, n2,textAdded, m4n4);
    if (expandedFilename) {FREE(expandedFilename); expandedFilename = NULL;}
    if (textAdded) freeArrayOfString(textAdded, m4n4);
    if (Format) {FREE(Format); Format = NULL;}
    if (separator){FREE(separator); separator = NULL;}

    switch(ierr)
    {
    case FPRINTFMAT_NO_ERROR:
        {
            LhsVar(1) = 0;
            if (filename) {FREE(filename); filename = NULL;}
            PutLhsVar();
        }
        break;
    case FPRINTFMAT_FOPEN_ERROR:
        {
            Scierror(999,_("%s: can not open file %s.\n"), fname, filename);
        }
        break;
    case FPRINTMAT_FORMAT_ERROR:
        {
            Scierror(999,_("%s: Invalid format.\n"), fname);
        }
        break;
    default:
    case FPRINTFMAT_ERROR:
        {
            Scierror(999,_("%s: error.\n"), fname);
        }
        break;
    }

    if (filename) {FREE(filename); filename = NULL;}
    return 0;
}
/*--------------------------------------------------------------------------*/

