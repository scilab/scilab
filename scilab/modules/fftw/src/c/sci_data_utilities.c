/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 - INRIA - Serge STEER
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#include "stack-c.h"
#include "MALLOC.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h" 
/*--------------------------------------------------------------------------*/
int getArrayOfDouble(void* _pvCtx, int *piAddr, int *ndims, int **dims, double **Ar, double **Ai)
{
    SciErr sciErr;
    int *piAddrChild = NULL;
    int *piOffset = NULL;
    int *piData = NULL;
    int nItems = 0;
    int iRows = 0;
    int iCols = 0;
    int iType = 0;

    sciErr = getVarType(_pvCtx, piAddr, &iType);
    if (iType == sci_matrix)
    {
        *ndims = 2;
        *dims = &(piAddr[1]);
        if (isVarComplex(_pvCtx, piAddr))
        {
            getComplexMatrixOfDouble(_pvCtx, piAddr, &iRows, &iCols, Ar, Ai);
        }
        else
        {
            getMatrixOfDouble(_pvCtx, piAddr, &iRows, &iCols, Ar);
            *Ai = NULL;
        }
        return 1;
    }
    else if (iType == sci_mlist)
    {
        sciErr = getListItemNumber(_pvCtx, piAddr, &nItems);
        if (nItems != 3) return 0;
        /*Check if first item is ["hm","dims","entries"] */
        sciErr = getListItemAddress(_pvCtx, piAddr, 1, &piAddrChild);
        sciErr = getVarType(_pvCtx, piAddrChild, &iType);
        if (iType != sci_strings) return 0;
        sciErr = getVarDimension(_pvCtx, piAddrChild, &iRows, &iCols);
        if (iRows*iCols != 3) return 0;
        /* Check if first entry of the first item is "hm" */
        piOffset = piAddrChild + 4;
        if (piOffset[1] - piOffset[0] != 2)  return 0;
        piData = piOffset + iRows * iCols + 1;
        if (piData[0] != 17 || piData[1] != 22) return 0; /* check "hm" */
        /* Get second item dims */
        sciErr = getListItemAddress(_pvCtx, piAddr, 2, &piAddrChild);
        sciErr = getVarType(_pvCtx, piAddrChild, &iType);
        if (iType != sci_ints) return 0;
        sciErr = getMatrixOfInteger32(_pvCtx, piAddrChild, &iRows, &iCols, dims);
        if (sciErr.iErr)  return 0;
        *ndims = iRows * iCols;
        /* Get thirds item entries */
        sciErr = getListItemAddress(_pvCtx, piAddr, 3, &piAddrChild);
        sciErr = getVarType(_pvCtx, piAddrChild, &iType);
        if (iType != sci_matrix) return 0;
        if (isVarComplex(_pvCtx, piAddrChild))
        {
            getComplexMatrixOfDouble(_pvCtx, piAddrChild, &iRows, &iCols, Ar, Ai);
        }
        else
        {
            getMatrixOfDouble(_pvCtx, piAddrChild, &iRows, &iCols, Ar);
            *Ai = NULL;
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

SciErr allocComplexArrayOfDouble(void* _pvCtx, int _iVar, int ndims, int *dims, double **Ar, double **Ai)
{
    SciErr sciErr;
    int *piAddr = NULL;

    if (ndims == 2)
    {
        sciErr = allocComplexMatrixOfDouble( _pvCtx, _iVar, dims[0], dims[1], Ar, Ai);
        if (sciErr.iErr) return sciErr;
    }
    else
    {
        int i = 0;
        int n = 1;
        const char * hmType[] = {"hm", "dims", "entries"};

        for (i = 0; i < ndims; i++)   n *= dims[i];

        sciErr = createMList(_pvCtx, _iVar, 3, &piAddr);
        if (sciErr.iErr) return sciErr;

        sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddr, 1, 1, 3, hmType);
        if (sciErr.iErr) return sciErr;
        sciErr = createMatrixOfInteger32InList(_pvCtx, _iVar, piAddr, 2, 1, ndims, dims);
        if (sciErr.iErr) return sciErr;

        sciErr = allocComplexMatrixOfDoubleInList(_pvCtx, _iVar, piAddr, 3, n, 1, Ar, Ai);
        if (sciErr.iErr) return sciErr;
    }
    return sciErr;
}

SciErr allocArrayOfDouble(void* _pvCtx, int _iVar,  int ndims, int *dims, double **Ar)
{
    SciErr sciErr;
    int *piAddr      = NULL;


    if (ndims == 2)
    {
        sciErr = allocMatrixOfDouble( _pvCtx, _iVar, dims[0], dims[1], Ar);
        if (sciErr.iErr) return sciErr;
    }
    else
    {
        int i = 0;
        int n = 1;
        const char * hmType[] = {"hm", "dims", "entries"};

        for (i = 0; i < ndims; i++) n *= dims[i];

        sciErr = createMList(_pvCtx,  _iVar, 3, &piAddr);
        if (sciErr.iErr) return sciErr;
        sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddr, 1, 1, 3, hmType);
        if (sciErr.iErr) return sciErr;
        sciErr = createMatrixOfInteger32InList(_pvCtx, _iVar, piAddr, 2, 1, ndims, dims);
        if (sciErr.iErr) return sciErr;

        sciErr = allocMatrixOfDoubleInList(_pvCtx, _iVar, piAddr, 3, n, 1, Ar);
        if (sciErr.iErr) return sciErr;
    }
    return sciErr;
}

SciErr getScalarIntArg(void* _pvCtx, int _iVar, char *fname, int *value)
{
    SciErr sciErr;
    int *piAddr = NULL;
    int iType = 0;
    int iPrec = 0;
    double t_d = 0.0;
    char t_c = 0;
    unsigned char t_uc = 0;
    short t_s = 0;
    unsigned short t_us = 0;
    int t_i = 0;
    unsigned int  t_ui = 0;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;

    sciErr = getVarAddressFromPosition(_pvCtx, _iVar, &piAddr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_STRING,  _("%s: Can not read input argument #%d.\n"), fname, _iVar);
        return sciErr;
    }

    //check type
    sciErr = getVarType(_pvCtx, piAddr, &iType);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_INT,  _("%s: Can not read input argument #%d.\n"), fname, _iVar);
        return sciErr;
    }

    if (!isScalar(_pvCtx, piAddr))
    {
        addErrorMessage(&sciErr, API_ERROR_GET_INT, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, _iVar);
        return sciErr;
    }

    if (iType == sci_matrix)
    {
        getScalarDouble(_pvCtx, piAddr, &t_d);
        *value = (int)t_d;
    }
    else if (iType == sci_ints)
    {
        sciErr = getMatrixOfIntegerPrecision(_pvCtx, piAddr, &iPrec);
        if (sciErr.iErr)
        {
            addErrorMessage(&sciErr, API_ERROR_GET_INT, _("%s: Can not read input argument #%d.\n"), fname, _iVar);
            return sciErr;
        }

        switch (iPrec)
        {
            case SCI_INT8 :
            {
                getScalarInteger8(_pvCtx, piAddr, &t_c);
                *value = (int)t_c;
            }
            case SCI_INT16 :
            {
                getScalarInteger16(_pvCtx, piAddr, &t_s);
                *value = (int)t_s;
            }
            case SCI_INT32 :
            {
                getScalarInteger32(_pvCtx, piAddr, &t_i);
                *value = (int)t_i;
            }
            case SCI_UINT8 :
            {
                getScalarUnsignedInteger8(_pvCtx, piAddr, &t_uc);
                *value = (int)t_uc;
            }
            case SCI_UINT16 :
            {
                getScalarUnsignedInteger16(_pvCtx, piAddr, &t_us);
                *value = (int)t_us;
            }
            case SCI_UINT32 :
            {
                getScalarUnsignedInteger32(_pvCtx, piAddr, &t_ui);
                *value = (int)t_ui;
            }
        }
    }
    else
    {
        addErrorMessage(&sciErr, API_ERROR_GET_INT,
                        _("%s: Wrong type for argument #%d: An integer or a floating point number expected.\n"),
                        fname, _iVar);
        return sciErr;
    }
    return sciErr;
}

