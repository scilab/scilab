/*  Scicos
*
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
* See the file ./license.txt
*/
#include <stdio.h>
#include <memory.h>
#include "machine.h"
#include "sci_malloc.h"  /* malloc */
#include "tree.h"

extern int C2F(isort)();

void Inver(int* vect, int nb)
{
    int i;
    for (i = 0; i < nb; i++)
    {
        vect[i] = -vect[i];
    }
}


/************************* function tree2 *********************************/
int ctree2(int *vect, int nb, int *depu, int *depuptr, int *outoin, int *outoinptr, int *ord, int *nord, int *ok)

{
    int fini = 0, i, j, k, m, ii;

    *ok = 1;
    for (j = 1; (j <= nb + 2) && (! fini); j++)
    {
        fini = 1;
        for (i = 0; i < nb; i++)
        {
            if (vect[i] == j - 1 )
            {
                if (j == nb + 2)
                {
                    *ok = 0;
                    *nord = 0;
                    return 0;
                }
                if (outoinptr[i + 1] - outoinptr[i] != 0)
                {
                    for (k = outoinptr[i]; k <= outoinptr[i + 1] - 1; k++)
                    {
                        ii = outoin[k - 1];
                        if (vect[ii - 1] > -1 && depu[depuptr[ii - 1] - 2 + outoin[outoinptr[nb] + k - 2]] == 1)
                        {
                            fini = 0;
                            vect[ii - 1] = j;
                        }
                    }
                }
            }
        }
    }
    Inver(vect, nb);
    C2F(isort)(vect, &nb, ord);
    for (m = 0 ; m < nb; m++)
    {
        if (vect[m] < 1)
        {
            if (m == 0)
            {
                *nord = nb;
                return 0;
            }
            else
            {
                *nord = nb - m;
                for (i = 0; i < *nord; i++)
                {
                    ord[i] = ord[i + nb - *nord];
                }
                return 0;
            }
        }
    }
    *nord = 0;
    return 0;
}

/* ===========================================   fin de tree2 ======================================== */
/* ********************************************** function tree3 ***************************************** */
int ctree3(int *vec, int nb, int *depu, int*depuptr, int *typl, int*bexe, int*boptr, int*blnk, int*blptr, int*ord, int*nord, int*ok)
{
    int fini = 0, i, j, m, nkk, kk;

    *ok = 1;
    for ( i = 0; i < nb; i++)
    {
        if ((vec[i] == 0) && (typl[i] == 1))
        {
            vec[i] = 1;
        }
    }
    for (j = 1; (j <= nb + 2) && (! fini); j++)
    {
        fini = 1;
        if (j == nb + 2)
        {
            *ok = 0;
            *nord = 0;
            return 0;
        }

        for (i = 0; i < nb; i++)
        {
            if (vec[i] > -1 && typl[i] != -1)
            {
                if (typl[i] == 1)
                {
                    nkk = boptr[i + 1] - boptr[i];
                    if (nkk != 0)
                    {
                        for (m = 0; m < nkk; m++)
                        {
                            kk = bexe[m + boptr[i] - 1];
                            if (typl[kk - 1] == 1)
                            {
                                if (vec[kk - 1] < vec[i] + 2)
                                {
                                    fini = 0;
                                    vec[kk - 1] = vec[i] + 2;
                                }
                            }
                            else
                            {
                                if (vec[kk - 1] < vec[i] + 1)
                                {
                                    fini = 0;
                                    vec[kk - 1] = vec[i] + 1;
                                }
                            }
                        }
                    }
                }
                else
                {
                    nkk = blptr[i + 1] - blptr[i];
                    if (nkk != 0)
                    {
                        for (m = 0; m < nkk; m++)
                        {
                            kk = blnk[m + blptr[i] - 1];
                            if (vec[kk - 1] > -1 && (depu[depuptr[kk - 1] - 2 + blnk[blptr[nb] + blptr[i] - 2]] == 1 || typl[kk - 1] == 1))
                            {
                                if (vec[kk - 1] < vec[i])
                                {
                                    fini = 0;
                                    vec[kk - 1] = vec[i];
                                }
                            }
                        }
                    }
                }
            }

        }
    }
    Inver(vec, nb);
    C2F(isort)(vec, &nb, ord);
    for (m = 0 ; m < nb; m++)
    {
        if (vec[m] < 1)
        {
            if (m == 0)
            {
                *nord = nb;
                return 0;
            }
            else
            {
                *nord = nb - m;
                for (i = 0; i < *nord; i++)
                {
                    ord[i] = ord[i + nb - *nord];
                }
                return 0;
            }
        }
    }
    *nord = 0;
    return 0;
}
/*===========================================   fin de tree3 ======================================== */
/********************** function tree4 *********************************************/
int ctree4(int* vec, int nb, int* nd, int nnd, int* typ_r, int* outoin,
           int* outoinptr, int* r1, int* r2, int* nr)
{
    int i, j, fini = 0, k, ii, nprt;
    *nr = 0;
    for (j = 1 ; (j < nb) &&  (! fini); j++)
    {
        fini = 1;
        for (i = 0; i < nb; i++)
        {
            if (vec[i] > -1)
            {
                if (outoinptr[i + 1] - outoinptr[i] != 0)
                {
                    for (k = outoinptr[i]; k < outoinptr[i + 1]; k++)
                    {
                        ii = outoin[k - 1];
                        if (typ_r[ii - 1] == 1)
                        {
                            nprt = outoin[k + outoinptr[nb] - 2];
                            if (nd[nprt + (ii - 1)*nnd] == 0)
                            {
                                r1[*nr] = ii;
                                r2[*nr] = nprt;
                                fini = 0;
                                vec[ii - 1] = 0;
                                nd[nprt + (ii - 1)*nnd] = 1;
                                nr[0]++;
                            }
                        }
                    }
                }
            }
        }
        if (fini == 1)
        {
            return 0;
        }
    }
    return 0;

} /* end function*/

/* =================== endfunction tree4 ========================================== */


