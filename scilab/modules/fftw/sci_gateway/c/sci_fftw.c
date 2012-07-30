/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006/2007 - INRIA - Alan LAYEC
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2012 - DIGITEO - Allan CORNET
 * Copyright (C) 2012 - INRIA - Serge STEER
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "stack-c.h"
#include "fftw_utilities.h"
#include "callfftw.h"
#include "MALLOC.h"
#include "gw_fftw.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
enum Scaling
{
    Divide = -1,
    None = 0,
    Multiply = 1,
};
/*--------------------------------------------------------------------------*/
extern void C2F(dscal)(int *n, double *da, double *dx, int *incx); /* blas routine */
/*--------------------------------------------------------------------------*/
static int getArrayOfDouble(void* pvApiCtx, int *piAddr, int *ndims, int **dims, double **Ar, double **Ai);
static SciErr allocArrayOfDouble(void* _pvCtx, int _iVar, int ndims, int *dims, double **Ar);
static SciErr allocComplexArrayOfDouble(void* _pvCtx, int _iVar, int ndims, int *dims, double **Ar, double **Ai);
static SciErr getScalarIntArg(void* _pvCtx, int _iVar, char *fname, int *value);
static SciErr getVectorIntArg(void* _pvCtx, int _iVar, char *fname, int *pndims, int **pDim);
static BOOL isHyperMatrixMlist(void* _pvCtx, int *piAddressVar);
static int sci_fft_gen(void* pvApiCtx, char *fname, int ndimsA, int *dimsA, double *Ar,  double *Ai, int isn, int iopt, guru_dim_struct gdim);
static int sci_fft_2args(void* pvApiCtx, char *fname, int ndimsA, int *dimsA, double *Ar,  double *Ai, int isn, int iopt);
static int sci_fft_3args(void* pvApiCtx, char *fname, int ndimsA, int *dimsA, double *Ar,  double *Ai, int isn, int iopt);
static int sci_fft_4args(void* pvApiCtx, char *fname, int ndimsA, int *dimsA, double *Ar,  double *Ai, int isn, int iopt);
/*--------------------------------------------------------------------------*/

int WITHMKL = 0;
/* fftw function.
 *
 * Scilab Calling sequence :
 *   fftw(A [,option])
 *   fftw(A,sign [,option])
 *   fftw(A,sel,sign [,option])
 *   fftw(A,sign,dim,incr [,option])
 *
 * Input : A : a scilab double complex or real vector, matrix or hypermatrix
 *
 *         sign : a scilab double or integer scalar (-1 or 1): the sign
 *                  in the exponential component
 *
 *         sel : a scilab double or integer vector, the selection of dimensions

 *         dim : a scilab double or integer vector: the dimensions
 *                  of the Fast Fourier Transform to perform
 *
 *         incr : a scilab double or integer vector: the increments
 *                  of the Fast Fourier Transform to perform
 *
 * Output : a scilab double complex or real array with same shape as A that
 *          gives the result of the transform.
 *
 */
