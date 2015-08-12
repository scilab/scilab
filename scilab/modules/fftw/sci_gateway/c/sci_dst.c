/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 - INRIA - Serge STEER
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "stack-c.h"
#include "fftw_utilities.h"
#include "sci_data_utilities.h"
#include "callfftw.h"
#include "MALLOC.h"
#include "gw_fftw.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "BOOL.h"
#include "sci_data_utilities.h"


/*--------------------------------------------------------------------------*/
extern void C2F(dscal)(int *n, double *da, double *dx, int *incx); /* blas routine */
extern void C2F(dset)(int *n, double *da, double *dx, int *incx); /* blas routine */
/*--------------------------------------------------------------------------*/
static int sci_dst_gen(void* _pvCtx, char *fname, int ndimsA, int *dimsA, double *Ar,  double *Ai, int isn, guru_dim_struct gdim, int iopt);
static int sci_dst_2args(void* _pvCtx, char *fname, int ndimsA, int *dimsA, double *Ar,  double *Ai, int isn, int iopt);
static int sci_dst_3args(void* _pvCtx, char *fname, int ndimsA, int *dimsA, double *Ar,  double *Ai, int isn, int iopt);
static int sci_dst_4args(void* _pvCtx, char *fname, int ndimsA, int *dimsA, double *Ar,  double *Ai, int isn, int iopt);
/*--------------------------------------------------------------------------*/

extern int WITHMKL;
/* fftw function.
*
* Scilab Calling sequence :
*   dst(A )
*   dst(A,sign )
*   dst(A,sel,sign )
*   dst(A,sign,dim,incr )
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
int sci_dst(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddr = NULL;
    int rhs = Rhs;
    int iTypeOne = 0;
    int iopt = 0;

    int ndimsA = 0;
    int *dimsA = NULL;
    double *Ar = NULL, *Ai = NULL;
    char *option = NULL;
    int isn = FFTW_FORWARD;
    WITHMKL = withMKL();
    /****************************************
    * Basic constraints on rhs arguments  *
    ****************************************/

    /* check min/max lhs/rhs arguments of scilab function */
    CheckRhs(1, 4);
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

    if ((iTypeOne == sci_list) || (iTypeOne == sci_tlist))
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

    sciErr = getVarAddressFromPosition(pvApiCtx, Rhs, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, Rhs);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddr))   /*  dst(...,option); */
    {
        if (isScalar(pvApiCtx, piAddr))
        {
            if (getAllocatedSingleString(pvApiCtx, piAddr, &option) == 0)
            {
                rhs = Rhs - 1;
            }
            else
            {
                Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
                return 0;
            }
        }
    }

    /********************  Checking if isn is given  ************************************************/
    if (rhs == 1)  /*only one rhs argument: forward dst*/
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
            Scierror(53, _("%s: Wrong value for input argument #%d: %d or %d expected.\n"), fname, 2, FFTW_FORWARD, FFTW_BACKWARD);
            return 0;
        }
    }

    if (option != NULL)
    {
        if (isn == FFTW_FORWARD)
        {
            if (strcmp("dst1", option) == 0)
            {
                iopt = 1;
            }
            else if (strcmp("dst2", option) == 0)
            {
                iopt = 2;
            }
            else if (strcmp("dst", option) == 0)
            {
                iopt = 0;
            }
            else if (strcmp("dst4", option) == 0)
            {
                iopt = 4;
            }
            else
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), fname, Rhs,
                         "\"dst\", \"dst1\",\"dst2\",\"dst4\"");
                freeAllocatedSingleString(option);
                option = NULL;
                return 0;
            }
        }
        else
        {
            if (strcmp("dst1", option) == 0)
            {
                iopt = 1;
            }
            else if (strcmp("dst3", option) == 0)
            {
                iopt = 3;
            }
            else if (strcmp("idst", option) == 0)
            {
                iopt = 0;
            }
            else if (strcmp("dst4", option) == 0)
            {
                iopt = 4;
            }
            else
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), fname, Rhs,
                         "\"idst\", \"dst1\",\"dst3\",\"dst4\"");
                freeAllocatedSingleString(option);
                option = NULL;
                return 0;
            }

        }
        freeAllocatedSingleString(option);
        option = NULL;

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
        /* dst(A ,sign)*/
        sci_dst_2args(pvApiCtx, fname, ndimsA, dimsA, Ar, Ai, isn, iopt);
    }
    else if (rhs == 3)
    {
        /* dst(A ,sign ,sel)*/
        sci_dst_3args(pvApiCtx, fname, ndimsA, dimsA, Ar, Ai, isn, iopt);
    }
    else if (rhs == 4)
    {
        /* dst(A ,sign ,dim,incr)*/
        sci_dst_4args(pvApiCtx, fname, ndimsA, dimsA, Ar, Ai, isn, iopt);
    }

    return 0;
}