SciErr getVectorIntArg(void* _pvCtx, int _iVar, char *fname, int *pndims, int **pDim)
{
    SciErr sciErr;
    int *piAddr = NULL;
    int iType = 0;
    int iPrec = 0;
    int mDim = 0;
    int nDim = 0;
    int *Dim = NULL;
    int ndims = 0;

    double* p_d = NULL;
    char* p_c = NULL;
    unsigned char* p_uc = NULL;
    short* p_s = NULL;
    unsigned short* p_us = NULL;
    int* p_i = NULL;
    unsigned int*  p_ui = NULL;
    int i = 0;

    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;

    getVarAddressFromPosition(_pvCtx, _iVar, &piAddr);

    //check type
    getVarType(_pvCtx, piAddr, &iType);

    if (isVarMatrixType(_pvCtx, piAddr) == 0)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_INT, _("%s: Wrong type for input argument #%d.\n"), fname, _iVar);
        return sciErr;
    }

    getVarDimension(_pvCtx, piAddr, &mDim, &nDim);

    ndims = mDim * nDim;
    *pndims = ndims;
    if (ndims <= 0)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_INT,
                        _("%s: Wrong size for input argument #%d.\n"), fname, _iVar);
        return sciErr;
    }
    if ((Dim = (int *)MALLOC(ndims * sizeof(int))) == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_INT,
                        _("%s: Cannot allocate more memory.\n"), fname);
        return sciErr;
    }
    *pDim = Dim;
    if (iType == sci_matrix)
    {
        sciErr = getMatrixOfDouble(_pvCtx, piAddr, &mDim, &nDim, &p_d);
        for (i = 0; i < ndims; i++)  Dim[i] = (int)(p_d[i]);
    }
    else if (iType == sci_ints)
    {
        getMatrixOfIntegerPrecision(_pvCtx, piAddr, &iPrec);
        switch (iPrec)
        {
            case SCI_INT8 :
                getMatrixOfInteger8(_pvCtx, piAddr, &mDim, &nDim, &p_c);
                for (i = 0; i < ndims; i++) Dim[i]  = (int)(p_c[i]);
                break;
            case SCI_INT16 :
                getMatrixOfInteger16(_pvCtx, piAddr, &mDim, &nDim, &p_s);
                for (i = 0; i < ndims; i++) Dim[i]  = (int)(p_s[i]);
                break;
            case SCI_INT32 :
                getMatrixOfInteger32(_pvCtx, piAddr, &mDim, &nDim, &p_i);
                for (i = 0; i < ndims; i++)  Dim[i]  = (int)(p_i[i]);
                break;
            case SCI_UINT8 :
                getMatrixOfUnsignedInteger8(_pvCtx, piAddr, &mDim, &nDim, &p_uc);
                for (i = 0; i < ndims; i++) Dim[i]  = (int)(p_uc[i]);
                break;
            case SCI_UINT16 :
                getMatrixOfUnsignedInteger16(_pvCtx, piAddr, &mDim, &nDim, &p_us);
                for (i = 0; i < ndims; i++) Dim[i]  = (int) p_us[i];
                break;
            case SCI_UINT32 :
                getMatrixOfUnsignedInteger32(_pvCtx, piAddr, &mDim, &nDim, &p_ui);
                for (i = 0; i < ndims; i++) Dim[i]  = (int)(p_ui[i]);
                break;
        }
    }
    else
    {
        FREE(Dim);
        Dim = NULL;
        addErrorMessage(&sciErr, API_ERROR_GET_INT,
                        _("%s: Wrong type for argument #%d: An array of floating point or integer numbers expected.\n"), fname, _iVar);
        return sciErr;
    }
    return sciErr;
}