int sci_fftw(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int *piAddr = NULL;
    char *option = NULL;
    int iopt = 0; /* automatic r2c or c2r transform use decision */
    int rhs = Rhs;
    int iTypeOne = 0;

    int ndimsA = 0;
    int *dimsA = NULL;
    double *Ar = NULL, *Ai = NULL;

    int isn = FFTW_FORWARD;
    WITHMKL = withMKL();
    /****************************************
     * Basic constraints on rhs arguments  *
     ****************************************/

    /* check min/max lhs/rhs arguments of scilab function */
    CheckRhs(1, 5);
    CheckLhs(1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddr, &iTypeOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if ((iTypeOne == sci_list) ||
            (iTypeOne == sci_tlist))
    {
        OverLoad(1);
        return 0;
    }

    if (iTypeOne == sci_mlist)
    {
        /* We allow overload for not hypermatrix type */
        if (!isHyperMatrixMlist(pvApiCtx, piAddr))
        {
            OverLoad(1);
            return 0;
        }
    }

    /* checking if last argument is a potential option argument (character string) */
    sciErr = getVarAddressFromPosition(pvApiCtx, Rhs, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, Rhs);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddr))   /*  fftw(...,option); */
    {
        if (isScalar(pvApiCtx, piAddr))
        {
            if (getAllocatedSingleString(pvApiCtx, piAddr, &option) == 0)
            {
                if (strcmp("symmetric", option) == 0)  iopt = 1; /*user assumes symmetry */
                else if (strcmp("nonsymmetric", option) == 0) iopt = 2; /*user claims full transform */
                else
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"),
                             fname, Rhs, "\"symmetric\"", "\"nonsymmetric\"");
                    freeAllocatedSingleString(option);
                    option = NULL;
                    return 0;
                }
                freeAllocatedSingleString(option);
                option = NULL;
                rhs = Rhs - 1;
            }
            else
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"),
                         fname, Rhs, "\"full\", \"same\"", "\"valid\"");
                return 0;
            }
        }
    }


    /********************  Checking if isn is given  ************************************************/
    if (rhs == 1)  /*only one rhs argument: forward fft*/
    {
        isn = FFTW_FORWARD; /* default value */
    }
    else   /*get isn out of second argument*/
    {
        sciErr = getScalarIntArg(pvApiCtx, 2, fname, &isn);
        if (sciErr.iErr)
        {
            Scierror(sciErr.iErr, getErrorMessage(sciErr));
            return 0;
        }
        /* check value of second rhs argument */
        if ((isn !=  FFTW_FORWARD) && (isn !=  FFTW_BACKWARD))
        {
            Scierror(53, _("%s: Wrong value for input argument #%d: %d or %d expected.\n"),
                     fname, 2, FFTW_FORWARD, FFTW_BACKWARD);
            return(0);
        }
    }

    /********************  getting the array A      ************************************************/
    getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (!getArrayOfDouble(pvApiCtx, piAddr, &ndimsA, &dimsA, &Ar, &Ai))
    {
        Scierror(999, _("%s: Wrong type for argument #%d: Array of floating point numbers expected.\n"),
                 fname, 1);
        return 0;
    }


    /********************  Select proper method     ************************************************/
    if (rhs < 3)
    {
        /* fftw(A ,sign [,option])*/
        sci_fft_2args(pvApiCtx, fname, ndimsA, dimsA, Ar, Ai, isn, iopt);
    }
    else if (rhs == 3)
    {
        /* fftw(A ,sign ,sel [,option])*/
        sci_fft_3args(pvApiCtx, fname, ndimsA, dimsA, Ar, Ai, isn, iopt);
    }
    else if (rhs == 4)
    {
        /* fftw(A ,sign ,dim,incr [option])*/
        sci_fft_4args(pvApiCtx, fname, ndimsA, dimsA, Ar, Ai, isn, iopt);
    }

    return 0;
}

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

int sci_fft_2args(void* pvApiCtx, char *fname, int ndimsA, int *dimsA, double *Ar,  double *Ai, int isn, int iopt)
{
    /*FFTW specific library variable */
    guru_dim_struct gdim = {0, NULL, 0, NULL};

    /* local variable */
    int ndims = 0; /* number of non singleton dimensions */
    int first_nonsingleton = -1;
    int i = 0, j = 0;
    int prd = 1;

    /* ignore singleton dimensions */
    first_nonsingleton = -1;
    ndims = 0;
    for (i = 0; i < ndimsA; i++)
    {
        if (dimsA[i] > 1)
        {
            ndims++;
            if (first_nonsingleton < 0) first_nonsingleton = i;
        }
    }

    /* void or scalar input gives void output or scalar*/
    if (ndims == 0 )
    {
        LhsVar(1) =  1;
        PutLhsVar();
        return(0);
    }

    gdim.rank = ndims;
    if ((gdim.dims = (fftw_iodim *)MALLOC(sizeof(fftw_iodim) * gdim.rank)) == NULL)
    {
        Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
        goto ERR;
    }

    j = 0;
    prd = 1;
    for (i = (first_nonsingleton); i < ndimsA; i++)
    {
        if (dimsA[i] > 1)
        {
            gdim.dims[j].n = dimsA[i];
            gdim.dims[j].is = prd;
            gdim.dims[j].os = prd;
            prd *= dimsA[i];
            j++;
        }
    }
    gdim.howmany_rank = 0;
    gdim.howmany_dims = NULL;


    if (!sci_fft_gen(pvApiCtx, fname, ndimsA, dimsA,  Ar,  Ai, isn, iopt, gdim))  goto ERR;


    /***********************************
     * Return results in lhs argument *
     ***********************************/

    PutLhsVar();
ERR:
    FREE(gdim.dims);
    FREE(gdim.howmany_dims);
    return(0);
}


