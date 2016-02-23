/*
 *   Copyright Bruno Pinçon, ESIAL-IECN, Inria CORIDA project
 *   <bruno.pincon@iecn.u-nancy.fr>
 *   contributor:  Antonio Manoel Ferreria Frasson, Universidade Federal do
 *                 Espírito Santo, Brazil. <frasson@ele.ufes.br>.
 *
 * PURPOSE: Scilab interfaces routines onto the UMFPACK sparse solver
 * (Tim Davis) and onto the TAUCS snmf choleski solver (Sivan Teledo)
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
/*
 * First here are some utilities for the interface :
 *
 *   1) When a user computes an LU fact with umf_lufact or a Choleski
 *      fact with taucs_chfact he/she get at the scilab level only a
 *      pointer on to the factorization : the memory for this factorization
 *      is outside scilab memory. In order to avoid problem with scilab
 *      pointer coming from other usage I manage in this interface a
 *      linked list of all the valid pointers onto "umfpack numerical
 *      factorization handle" and the same for the Choleski factorizations.
 *      These 2 lists are called
 *
 *         ListNumeric
 *         ListCholFactor
 *
 *      And so you will find here 3 routines to deal with :
 *
 *         AddAdrToList, RetrieveAdrFromList, IsAdrInList
 *
 *   2) some others  utility routines are used :
 *
 *         sci_sparse_to_ccs_sparse: scilab sparse format -> CCS format (for umfpack)
 *
 *         spd_sci_sparse_to_taucs_sparse: transform a scilab sparse (supposed to be spd)
 *                                         in the format wait by taucs
 *
 *         TransposeMatrix: used only to solve x = b/A  (umfpack(b,"/",A)
 *
 *         UmfErrorMes : to deal with some few messages from umfpack
 *
 *         test_size_for_sparse : to see if stk have enough places to
 *                                store a sparse (see comments)
 *         test_size_for_mat : the same for classic matrix
 */

#include <math.h>
#include "sci_malloc.h"
#include "sciumfpack.h"
#include "taucs_scilab.h"
#include "common_umfpack.h"
#include "localization.h"

int AddAdrToList(Adr adr, int it_flag, CellAdr **L)
{
    CellAdr *NewCell;
    if ( (NewCell = MALLOC(sizeof(CellAdr))) == NULL )
    {
        return 0;
    }
    else
    {
        NewCell->adr = adr;
        NewCell->it = it_flag;
        NewCell->next = *L;
        *L = NewCell;
        return 1;
    }
}

int RetrieveAdrFromList(Adr adr, CellAdr **L, int *it_flag)
{
    /* teste si l'adresse adr est presente ds la liste L, si oui
       on la retire et la fonction renvoie 1, sinon 0  */
    CellAdr * Cell;

    if ( *L == NULL )
    {
        return 0;
    }
    else if ( (*L)->adr == adr )
    {
        Cell = *L;
        *it_flag = Cell->it;
        *L = (*L)->next;
        FREE(Cell);
        return 1;
    }
    else
    {
        return ( RetrieveAdrFromList(adr, &((*L)->next), it_flag));
    }
}

int IsAdrInList(Adr adr, CellAdr *L, int *it_flag)
{
    /* teste si l'adresse adr est presente ds la liste L, si oui
       la fonction renvoie 1, sinon 0. On renvoit aussi it */

    if ( L == NULL )
    {
        return 0;
    }
    else if ( L->adr == adr )
    {
        *it_flag = L->it;
        return 1;
    }
    else
    {
        return ( IsAdrInList(adr, L->next, it_flag) );
    }
}


void TransposeMatrix(double A[], int ma, int na, double At[])
{
    /*  compute At the (na,ma) matrix gotten by the
     *  transposition of the (ma, na) matrix A.  A and
     *  At are in fact simple 1-d arrays with the fortran
     *  storage convention :
     *     A(i,j) = A[i + ma*j] , At(i,j) = At[i + na*j]
     *
     *     At(i,j) = A(j,i) = A[j + ma*i]
     */
    int i, j;
    for ( j = 0 ; j < ma ; j++ )
        for ( i = 0 ; i < na ; i++ )
        {
            At[i + na * j] = A[j + ma * i];
        }
}

