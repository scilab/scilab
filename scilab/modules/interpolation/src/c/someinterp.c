/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Bruno Pincon
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*/
/* a routine for n-dim linear interpolation together
 * with its utility routines
 */

#include <math.h>
#include "someinterp.h"
#include "isanan.h"
#include "returnanan.h"
enum {NOT_A_KNOT, NATURAL, CLAMPED, PERIODIC, FAST, FAST_PERIODIC,
      MONOTONE, BY_ZERO, C0, LINEAR, BY_NAN
     };

static int isearch(double t, double x[], int n)
{
    /*     PURPOSE
     *        x[0..n-1] being an array (with strict increasing order and n >=2)
     *        representing intervals, this routine return i such that :
     *
     *           x[i] <= t <= x[i+1]
     *
     *        and -1 if t is not in [x[0], x[n-1]]
     */
    int i1, i2, i;
    if ( x[0] <= t  &&  t <= x[n - 1] )
    {
        i1 = 0;
        i2 = n - 1;
        while ( i2 - i1 > 1 )
        {
            i = (i1 + i2) / 2;
            if ( t <= x[i] )
            {
                i2 = i;
            }
            else
            {
                i1 = i;
            }
        }
        return (i1);
    }
    else
    {
        return (-1);
    }
}

static void fast_int_search(double xx, double x[], int nx, int *i)
{
    if ( *i == -1 )
    {
        *i = isearch(xx, x, nx);
    }
    else if ( !  (x[*i] <= xx && xx <= x[*i + 1]) )
    {
        *i = isearch(xx, x, nx);
    }
}


static void coord_by_periodicity(double *t, double x[], int n, int *i)
{
    /*
     *     PURPOSE
     *        recompute t such that t in [x[0], x[n-1]] by periodicity :
     *        and then the interval i of this new t
     */
    double r, L;
    L = x[n - 1] - x[0];
    r = (*t - x[0]) / L;
    if (r >= 0.0)
    {
        *t = x[0] + (r - floor(r)) * L;
    }
    else
    {
        *t = x[n - 1] + (r - ceil(r)) * L;
    }

    /*  some cautions in case of roundoff errors (is necessary ?) */
    if (*t < x[0])
    {
        *t = x[0];
        *i = 0;
    }
    else if (*t > x[n - 1])
    {
        *t = x[n - 1];
        *i  = n - 2;
    }
    else
    {
        *i = isearch(*t, x, n);
    }
}
void nlinear_interp(double **x , double val[], int dim[], int n,
                    double **xp, double yp[], int np, int outmode,
                    double u[], double v[], int ad[], int k[])
{
    /*  interpolation lineaire nb_dim-dimensionnelle
     *  --------------------------------------------

     interface scilab ?

     yp = linear_interpn(xp1, ..., xpN, x1, ..., xN, val, outmode)



     *     x[j][] : the grid abscissae in the dim j
     *     dim[j] : nb of points in the dim j
     *     n      : number of dimension
     *     val[]  : array of the grid node values, for instance if nbdim = 3
     *              and dim = [nx ny nz] then val(i,j,k) is stored in
     *              i + nx( j + ny k )
     *     xp[][] : the coordinates where we have to interpolate
     *              the coordinate of the i th point are stored
     *              at xp[0][i] ..... xp[n-1][i]
     *     yp[]   : the result (an array 0...np-1)
     *     np     : nb of points for the evaluation
     *     outmode: specify the method of evaluation when a point is
     *              outside the grid
     *     u, v, ad, k : work arrays
     */

    int i, j, l, p, temp, b,/* toto,*/ two_p_n;
    double xx;

    /*
     *   calcul des decalages d'indices pour retrouver les valeurs
     *   de l'hypercube encadrant le point à interpoler
     */
    ad[0] = 0;
    ad[1] = 1;
    temp = 1 ;
    p = 1;
    for ( j = 0; j < n - 1; j++)
    {
        temp = temp * dim[j];
        p = 2 * p;
        for ( i = 0; i < p; i++ )
        {
            ad[p + i] = ad[i] + temp;
        }
    };
    /* a ce niveau on a  p = 2^(n-1)  */
    two_p_n = 2 * p;

    /* initialisation pour recherche d'intervalle rapide */
    for ( j = 0; j < n; j++ )
    {
        k[j] = -1;
    }

    for ( i = 0; i < np; i++ )
    {
        /* interpolation du i eme point */

        /*  1 - recherche des intervalles  */
        for ( j = 0; j < n; j++ )
        {
            xx = xp[j][i];
            if ( C2F(isanan)(&xx) )
            {
                v[0] = C2F(returnanan)();
                goto fin;
            }
            fast_int_search(xx, x[j], dim[j], &(k[j]));
            if ( k[j] == -1 )   /* le point est a l'exterieur */
                switch (outmode)
                {
                    case BY_NAN :
                        v[0] = C2F(returnanan)();
                        goto fin;

                    case BY_ZERO :
                        v[0] = 0.0;
                        goto fin;

                    case NATURAL :
                        if (xx < x[j][0])
                        {
                            k[j] = 0;
                        }
                        else
                        {
                            k[j] = dim[j] - 2;
                        }
                        break;

                    case C0 :
                        if (xx < x[j][0])
                        {
                            u[j] = 0.0;
                            k[j] = 0;
                        }
                        else
                        {
                            u[j] = 1.0;
                            k[j] = dim[j] - 2;
                        }
                        continue;

                    case PERIODIC :
                        coord_by_periodicity(&xx, x[j], dim[j], &(k[j]));
                        break;

                }
            u[j] = (xx - x[j][k[j]]) / ( x[j][k[j] + 1] -  x[j][k[j]]); /* coord bary */
        }

        /* 2 - calcul de l'indice de base */
        b = k[n - 1];
        for ( j = n - 2; j >= 0; j-- )
        {
            b = k[j] + dim[j] * b;
        }

        /* 3 - mise des valeurs de l'hypercube dans v */
        for ( j = 0; j < two_p_n; j++ )
        {
            v[j] = val[b + ad[j]];
        }

        /* 4 - interpolation */
        temp = 1;
        p = two_p_n;
        for ( j = 0; j < n ; j++ )
        {
            for ( l = 0; l < two_p_n; l += 2 * temp)
            {
                v[l] = v[l] * (1.0 - u[j]) + v[l + temp] * u[j];
            }
            p = p / 2;
            temp = 2 * temp;
        }

        /* 5 - on met le resultat a sa place */
fin:
        yp[i] = v[0];

    }
}