int  sci_fft_3args(void* pvApiCtx, char *fname, int ndimsA, int *dimsA, double *Ar,  double *Ai, int isn, int iopt)
{
    /* API variables */
    SciErr sciErr;
    int *piAddr = NULL;

    int *Sel = NULL;
    int rank = 0;

    /*FFTW specific library variable */
    guru_dim_struct gdim = {0, NULL, 0, NULL};
    /* local variable */
    int ndims = 0;
    int first_nonsingleton = -1;
    int ih = 0;
    int pd = 1; /* used to store prod(Dims(1:sel(k-1)))*/
    int pds = 1; /* used to store prod(Dims(sel(k-1):sel(k)))*/
    int i = 0, j = 0;

    /* ignore singleton dimensions */
    first_nonsingleton = -1;
    ndims = 0;
    for (i = 0; i < ndimsA; i++)
    {
        if (dimsA[i] > 1)
        {
            ndims++;
            if (first_nonsingleton < 0) first_nonsingleton = i;
        }
    }

    /* void or scalar input gives void output or scalar*/
    if (ndims == 0 )
    {
        LhsVar(1) =  1;
        PutLhsVar();
        return(0);
    }


    /******************** get and check third argument (sel) ****************************************/
    getVarAddressFromPosition(pvApiCtx, 3, &piAddr);
    if (isVarMatrixType(pvApiCtx, piAddr) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d.\n"), fname, 3);
        goto ERR;
    }
    sciErr = getVectorIntArg(pvApiCtx, 3, fname, &rank, &Sel);
    if (sciErr.iErr)
    {
        Scierror(sciErr.iErr, getErrorMessage(sciErr));
        goto ERR;
    }
    /* size of Sel must be less than ndimsA */
    if (rank <= 0 || rank >= ndimsA)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Must be between %d and %d.\n"), fname, 3, 1, ndimsA - 1);
        goto ERR;
    }
    /* check values of Sel[i] */
    for (i = 0; i < rank; i++)
    {
        if (Sel[i] <= 0)
        {
            Scierror(999, _("%s: Wrong values for input argument #%d: Positive integers expected.\n"), fname, 3);
            goto ERR;
        }
        if (Sel[i] > ndimsA)
        {
            Scierror(999, _("%s: Wrong values for input argument #%d: Elements must be less than %d.\n"), fname, 3, ndimsA);
            goto ERR;
        }
        if (i > 0 && Sel[i] <= Sel[i - 1])
        {
            Scierror(999, _("%s: Wrong values for input argument #%d: Elements must be in increasing order.\n"), fname, 3);
            goto ERR;
        }
    }

    /* Create  gdim struct */
    gdim.rank = rank;
    if ((gdim.dims = (fftw_iodim *)MALLOC(sizeof(fftw_iodim) * gdim.rank)) == NULL)
    {
        Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
        goto ERR;
    }

    pd = 1; /* used to store prod(Dims(1:sel(k-1)))*/
    pds = 1; /* used to store prod(Dims(sel(k-1):sel(k)))*/
    j = 0;
    for (i = 0; i < ndimsA; i++)
    {
        if (j >= gdim.rank) break;
        if (Sel[j] == i + 1)
        {
            gdim.dims[j].n = dimsA[i];
            gdim.dims[j].is = pd;
            gdim.dims[j].os = pd;
            j++;
        }
        pd *= dimsA[i];
    }
    /* Compute howmany_rank based on jumps in the Sel sequence */
    gdim.howmany_rank = 0;
    if ((Sel[0] != 1) && (Sel[0] != ndimsA)) gdim.howmany_rank++;

    for (i = 1; i <= rank - 1; i++)
        if (Sel[i] != Sel[i - 1] + 1) gdim.howmany_rank++;

    if ((Sel[rank - 1] != ndimsA) || (rank == 1)) gdim.howmany_rank++;

    /* Fill the howmany_dims struct */
    if (gdim.howmany_rank > 0)
    {
        /* it must be the case */
        if ((gdim.howmany_dims = (fftw_iodim *)MALLOC(gdim.howmany_rank * sizeof(fftw_iodim))) == NULL)
        {
            Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
            goto ERR;
        }
        pd = 1;
        for (j = 1; j <= (Sel[0] - 1); j++) pd *= dimsA[j - 1]; /*prod(Dims(1:(sel(1)-1)))*/
        ih = 0;
        if ((Sel[0] != 1) && (Sel[0] != ndimsA))
        {
            /* First seleted dimension */
            gdim.howmany_dims[ih].is = 1;
            gdim.howmany_dims[ih].os = 1;
            gdim.howmany_dims[ih].n = pd;
            ih++;
        }
        pd *= dimsA[Sel[0] - 1]; /*prod(Dims(1:sel(1)))*/
        for (i = 2; i <= rank; i++)
        {
            /* intermediate selected dimensions */
            if (Sel[i - 1] != Sel[i - 2] + 1)
            {
                pds = 1;
                for (j = (Sel[i - 2] + 1); j <= (Sel[i - 1] - 1); j++) pds *= dimsA[j - 1]; /*prod(Dims(sel(i-1)+1:(sel(i)-1)))*/
                gdim.howmany_dims[ih].is = pd;
                gdim.howmany_dims[ih].os = pd;
                gdim.howmany_dims[ih].n = pds;
                ih++;
            }
            pd *= pds * dimsA[Sel[i - 1] - 1]; /*prod(Dims(1:sel(i)))*/
        }

        if (Sel[rank - 1] != ndimsA)
        {
            /* last selected dimension*/
            pds = 1;
            for (j = (Sel[rank - 1] + 1); j <= ndimsA; j++) pds *= dimsA[j - 1]; /*prod(Dims(sel(i-1)+1:(sel(i)-1)))*/
            gdim.howmany_dims[ih].is = pd;
            gdim.howmany_dims[ih].os = pd;
            gdim.howmany_dims[ih].n = pds;
            ih++;
        }
        else if (rank == 1)
        {
            /* the only selected dimension is the last one */
            gdim.howmany_dims[ih].is = 1;
            gdim.howmany_dims[ih].os = 1;
            gdim.howmany_dims[ih].n = pd / dimsA[Sel[0] - 1];
            ih++;
        }
    }

    if (!sci_fft_gen(pvApiCtx, fname, ndimsA, dimsA, Ar,  Ai, isn, iopt, gdim))  goto ERR;
    /***********************************
     * Return results in lhs argument *
     ***********************************/

    PutLhsVar();