BOOL isHyperMatrixMlist(void* _pvCtx, int *piAddressVar)
{
    char **fields = NULL;
    SciErr sciErr;
    int iType = 0;
    int m = 0, n = 0;

    if (piAddressVar == NULL)
    {
        return FALSE;
    }

    sciErr = getVarType(_pvCtx, piAddressVar, &iType);
    if (sciErr.iErr)
    {
        return FALSE;
    }

    if (iType == sci_mlist)
    {
        int* piAddrChild  = NULL;
        int iItem   = 0;

        sciErr = getListItemNumber(pvApiCtx, piAddressVar, &iItem);
        if (sciErr.iErr)
        {
            return FALSE;
        }

        sciErr = getListItemAddress(pvApiCtx, piAddressVar, 1, &piAddrChild);
        if (sciErr.iErr)
        {
            return FALSE;
        }

        if (!isStringType(_pvCtx, piAddrChild))
        {
            return FALSE;
        }

        if (getAllocatedMatrixOfString(_pvCtx, piAddrChild, &m, &n , &fields) == 0)
        {
            if (strcmp(fields[0], "hm") != 0)
            {
                freeAllocatedMatrixOfString(m, n, fields);
                fields = NULL;
                return FALSE;
            }
            freeAllocatedMatrixOfString(m, n, fields);
            fields = NULL;
        }
        else
        {
            return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}
