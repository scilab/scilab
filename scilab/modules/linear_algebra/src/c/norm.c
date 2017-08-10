/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
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

#include "norm.h"
#include "sci_malloc.h"

#ifdef _MSC_VER
int la_isinf(double dbl)
{
    //check finite and non NaN values
    if (_finite(dbl) == 0 && dbl == dbl)
    {
        if (dbl < 0)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }

    return 0;
}
#endif

// Lapack routines, for complex and real input
extern double C2F(dlange) (const char *norm, int *m, int *n, double *A, int *lda, double *work);
extern int     C2F(dgesdd) (const char *job, int *m, int *n, double *A, int *lda,
                            double *s, double *u, int *ldu, double *vt, int *ldvt,
                            double *work, int *lwork, int *iwork, int *info);
extern double C2F(zlange) (const char *norm, int *m, int *n, doublecomplex *A, int *lda, double *work);
extern int     C2F(zgesdd) (const char *job, int *m, int *n, doublecomplex *A, int *lda,
                            double *s, doublecomplex *u, int *ldu, doublecomplex *vt, int *ldvt,
                            doublecomplex *work, int *lwork, double *rwork, int *iwork, int *info);

double normString (double *A, int iRows, int iCols, char *flag)
{
    double ret = 0;
    double *work = NULL;

    if (strcmp(flag, "inf") == 0 || strcmp(flag, "i") == 0)
    {
        work = (double *)CALLOC(Max(1, iRows), sizeof(double));

        // Call Lapack routine for computation of the infinite norm.
        ret = C2F(dlange)("I", &iRows, &iCols, A, &iRows, work);

        FREE(work);
        return ret;
    }

    if (strcmp(flag, "fro") == 0 || strcmp(flag, "f") == 0)
    {
        // Call Lapack routine for computation of the Frobenius norm.
        ret = C2F(dlange)("F", &iRows, &iCols, A, &iRows, NULL);

        return ret;
    }

    return 0;
}

double normStringC (doublecomplex *A, int iRows, int iCols, char *flag)
{
    double ret = 0;
    double *work = NULL;

    if (strcmp(flag, "inf") == 0 || strcmp(flag, "i") == 0)
    {
        work = (double *)MALLOC(Max(1, iRows) * sizeof(double));

        // Call Lapack routine for computation of the infinite norm.
        ret = C2F(zlange)("I", &iRows, &iCols, A, &iRows, work);

        FREE(work);
        return ret;
    }

    if (strcmp(flag, "fro") == 0 || strcmp(flag, "f") == 0)
    {
        // Call Lapack routine for computation of the Frobenius norm.
        ret = C2F(zlange)("F", &iRows, &iCols, A, &iRows, NULL);

        return ret;
    }

    return 0;
}

double normP (double *A, int iRows, int iCols, double p)
{
    double ret = 0, minA, scale = 0, x = 0;
    double *S, *work;
    int *iwork;
    int i, maxRC, minRC, lwork, info, one = 1;

    maxRC = Max(iRows, iCols);
    minRC = Min(iRows, iCols);
    lwork = 3 * minRC + Max(maxRC, 7 * minRC);

    if (ISNAN(p)) // p = %nan is a special case, return 0./0 = %nan.
    {
        double a = 1.0;
        double b = 1.0;
        ret = (b - a) / (a - b);
        return ret;
    }

    //
    // /!\ la_isinf return only 0 or 1 on non Linux platforms
    //
    if (la_isinf(p) != 0 && p < 0) // p = -%inf is a special case, return min(abs(A)).
    {

        minA = Abs(A[0]);
        for (i = 0; i < iRows; ++i)
        {
            minA = Min(minA, Abs(A[i]));
        }
        return minA;
    }
    if (p == 0) // p = 0 is a special case, return 1./0 = %inf.
    {
        double a = 1.0;
        double b = 1.0;
        ret = 1. / (a - b);
        return ret;
    }
    if (p == 1) // Call the Lapack routine for computation of norm 1.
    {
        ret = C2F(dlange)("1", &iRows, &iCols, A, &iRows, NULL);
        return ret;
    }
    if (p == 2) // Call the Lapack routine for computation of norm 2.
    {
        if (iCols == 1) // In the vector case, doing a direct calculation is faster.
        {
            for (i = 0; i < iRows; ++i)
            {
                scale = Max(Abs(A[i]), scale);
            }
            if (scale == 0.0)
            {
                return 0;
            }
            else
            {
                for (i = 0; i < iRows; ++i)
                {
                    x = A[i] / scale;
                    ret += x * x;
                }
                return scale * sqrt(ret);
            }
        }
        // Allocating workspaces.
        S     = (double *)MALLOC(minRC * sizeof(double));
        work  = (double *)MALLOC(Max(1, lwork) * sizeof(double));
        iwork = (int *)MALLOC(8 * minRC * sizeof(int));

        // Not computing singular vectors, so arguments 7, 8, 9 and 10 are dummies.
        C2F(dgesdd)("N", &iRows, &iCols, A, &iRows, S, NULL, &one, NULL, &one, work, &lwork, iwork, &info);
        if (info < 0)
        {
            // Lapack provides its own error messages. Return.
            FREE(S);
            FREE(work);
            FREE(iwork);
            return 0;
        }
        else
        {
            if (info > 0)
            {
                // Lapack provides its own error messages. Return.
                FREE(S);
                FREE(work);
                FREE(iwork);
                return 0;
            }
            else
            {
                // info = 0: successful termination.
                // The largest singular value of A is stored in the first element of S, return it.
                ret = S[0];
                FREE(S);
                FREE(work);
                FREE(iwork);
                return ret;
            }
        }
    }
    // Here, A is a vector of length iRows, return sum(abs(A(i))^p))^(1/p).
    if ((int) p == p && (int) p % 2 == 0) // No need to call Abs if p is divisible by 2.
    {
        for (i = 0; i < iRows; ++i)
        {
            ret += pow(A[i], p);
        }
    }
    else
    {
        for (i = 0; i < iRows; ++i)
        {
            ret += pow(Abs(A[i]), p);
        }
    }
    return pow(ret, 1. / p);
}