ERR:
    FREE(gdim.dims);
    FREE(gdim.howmany_dims);
    return(0);
}

int sci_fft_4args(void* pvApiCtx, char *fname, int ndimsA, int *dimsA, double *Ar,  double *Ai, int isn, int iopt)
{
    /* API variables */
    SciErr sciErr;
    int *piAddr = NULL;

    /* Input  array variables */
    int *Dim1 = NULL;
    int ndims = 0;
    int *Incr = NULL;
    int nincr = 0;

    /*FFTW specific library variable */
    guru_dim_struct gdim = {0, NULL, 0, NULL};
    /* input/output address for transform variables */

    /* local variable */
    int *Dim = NULL, *Sel = NULL;
    int pd = 1;
    int pds = 1;
    int nd = 0;
    int rank = 0;
    int i = 0, j = 0, k = 0, lA = 1;

    for (i = 0; i < ndimsA; i++)
    {
        lA *= dimsA[i];
    }

    /* void or scalar input gives void output or scalar*/
    if (lA <= 1 )
    {
        LhsVar(1) =  1;
        PutLhsVar();
        return(0);
    }

    /******************** get and check third argument (dim) ****************************************/
    getVarAddressFromPosition(pvApiCtx, 3, &piAddr);
    if (isVarMatrixType(pvApiCtx, piAddr) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d.\n"), fname, 3);
        goto ERR;
    }
    sciErr = getVectorIntArg(pvApiCtx, 3, fname, &ndims, &Dim1);
    if (sciErr.iErr)
    {
        Scierror(sciErr.iErr, getErrorMessage(sciErr));
        goto ERR;
    }
    /* check values of Dim1[i} */
    pd = 1;
    for (i = 0; i < ndims; i++)
    {
        if (Dim1[i] <= 1)
        {
            Scierror(999, _("%s: Wrong values for input argument #%d: Elements must be greater than %d.\n"), fname, 3, 1);
            goto ERR;
        }
        pd *= Dim1[i];
    }
    if ( pd > lA)
    {
        Scierror(999, _("%s: Wrong values for input argument #%d: Must be less than %d.\n"), fname, 3, lA);
        goto ERR;
    }
    if (lA % pd)
    {
        Scierror(999, _("%s: Wrong values for input argument #%d: Must be a divisor of %d.\n"), fname, 3, lA);
        goto ERR;
    }
    /******************** get and check fourth argument (incr) ****************************************/
    sciErr = getVectorIntArg(pvApiCtx, 4, fname, &nincr, &Incr);
    if (sciErr.iErr)
    {
        Scierror(sciErr.iErr, getErrorMessage(sciErr));
        goto ERR;
    }
    if (nincr != ndims)
    {
        Scierror(999, _("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"), fname, 3, 4);
        goto ERR;
    }

    /* check values of Incr[i] */
    if (Incr[0] <= 0)
    {
        Scierror(999, _("%s: Wrong values for input argument #%d: Positive integers expected.\n"), fname, 4);
        goto ERR;
    }
    for (i = 0; i < ndims; i++)
    {
        if (lA % Incr[i])
        {
            Scierror(999, _("%s: Wrong values for input argument #%d: Elements must be divisors of %d.\n"), fname, 3, lA);
            goto ERR;
        }
        if (i > 0 && (Incr[i] <= Incr[i - 1]))
        {
            Scierror(999, _("%s: Wrong values for input argument #%d: Elements must be in increasing ""order.\n"), fname, 4);
            goto ERR;
        }
    }
    if ((Dim = (int *)MALLOC((2 * ndims + 1) * sizeof(int))) == NULL)
    {
        Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
        goto ERR;
    }
    if ((Sel = (int *)MALLOC((ndims) * sizeof(int))) == NULL)
    {
        Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
        goto ERR;
    }


    /*Transform  Dim1 and Incr into Dim and Sel and check validity*/

    nd = 0;
    pd = 1;
    if (Incr[0] != 1)
    {
        Dim[nd++] = Incr[0];
        pd *= Incr[0];
    }
    Dim[nd++] = Dim1[0];
    pd *= Dim1[0];
    Sel[0] = nd;

    for (k = 1; k < ndims; k++)
    {
        if (Incr[k] % pd != 0)
        {
            Scierror(999, _("%s: Incompatible input arguments #%d and #%d.\n"), fname, 3, 4);
            goto ERR;
        }
        if (Incr[k] != pd)
        {
            Dim[nd++] = (int)(Incr[k] / pd);
            pd = Incr[k];
        }
        Dim[nd++] = Dim1[k];
        pd *= Dim1[k];
        Sel[k] = nd;
    }
    if (pd < lA)
    {
        if (lA % pd != 0)
        {
            Scierror(999, _("%s: Incompatible input arguments #%d and #%d.\n"), fname, 3, 4);
            goto ERR;
        }
        Dim[nd++] = (int)(lA / pd);
    }

    rank = ndims;
    ndims = nd;

    /* now one  same algorithm than sci_fft_3args applies */
    /* Create  gdim struct */
    gdim.rank = rank;
    if ((gdim.dims = (fftw_iodim *)MALLOC(sizeof(fftw_iodim) * gdim.rank)) == NULL)
    {
        Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
        goto ERR;
    }

    pd = 1; /* used to store prod(Dims(1:sel(k-1)))*/
    pds = 1; /* used to store prod(Dims(sel(k-1):sel(k)))*/
    j = 0;
    for (i = 0; i < ndims; i++)
    {
        if (j >= gdim.rank) break;
        if (Sel[j] == i + 1)
        {
            gdim.dims[j].n = Dim[i];
            gdim.dims[j].is = pd;
            gdim.dims[j].os = pd;
            j++;
        }
        pd *= Dim[i];
    }
    /* Compute howmany_rank based on jumps in the Sel sequence */
    gdim.howmany_rank = 0;
    if ((Sel[0] != 1) && (Sel[0] != ndims)) gdim.howmany_rank++;

    for (i = 1; i <= rank - 1; i++)
    {
        if (Sel[i] != Sel[i - 1] + 1) gdim.howmany_rank++;
    }
    if ((Sel[rank - 1] != ndims) || (rank == 1)) gdim.howmany_rank++;
    /* Fill the howmany_dims struct */
    if (gdim.howmany_rank > 0)
    {
        /* it must be the case */
        int ih = 0;

        if ((gdim.howmany_dims = (fftw_iodim *)MALLOC(gdim.howmany_rank * sizeof(fftw_iodim))) == NULL)
        {
            Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
            goto ERR;
        }
        pd = 1;
        for (j = 1; j <= (Sel[0] - 1); j++) pd *= Dim[j - 1]; /*prod(Dims(1:(sel(1)-1)))*/
        ih = 0;
        if ((Sel[0] != 1) && (Sel[0] != ndims))
        {
            /* First seleted dimension */
            gdim.howmany_dims[ih].is = 1;
            gdim.howmany_dims[ih].os = 1;
            gdim.howmany_dims[ih].n = pd;
            ih++;
        }
        pd *= Dim[Sel[0] - 1]; /*prod(Dims(1:sel(1)))*/
        for (i = 2; i <= rank; i++)
        {
            /* intermediate selected dimensions */
            if (Sel[i - 1] != Sel[i - 2] + 1)
            {
                pds = 1;
                for (j = (Sel[i - 2] + 1); j <= (Sel[i - 1] - 1); j++) pds *= Dim[j - 1]; /*prod(Dims(sel(i-1)+1:(sel(i)-1)))*/
                gdim.howmany_dims[ih].is = pd;
                gdim.howmany_dims[ih].os = pd;
                gdim.howmany_dims[ih].n = pds;
                ih++;
            }
            pd *= pds * Dim[Sel[i - 1] - 1]; /*prod(Dims(1:sel(i)))*/
        }

        if (Sel[rank - 1] != ndims)
        {
            /* last selected dimension*/
            pds = 1;
            for (j = (Sel[rank - 1] + 1); j <= ndims; j++) pds *= Dim[j - 1]; /*prod(Dims(sel(i-1)+1:(sel(i)-1)))*/
            gdim.howmany_dims[ih].is = pd;
            gdim.howmany_dims[ih].os = pd;
            gdim.howmany_dims[ih].n = pds;
            ih++;
        }
        else if (rank == 1) /* the only selected dimension is the last one */
        {
            gdim.howmany_dims[ih].is = 1;
            gdim.howmany_dims[ih].os = 1;
            gdim.howmany_dims[ih].n = pd / Dim[Sel[0] - 1];
            ih++;
        }
    }
    if (!sci_fft_gen(pvApiCtx, fname, ndimsA, dimsA, Ar,  Ai, isn, iopt, gdim))  goto ERR;

    /***********************************
     * Return results in lhs argument *
     ***********************************/

    PutLhsVar();
ERR:
    FREE(Dim1);
    FREE(Incr);
    FREE(Dim);
    FREE(Sel);
    FREE(gdim.dims);
    FREE(gdim.howmany_dims);
    return(0);
}
/*--------------------------------------------------------------------------*/
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

        sciErr = getListItemNumber(_pvCtx, piAddressVar, &iItem);
        if (sciErr.iErr)
        {
            return FALSE;
        }

        sciErr = getListItemAddress(_pvCtx, piAddressVar, 1, &piAddrChild);
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
/*--------------------------------------------------------------------------*/
int sci_fft_gen(void* pvApiCtx, char *fname, int ndimsA, int *dimsA, double *Ar,  double *Ai, int isn, int iopt, guru_dim_struct gdim)
{
    /* API variables */
    SciErr sciErr;

    /* Input  array variables */
    int  isrealA = (Ai == NULL), issymA = 1, lA = 1;
    /*for MKL*/
    int isrealA_save = isrealA ;

    /*FFTW specific library variable */
    enum Scaling scale = None;
    enum Plan_Type type;
    fftw_plan p;

    /* input/output address for transform variables */
    double *ri = NULL, *ii = NULL, *ro = NULL, *io = NULL;


    /* local variable */
    int one = 1;
    int i = 0;


    for (i = 0; i < ndimsA; i++)
    {
        lA *= dimsA[i];
    }


    if (iopt == 0)
    {
        /* automatically selected algorithm*/
        issymA =  check_array_symmetry(Ar, Ai, gdim);
        if (issymA < 0 )
        {
            Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
            goto ERR;
        }
    }

    else if (iopt == 1)
    {
        issymA = 1; /* user forces symmetry */
    }
    else
    {
        issymA = 0;
    }
    LhsVar(1) = 1; /* assume inplace transform*/

    if (WITHMKL)
    {
        double dzero = 0.0;
        if (isrealA)
        {
            /*MKL does not implement the r2c nor r2r guru split methods, make A complex */
            if (issymA)
            {
                /* result will be real, the imaginary part of A can be allocated alone */
                sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, 1, lA, &Ai);
                if (sciErr.iErr)
                {
                    Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
                    goto ERR;
                }
                C2F(dset)(&lA, &dzero, Ai, &one);
            }
            else
            {
                /* result will be complex, realloc A for inplace computation */
                sciErr = allocComplexArrayOfDouble(pvApiCtx, Rhs + 1, ndimsA, dimsA, &ri, &Ai);
                if (sciErr.iErr)
                {
                    Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
                    goto ERR;
                }
                C2F(dcopy)(&lA, Ar, &one, ri, &one);
                Ar = ri;
                C2F(dset)(&lA, &dzero, Ai, &one);
                LhsVar(1) = Rhs + 1;
                isrealA = 0;
            }
        }
    }

    if (!isrealA && issymA) /* A is complex but result is real */
    {
        /* result will be complex, realloc real part of A for real part inplace computation */
        sciErr = allocArrayOfDouble(pvApiCtx, Rhs + 1, ndimsA, dimsA, &ri);
        if (sciErr.iErr)
        {
            Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
            goto ERR;
        }
        C2F(dcopy)(&lA, Ar, &one, ri, &one);
        Ar = ri;
        LhsVar(1) = Rhs + 1;
    }

    /* Set pointers on real and imaginary part of the input */
    ri = Ar;
    ii = Ai;

    scale = None; /*no scaling needed */
    if (isn == FFTW_BACKWARD) scale = Divide;
    if (isrealA)
    {
        /*A is real */
        if (issymA)
        {
            /*r2r =  isrealA &&  issymA*/
            /* there is no general plan able to compute r2r transform so it is tranformed into
               a R2c plan. The computed imaginary part will be zero*/
            sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, 1, lA,  &io);
            if (sciErr.iErr)
            {
                Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
                goto ERR;
            }
            type = R2C_PLAN;
            ro = Ar;
        }
        else
        {
            /*r2c =  isrealA && ~issymA;*/
            /* transform cannot be done in place */
            sciErr = allocComplexArrayOfDouble(pvApiCtx, Rhs + 1, ndimsA, dimsA, &ro, &io);
            if (sciErr.iErr)
            {
                Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
                goto ERR;
            }
            LhsVar(1) = Rhs + 1;
            type = R2C_PLAN; /* fftw_plan_guru_split_dft_r2c plans for an FFTW_FORWARD transform*/
            if (isn == FFTW_BACKWARD)
            {
                /*transform problem into a FORWARD fft*/
                /*ifft(A)=conj(fft(A/N)) cas vect*/
                /* pre traitement A must be  divided by N cas vect*/
                /* post treatment result must conjugated */
            }
        }
    }
    else
    {
        /* A is complex */
        if (!WITHMKL && issymA) /*result is real*/
        {
            /*c2r =  ~isrealA &&  issymA*/
            ro = ri;
            io = NULL;

            type = C2R_PLAN; /*fftw_plan_guru_split_dft_c2r plans for an FFTW_BACKWARD transform*/
            if (isn == FFTW_FORWARD)
            {
                /*transform problem into a BACKWARD fft : fft(A)=ifft(conj(A))*/
                double minusone = -1.0;
                C2F(dscal)(&lA, &minusone, ii, &one);
            }
        }
        else
        {
            /*c2c =  ~isrealA && ~issymA;*/
            /* use inplace transform*/
            type = C2C_PLAN; /*  fftw_plan_guru_split_dft plans for an FFTW_FORWARD transform*/
            if (isn == FFTW_BACKWARD)
            {
                /*transform problem into a FORWARD fft*/
                /* ifft(A) = %i*conj(fft(%i*conj(A)/N) */
                /* reverse input */
                ri = Ai;
                ii = Ar;
                /* reverse output */
                ro = Ai;
                io = Ar;
            }
            else
            {
                ro = ri;
                io = ii;
            }
        }
    }
    /* Set Plan */
    p = GetFFTWPlan(type, &gdim, ri, ii, ro, io, getCurrentFftwFlags(), isn);
    if (p == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        goto ERR;
    }
    /* pre-treatment */
    if (scale != None)
    {
        double ak = 1.0;
        for (i = 0; i < gdim.rank; i++) ak = ak * ((double)(gdim.dims[i].n));
        if (scale == Divide) ak = 1.0 / ak;
        C2F(dscal)(&lA, &ak, ri, &one);
        if (isrealA == 0) C2F(dscal)(&lA, &ak, ii, &one);
    }
    /* execute FFTW plan */
    ExecuteFFTWPlan(type, p, ri, ii, ro, io);
    /* Post treatment */
    switch (type)
    {
        case R2R_PLAN:
            if (complete_array(ro, NULL, gdim) == -1)
            {
                Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
                goto ERR;
            }
            break;
        case C2R_PLAN:
            break;
        case R2C_PLAN:
            if (issymA)
            {
                /*R2C has been used to solve an r2r problem*/
                if (complete_array(ro, NULL, gdim) == -1)
                {
                    Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
                    goto ERR;
                }
            }
            else
            {
                if (complete_array(ro, io, gdim) == -1)
                {
                    Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
                    goto ERR;
                }
                if (isn == FFTW_BACKWARD)
                {
                    /*conjugate result */
                    double ak = -1.0;
                    C2F(dscal)(&lA, &ak, io, &one);
                }
            }
            break;
        case C2C_PLAN:
            if (WITHMKL && isrealA_save)
            {
                if (isn == FFTW_FORWARD)
                {
                    if (complete_array(ro, io, gdim) == -1)
                    {
                        Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
                        goto ERR;
                    }
                }
                else
                {
                    if (complete_array(io, ro, gdim) == -1)
                    {
                        Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
                        goto ERR;
                    }
                }
            }
            break;
    }

    return(1);
ERR:
    return(0);
}
