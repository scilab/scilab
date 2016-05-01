/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Antoine ELIAS
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
/*--------------------------------------------------------------------------*/

#include "signal_gw.hxx"
#include "double.hxx"
#include "function.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "localization.h"
#include "Scierror.h"
#include "elem_common.h"

    extern void C2F(fft842)(int *inverse, int *signal_length, double *signal_real, double *signal_imaginary, int *error);
    extern void C2F(dfft2)(double *signal_real, double *signal_imaginary, int *nseg, int *n, int *nspn, int *inverse, int *error, int *buffer_data, int *buffer_size);
}

#define POW2_15     32768

bool isPowerOf2(int _iVal);

int maxfactor(int n);
int fft_1dim(double *signal_real, double *signal_imaginary, int signal_length, int inverse, int *buffer_data, int buffer_size);
int fft_2dim(double *signal_real, double *signal_imaginary, int signal_rows, int signal_cols, int inverse, int *buffer_data, int buffer_size);
int fft_ndim(double *signal_real, double *signal_imaginary, int signal_length, int dimensions_length, int dimension_stride, int inverse, int *buffer_data, int buffer_size);

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_fft(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iDimLength          = 0;
    int iDimCount           = 0;
    int iInc                = 0;
    int iWay                = -1;
    int iSize               = 0;
    int iOne                = 1;
    int iErr                = 0;
    double dblZero          = 0;

    //workspace
    int iWS                 = 0;
    int* piWS               = NULL;
    types::Double* pIn1     = NULL;

    //check input parameters
    if (in.size() != 1 && in.size() != 2 && in.size() != 4)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d or %d expected.\n"), "fft", 1, 4);
        return types::Function::Error;
    }

    switch (in.size())
    {
        case 4 :
            //check fourth input parameter : inc
            if (in[3]->isDouble() == false || in[3]->getAs<types::Double>()->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Scalar expected.\n"), "fft", 4);
                return types::Function::Error;
            }

            iInc = (int)in[3]->getAs<types::Double>()->get(0);

            //check third input parameter : dim
            if (in[2]->isDouble() == false || in[2]->getAs<types::Double>()->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Scalar expected.\n"), "fft", 3);
                return types::Function::Error;
            }

            iDimLength = (int)in[2]->getAs<types::Double>()->get(0);
            iDimCount = 3; //any value > 2 (used as a flag)

        case 2 :
            //check third input parameter : way
            if (in[1]->isDouble() == false || in[1]->getAs<types::Double>()->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Scalar expected.\n"), "fft", 2);
                return types::Function::Error;
            }

            iWay = (int)in[1]->getAs<types::Double>()->get(0);
            if (iWay != -1 && iWay != 1)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "fft", 2, "-1 1");
                return types::Function::Error;
            }

        case 1:
            if (in[0]->isDouble() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Scalar expected.\n"), "fft", 1);
                return types::Function::Error;
            }

            pIn1 = in[0]->getAs<types::Double>();

            iDimCount = std::max(iDimCount, ((pIn1->getRows() == 1 || pIn1->getCols() == 1) ? 1 : 2));
            iSize = pIn1->getSize();
            break;
        default :
        {
            Scierror(77, _("%s: Wrong number of input argument(s): %d or %d expected.\n"), "fft", 1, 4);
            return types::Function::Error;
        }
    }

    types::Double* pOut = pIn1->clone()->getAs<types::Double>();
    pOut->setComplex(true);

    //alloc workspace required by dfft2
    iWS = 8 * maxfactor(iDimLength == 0 ? iSize : iDimLength) + 24;
    piWS = (int*)MALLOC(iWS * sizeof(int));
    if (piWS == NULL)
    {
        Scierror(999, _("%s : Memory allocation error.\n"), "fft");
        return types::Function::Error;
    }
    switch (iDimCount)
    {
        case 1 :
            iErr = fft_1dim(pOut->getReal(), pOut->getImg(), iSize, iWay, piWS, iWS);
            break;
        case 2 :
            iErr = fft_2dim(pOut->getReal(), pOut->getImg(), pOut->getRows(), pOut->getCols(), iWay, piWS, iWS);
            if (iErr == 1)
            {
                Scierror(999, _("%s : Memory allocation error.\n"), "fft");
                return types::Function::Error;
            }
            break;
        default :
            iErr = fft_ndim(pOut->getReal(), pOut->getImg(), iSize, iDimLength, iInc, iWay, piWS, iWS);
            break;
    }
    double *df = pOut->getImg();
    bool cplx = false;
    for (int i = 0; i < iSize; i++)
    {
        if (df[i] != 0)
        {
            cplx = true;
            break;
        }
    }
    if (cplx == false)
    {
        pOut->setComplex(false);
    }

    FREE(piWS);

    out.push_back(pOut);
    return types::Function::OK;
}

