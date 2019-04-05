/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2019 - Stéphane MOTTELET
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "double.hxx"
#include "function.hxx"
#include "statistics_gw.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "elem_common.h"
}

/* ==================================================================== */
types::Function::ReturnValue sci_percent_cov(types::typed_list& in, int _iRetCount, types::typed_list& out)
{
    types::Double* pDblOut = nullptr;

    if (in.size() != 2 && in.size() != 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d or %d expected.\n"), "cov", 2, 3);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected."), "cov", 1);
        return types::Function::Error;
    }

    for (int i = 0; i < in.size(); i++)
    {
        if (in[i]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: %s expected.\n"), "cov", i + 1, "double");
            return types::Function::Error;
        }
    }

    if (in[in.size() - 1]->getAs<types::Double>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong dimensions for input argument #%d: A scalar expected.\n"), "cov", in.size());
        return types::Function::Error;
    }

    for (int i = 0; i < in.size() - 1; i++)
    {
        if (in[i]->getAs<types::Double>()->getDims() > 2)
        {
            Scierror(999, _("%s: Wrong dimensions for input argument #%d: A column vector or matrix expected.\n"), "cov", i + 1);
            return types::Function::Error;
        }
    }

    if (in[0]->getAs<types::Double>()->isEmpty() || (in.size() == 3 && in[1]->getAs<types::Double>()->isEmpty()))
    {
        pDblOut = types::Double::Empty();
        out.push_back(pDblOut);
        return types::Function::OK;
    }

    double dblr1 = in[in.size() - 1]->getAs<types::Double>()->get(0);

    types::Double* pDblX = in[0]->getAs<types::Double>();
    int* piXDims = pDblX->getDimsArray();
    int iNrow = piXDims[0];
    int iNcolX = piXDims[1];
    int iOne = 1;
    double dblOne = 1.0;
    double dblMinusOne = -1.0;
    double dblZero = 0.0;
    double dblr2 = -dblr1 / (double)iNrow;

    // Real and imaginary part of input matrix X1 + %i * X2
    double* pdblX[3] = { pDblX->get(), pDblX->getImg(), NULL };
    double* pdblOut[2];
    // Vector of ones for row-wise sums
    types::Double* pDblU = new types::Double(iNrow, 1, false);
    pDblU->setOnes();

    if (in.size() == 3) // covi(X,Y,r) when X and Y are matrices
    {
        double dblMinusr1 = -dblr1;
        double dblMinusr2 = -dblr2;
        types::Double* pDblY = in[1]->getAs<types::Double>();
        int* piYDims = pDblY->getDimsArray();
        int iNcolY = piYDims[1];
        // Real and imaginary part of input matrix Y = Y1 + %i * Y2
        double* pdblY[3] = { pDblY->get(), pDblY->getImg(), NULL };

        if (piXDims[0] != piYDims[0])
        {
            Scierror(999, _("%s: Incompatible input arguments #%d and #%d: Same number of rows expected.\n"), "cov", 1, 2);
            return types::Function::Error;
        }

        pDblOut = new types::Double(iNcolX, iNcolY, pDblX->isComplex() || pDblY->isComplex());
        pdblOut[0] = pDblOut->get();
        pdblOut[1] = pDblOut->getImg();
        // Vector for row-wise sums
        double *pdblSumX = new double[iNcolX];
        double *pdblSumY[2] = { new double[iNcolY], new double[iNcolY] };

        pDblOut->setZeros();

        for (int j = 0; pdblY[j] != NULL; j++)
        {
            // pre-computation of sum(Yj,'r')
            C2F(dgemv)((char*)"T", &iNrow, &iNcolY, &dblOne, pdblY[j], &iNrow, pDblU->get(), &iOne, &dblZero, &pdblSumY[j][0], &iOne);
        }
        for (int i = 0; pdblX[i] != NULL; i++)
        {
            // Computation of sum(Xi,'r')
            C2F(dgemv)((char*)"T", &iNrow, &iNcolX, &dblOne, pdblX[i], &iNrow, pDblU->get(), &iOne, &dblZero, pdblSumX, &iOne);
            for (int j = 0; pdblY[j] != NULL; j++)
            {
                // +/- r1 * Xi'*Yj
                C2F(dgemm)((char*)"T", (char*)"N", &iNcolX, &iNcolY, &iNrow, i == 1 && j == 0 ? &dblMinusr1 : &dblr1,
                    pdblX[i], &iNrow, pdblY[j], &iNrow, &dblOne, pdblOut[i^j], &iNcolX);
                // +/- r2 * (Xi'*U)*(Yj'*U)'
                C2F(dger)(&iNcolX, &iNcolY, i == 1 && j == 0 ? &dblMinusr2 : &dblr2,
                    pdblSumX, &iOne, &pdblSumY[j][0], &iOne, pdblOut[i^j], &iNcolX);
            }
        }
        delete[] pdblSumX;
        delete[] pdblSumY[0];
        delete[] pdblSumY[1];
    }
    else // (in.size() == 2) covi(X,r)
    {
        double* pdblSrc = NULL;

        // Output matrix Re(cov) + %i*Im(cov)
        pDblOut = new types::Double(iNcolX, iNcolX, pDblX->isComplex());
        pdblOut[0] = pDblOut->get();
        pdblOut[1] = pDblOut->getImg();

        // Vector for row-wise sums
        double* pdblSumX[2] = { new double[iNcolX], new double[iNcolX] };

        pDblOut->setZeros();

        for (int i = 0; pdblX[i] != NULL; i++)
        {
            // Re(cov) += r1 * X'*X + r2 * (X'*U)*(U'*X) with X=X1 then X2 if complex input
            // Computation of sum(Xi,'r')
            C2F(dgemv)((char*)"T", &iNrow, &iNcolX, &dblOne, pdblX[i], &iNrow, pDblU->get(), &iOne, &dblZero, &pdblSumX[i][0], &iOne);
            // Below computations are done on upper triangle only
            C2F(dsyrk)((char*)"U", (char*)"T", &iNcolX, &iNrow, &dblr1, pdblX[i], &iNrow, &dblOne, pdblOut[0], &iNcolX);
            C2F(dsyr)((char*)"U", &iNcolX, &dblr2, &pdblSumX[i][0], &iOne, pdblOut[0], &iNcolX);
        }

        if (pDblX->isComplex())
        {
            // Im(cov) = r1 * X1'*X2 + r2 * (X1'*U)*(U'*X2) - transpose of previous term
            // BLAS has no skew symmetric optimized functions, we compute whole first term
            C2F(dgemm)((char*)"T", (char*)"N", &iNcolX, &iNcolX, &iNrow, &dblr1, pdblX[0], &iNrow, pdblX[1], &iNrow, &dblZero, pdblOut[1], &iNcolX);
            C2F(dger)(&iNcolX, &iNcolX, &dblr2, &pdblSumX[0][0], &iOne, &pdblSumX[1][0], &iOne, pdblOut[1], &iNcolX);
            // then substract transpose
            pdblSrc = pdblOut[1];
            for (int iLen = iNcolX; iLen > 0; iLen--, pdblSrc += (iNcolX + 1))
            {
                C2F(daxpy)(&iLen, &dblMinusOne, pdblSrc, &iOne, pdblSrc, &iNcolX);
                C2F(dcopy)(&iLen, pdblSrc, &iNcolX, pdblSrc, &iOne);
                C2F(dscal)(&iLen, &dblMinusOne, pdblSrc, &iOne);
            }
        }

        // Real part: copy upper triangle into lower triangle (no BLAS function does this)
        pdblSrc = pdblOut[0];
        for (int iLen = iNcolX; iLen > 1; iLen--, pdblSrc += (iNcolX + 1))
        {
            C2F(dcopy)(&iLen, pdblSrc, &iNcolX, pdblSrc, &iOne);
        }

        delete[] pdblSumX[0];
        delete[] pdblSumX[1];
    }

    pDblU->killMe();
    out.push_back(pDblOut);
    return types::Function::OK;
}
