/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Adeline CARNIS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "eigs_dependencies.h"

extern int C2F(dgemv) (char* trans, int* m, int* n, double* alpha, double* A,
                       int* lda, double* x, int* incx, double* beta, double* y, int* incy);
extern double C2F(ddot) (int *n, double* x, int* incx, double* y, int* incy);

void process_dneupd_data(double* DR, double* DI, double* Z, int N, int nev, double* AR,
                         doublecomplex* eigenvalue, doublecomplex* eigenvector,
                         int sigma_imaginary)
{
    /* if sigma_imaginary there is an extra step to compute the eigenvalues
       as explained in the dneupd user guide */

    double* temp1 = NULL;
    double* temp2 = NULL;

    int i = 0;
    int j = 0;

    double alpha = 1;
    double beta = 0;
    int iOne = 1;
    double real_part;
    double imag_part;

    if ( sigma_imaginary )
    {
        temp1 = (double*) malloc(N * sizeof(double));
        temp2 = (double*) malloc(N * sizeof(double));

        while (i < nev)
        {
            if (DI[i] == 0)
            {
                C2F(dgemv) ("n", &N, &N, &alpha, AR, &N, Z + N * i, &iOne, &beta, temp1, &iOne);
                eigenvalue[i].r = C2F(ddot) (&N, Z + N * i, &iOne, temp1, &iOne);
                eigenvalue[i].i = 0;
                i = i + 1;
            }
            else
            {
                C2F(dgemv) ("n", &N, &N, &alpha, AR, &N, Z + N * i, &iOne, &beta, temp1, &iOne);
                C2F(dgemv) ("n", &N, &N, &alpha, AR, &N, Z + N * (i + 1), &iOne, &beta, temp2, &iOne);
                real_part = C2F(ddot) (&N, Z + N * i, &iOne, temp1, &iOne) + \
                            C2F(ddot) (&N, Z + N * (i + 1), &iOne, temp2, &iOne);
                imag_part = C2F(ddot) (&N, Z + N * i, &iOne, temp2, &iOne) - \
                            C2F(ddot) (&N, Z + N * (i + 1), &iOne, temp1, &iOne);
                eigenvalue[i].r = real_part;
                eigenvalue[i].i = imag_part;
                eigenvalue[i + 1].r = real_part;
                eigenvalue[i + 1].i = -imag_part;
                i = i + 2;
            }
        }
        free(temp1);
        free(temp2);
    }
    else
    {
        for (i = 0; i < nev + 1; i++)
        {
            eigenvalue[i].r = DR[i];
            eigenvalue[i].i = DI[i];
        }
    }

    if (eigenvector)
    {
        i = 0;

        while (i < nev)
        {
            if (DI[i] != 0)
            {
                for (j = 0; j < N; j++)
                {
                    eigenvector[i * N + j].r = Z[i * N + j];
                    eigenvector[i * N + j].i = Z[(i + 1) * N + j];
                    eigenvector[(i + 1) * N + j].r = Z[i * N + j];
                    eigenvector[(i + 1) * N + j].i = -Z[(i + 1) * N + j];
                }

                i = i + 2;
            }
            else
            {
                for (j = 0; j < N; j++)
                {
                    eigenvector[i * N + j].r = Z[i * N + j];
                    eigenvector[i * N + j].i = 0;
                }

                i = i + 1;
            }
        }
    }
}
