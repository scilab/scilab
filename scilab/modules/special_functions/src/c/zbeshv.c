/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2005-2008 - INRIA - Serge STEER
* Copyright (C) 2011 - DIGITEO - Allan CORNET
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
#include <string.h>
#include "zbeshv.h"
#include "machine.h"
#include "core_math.h"
#include "returnanan.h"
#include "numericconstants_interface.h"
/*--------------------------------------------------------------------------*/
/* fortran subroutines */
extern int C2F(dset)();
extern int C2F(dscal)();
extern int C2F(wscal)();
extern int C2F(zbesh)();
extern int C2F(dcopy)();
/*--------------------------------------------------------------------------*/
static int zbeshg(double *x1r, double *x1i, double *alpha,
                  int *kode, int *k, int *n, double *yr,
                  double *yi, int *nz, double *wr,
                  double *wi, int *ierr);
/*--------------------------------------------------------------------------*/
/* compute besseli function for x and alpha given by vectors */
/* w : working array of size 2*na (used only if nz>0 and alpha */
/* contains negative values */
int zbeshv(double *xr, double *xi, int *nx,
           double *alpha, int *na, int *kode, int *k, double
           *yr, double *yi, double *wr, double *wi, int *ierr)
{
    double eps = nc_eps_machine();
    int iOne = 1;
    int i = 0, j = 0, nz = 0;

    *ierr = 0;
    if (*na < 0)
    {
        /* element wise case x and alpha are supposed to have the same size */
        for (i = 1; i <= *nx; ++i)
        {
            int ier = 0;
            zbeshg(&xr[i - 1], &xi[i - 1], &alpha[i - 1], kode, k, &iOne, &yr[i - 1],
                   &yi[i - 1], &nz, &wr[1], &wi[0], &ier);
            *ierr = Max(*ierr, ier);
        }
    }
    else if (*na == 1)
    {
        for (i = 1; i <= *nx; ++i)
        {
            int ier = 0;
            zbeshg(&xr[i - 1], &xi[i - 1], &alpha[0], kode, k, &iOne, &yr[i - 1],
                   &yi[i - 1], &nz, &wr[0], &wi[0], &ier);
            *ierr = Max(*ierr, ier);
        }
    }
    else
    {
        /* compute besselh(x(i),y(j)), i=1,nx,j=1,na */
        double dTmp = 0;
        int n = 0;
        int  l = 1;
L5:
        n = 0;
L10:
        ++n;
        j = l + n;
        if (j <= *na && (dTmp = alpha[j] + 1 - alpha[j - 1], fabs(dTmp)) <= eps)
        {
            goto L10;
        }
        for (i = 1; i <= *nx; ++i)
        {
            int ier = 0;
            zbeshg(&xr[i - 1], &xi[i - 1], &alpha[l - 1], kode, k, &n, &wr[0], &wi[0],
                   &nz, &wr[*na], &wi[*na], &ier);

            *ierr = Max(*ierr, ier);

            C2F(dcopy)(&n, &wr[0], &iOne, &yr[(i + (l - 1) * *nx) - 1], nx);

            C2F(dcopy)(&n, &wi[0], &iOne, &yi[(i + (l - 1) * *nx) - 1], nx);
        }

        l = j;

        if (l <= *na)
        {
            goto L5;
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static int zbeshg(double *x1r, double *x1i, double *alpha,
                  int *kode, int *k, int *n, double *yr,
                  double *yi, int *nz, double *wr,
                  double *wi, int *ierr)
{
    int iOne = 1;
    int iTwo = 2;
    double dNegOne = -1.;

    double nan = C2F(returnanan)();

    int iVal = 0;
    int nn = 0;
    double xr = *x1r;
    double xi = *x1i;

    /* extends cbesi for the case where alpha is negative */

    if (ISNAN(xr) || ISNAN(xi) || ISNAN(*alpha))
    {
        /* NaN case */
        C2F(dset)(n, &nan, &yr[0], &iOne);
        C2F(dset)(n, &nan, &yi[0], &iOne);

        *ierr = 4;
    }
    else if (*alpha >= 0.)
    {
        C2F(zbesh)(&xr, &xi, alpha, kode, k, n, &yr[0], &yi[0], nz, ierr);
        if (*ierr == 1 || *ierr == 2 || *ierr >= 4)
        {
            C2F(dset)(n, &nan, &yr[0], &iOne);
            C2F(dset)(n, &nan, &yi[0], &iOne);
        }
    }
    else if (*alpha == (int)(*alpha))
    {
        double a1 = 0.;
        /* alpha < 0 and int, */
        /*  transform to positive value of alpha */
        if (*alpha - 1 + *n >= 0.)
        {
            /* 0 is between alpha and alpha+n */
            a1 = 0.;
            /* Computing MIN */
            nn = Min(*n, (int) (-(*alpha)));
        }
        else
        {
            a1 = -(*alpha - 1 + *n);
            nn = *n;
        }
        C2F(zbesh)(&xr, &xi, &a1, kode, k, n, &wr[0], &wi[0], nz, ierr);
        if (*ierr == 1 || *ierr == 2 || *ierr >= 4)
        {
            C2F(dset)(n, &nan, &yr[0], &iOne);
            C2F(dset)(n, &nan, &yi[0], &iOne);
        }
        else
        {
            if (*n > nn)
            {
                /* 0 is between alpha and alpha+n */
                iVal = *n - nn;
                C2F(dcopy)(&iVal, &wr[0], &iOne, &yr[nn], &iOne);
                C2F(dcopy)(&iVal, &wi[0], &iOne, &yi[nn], &iOne);
                C2F(dcopy)(&nn, &wr[1], &iOne, &yr[0], &iOne);
                C2F(dcopy)(&nn, &wi[1], &iOne, &yi[0], &iOne);
            }
            else
            {
                /* alpha and alpha+n are negative */
                C2F(dcopy)(n, &wr[0], &iOne, &yr[0], &iOne);
                C2F(dcopy)(n, &wi[0], &iOne, &yi[0], &iOne);
            }
        }
        iVal = (nn - (((int) fabs(*alpha) + 1) % 2) + 1) / 2;
        C2F(dscal)(&iVal, &dNegOne, &yr[((int) fabs(*alpha) + 1) % 2], &iTwo);
        C2F(dscal)(&iVal, &dNegOne, &yi[((int) fabs(*alpha) + 1) % 2], &iTwo);
    }
    else
    {
        int nz1 = 0;
        double a1 = 0.;
        /* first alpha is negative non int, transform to positive value of alpha */
        if (*alpha - 1. + *n >= 0.)
        {
            /* 0 is between alpha and alpha+n */
            nn = (int) (-(*alpha)) + 1;
        }
        else
        {
            nn = *n;
        }

        /* compute for negative value of alpha+k, transform problem for */
        /* a1:a1+(nn-1) with a1 positive  a1+k =abs(alpha+nn-k) */
        a1 = -(*alpha - 1. + nn);
        C2F(zbesh)(&xr, &xi, &a1, kode, k, n, &wr[0], &wi[0], &nz1, ierr);
        *nz = Max(nz1, 0);
        if (*ierr == 0)
        {
            double a = cos(a1 * M_PI);
            double b = sin(a1 * M_PI);
            if (*k == 1)
            {
                C2F(wscal)(&nn, &a, &b, &wr[0], &wi[0], &iOne);
            }
            else
            {
                double dNegB = -b;
                C2F(wscal)(&nn, &a, &dNegB, &wr[0], &wi[0], &iOne);
            }
            /* change sign to take into account that sin((a1+k)*pi) and cos((a1+k)*pi) */
            /* changes sign with k */
            if (nn >= 2)
            {
                iVal = nn / 2;
                C2F(dscal)(&iVal, &dNegOne, &wr[1], &iTwo);
                C2F(dscal)(&iVal, &dNegOne, &wi[1], &iTwo);
            }
        }
        else if (*ierr == 1 || *ierr == 2 || *ierr >= 4)
        {
            C2F(dset)(&nn, &nan, &wr[0], &iOne);
            C2F(dset)(&nn, &nan, &wi[0], &iOne);
        }

        /* store the result in the correct order */
        C2F(dcopy)(&nn, &wr[0], &iOne, &yr[0], &iOne);
        C2F(dcopy)(&nn, &wi[0], &iOne, &yi[0], &iOne);

        /* compute for positive value of alpha+k is any */
        if (*n > nn)
        {
            int ier = 0;
            a1 = 1. - a1;
            iVal = *n - nn;
            C2F(zbesh)(&xr, &xi, &a1, kode, k, &iVal, &yr[nn], &yi[nn], nz, &ier);
            if (ier == 1 || ier == 2 || ier >= 4)
            {
                iVal = *n - nn;
                C2F(dset)(&iVal, &nan, &yr[nn], &iOne);
                C2F(dset)(&iVal, &nan, &yi[nn], &iOne);
            }
            *ierr = Max(*ierr, ier);
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