int sci_dst_2args(void* _pvCtx, char *fname, int ndimsA, int *dimsA, double *Ar,  double *Ai, int isn, int iopt)
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
            if (first_nonsingleton < 0)
            {
                first_nonsingleton = i;
            }
        }
    }

    /* void or scalar input gives void output or scalar*/
    if (ndims == 0 )
    {
        AssignOutputVariable(_pvCtx, 1) =  1;
        ReturnArguments(_pvCtx);
        return 0;
    }

    gdim.rank = ndims;
    if ((gdim.dims = (fftw_iodim *)MALLOC(sizeof(fftw_iodim) * gdim.rank)) == NULL)
    {
        Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return 0;
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


    if (!sci_dst_gen(_pvCtx, fname, ndimsA, dimsA,  Ar,  Ai, isn, gdim, iopt))
    {
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return 0;
    }

    /***********************************
    * Return results in lhs argument *
    ***********************************/

    ReturnArguments(_pvCtx);

    FREE(gdim.dims);
    FREE(gdim.howmany_dims);
    return 0;
}


int  sci_dst_3args(void* _pvCtx, char *fname, int ndimsA, int *dimsA, double *Ar,  double *Ai, int isn, int iopt)
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
            if (first_nonsingleton < 0)
            {
                first_nonsingleton = i;
            }
        }
    }

    /* void or scalar input gives void output or scalar*/
    if (ndims == 0 )
    {
        AssignOutputVariable(_pvCtx, 1) =  1;
        ReturnArguments(_pvCtx);
        return 0;
    }


    /******************** get and check third argument (sel) ****************************************/
    getVarAddressFromPosition(pvApiCtx, 3, &piAddr);
    if (isVarMatrixType(pvApiCtx, piAddr) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d.\n"), fname, 3);
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return 0;
    }
    sciErr = getVectorIntArg(pvApiCtx, 3, fname, &rank, &Sel);
    if (sciErr.iErr)
    {
        Scierror(sciErr.iErr, getErrorMessage(sciErr));
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return 0;
    }
    /* size of Sel must be less than ndimsA */
    if (rank <= 0 || rank >= ndimsA)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Must be between %d and %d.\n"), fname, 3, 1, ndimsA - 1);
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return 0;
    }
    /* check values of Sel[i] */
    for (i = 0; i < rank; i++)
    {
        if (Sel[i] <= 0)
        {
            Scierror(999, _("%s: Wrong values for input argument #%d: Positive integers expected.\n"), fname, 3);
            FREE(gdim.dims);
            FREE(gdim.howmany_dims);
            return 0;
        }
        if (Sel[i] > ndimsA)
        {
            Scierror(999, _("%s: Wrong values for input argument #%d: Elements must be less than %d.\n"), fname, 3, ndimsA);
            FREE(gdim.dims);
            FREE(gdim.howmany_dims);
            return 0;
        }
        if (i > 0 && Sel[i] <= Sel[i - 1])
        {
            Scierror(999, _("%s: Wrong values for input argument #%d: Elements must be in increasing order.\n"), fname, 3);
            FREE(gdim.dims);
            FREE(gdim.howmany_dims);
            return 0;
        }
    }

    /* Create  gdim struct */
    gdim.rank = rank;
    if ((gdim.dims = (fftw_iodim *)MALLOC(sizeof(fftw_iodim) * gdim.rank)) == NULL)
    {
        Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return 0;
    }

    pd = 1; /* used to store prod(Dims(1:sel(k-1)))*/
    pds = 1; /* used to store prod(Dims(sel(k-1):sel(k)))*/
    j = 0;
    for (i = 0; i < ndimsA; i++)
    {
        if (j >= gdim.rank)
        {
            break;
        }
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
    if ((Sel[0] != 1) && (Sel[0] != ndimsA))
    {
        gdim.howmany_rank++;
    }
    for (i = 1; i <= rank - 1; i++)
    {
        if (Sel[i] != Sel[i - 1] + 1)
        {
            /*check if all dimensions between Sel[i-1]+1 and Sel[i]-1 are
              equal to one, in this case they can be ignored and there is
              no jump*/
            for (j = Sel[i - 1] + 1; j <= Sel[i] - 1; j++)
            {
                if (dimsA[j - 1] != 1)
                {
                    gdim.howmany_rank++;
                    break;
                }
            }
        }
    }

    if ((Sel[rank - 1] != ndimsA) || (rank == 1))
    {
        gdim.howmany_rank++;
    }
    /* Fill the howmany_dims struct */
    if (gdim.howmany_rank > 0)
    {
        /* it must be the case */
        if ((gdim.howmany_dims = (fftw_iodim *)MALLOC(gdim.howmany_rank * sizeof(fftw_iodim))) == NULL)
        {
            Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
            FREE(gdim.dims);
            FREE(gdim.howmany_dims);
            return 0;
        }
        pd = 1;
        for (j = 1; j <= (Sel[0] - 1); j++)
        {
            pd *= dimsA[j - 1];    /*prod(Dims(1:(sel(1)-1)))*/
        }
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
        for (i = 1; i <= rank - 1; i++)
        {
            /* intermediate selected dimensions */
            if (Sel[i] != Sel[i - 1] + 1)
            {
                pds = 1;
                for (j = (Sel[i - 1] + 1); j <= (Sel[i] - 1); j++)
                {
                    pds *= dimsA[j - 1];    /*prod(Dims(sel(i-1)+1:(sel(i)-1)))*/
                }
                /*check again if all dimensions between Sel[i-1]+1 and
                  Sel[i]-1 are equal to one, in this case they can be
                  ignored and there is no jump*/
                for (j = (Sel[i - 1] + 1); j <= (Sel[i] - 1); j++)
                {
                    if (dimsA[j - 1] != 1)
                    {
                        gdim.howmany_dims[ih].is = pd;
                        gdim.howmany_dims[ih].os = pd;
                        gdim.howmany_dims[ih].n = pds;
                        ih++;
                        break;
                    }
                }
            }
            pd *= pds * dimsA[Sel[i] - 1]; /*prod(Dims(1:sel(i)))*/
        }

        if (Sel[rank - 1] != ndimsA)
        {
            /* last selected dimension*/
            pds = 1;
            for (j = (Sel[rank - 1] + 1); j <= ndimsA; j++)
            {
                pds *= dimsA[j - 1];    /*prod(Dims(sel(i-1)+1:(sel(i)-1)))*/
            }
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

    if (!sci_dst_gen(_pvCtx, fname, ndimsA, dimsA, Ar,  Ai, isn, gdim, iopt))
    {
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return 0;
    }

    /***********************************
    * Return results in lhs argument *
    ***********************************/

    ReturnArguments(_pvCtx);

    FREE(gdim.dims);
    FREE(gdim.howmany_dims);
    return 0;
}

int sci_dst_4args(void* _pvCtx, char *fname, int ndimsA, int *dimsA, double *Ar,  double *Ai, int isn, int iopt)
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
        AssignOutputVariable(_pvCtx, 1) =  1;
        ReturnArguments(_pvCtx);
        return 0;
    }

    /******************** get and check third argument (dim) ****************************************/
    getVarAddressFromPosition(pvApiCtx, 3, &piAddr);
    if (isVarMatrixType(pvApiCtx, piAddr) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d.\n"), fname, 3);
        FREE(Dim1);
        FREE(Incr);
        FREE(Dim);
        FREE(Sel);
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return 0;
    }
    sciErr = getVectorIntArg(pvApiCtx, 3, fname, &ndims, &Dim1);
    if (sciErr.iErr)
    {
        Scierror(sciErr.iErr, getErrorMessage(sciErr));
        FREE(Dim1);
        FREE(Incr);
        FREE(Dim);
        FREE(Sel);
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return 0;
    }
    /* check values of Dim1[i] */
    pd = 1;
    for (i = 0; i < ndims; i++)
    {
        if (Dim1[i] <= 1)
        {
            Scierror(999, _("%s: Wrong values for input argument #%d: Elements must be greater than %d.\n"), fname, 3, 1);
            FREE(Dim1);
            FREE(Incr);
            FREE(Dim);
            FREE(Sel);
            FREE(gdim.dims);
            FREE(gdim.howmany_dims);
            return 0;
        }
        pd *= Dim1[i];
    }
    if ( pd > lA)
    {
        Scierror(999, _("%s: Wrong values for input argument #%d: Must be less than %d.\n"), fname, 3, lA);
        FREE(Dim1);
        FREE(Incr);
        FREE(Dim);
        FREE(Sel);
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return 0;
    }
    if (lA % pd)
    {
        Scierror(999, _("%s: Wrong values for input argument #%d: Must be a divisor of %d.\n"), fname, 3, lA);
        FREE(Dim1);
        FREE(Incr);
        FREE(Dim);
        FREE(Sel);
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return 0;
    }
    /******************** get and check fourth argument (incr) ****************************************/
    sciErr = getVectorIntArg(pvApiCtx, 4, fname, &nincr, &Incr);
    if (sciErr.iErr)
    {
        Scierror(sciErr.iErr, getErrorMessage(sciErr));
        FREE(Dim1);
        FREE(Incr);
        FREE(Dim);
        FREE(Sel);
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return 0;
    }
    if (nincr != ndims)
    {
        Scierror(999, _("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"), fname, 3, 4);
        FREE(Dim1);
        FREE(Incr);
        FREE(Dim);
        FREE(Sel);
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return 0;
    }

    /* check values of Incr[i] */
    if (Incr[0] <= 0)
    {
        Scierror(999, _("%s: Wrong values for input argument #%d: Positive integers expected.\n"), fname, 4);
        FREE(Dim1);
        FREE(Incr);
        FREE(Dim);
        FREE(Sel);
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return 0;
    }
    for (i = 0; i < ndims; i++)
    {
        if (lA % Incr[i])
        {
            Scierror(999, _("%s: Wrong values for input argument #%d: Elements must be divisors of %d.\n"), fname, 3, lA);
            FREE(Dim1);
            FREE(Incr);
            FREE(Dim);
            FREE(Sel);
            FREE(gdim.dims);
            FREE(gdim.howmany_dims);
            return 0;
        }
        if (i > 0 && (Incr[i] <= Incr[i - 1]))
        {
            Scierror(999, _("%s: Wrong values for input argument #%d: Elements must be in increasing ""order.\n"), fname, 4);
            FREE(Dim1);
            FREE(Incr);
            FREE(Dim);
            FREE(Sel);
            FREE(gdim.dims);
            FREE(gdim.howmany_dims);
            return 0;
        }
    }
    if ((Dim = (int *)MALLOC((2 * ndims + 1) * sizeof(int))) == NULL)
    {
        Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
        FREE(Dim1);
        FREE(Incr);
        FREE(Dim);
        FREE(Sel);
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return 0;
    }
    if ((Sel = (int *)MALLOC((ndims) * sizeof(int))) == NULL)
    {
        Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
        FREE(Dim1);
        FREE(Incr);
        FREE(Dim);
        FREE(Sel);
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return 0;
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
            FREE(Dim1);
            FREE(Incr);
            FREE(Dim);
            FREE(Sel);
            FREE(gdim.dims);
            FREE(gdim.howmany_dims);
            return 0;
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
            FREE(Dim1);
            FREE(Incr);
            FREE(Dim);
            FREE(Sel);
            FREE(gdim.dims);
            FREE(gdim.howmany_dims);
            return 0;
        }
        Dim[nd++] = (int)(lA / pd);
    }

    rank = ndims;
    ndims = nd;
    /* now  same algorithm than sci_dst_3args applies */
    /* Create  gdim struct */
    gdim.rank = rank;
    if ((gdim.dims = (fftw_iodim *)MALLOC(sizeof(fftw_iodim) * gdim.rank)) == NULL)
    {
        Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
        FREE(Dim1);
        FREE(Incr);
        FREE(Dim);
        FREE(Sel);
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return 0;
    }

    pd = 1; /* used to store prod(Dims(1:sel(k-1)))*/
    pds = 1; /* used to store prod(Dims(sel(k-1):sel(k)))*/
    j = 0;
    for (i = 0; i < ndims; i++)
    {
        if (j >= gdim.rank)
        {
            break;
        }
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
    if ((Sel[0] != 1) && (Sel[0] != ndims))
    {
        gdim.howmany_rank++;
    }

    for (i = 1; i <= rank - 1; i++)
    {
        if (Sel[i] != Sel[i - 1] + 1)
        {
            /*check if all dimensions between Sel[i-1]+1 and Sel[i]-1 are
              equal to one, in this case they can be ignored and there is
              no jump*/
            for (j = Sel[i - 1] + 1; j <= Sel[i] - 1; j++)
            {
                if (Dim[j - 1] != 1)
                {
                    gdim.howmany_rank++;
                    break;
                }
            }
        }
    }
    if ((Sel[rank - 1] != ndims) || (rank == 1))
    {
        gdim.howmany_rank++;
    }
    /* Fill the howmany_dims struct */
    if (gdim.howmany_rank > 0)
    {
        /* it must be the case */
        int ih = 0;

        if ((gdim.howmany_dims = (fftw_iodim *)MALLOC(gdim.howmany_rank * sizeof(fftw_iodim))) == NULL)
        {
            Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
            FREE(Dim1);
            FREE(Incr);
            FREE(Dim);
            FREE(Sel);
            FREE(gdim.dims);
            FREE(gdim.howmany_dims);
            return 0;
        }
        pd = 1;
        for (j = 1; j <= (Sel[0] - 1); j++)
        {
            pd *= Dim[j - 1];    /*prod(Dims(1:(sel(1)-1)))*/
        }
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

        for (i = 1; i <= rank - 1; i++)
        {
            /* intermediate selected dimensions */
            if (Sel[i] != Sel[i - 1] + 1)
            {
                pds = 1;
                for (j = (Sel[i - 1] + 1); j <= (Sel[i] - 1); j++)
                {
                    pds *= Dim[j - 1];    /*prod(Dims(sel(i-1)+1:(sel(i)-1)))*/
                }
                /*check again if all dimensions between Sel[i-1]+1 and
                  Sel[i]-1 are equal to one, in this case they can be
                  ignored and there is no jump*/
                for (j = (Sel[i - 1] + 1); j <= (Sel[i] - 1); j++)
                {
                    if (Dim[j - 1] != 1)
                    {
                        gdim.howmany_dims[ih].is = pd;
                        gdim.howmany_dims[ih].os = pd;
                        gdim.howmany_dims[ih].n = pds;
                        ih++;
                        break;
                    }
                }
            }

            pd *= pds * Dim[Sel[i] - 1]; /*prod(Dims(1:sel(i)))*/
        }

        if (Sel[rank - 1] != ndims)
        {
            /* last selected dimension*/
            pds = 1;
            for (j = (Sel[rank - 1] + 1); j <= ndims; j++)
            {
                pds *= Dim[j - 1];    /*prod(Dims(sel(i-1)+1:(sel(i)-1)))*/
            }
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
    if (!sci_dst_gen(_pvCtx, fname, ndimsA, dimsA, Ar,  Ai, isn, gdim, iopt))
    {
        FREE(Dim1);
        FREE(Incr);
        FREE(Dim);
        FREE(Sel);
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return 0;
    }

    /***********************************
    * Return results in lhs argument *
    ***********************************/

    FREE(Dim1);
    FREE(Incr);
    FREE(Dim);
    FREE(Sel);
    FREE(gdim.dims);
    FREE(gdim.howmany_dims);

    ReturnArguments(_pvCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
int sci_dst_gen(void* _pvCtx, char *fname, int ndimsA, int *dimsA, double *Ar,  double *Ai, int isn, guru_dim_struct gdim, int iopt)
{

    /* Input  array variables */
    int  isrealA = (Ai == NULL),  lA = 1;
    double half = 0.5;

    /*FFTW specific library variable */
    enum Plan_Type type;
    fftw_r2r_kind *kind = NULL;
    fftw_plan p = NULL;

    /* for MKL special cases */
    int * dims1 = NULL;
    int * incr1 = NULL;

    /* local variable */
    int one = 1;
    int i = 0;
    int errflag = 0;

    for (i = 0; i < ndimsA; i++)
    {
        lA *= dimsA[i];
    }


    AssignOutputVariable(_pvCtx, 1) =  1;/* assume inplace transform*/
    /* Set pointers on real and imaginary part of the input */

    /* use inplace transform*/
    type = R2R_PLAN;
    if ((kind = (fftw_r2r_kind *)MALLOC(sizeof(fftw_r2r_kind) * gdim.rank)) == NULL)
    {
        Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
        return 0;
    }


    if (isn == -1)
    {
        if (iopt == 0 || iopt == 1)
            for (i = 0; i < gdim.rank; i++)
            {
                kind[i] = FFTW_RODFT00;
            }
        else if (iopt == 2)
            for (i = 0; i < gdim.rank; i++)
            {
                kind[i] = FFTW_RODFT10;
            }
        else if (iopt == 4)
            for (i = 0; i < gdim.rank; i++)
            {
                kind[i] = FFTW_RODFT11;
            }
    }
    else
    {
        if (iopt == 0 || iopt == 1)
            for (i = 0; i < gdim.rank; i++)
            {
                kind[i] = FFTW_RODFT00;
            }
        else if (iopt == 3)
            for (i = 0; i < gdim.rank; i++)
            {
                kind[i] = FFTW_RODFT01;
            }
        else if (iopt == 4)
            for (i = 0; i < gdim.rank; i++)
            {
                kind[i] = FFTW_RODFT11;
            }
    }

    if (!WITHMKL || gdim.howmany_rank <= 1)
    {
        /* Set Plan */
        p = GetFFTWPlan(type, &gdim, Ar, NULL, Ar, NULL, getCurrentFftwFlags(), isn , kind, &errflag);

        if (errflag == 1)
        {
            Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
            return 0;
        }
        else if (errflag == 2)
        {
            Scierror(999, _("%s: Creation of requested fftw plan failed.\n"), fname);
            return 0;
        }
        /* execute FFTW plan */
        ExecuteFFTWPlan(type, p, Ar, NULL, Ar, NULL);
        if (!isrealA)
        {
            ExecuteFFTWPlan(type, p, Ai, NULL, Ai, NULL);
        }

    }
    else
    {
        /*FFTW MKL does not implement yet guru plan with howmany_rank>1             */
        /*   associated loops described in gdim.howmany_rank and  gdim.howmany_dims */
        /*   are implemented here by a set of call with howmany_rank==1             */
        fftw_iodim *howmany_dims = gdim.howmany_dims;
        int howmany_rank = gdim.howmany_rank;
        int i1 = 0, i2 = 0;
        int nloop = 0;
        int t = 0;


        gdim.howmany_rank = 0;
        gdim.howmany_dims = NULL;

        p = GetFFTWPlan(type, &gdim, Ar, NULL, Ar, NULL, getCurrentFftwFlags(), isn , kind, &errflag);
        if (errflag == 1)
        {
            Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
            FREE(dims1);
            FREE(incr1);
            return 0;
        }
        else if (errflag == 2)
        {
            Scierror(999, _("%s: Creation of requested fftw plan failed.\n"), fname);
            FREE(dims1);
            FREE(incr1);
            return 0;
        }

        /* flatten  nested loops: replace howmany_rank nested loops by a single one*/
        /* Build temporary arrays used by flatened loop */
        if ((dims1 = (int *)MALLOC(sizeof(int) * howmany_rank)) == NULL)
        {
            Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
            FREE(dims1);
            FREE(incr1);
            return 0;
        }
        dims1[0] = howmany_dims[0].n;
        for (i = 1; i < howmany_rank; i++)
        {
            dims1[i] = dims1[i - 1] * howmany_dims[i].n;
        }
        nloop = dims1[howmany_rank - 1];

        if ((incr1 = (int *)MALLOC(sizeof(int) * howmany_rank)) == NULL)
        {
            Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
            FREE(dims1);
            FREE(incr1);
            return 0;
        }
        t = 1;
        for (i = 0; i < howmany_rank; i++)
        {
            t += (howmany_dims[i].n - 1) * howmany_dims[i].is;
            incr1[i] = t;
        }
        /*loop on each "plan" */
        i = 0; /*index on the first plan entry */
        for (i1 = 1; i1 <= nloop; i1++)
        {
            /* the input and output are assumed to be complex because
               within MKL real cases are transformed to complex ones in
               previous steps of sci_dst_gen*/
            ExecuteFFTWPlan(type, p, &Ar[i], NULL, &Ar[i], NULL);
            if (!isrealA)
            {
                ExecuteFFTWPlan(type, p, &Ai[i], NULL, &Ai[i], NULL);
            }

            i += howmany_dims[0].is;
            /* check if  a loop ends*/
            for (i2 = howmany_rank - 2; i2 >= 0; i2--)
            {
                if ((i1 % dims1[i2]) == 0)
                {
                    /*loop on dimension i2 ends, compute jump on the first plan entry index*/
                    i += howmany_dims[i2 + 1].is - incr1[i2];
                    break;
                }
            }
        }
        /* free temporary arrays */
        FREE(dims1);
        FREE(incr1);
        /* reset initial value of gdim for post treatment*/
        gdim.howmany_rank = howmany_rank;
        gdim.howmany_dims = howmany_dims;

    }

    /* normalization */
    if (iopt == 0)
    {
        if (isn == -1)
        {
            C2F(dscal)(&lA, &half, Ar, &one);
            if (!isrealA)
            {
                C2F(dscal)(&lA, &half, Ai, &one);
            }
        }
        else
        {
            if (dst_scale_array(Ar, Ai, gdim, isn) == -1)
            {
                Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
                return 0;
            }
        }
    }
    return 1;
}