int fft_1dim(double *signal_real, double *signal_imaginary, int signal_length, int inverse, int *buffer_data, int buffer_size)
{
    int error = 0;
    int one = 1;

    if (isPowerOf2(signal_length) && signal_length < POW2_15)
    {
        C2F(fft842)(&inverse, &signal_length, signal_real, signal_imaginary, &error);
    }
    else
    {
        C2F(dfft2)(signal_real, signal_imaginary, &one, &signal_length, &one, &inverse, &error, buffer_data, &buffer_size);
    }

    return error;
}

int fft_2dim(double *signal_real, double *signal_imaginary, int signal_rows, int signal_cols, int inverse, int *buffer_data, int buffer_size)
{
    int error = 0;
    int one = 1;

    if (isPowerOf2(signal_rows) && signal_rows < POW2_15)
    {
        for (int i = 0 ; i < signal_cols ; i++)
        {
            C2F(fft842)(&inverse, &signal_rows, &(signal_real[signal_rows * i]), &(signal_imaginary[signal_rows * i]), &error);
        }
    }
    else
    {
        C2F(dfft2)(signal_real, signal_imaginary, &signal_cols, &signal_rows, &one, &inverse, &error, buffer_data, &buffer_size);
    }

    if (isPowerOf2(signal_cols) && signal_cols < POW2_15)
    {
        double* temp_real = NULL;
        double* temp_imaginary = NULL;

        temp_real = (double *)MALLOC(signal_cols * sizeof(double));
        temp_imaginary = (double *)MALLOC(signal_cols * sizeof(double));

        if (temp_real == NULL || temp_imaginary == NULL)
        {
            return 1;
        }

        for (int i = 0 ; i < signal_rows ; i++)
        {
            C2F(dcopy)(&signal_cols, &(signal_real[i]), &signal_rows, temp_real, &one);
            C2F(dcopy)(&signal_cols, &(signal_imaginary[i]), &signal_rows, temp_imaginary, &one);
            C2F(fft842)(&inverse, &signal_cols, temp_real, temp_imaginary, &error);
            C2F(dcopy)(&signal_cols, temp_real, &one, &(signal_real[i]), &signal_rows);
            C2F(dcopy)(&signal_cols, temp_imaginary, &one, &(signal_imaginary[i]), &signal_rows);
        }

        FREE(temp_imaginary);
        temp_imaginary = NULL;
        free(temp_real);
        temp_real = NULL;
    }
    else
    {
        /* erroneous implementation suspected */
        C2F(dfft2)(signal_real, signal_imaginary, &one, &signal_cols, &signal_rows, &inverse, &error, buffer_data, &buffer_size);
    }

    return error;
}

int fft_ndim(double *signal_real, double *signal_imaginary, int signal_length, int dimensions_length, int dimension_stride, int inverse, int *buffer_data, int buffer_size)
{
    /* translated litterally from Fortran... but... wtf ?! */
    int error = 0;
    int nseg = signal_length / dimensions_length / dimension_stride;
    C2F(dfft2)(signal_real, signal_imaginary, &nseg, &dimensions_length, &dimension_stride, &inverse, &error, buffer_data, &buffer_size);

    return error;
}


int maxfactor(int n)
{
    int nfac[15];
    int m = 0, j = 0, jj = 0, k = 0, kt = 0, max = 0;

    for (k = abs(n) ; k % 16 == 0 ; k /= 16)
    {
        m++;
        nfac[m - 1] = 4;
    }

    for (j = 3, jj = 9 ; jj <= k ; j += 2, jj = j * j)
    {
        if (k % jj != 0)
        {
            continue;
        }
        m++;
        nfac[m - 1] = j;
        k /= jj;
    }

    if (k <= 4)
    {
        kt = m;
        nfac[m] = k;
        if (k != 1)
        {
            m++;
        }
    }
    else
    {
        if (k % 4 == 0)
        {
            m++;
            nfac[m - 1] = 2;
            k /= 4;
        }

        kt = m;
        for (j = 2 ; j <= k ; j = ((j + 1) / 2) * 2 + 1)
        {
            if (k % j != 0)
            {
                continue;
            }

            m++;
            nfac[m - 1] = j;
            k /= j;
        }
    }

    if (kt != 0)
    {
        for (j = kt ; j > 0 ; j--)
        {
            m++;
            nfac[m - 1] = nfac[j - 1];
        }
    }

    /* get nfac maximum */
    for (j = 0, max = nfac[0]; j < m ; j++)
    {
        max = (nfac[j] > max) ? nfac[j] : max;
    }

    return max;
}

bool isPowerOf2(int _iVal)
{
    return (_iVal & (_iVal - 1)) == 0;
}