int SciSparseToCcsSparse(SciSparse *A, CcsSparse *B)
{
    int one = 1, nel = A->nel, m = A->m, n = A->n, it = A->it;
    int k, kb, i, j, count;

    B->m = m;
    B->n = n;
    B->nel = nel;
    B->it = it;
    B->R = (double*)MALLOC(nel * (it + 1) * sizeof(double));
    if ( it == 1 )
    {
        B->I = B->R + nel;
    }
    else
    {
        B->I = NULL;
    }
    B->p = (int*)MALLOC((n + 1) * sizeof(int));
    B->irow = (int*)MALLOC(nel * sizeof(int));

    for ( i = 0 ; i <= n ; i++ )
    {
        B->p[i] = 0;
    }

    for ( k = 0 ; k < nel ; k++ )
    {
        B->p[A->icol[k]]++;    /* this is because  A->icol[k] is 1-based (and not 0-based) */
    }

    for ( i = 2 ; i <= n ; i++ )
    {
        B->p[i] += B->p[i - 1];
    }

    k = 0;
    for ( i = 0 ; i < m ; i++ )
        for ( count = 0 ; count < A->mnel[i] ; count++ )
        {
            j = A->icol[k] - 1;
            kb = B->p[j];  /* "pointeur" actuel sur la colonne j */
            B->irow[kb] = i;
            B->R[kb] = A->R[k];
            if (it == 1)
            {
                B->I[kb] = A->I[k];
            }
            B->p[j]++;
            k++;
        }

    for ( i = n - 1 ; i > 0 ; i-- )
    {
        B->p[i] = B->p[i - 1];
    }
    B->p[0] = 0;

    return 1;
}

void freeCcsSparse(CcsSparse _Sp)
{
    FREE(_Sp.R);
    FREE(_Sp.p);
    FREE(_Sp.irow);
}

char *UmfErrorMes(int num_error)
{
    /* to deal with various umfpack error indicator */
    char *mes1 = _("singular matrix");
    char *mes2 = _("not enough memory");
    char *mes3 = _("internal error");
    char *mes4 = _("invalid matrix");
    /*  normallly with the different controls in the interface
     *  the others errors may not occurred but we put anyway
     *  this last one :
     */
    char *mes5 = "unidentified error";

    switch (num_error)
    {
        case UMFPACK_WARNING_singular_matrix:
            return (mes1);
        case UMFPACK_ERROR_out_of_memory:
            return (mes2);
        case UMFPACK_ERROR_internal_error:
            return (mes3);
        case UMFPACK_ERROR_invalid_matrix:
            return (mes4);
        default:
            return (mes5);
    };
}

int is_sparse_upper_triangular(SciSparse *A)
{
    int i, k = 0, nb_elem_row_i;
    for ( i = 0 ; i < A->m ; i++ )
    {
        nb_elem_row_i = A->mnel[i];
        if (nb_elem_row_i > 0  &&  A->icol[k] <= i)
        {
            return 0;
        }
        k += nb_elem_row_i;
    }
    return 1;
}

int spd_sci_sparse_to_taucs_sparse(SciSparse *A, taucs_ccs_matrix *B)
{
    /*
     *  this function create a taucs sparse symmetric matrix (with only the lower
     *  triangle part) from a supposed symmetric spd scilab sparse one's.
     *  This is to put a sci sparse in the format wait by taucs routines
     *
     *  The scilab sparse may be only upper triangular
     */
    int one = 1, B_nel, n = A->n;
    int i, k, l, p, nnz;

    B->values = NULL;
    B->colptr = NULL;
    B->rowind = NULL;

    if ( A->m != A->n  ||  A->m <= 0  ||  A->it == 1 )
    {
        return ( MAT_IS_NOT_SPD );
    }

    if ( is_sparse_upper_triangular(A) )
    {
        B_nel = A->nel;
    }
    else
    {
        B_nel = n + (A->nel - n) / 2;
    }

    /* form the taucs sparse matrix struct */
    B->m = n;
    B->n = n;
    B->flags =  TAUCS_SYMMETRIC | TAUCS_LOWER;

    B->values = (double*)MALLOC(B_nel * sizeof(double));
    B->colptr = (int*)MALLOC((n + 1) * sizeof(int));
    B->rowind = (int*)MALLOC(B_nel * sizeof(int));

    nnz = 0;
    k = 0;
    for ( i = 0 ; i < n ; i++ )
    {
        if ( A->mnel[i] > 0 )
        {
            l = 0;
            while ( l < A->mnel[i]  &&  A->icol[k + l] < i + 1 ) /* go to the diagonal element */
            {
                l++;
            }
            if ( l >= A->mnel[i] )          /* no element A_ij with j >= i => A_ii = 0  */
            {
                return ( MAT_IS_NOT_SPD );
            }
            else if ( A->icol[k + l] > i + 1 ) /* A_ii = 0 */
            {
                return ( MAT_IS_NOT_SPD );
            }
            else if ( A->R[k + l] <= 0 )    /* A_ii <= 0 */
            {
                return ( MAT_IS_NOT_SPD );
            }
            else                            /* normal case A_ii > 0 */
            {
                if ( nnz + A->mnel[i] - l > B_nel )
                {
                    return ( MAT_IS_NOT_SPD );
                }
                B->colptr[i] = nnz;
                for ( p = l ; p < A->mnel[i] ; p++)
                {
                    B->values[nnz] = A->R[k + p];
                    B->rowind[nnz] = A->icol[k + p] - 1;
                    nnz++;
                }
                k = k + A->mnel[i];
            }
        }
        else
        {
            return ( MAT_IS_NOT_SPD );
        }
    }
    if ( nnz != B_nel )
    {
        return ( MAT_IS_NOT_SPD );
    }

    B->colptr[n] = nnz;

    return ( A_PRIORI_OK );
}