double normPC (doublecomplex *A, int iRows, int iCols, double p)
{
    double ret = 0, sqmod, minA;
    double *S, *rwork;
    doublecomplex *work;
    int *iwork;
    int i, maxRC, minRC, lwork, lrwork, info, one = 1;

    maxRC  = Max(iRows, iCols);
    minRC  = Min(iRows, iCols);
    lwork  = 2 * minRC + maxRC;
    lrwork = 5 * minRC;

    if (ISNAN(p)) // p = %nan is a special case, return 0./0 = %nan.
    {
        double a = 1.0;
        double b = 1.0;
        ret = (a - b) / (a - b);
        return ret;
    }

    if (la_isinf(p) != 0 && p < 0) // p = -%inf is a special case, return min(abs(A)).
    {
        minA = sqrt(A[0].r * A[0].r + A[0].i * A[0].i); // Retrieving A[0] modulus.
        for (i = 0; i < iRows; ++i)
        {
            sqmod = A[i].r * A[i].r + A[i].i * A[i].i; // Retrieving A[i] modulus^2, starting at zero in case A has only one element.
            minA = Min(minA, sqrt(sqmod)); // min(minA, modulus(A[i])).
        }
        return minA;
    }
    if (p == 0) // p = 0 is a special case, return 1./0 = %inf.
    {
        double a = 1.0;
        double b = 1.0;
        ret = 1. / (a - b);
        return ret;
    }
    if (p == 1) // Call the Lapack routine for computation of norm 1.
    {
        ret = C2F(zlange)("1", &iRows, &iCols, A, &iRows, NULL);

        return ret;
    }
    if (p == 2) // Call the Lapack routine for computation of norm 2.
    {
        if (iCols == 1) // In the vector case, doing a direct calculation is faster.
        {
            for (i = 0; i < iRows; ++i)
            {
                ret += A[i].r * A[i].r + A[i].i * A[i].i; // Retrieving A[i] modulus^2.
            }
            return sqrt(ret);
        }
        // Allocating workspaces.
        S     = (double *)MALLOC(minRC * sizeof(double));
        work  = (doublecomplex *)MALLOC(Max(1, lwork) * sizeof(doublecomplex));
        rwork = (double *)MALLOC(Max(1, lrwork) * sizeof(double));
        iwork = (int *)MALLOC(8 * minRC * sizeof(int));

        // Not computing singular vectors, so arguments 7, 8, 9 and 10 are dummies.
        C2F(zgesdd)("N", &iRows, &iCols, A, &iRows, S, NULL, &one, NULL, &one, work, &lwork, rwork, iwork, &info);
        if (info < 0)
        {
            // Lapack provides its own error messages. Return.
            // Since the arguments have all been checked before, this error should not occur.
            FREE(S);
            FREE(work);
            FREE(rwork);
            FREE(iwork);
            return 0;
        }
        else
        {
            if (info > 0)
            {
                // Lapack provides its own error messages. Return.
                // Since the arguments have all been checked before, this error should not occur.
                FREE(S);
                FREE(work);
                FREE(rwork);
                FREE(iwork);
                return 0;
            }
            else
            {
                // info = 0: successful termination.
                // The largest singular value of A is stored in the first element of S, return it.
                ret = S[0];
                FREE(S);
                FREE(work);
                FREE(rwork);
                FREE(iwork);
                return ret;
            }
        }
    }
    // Here, A is a vector of length iRows, return sum(abs(A(i))^p))^(1/p).
    for (i = 0; i < iRows; ++i)
    {
        sqmod = A[i].r * A[i].r + A[i].i * A[i].i; // Retrieving A[i] modulus^2.
        ret += pow(sqmod, p / 2); // sum(modulus(A[i])^p).
    }
    return pow(ret, 1. / p); // sum(modulus(A[i])^p)^(1/p).
}