void freeTaucsSparse(taucs_ccs_matrix _Sp)
{
    free(_Sp.values);
    free(_Sp.colptr);
    free(_Sp.rowind);
}

void residu_with_prec(SciSparse *A, double x[], double b[], double r[], double *rn)
{
    /*  un essai de calcul de residu : r = Ax - b, en precision double etendue */
    int i, j, k, l;
    long double temp, norm2;

    norm2 = 0.0;
    k = 0;
    for ( i = 0 ; i < A->m ; i++ )
    {
        temp = 0.0;
        for ( l = 0 ; l < A->mnel[i] ; l++ )
        {
            j = A->icol[k] - 1;
            temp += (long double) A->R[k]  *  (long double) x[j];
            k++;
        }
        temp -=  (long double) b[i];
        r[i] = (double) temp;
        norm2 += temp * temp;
    }
    *rn = (double) sqrt((double)norm2);
    return;
}

void residu_with_prec_for_chol(SciSparse *A, double x[], double b[], double r[],
                               double *rn, int A_is_upper_triangular, long double wk[])
{
    /*  the same than the previous routine but this one take care of the fact that
     *  when A_is_upper_triangular=1 only the upper triangle part of A is stored */
    int i, j, k, l;
    long double norm2 = 0.0;

    if ( ! A_is_upper_triangular )
    {
        residu_with_prec(A, x, b, r, rn);
    }
    else
    {
        /* A*x-b but only the upper triangle of A is stored */
        for ( i = 0 ; i < A->m ; i++ )
        {
            wk[i] = -(long double) b[i];
        }
        k = 0;
        for ( i = 0 ; i < A->m ; i++ )
        {
            for ( l = 0 ; l < A->mnel[i] ; l++ )
            {
                j = A->icol[k] - 1;
                wk[i] += (long double) A->R[k]  *  (long double) x[j];
                if ( j != i )
                {
                    wk[j] += (long double) A->R[k]  *  (long double) x[i];
                }
                k++;
            }
        }
        for ( i = 0 ; i < A->m ; i++ )
        {
            r[i] = (double) wk[i];
            norm2 += wk[i] * wk[i];
        }
        *rn = (double) sqrt((double)norm2);
    }
    return;
}


void cmplx_residu_with_prec(SciSparse *A,
                            double xr[], double xi[],
                            double br[], double bi[],
                            double rr[], double ri[],
                            double *rn)
{
    /*  the same for complex system */
    int i, j, k, l;
    long double tempr, tempi, norm2;

    norm2 = 0.0;
    k = 0;
    for ( i = 0 ; i < A->m ; i++ )
    {
        tempr = 0.0;
        tempi = 0.0;
        for ( l = 0 ; l < A->mnel[i] ; l++ )
        {
            j = A->icol[k] - 1;
            tempr +=   (long double) A->R[k]  *  (long double) xr[j]
                       - (long double) A->I[k]  *  (long double) xi[j];
            tempi +=   (long double) A->I[k]  *  (long double) xr[j]
                       + (long double) A->R[k]  *  (long double) xi[j];
            k++;
        }
        tempr -=  (long double) br[i];
        tempi -=  (long double) bi[i];
        rr[i] = (double) tempr;
        ri[i] = (double) tempi;
        norm2 += tempr * tempr + tempi * tempi;
    }
    *rn = (double) sqrt((double)norm2);
    return;
}
