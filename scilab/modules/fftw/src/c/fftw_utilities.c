/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2007 - INRIA - Alan LAYEC
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2012 - INRIA - Serge STEER
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
#include <math.h>
#include "fftw_utilities.h"
#include "sci_malloc.h"
#include "callfftw.h"
int check_1D_symmetry(double *Ar, double *Ai, int nA, int iA);
int check_2D_symmetry(double *Ar, double *Ai, int mA, int iA, int nA, int jA);
int check_ND_symmetry(double *Ar, double *Ai, int ndims, int *dims, int *incr);

void complete_1D_array(double *Ar, double *Ai, int nA, int iA);
void complete_2D_array(double *Ar, double *Ai, int mA, int iA, int nA, int jA);
int complete_ND_array(double *Ar, double *Ai, int ndims, int *dims, int *incr);
void dct_scale_1D_array(double *Ar, double *Ai, int nA, int iA, int isn, double fact);
void dct_scale_2D_array(double *Ar, double *Ai, int mA, int iA, int nA, int jA, int isn, double fact);
int dct_scale_ND_array(double *Ar, double *Ai, int ndims, int *dims, int *incr, int isn, double fact);

void dst_scale_1D_array(double *Ar, double *Ai, int nA, int iA, int isn, double fact);
void dst_scale_2D_array(double *Ar, double *Ai, int mA, int iA, int nA, int jA, int isn, double fact);
int dst_scale_ND_array(double *Ar, double *Ai, int ndims, int *dims, int *incr, int isn, double fact);

/*--------------------------------------------------------------------------*/
/* definition of structures to store parameters
 * of FFTW planners - set here default value -
 */
FFTW_Plan_struct Sci_Forward_Plan =
{
    0,                  /* int plan_type            */
    NULL,               /* fftw_plan p              */
    {0, NULL, 0, NULL}, /* guru_dim_struct gdim     */
    FFTW_ESTIMATE,      /* unsigned flags           */
    NULL                /* kind                     */
};

FFTW_Plan_struct Sci_Backward_Plan =
{
    0,                  /* int plan_type            */
    NULL,               /* fftw_plan p              */
    {0, NULL, 0, NULL}, /* guru_dim_struct gdim     */
    FFTW_ESTIMATE,      /* unsigned flags           */
    NULL                /* kind                     */
};
/*--------------------------------------------------------------------------*/
FFTW_Plan_struct *getSci_Backward_Plan(void)
{
    return &Sci_Backward_Plan;
}
/*--------------------------------------------------------------------------*/
FFTW_Plan_struct *getSci_Forward_Plan(void)
{
    return &Sci_Forward_Plan;
}
/*--------------------------------------------------------------------------*/
unsigned cur_fftw_flags = FFTW_ESTIMATE;
/*--------------------------------------------------------------------------*/
unsigned int getCurrentFftwFlags(void)
{
    return cur_fftw_flags;
}
/*--------------------------------------------------------------------------*/
void setCurrentFftwFlags(unsigned int newFftwFlags)
{
    cur_fftw_flags = newFftwFlags;
}
/*--------------------------------------------------------------------------*/
/* Free a FFTW_Plan_struct
 *
 * Input : FFTW_Plan_struct *Sci_Plan
 *
 * Output : int, return 1.
 *
 */
int FreeFFTWPlan(FFTW_Plan_struct *Sci_Plan)
{
    if (Sci_Plan->p != NULL)
    {
        call_fftw_destroy_plan(Sci_Plan->p);
        Sci_Plan->p = NULL;
    }
    if (Sci_Plan->gdim.rank != 0)
    {
        Sci_Plan->gdim.rank = 0;
        FREE(Sci_Plan->gdim.dims);
        Sci_Plan->gdim.dims = NULL;
        FREE(Sci_Plan->kind);
        Sci_Plan->kind = NULL;
    }
    if (Sci_Plan->gdim.howmany_rank != 0)
    {
        Sci_Plan->gdim.howmany_rank = 0;
        FREE(Sci_Plan->gdim.howmany_dims);
        Sci_Plan->gdim.howmany_dims = NULL;
    }

    return (1);
}
/*--------------------------------------------------------------------------*/
/* Return a valid plan ptr.
 * This function search in the Sci_xx_Plan structures if
 * the given input parameters follows an already stored
 * set of parameters.
 * If found then return an already stored plan ptr.
 * If not found, then returns
 * a new set of parameters (and a new plan)
 * with fftw_plan_guru_split_dft
 * and store it in Sci_xx_Plan structures
 *
 * Input : guru_dim_struct *gdim
 *         double *ri, double *ii
 *         double *ro, double *io
 *         unsigned flags, int isn
 *
 * Output : fftw_plan
 *
 *
 */
fftw_plan GetFFTWPlan(enum Plan_Type type, guru_dim_struct *gdim,
                      double *ri, double *ii,
                      double *ro, double *io,
                      unsigned flags, int isn, fftw_r2r_kind *kind, int *errflag)
{
    FFTW_Plan_struct *Sci_Plan;
    int i = 0;

    *errflag = 0;

    if (isn == -1)
    {
        Sci_Plan = &Sci_Backward_Plan;
    }
    else
    {
        Sci_Plan = &Sci_Forward_Plan;
    }

    /* plan must be changed */
    FreeFFTWPlan(Sci_Plan);

    Sci_Plan->plan_type = type;
    if (gdim->rank != 0)
    {
        Sci_Plan->gdim.rank = gdim->rank;
        if ((Sci_Plan->gdim.dims = (fftw_iodim *) MALLOC(sizeof(fftw_iodim) * (gdim->rank))) == NULL)
        {
            *errflag = 1;
            return (NULL);
        }
        for (i = 0; i < gdim->rank; i++)
        {
            Sci_Plan->gdim.dims[i].n  = gdim->dims[i].n;
            Sci_Plan->gdim.dims[i].is = gdim->dims[i].is;
            Sci_Plan->gdim.dims[i].os = gdim->dims[i].os;
        }

        if (kind != NULL)
        {
            if ((Sci_Plan->kind = (fftw_r2r_kind *) MALLOC(sizeof(fftw_r2r_kind) * (gdim->rank))) == NULL)
            {
                FREE(Sci_Plan->gdim.dims);
                *errflag = 1;
                return (NULL);
            }
            for (i = 0; i < gdim->rank; i++)
            {
                Sci_Plan->kind[i]  = kind[i];
            }
        }
    }

    if (gdim->howmany_rank != 0)
    {
        Sci_Plan->gdim.howmany_rank = gdim->howmany_rank;
        if ((Sci_Plan->gdim.howmany_dims = (fftw_iodim *) MALLOC(sizeof(fftw_iodim) * (gdim->howmany_rank))) == NULL)
        {
            FREE(Sci_Plan->gdim.dims);
            *errflag = 1;
            return (NULL);
        }
        for (i = 0; i < gdim->howmany_rank; i++)
        {
            Sci_Plan->gdim.howmany_dims[i].n  = gdim->howmany_dims[i].n;
            Sci_Plan->gdim.howmany_dims[i].is = gdim->howmany_dims[i].is;
            Sci_Plan->gdim.howmany_dims[i].os = gdim->howmany_dims[i].os;
        }
    }

    Sci_Plan->flags = cur_fftw_flags;

    switch (type)
    {
        case C2C_PLAN:
            Sci_Plan->p = call_fftw_plan_guru_split_dft(Sci_Plan->gdim.rank,
                          Sci_Plan->gdim.dims,
                          Sci_Plan->gdim.howmany_rank,
                          Sci_Plan->gdim.howmany_dims,
                          ri, ii, ro, io,
                          Sci_Plan->flags);
            break;
        case C2R_PLAN:
            Sci_Plan->p = call_fftw_plan_guru_split_dft_c2r(Sci_Plan->gdim.rank,
                          Sci_Plan->gdim.dims,
                          Sci_Plan->gdim.howmany_rank,
                          Sci_Plan->gdim.howmany_dims,
                          ri, ii, ro, flags);
            break;
        case R2C_PLAN:
            Sci_Plan->p = call_fftw_plan_guru_split_dft_r2c(Sci_Plan->gdim.rank,
                          Sci_Plan->gdim.dims,
                          Sci_Plan->gdim.howmany_rank,
                          Sci_Plan->gdim.howmany_dims,
                          ri, ro, io, flags);

            break;
        case R2R_PLAN:
            Sci_Plan->p = call_fftw_plan_guru_split_dft_r2r(Sci_Plan->gdim.rank,
                          Sci_Plan->gdim.dims,
                          Sci_Plan->gdim.howmany_rank,
                          Sci_Plan->gdim.howmany_dims,
                          ri, ro, kind, flags);
            break;
    }

    if (Sci_Plan->p == NULL)
    {
        *errflag = 2;
    }

    return (Sci_Plan->p);
}
/*--------------------------------------------------------------------------*/
/* Check if two guru_dim structures are equal
 *
 * Input : guru_dim_struct *gdim1
 *         guru_dim_struct *gdim2
 *
 * Output : int, return 0 if False, else 1.
 *
 */
int CheckGuruDims(guru_dim_struct *gdim1, guru_dim_struct *gdim2)
{
    int i;

    if ( (gdim1->rank == gdim2->rank) &&
            (gdim1->howmany_rank == gdim2->howmany_rank))
    {
        for (i = 0; i < gdim1->rank; i++)
        {
            if (gdim1->dims[i].n  != gdim2->dims[i].n)
            {
                return (0);
            }
            if (gdim1->dims[i].is != gdim2->dims[i].is)
            {
                return (0);
            }
            if (gdim1->dims[i].os != gdim2->dims[i].os)
            {
                return (0);
            }
        }
        for (i = 0; i < gdim1->howmany_rank; i++)
        {
            if (gdim1->howmany_dims[i].n  != gdim2->howmany_dims[i].n)
            {
                return (0);
            }
            if (gdim1->howmany_dims[i].is != gdim2->howmany_dims[i].is)
            {
                return (0);
            }
            if (gdim1->howmany_dims[i].os != gdim2->howmany_dims[i].os)
            {
                return (0);
            }
        }
        return (1);
    }
    else
    {
        return (0);
    }
}
/*--------------------------------------------------------------------------*/
/* Check if kind array  are equal
 *
 * Input : fftw_r2r_kind *kind1
 *         gfftw_r2r_kind *kind2
 *         int rank
 *
 * Output : int, return 0 if False, else 1.
 *
 */
int CheckKindArray(fftw_r2r_kind *kind1, fftw_r2r_kind *kind2, int rank)
{
    int i;
    if ((kind1 == NULL) && (kind2 == NULL))
    {
        return (1);
    }
    if ((kind1 == NULL) || (kind2 == NULL))
    {
        return (0);
    }
    for (i = 0; i < rank; i++)
    {
        if (kind1[i]  != kind2[i])
        {
            return (0);
        }
    }
    return (1);
}
/*--------------------------------------------------------------------------*/
/* call different fftw_execute_split_dft_xxx procedures according to type input
 *
 * Input :  Plan_Type type
 *          fftw_plan p
 *          double *ri
 *          double *ii
 * Output : double *ro
 *          double *io
 */
void ExecuteFFTWPlan(enum Plan_Type type, const fftw_plan p, double *ri, double *ii, double *ro, double *io)
{
    switch (type)
    {
        case C2C_PLAN:
            call_fftw_execute_split_dft(p, ri, ii, ro, io);
            break;
        case C2R_PLAN:
            call_fftw_execute_split_dft_c2r(p, ri, ii, ro);
            break;
        case R2C_PLAN:
            call_fftw_execute_split_dft_r2c(p, ri, ro, io);
            break;
        case R2R_PLAN:
            call_fftw_execute_split_dft_r2r(p, ri, ro);
            break;
    }
}
/*--------------------------------------------------------------------------*/
int is_real(double *Ar, double *Ai, int ndims, int *dims)
{
    double zero = 0.0;
    int t = 1;
    int i = 0;
    int lA = 1;


    for (i = 0; i < ndims; i++)
    {
        lA = lA * dims[i];
    }

    /*Check if A is real*/
    if (Ai != NULL)
    {
        for (i = 0; i < lA; i++)
        {
            if (Ai[i] != zero)
            {
                t = 0;
                break;
            }
        }
    }
    return t;
}

/*--------------------------------------------------------------------------
 * Check if a 1D array A  is "symmetric" or hermitian symmetric for fft.
 * A==conj(A([1 $:-1:2]))
 * Ar : pointer on real part array
 * Ai : pointer on imaginary part array or NULL
 * nA : number of elements
 * iA : increment between 2 consecutive element of the array
 */

int check_1D_symmetry(double *Ar, double *Ai, int nA, int iA)
{
    int i = 0;
    int nas2 = (int)(nA / 2);
    double zero = 0.0;
    //Checking real part
    if ( nA % 2 == 0)
    {
        /* A length is even */
        for (i = 1; i < nas2; i++)
        {
            if (Ar[iA * i] != Ar[iA * (nA - i)])
            {
                return 0;
            }
        }
    }
    else
    {
        /* A length is odd */
        for (i = 1; i <= nas2; i++)
        {
            if (Ar[iA * i] != Ar[iA * (nA - i)])
            {
                return 0;
            }
        }
    }
    if (Ai == NULL)
    {
        return 1;
    }
    //Checking imaginary part
    if ( nA % 2 == 0)
    {
        /* A length is even */
        if (Ai[0] != zero || Ai[iA * (nA / 2)] != zero)
        {
            return 0;
        }
        for (i = 1; i < nas2; i++)
        {
            if (Ai[iA * i] != -Ai[iA * (nA - i)])
            {
                return 0;
            }
        }
    }
    else
    {
        /* A length is odd */
        if (Ai[0] != zero)
        {
            return 0;
        }
        for (i = 1; i <= nas2; i++)
        {
            if (Ai[iA * i] != -Ai[iA * (nA - i)])
            {
                return 0;
            }
        }
    }
    return 1;
}
/*--------------------------------------------------------------------------
 * Check if a 2D array A  is "symmetric" or hermitian symmetric for fft.
 * A==conj(A([1 $:-1:2],[1 $:-1:2])
 * Ar : pointer on real part array
 * Ai : pointer on imaginary part array or NULL
 * mA : number of rows
 * nA : number of columns
 * iA : increment between 2 consecutive element of a row
 * jA : increment between 2 consecutive element of a column
 */

int check_2D_symmetry(double *Ar, double *Ai, int mA, int iA, int nA, int jA)
{
    int l1 = 0;
    int l2 = 0;
    int k = 0;
    int l = 0;
    int nAs2 = (int)(nA / 2) + 1; /* A VERIFIER */

    /* Check first column */
    if (!check_1D_symmetry(Ar, Ai, mA, iA))
    {
        return 0;
    }
    /* Check first row */
    if (!check_1D_symmetry(Ar, Ai, nA, jA))
    {
        return 0;
    }

    /* Check A(2:$,2:$) block */
    if (Ai == NULL)
    {
        for (k = 1; k < nAs2; k++)
        {
            l1 = jA * k + iA ;
            l2 = jA * (nA - k) + iA * (mA - 1);
            for (l = 1; l < mA; l++)
            {
                if (Ar[l1] != Ar[l2])
                {
                    return 0;
                }
                l1 += iA;
                l2 -= iA;
            }
        }
    }
    else
    {
        for (k = 1; k < nAs2; k++)
        {
            l1 = jA * k + iA ;
            l2 = jA * (nA - k) + iA * (mA - 1);
            for (l = 1; l < mA; l++)
            {
                if ((Ar[l1] != Ar[l2]) || (Ai[l1] != -Ai[l2]))
                {
                    return 0;
                }
                l1 += iA;
                l2 -= iA;
            }
        }
    }
    return 1;
}

/*--------------------------------------------------------------------------
 * Check if a N-D array A  is "symmetric" or hermitian symmetric for fft
 * A==conj(A([1 $:-1:2],...,[1 $:-1:2])
 * Ar : pointer on real part array
 * Ai : pointer on imaginary part array or NULL
 * mA : number of rows
 * nA : number of columns
 * iA : increment between 2 consecutive element of a row
 * jA : increment between 2 consecutive element of a column
 */

int check_ND_symmetry(double *Ar, double *Ai, int ndims, int *dims, int *incr)
{
    int i = 0, j = 0, l = 0;
    int r = 0;
    int l1 = 0;/* current 1-D index in array*/
    int l2 = 0;/* associated symmetric value 1-D index */
    int *temp = NULL;
    int *dims1 = NULL;
    int *incr1 = NULL;
    int nSub = 0, nSubs2 = 0;
    int k = 0, step = 0;

    if (ndims == 2)
    {
        r = check_2D_symmetry(Ar, Ai, dims[0], incr[0], dims[1], incr[1]);
        return r;
    }
    else if  (ndims == 1)
    {
        r = check_1D_symmetry(Ar, Ai, dims[0], incr[0]);
        return r;
    }

    if  ((temp =  (int *)MALLOC(sizeof(int) * (2 * ndims))) == NULL)
    {
        return -1;
    }
    dims1 = temp;
    incr1 = temp + ndims;

    for (i = 0; i < ndims; i++)
    {
        /* remove current dimension and increment out of  dims ans incr */
        l = 0;
        for   (j = 0; j < ndims; j++)
        {
            if (j != i)
            {
                dims1[l] = dims[j];
                incr1[l] = incr[j];
                l++;
            }
        }
        r = check_ND_symmetry(Ar, Ai, ndims - 1, dims1, incr1);
        if (r != 1)
        {
            dims1 = NULL;
            incr1 = NULL;
            FREE(temp);
            return r;
        }
    }

    /* check bloc A(2:$,....,2:$)*/
    /*A(2,...,2) index*/
    l1 = 0;
    for (i = 0; i < ndims; i++)
    {
        l1 += incr[i];
    }
    /*A($,...,$) index*/
    l2 = 0;
    for (i = 0; i < ndims; i++)
    {
        l2 += (dims[i] - 1) * incr[i];
    }


    /* cumprod(size(A(2:$,....,2:$)))*/
    incr1[0] = dims[0] - 1;
    for (i = 1; i < (ndims - 1); i++)
    {
        incr1[i] = incr1[i - 1] * (dims[i] - 1) ;
    }
    /* steps*/
    dims1[0] = (dims[0] - 2) * incr[0];
    for (i = 1; i < (ndims - 1); i++)
    {
        dims1[i] = dims1[i - 1] + (dims[i] - 2) * incr[i];
    }

    /*  A(2:$,....,2:$) block number of elements*/
    nSub = 1;
    for (i = 0; i < ndims; i++)
    {
        nSub *= (dims[i] - 1);
    }

    nSubs2 = (int)(nSub / 2);


    if (Ai == NULL)
    {
        /* Real case */
        for (i = 0; i < nSubs2; i++)
        {

            if (Ar[l1] != Ar[l2])
            {
                return 0;
            }
            step = incr[0];
            for (j = ndims - 2; j >= 0; j--)
            {
                if ((i + 1) % incr1[j] == 0)
                {
                    step = -dims1[j] + incr[j + 1] ;
                    break;
                }
            }
            l1 += step;
            l2 -= step;
        }
    }
    else    /* Complex case */
    {
        for (i = 0; i < nSubs2; i++)
        {
            if (Ar[l1] != Ar[l2] || Ai[l1] != -Ai[l2])
            {
                return 0;
            }
            step = incr[0];
            for (j = ndims - 2; j >= 0; j--)
            {
                if ((i + 1) % incr1[j] == 0)
                {
                    step = -dims1[j] + incr[j + 1] ;
                    break;
                }
            }
            l1 += step;
            l2 -= step;
        }
    }
    dims1 = NULL;
    incr1 = NULL;
    FREE(temp);
    return 1;
}



int check_array_symmetry(double *Ar, double *Ai, guru_dim_struct gdim)
{
    int ndims = gdim.rank;
    int * dims = NULL;
    int * incr = NULL;
    int r = -1;
    int i = 0, j = 0, k = 0;

    if (gdim.howmany_rank == 0)
    {
        switch (gdim.rank)
        {
            case 1:
                return check_1D_symmetry(Ar, Ai, gdim.dims[0].n, gdim.dims[0].is);
            case 2:
                return check_2D_symmetry(Ar, Ai, gdim.dims[0].n, gdim.dims[0].is, gdim.dims[1].n, gdim.dims[1].is);
            default: /*general N-D case*/
                if ((dims = (int *)MALLOC(sizeof(int) * gdim.rank)) == NULL)
                {
                    return -1;
                }
                if ((incr = (int *)MALLOC(sizeof(int) * gdim.rank)) == NULL)
                {
                    FREE(dims);
                    return -1;
                }
                for (i = 0; i < ndims; i++)
                {
                    dims[i] = gdim.dims[i].n;
                    incr[i] = gdim.dims[i].is;
                }
                r = check_ND_symmetry(Ar, Ai, ndims, dims, incr);
                FREE(dims);
                FREE(incr);
                return r;
        }
    }
    else
    {
        int m = 0;
        int p = 1;
        int *dims1 = NULL;
        int *incr1 = NULL;
        int ir = 0;

        if ((dims1 = (int *)MALLOC(sizeof(int) * gdim.howmany_rank)) == NULL)
        {
            return -1;
        }
        dims1[0] = gdim.howmany_dims[0].n;
        for (i = 1; i < gdim.howmany_rank; i++)
        {
            dims1[i] = dims1[i - 1] * gdim.howmany_dims[i].n;
        }
        m = dims1[gdim.howmany_rank - 1];

        if ((incr1 = (int *)MALLOC(sizeof(int) * gdim.howmany_rank)) == NULL)
        {
            FREE(dims1);
            return -1;
        }
        p = 1;
        for (i = 0; i < gdim.howmany_rank; i++)
        {
            p += (gdim.howmany_dims[i].n - 1) * gdim.howmany_dims[i].is;
            incr1[i] = p;
        }
        switch (gdim.rank)
        {
            case 1:
                if (Ai == NULL)
                {
                    /* multiple 1D fft */
                    for (ir = 0; ir < gdim.howmany_rank; ir++)
                    {
                        j = 0;
                        for (i = 1; i <= m; i++)
                        {
                            if ((r = check_1D_symmetry(Ar + j, NULL, gdim.dims[0].n, gdim.dims[0].is)) != 1 )
                            {
                                FREE(dims1);
                                FREE(incr1);
                                return r;
                            }
                            j += gdim.howmany_dims[0].is;
                            for (k = gdim.howmany_rank - 2; k >= 0; k--)
                            {
                                if (i % dims1[k] == 0)
                                {
                                    j += -incr1[k] + gdim.howmany_dims[k + 1].is;
                                    break;
                                }
                            }
                        }
                    }
                }
                else
                {
                    for (ir = 0; ir < gdim.howmany_rank; ir++)
                    {
                        j = 0;
                        for (i = 1; i <= m; i++)
                        {
                            if ((r = check_1D_symmetry(Ar + j, Ai + j, gdim.dims[0].n, gdim.dims[0].is)) != 1 )
                            {
                                FREE(dims1);
                                FREE(incr1);
                                return r;
                            }
                            j += gdim.howmany_dims[0].is;
                            for (k = gdim.howmany_rank - 2; k >= 0; k--)
                            {
                                if (i % dims1[k] == 0)
                                {
                                    j += -incr1[k] + gdim.howmany_dims[k + 1].is;
                                    break;
                                }
                            }
                        }
                    }
                }
                FREE(dims1);
                FREE(incr1);
                return 1;
            case 2:  /* multiple 2D fft */
                if (Ai == NULL)
                {
                    for (ir = 0; ir < gdim.howmany_rank; ir++)
                    {
                        j = 0;
                        for (i = 1; i <= m; i++)
                        {
                            if ((r = check_2D_symmetry(Ar + j, NULL, gdim.dims[0].n, gdim.dims[0].is,
                                                       gdim.dims[1].n, gdim.dims[1].is)) != 1 )
                            {
                                FREE(dims1);
                                FREE(incr1);
                                return r;
                            }
                            j += gdim.howmany_dims[0].is;

                            for (k = gdim.howmany_rank - 2; k >= 0; k--)
                            {
                                if (i % dims1[k] == 0)
                                {
                                    j += -incr1[k] + gdim.howmany_dims[k + 1].is;
                                    break;
                                }
                            }
                        }
                    }
                }
                else
                {
                    for (ir = 0; ir < gdim.howmany_rank; ir++)
                    {
                        j = 0;
                        for (i = 1; i <= m; i++)
                        {
                            if ((r = check_2D_symmetry(Ar + j, Ai + j, gdim.dims[0].n, gdim.dims[0].is,
                                                       gdim.dims[1].n, gdim.dims[1].is)) != 1 )
                            {
                                FREE(dims1);
                                FREE(incr1);
                                return r;
                            }
                            j += gdim.howmany_dims[0].is;
                            for (k = gdim.howmany_rank - 2; k >= 0; k--)
                            {
                                if (i % dims1[k] == 0)
                                {
                                    j += -incr1[k] + gdim.howmany_dims[k + 1].is;
                                    break;
                                }
                            }
                        }
                    }
                }
                FREE(dims1);
                FREE(incr1);
                return 1;
            default: /*general N-D case*/
                if ((dims = (int *)MALLOC(sizeof(int) * gdim.rank)) == NULL)
                {
                    FREE(dims1);
                    FREE(incr1);
                    return -1;
                }
                if ((incr = (int *)MALLOC(sizeof(int) * gdim.rank)) == NULL)
                {
                    FREE(dims);
                    FREE(dims1);
                    FREE(incr1);
                    return -1;
                }
                for (i = 0; i < ndims; i++)
                {
                    dims[i] = gdim.dims[i].n;
                    incr[i] = gdim.dims[i].is;
                }
                for (ir = 0; ir < gdim.howmany_rank; ir++)
                {
                    j = 0;
                    for (i = 1; i <= m; i++)
                    {
                        if (Ai == NULL)
                        {
                            r = check_ND_symmetry(Ar + j, NULL, ndims, dims, incr);
                        }
                        else
                        {
                            r = check_ND_symmetry(Ar + j, Ai + j, ndims, dims, incr);
                        }
                        if (r <= 0)
                        {
                            FREE(dims);
                            FREE(dims1);
                            FREE(incr);
                            FREE(incr1);
                            return r;
                        }
                        j += gdim.howmany_dims[0].is;
                        for (k = gdim.howmany_rank - 2; k >= 0; k--)
                        {
                            if (i % dims1[k] == 0)
                            {
                                j += -incr1[k] + gdim.howmany_dims[k + 1].is;
                                break;
                            }
                        }
                    }
                }
                FREE(dims);
                FREE(dims1);
                FREE(incr);
                FREE(incr1);
                return 1;
        }
    }
    return 1;
}
/*--------------------------------------------------------------------------
 * "symmetrizing" a vector A of length nA modifying the second half part of the vector
 * nA even: A=[a0 A1 conj(A1($:-1:1))]
 * nA odd : A=[a0 A1 am conj(A1($:-1:1))]
 */

void complete_1D_array(double *Ar, double *Ai, int nA, int iA)
{

    if (nA > 2)
    {
        int nAs2 = (int)(nA / 2);
        int n = (nA % 2 == 0) ? nAs2 - 1 : nAs2;
        int l1 = iA; /* ignore first element */
        int l2 = (nA - 1) * iA;
        int i = 0;
        if (Ai == NULL)
        {
            for (i = 0; i < n; i++)
            {
                Ar[l2] = Ar[l1];
                l1 += iA;
                l2 -= iA;
            }
        }
        else
        {
            for (i = 0; i < n; i++)
            {
                Ar[l2] = Ar[l1];
                Ai[l2] = -Ai[l1];
                l1 += iA;
                l2 -= iA;
            }
        }
    }
}

/*--------------------------------------------------------------------------
 * "symmetrizing" a mA x nA array modifying the second half part of the columns
 * nA even: A=[a11 A12 conj(A12($:-1:1))
 *             A21 A22 conj(A22($:-1:1,$:-1:1))]
 *
 * nA odd : A=[a11 A12 am  conj(A12($:-1:1))
 A21 A22 A2m conj(A22($:-1:1,$:-1:1))]]
*/

void complete_2D_array(double *Ar, double *Ai, int mA, int iA, int nA, int jA)
{
    if (nA > 2)
    {
        int n = (nA % 2 == 0) ? (int)(nA / 2) - 1 : (int)(nA / 2);
        int i = 0, j = 0; /* loop variables */
        int l1 = jA + iA; /* the index of the first element of the A22 block A(2,2)*/
        int l2 = (mA - 1) * iA + (nA - 1) * jA; /* the index of the last element of the A22 block A(mA,nA)*/
        int step = 0;
        /* first column  */
        /*could not be useful because fftw only skip half of the rightmost dimension but it may be not exactly symmetric */

        complete_1D_array(Ar, Ai, mA, iA);

        /* first row */
        complete_1D_array(Ar, Ai, nA, jA);

        /* A22 block */
        if (Ai == NULL)
        {
            for (j = 0; j < n; j++)
            {
                for (i = 1; i < mA; i++)
                {
                    Ar[l2] = Ar[l1];
                    l1 += iA;
                    l2 -= iA;
                }
                step = -(mA - 1) * iA + jA;
                l1 += step;
                l2 -= step;
            }
        }
        else
        {
            for (j = 0; j < n; j++)
            {
                for (i = 1; i < mA; i++)
                {
                    Ar[l2] = Ar[l1];
                    Ai[l2] = -Ai[l1];
                    l1 += iA;
                    l2 -= iA;
                }
                step = -(mA - 1) * iA + jA;
                l1 += step;
                l2 -= step;
            }
        }
    }
}

int complete_ND_array(double *Ar, double *Ai, int ndims, int *dims, int *incr)
{
    int i = 0, j = 0, l = 0;
    int r = 0;
    int l1 = 0;/* current 1-D index in array*/
    int l2 = 0;/* associated symmetric value 1-D index */

    int *temp = NULL;
    int *dims1 = NULL;
    int *incr1 = NULL;
    int nSub = 0, nSubs2 = 0, step = 0, k = 0;

    if (ndims == 2)
    {
        complete_2D_array(Ar, Ai, dims[0], incr[0], dims[1], incr[1]);
        return 0;
    }
    else if (ndims == 1)
    {
        complete_1D_array(Ar, Ai, dims[0], incr[0]);
        return 0;
    }
    if  ((temp =  (int *)MALLOC(sizeof(int) * (2 * ndims))) == NULL)
    {
        return -1;
    }
    dims1 = temp;
    incr1 = temp + ndims;

    for (i = 0; i < ndims; i++)
    {
        /* remove current dimension and increment out of  dims ans incr */
        l = 0;
        for   (j = 0; j < ndims; j++)
        {
            if (j != i)
            {
                dims1[l] = dims[j];
                incr1[l] = incr[j];
                l++;
            }
        }
        r = complete_ND_array(Ar, Ai, ndims - 1, dims1, incr1);
        if (r < 0)
        {
            dims1 = NULL;
            incr1 = NULL;
            FREE(temp);
            return r;
        }
    }

    /* check bloc A(2:$,....,2:$)*/
    l1 = 0;
    for (i = 0; i < ndims; i++)
    {
        l1 += incr[i];
    }
    /*A($,...,$) index*/
    l2 = 0;
    for (i = 0; i < ndims; i++)
    {
        l2 += (dims[i] - 1) * incr[i];
    }


    /* cumprod(size(A(2:$,....,2:$)))*/
    incr1[0] = dims[0] - 1;
    for (i = 1; i < (ndims - 1); i++)
    {
        incr1[i] = incr1[i - 1] * (dims[i] - 1) ;
    }
    /* steps*/
    dims1[0] = (dims[0] - 2) * incr[0];
    for (i = 1; i < (ndims - 1); i++)
    {
        dims1[i] = dims1[i - 1] + (dims[i] - 2) * incr[i];
    }

    /*  A(2:$,....,2:$) block number of elements*/
    nSub = 1;
    for (i = 0; i < ndims; i++)
    {
        nSub *= (dims[i] - 1);
    }

    nSubs2 = (int)(nSub / 2);

    if (Ai == 0)
    {
        /* Real case */
        for (i = 0; i < nSubs2; i++)
        {
            Ar[l2] = Ar[l1];
            step = incr[0];
            for (j = ndims - 2; j >= 0; j--)
            {
                if ((i + 1) % incr1[j] == 0)
                {
                    step = -dims1[j] + incr[j + 1] ;
                    break;
                }
            }
            l1 += step;
            l2 -= step;
        }
    }
    else    /* Complex case */
    {
        for (i = 0; i < nSubs2; i++)
        {
            Ar[l2] = Ar[l1];
            Ai[l2] = -Ai[l1];
            step = incr[0];
            for (j = ndims - 2; j >= 0; j--)
            {
                if ((i + 1) % incr1[j] == 0)
                {
                    step = -dims1[j] + incr[j + 1] ;
                    break;
                }
            }
            l1 += step;
            l2 -= step;
        }
    }
    dims1 = NULL;
    incr1 = NULL;
    FREE(temp);
    return 1;
}

int complete_array(double *Ar, double *Ai, guru_dim_struct gdim)
{
    int ndims = gdim.rank;
    int * dims = NULL;
    int * incr = NULL;
    int r = -1;
    int i = 0, j = 0, k = 0;
    if (gdim.howmany_rank == 0)
    {
        switch (gdim.rank)
        {
            case 1:
                complete_1D_array(Ar, Ai, gdim.dims[0].n, gdim.dims[0].is);
                return 0;
            case 2:
                complete_2D_array(Ar, Ai, gdim.dims[0].n, gdim.dims[0].is, gdim.dims[1].n, gdim.dims[1].is);
                return 0;
            default: /*general N-D case*/
                if ((dims = (int *)MALLOC(sizeof(int) * gdim.rank)) == NULL)
                {
                    return -1;
                }
                if ((incr = (int *)MALLOC(sizeof(int) * gdim.rank)) == NULL)
                {
                    FREE(dims);
                    return -1;
                }
                for (i = 0; i < ndims; i++)
                {
                    dims[i] = gdim.dims[i].n;
                    incr[i] = gdim.dims[i].is;
                }
                r = complete_ND_array(Ar, Ai, ndims, dims, incr);
                FREE(dims);
                FREE(incr);
                return r;
        }
    }
    else
    {
        int m = 0;
        int *dims1 = NULL;
        int *incr1 = NULL;
        int hrank = gdim.howmany_rank;

        if ((dims1 = (int *)MALLOC(sizeof(int) * hrank)) == NULL)
        {
            return -1;
        }
        dims1[0] = gdim.howmany_dims[0].n;
        for (i = 1; i < hrank; i++)
        {
            dims1[i] = dims1[i - 1] * gdim.howmany_dims[i].n;
        }
        m = dims1[gdim.howmany_rank - 1];

        if ((incr1 = (int *)MALLOC(sizeof(int) * hrank)) == NULL)
        {
            FREE(dims1);
            return -1;
        }
        incr1[0] = gdim.howmany_dims[0].n * gdim.howmany_dims[0].is;
        for (i = 1; i < hrank; i++)
        {
            incr1[i] = incr1[i - 1] + (gdim.howmany_dims[i].n - 1) * gdim.howmany_dims[i].is;;
        }
        switch (gdim.rank)
        {
            case 1: /* multiple 1D fft */
                if (Ai == NULL)
                {
                    j = 0;
                    for (i = 1; i <= m; i++)
                    {
                        complete_1D_array(Ar + j, NULL, gdim.dims[0].n, gdim.dims[0].is);
                        j += gdim.howmany_dims[0].is;
                        for (k = hrank - 2; k >= 0; k--)
                        {
                            if (i % dims1[k] == 0)
                            {
                                j += -incr1[k] + gdim.howmany_dims[k + 1].is;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    j = 0;
                    for (i = 1; i <= m; i++)
                    {
                        complete_1D_array(Ar + j, Ai + j, gdim.dims[0].n, gdim.dims[0].is);
                        j += gdim.howmany_dims[0].is;
                        for (k = hrank - 2; k >= 0; k--)
                        {
                            if (i % dims1[k] == 0)
                            {
                                j += -incr1[k] + gdim.howmany_dims[k + 1].is;
                                break;
                            }
                        }
                    }
                }
                FREE(dims1);
                FREE(incr1);
                return 0;
            case 2: /* multiple 2D fft */
                if (Ai == NULL)
                {
                    j = 0;
                    for (i = 1; i <= m; i++)
                    {
                        complete_2D_array(Ar + j, NULL, gdim.dims[0].n, gdim.dims[0].is, gdim.dims[1].n, gdim.dims[1].is);
                        j += gdim.howmany_dims[0].is;
                        for (k = hrank - 2; k >= 0; k--)
                        {
                            if (i % dims1[k] == 0)
                            {
                                j += -incr1[k] + gdim.howmany_dims[k + 1].is;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    j = 0;
                    for (i = 1; i <= m; i++)
                    {
                        complete_2D_array(Ar + j, Ai + j, gdim.dims[0].n, gdim.dims[0].is, gdim.dims[1].n, gdim.dims[1].is);

                        j += gdim.howmany_dims[0].is;
                        for (k = hrank - 2; k >= 0; k--)
                        {
                            if (i % dims1[k] == 0)
                            {
                                j += -incr1[k] + gdim.howmany_dims[k + 1].is;
                                break;
                            }
                        }
                    }
                }
                FREE(dims1);
                FREE(incr1);
                return 0;
            default:  /* multiple ND fft */
                if ((dims = (int *)MALLOC(sizeof(int) * gdim.rank)) == NULL)
                {
                    FREE(dims1);
                    FREE(incr1);
                    return -1;
                }
                if ((incr = (int *)MALLOC(sizeof(int) * gdim.rank)) == NULL)
                {
                    FREE(dims);
                    FREE(dims1);
                    FREE(incr1);
                    return -1;
                }
                for (i = 0; i < ndims; i++)
                {
                    dims[i] = gdim.dims[i].n;
                    incr[i] = gdim.dims[i].is;
                }
                j = 0;
                for (i = 1; i <= m; i++)
                {
                    if (Ai == NULL)
                    {
                        r = complete_ND_array(Ar + j, NULL, ndims, dims, incr);
                    }
                    else
                    {
                        r = complete_ND_array(Ar + j, Ai + j, ndims, dims, incr);
                    }
                    if (r < 0)
                    {
                        FREE(dims);
                        FREE(dims1);
                        FREE(incr);
                        FREE(incr1);
                        return r;
                    }
                    j += gdim.howmany_dims[0].is;
                    for (k = hrank - 2; k >= 0; k--)
                    {
                        if (i % dims1[k] == 0)
                        {
                            j += -incr1[k] + gdim.howmany_dims[k + 1].is;
                            break;
                        }
                    }
                }
                FREE(dims);
                FREE(dims1);
                FREE(incr);
                FREE(incr1);
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------
 * Check if Scilab is linked with MKL library * Some fftw functions
 * are not yet implemented in MKL in particular wisdom; guru_split real case
 * functions and  guru_split complex with homany_rank>1
 */

int withMKL(void)
{
    static int iWithMKL = -1;
    if (iWithMKL == -1)
    {
        char* str = NULL;
        iWithMKL = 1;
        str = call_fftw_export_wisdom_to_string();
        if (str)
        {
            iWithMKL = 0;
            // According to the FFTW documentation we should free str
            // string but doing makes Scilab crash!?
            //free(str);
        }
    }

    return iWithMKL;
}/*--------------------------------------------------------------------------*/




void dct_scale_1D_array(double *Ar, double *Ai, int nA, int iA, int isn, double fact)
{
    /* fact: multiplication factor for all terms but the first one*/
    double s, s0;
    int i = 0;

    if (isn == -1)
    {
        s0 = fact * 0.5 / sqrt(nA);
    }
    else
    {
        s0 = fact / sqrt(nA); /* 2.0*sqrt(nA)/(2*nA) */
    }
    s = fact / sqrt(2.0 * nA); /* sqrt(2.0*nA)/(2*nA) */
    if (Ai == NULL)
    {
        Ar[0] *= s0;
        for (i = 1; i < nA; i++)
        {
            Ar[i * iA] *= s;
        }
    }
    else
    {
        Ar[0] *= s0;
        Ai[0] *= s0;
        for (i = 1; i < nA; i++)
        {
            Ar[i * iA] *= s;
            Ai[i * iA] *= s;
        }

    }
}



void dct_scale_2D_array(double *Ar, double *Ai, int mA, int iA, int nA, int jA, int isn, double fact)
{
    int j = 0; /* loop variables */
    double s, s0;
    s = fact / sqrt(2 * nA);
    s0 = fact / sqrt(nA);
    if (isn == -1)
    {
        s0 *= 0.5;
    }

    /* first column  */
    dct_scale_1D_array(Ar, Ai, mA, iA, isn, s0);
    /* other columns */
    if (Ai == NULL)
    {
        for (j = 1; j < nA; j++)
        {
            dct_scale_1D_array(&Ar[j * jA], NULL, mA, iA, isn, s);
        }
    }
    else
    {
        for (j = 1; j < nA; j++)
        {
            dct_scale_1D_array(&Ar[j * jA], &Ai[j * jA], mA, iA, isn, s);
        }
    }
}

int dct_scale_ND_array(double *Ar, double *Ai, int ndims, int *dims, int *incr, int isn, double fact)
{
    int i = 0;
    double s = 1.0, s0 = 1.0;

    if (ndims == 2)
    {
        dct_scale_2D_array(Ar, Ai, dims[0], incr[0], dims[1], incr[1], isn, fact);
    }
    else if (ndims == 1)
    {
        dct_scale_1D_array(Ar, Ai, dims[0], incr[0], isn, fact);
    }
    else
    {
        /* Decompose recursively along the first array dimension
           A_scaled(i,:,...,:)=s1(i)*scale(A(i,:,...,:))
           with
           s1(1) = 1/(2*sqrt(n1) and  s1(i>1) = 1/(sqrt(2*n1)
        */
        s = fact / sqrt(2.0 * dims[0]);
        s0 = fact / sqrt(dims[0]);
        if (isn == -1)
        {
            s0 *= 0.5;
        }

        if (Ai == NULL)
        {
            /* first index: s1(1)*/
            dct_scale_ND_array(Ar, Ai, ndims - 1, dims + 1, incr + 1, isn, s0);
            /* next indexes: s1(i>1)*/
            for (i = 1; i < dims[0]; i++)
            {
                dct_scale_ND_array(&Ar[i * incr[0]], NULL, ndims - 1, dims + 1, incr + 1, isn, s);
            }
        }
        else
        {
            dct_scale_ND_array(Ar, Ai, ndims - 1, dims + 1, incr + 1, isn, s0);

            for (i = 1; i < dims[0]; i++)
            {
                dct_scale_ND_array(&Ar[i * incr[0]], &Ai[i * incr[0]], ndims - 1, dims + 1, incr + 1, isn, s);
            }
        }
    }
    return 0;

}


int dct_scale_array(double *Ar, double *Ai, guru_dim_struct gdim, int isn)
{
    int * dims = NULL;
    int * incr = NULL;
    int *dims1 = NULL;
    int *incr1 = NULL;

    int i = 0, j = 0, k = 0;
    if (gdim.howmany_rank == 0)
    {
        switch (gdim.rank)
        {
            case 1:
                dct_scale_1D_array(Ar, Ai, gdim.dims[0].n, gdim.dims[0].is, isn, (double)1.0);
                return 0;
            case 2:
                dct_scale_2D_array(Ar, Ai, gdim.dims[0].n, gdim.dims[0].is, gdim.dims[1].n, gdim.dims[1].is, isn, (double)1.0);
                return 0;
            default: /*general N-D case*/
                if ((dims = (int *)MALLOC(sizeof(int) * gdim.rank)) == NULL)
                {
                    goto ERR;
                }
                if ((incr = (int *)MALLOC(sizeof(int) * gdim.rank)) == NULL)
                {
                    goto ERR;
                }
                for (i = 0; i < gdim.rank; i++)
                {
                    dims[i] = gdim.dims[i].n;
                    incr[i] = gdim.dims[i].is;
                }
                dct_scale_ND_array(Ar, Ai, gdim.rank, dims, incr, isn, (double)1.0);
        }
    }
    else
    {
        int m = 0;
        int hrank = gdim.howmany_rank;
        if ((dims1 = (int *)MALLOC(sizeof(int) * hrank)) == NULL)
        {
            goto ERR;
        }
        dims1[0] = gdim.howmany_dims[0].n;
        for (i = 1; i < hrank; i++)
        {
            dims1[i] = dims1[i - 1] * gdim.howmany_dims[i].n;
        }
        m = dims1[gdim.howmany_rank - 1];

        if ((incr1 = (int *)MALLOC(sizeof(int) * hrank)) == NULL)
        {
            goto ERR;
        }

        incr1[0] = gdim.howmany_dims[0].n * gdim.howmany_dims[0].is;
        for (i = 1; i < hrank; i++)
        {
            incr1[i] = incr1[i - 1] + (gdim.howmany_dims[i].n - 1) * gdim.howmany_dims[i].is;;
        }
        switch (gdim.rank)
        {
            case 1: /* multiple 1D dct */
                if (Ai == NULL)
                {
                    j = 0;
                    for (i = 1; i <= m; i++)
                    {
                        dct_scale_1D_array(Ar + j, NULL, gdim.dims[0].n, gdim.dims[0].is, isn, (double)1.0);
                        j += gdim.howmany_dims[0].is;
                        for (k = hrank - 2; k >= 0; k--)
                        {
                            if (i % dims1[k] == 0)
                            {
                                j += -incr1[k] + gdim.howmany_dims[k + 1].is;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    j = 0;
                    for (i = 1; i <= m; i++)
                    {
                        dct_scale_1D_array(Ar + j, Ai + j, gdim.dims[0].n, gdim.dims[0].is, isn, (double)1.0);
                        j += gdim.howmany_dims[0].is;
                        for (k = hrank - 2; k >= 0; k--)
                        {
                            if (i % dims1[k] == 0)
                            {
                                j += -incr1[k] + gdim.howmany_dims[k + 1].is;
                                break;
                            }
                        }
                    }
                }
                break;
            case 2: /* multiple 2D dct */
                if (Ai == NULL)
                {
                    j = 0;
                    for (i = 1; i <= m; i++)
                    {
                        dct_scale_2D_array(&Ar[j], NULL, gdim.dims[0].n, gdim.dims[0].is, gdim.dims[1].n, gdim.dims[1].is, isn, (double)1.0);
                        j += gdim.howmany_dims[0].is;
                        for (k = hrank - 2; k >= 0; k--)
                        {
                            if (i % dims1[k] == 0)
                            {
                                j += -incr1[k] + gdim.howmany_dims[k + 1].is;
                                break;
                            }
                        }
                    }
                }

                else
                {
                    j = 0;
                    for (i = 1; i <= m; i++)
                    {
                        dct_scale_2D_array(&Ar[j], &Ai[j], gdim.dims[0].n, gdim.dims[0].is, gdim.dims[1].n, gdim.dims[1].is, isn, (double)1.0);

                        j += gdim.howmany_dims[0].is;
                        for (k = hrank - 2; k >= 0; k--)
                        {
                            if (i % dims1[k] == 0)
                            {
                                j += -incr1[k] + gdim.howmany_dims[k + 1].is;
                                break;
                            }
                        }
                    }
                }
                break;
            default:  /* multiple ND dct */
                if ((dims = (int *)MALLOC(sizeof(int) * gdim.rank)) == NULL)
                {
                    goto ERR;
                }
                if ((incr = (int *)MALLOC(sizeof(int) * gdim.rank)) == NULL)
                {
                    goto ERR;
                }

                for (i = 0; i < gdim.rank; i++)
                {
                    dims[i] = gdim.dims[i].n;
                    incr[i] = gdim.dims[i].is;
                }
                j = 0;
                for (i = 1; i <= m; i++)
                {
                    if (Ai == NULL)
                    {
                        dct_scale_ND_array(Ar + j, NULL, gdim.rank, dims, incr, isn, (double)1.0);
                    }
                    else
                    {
                        dct_scale_ND_array(Ar + j, Ai + j, gdim.rank, dims, incr, isn, (double)1.0);
                    }

                    j += gdim.howmany_dims[0].is;
                    for (k = hrank - 2; k >= 0; k--)
                    {
                        if (i % dims1[k] == 0)
                        {
                            j += -incr1[k] + gdim.howmany_dims[k + 1].is;
                            break;
                        }
                    }
                }
        }

    }
    FREE(dims);
    FREE(incr);
    FREE(dims1);
    FREE(incr1);
    return 0;

ERR:
    FREE(dims);
    FREE(incr);
    FREE(dims1);
    FREE(incr1);
    return -1;
}

void dst_scale_1D_array(double *Ar, double *Ai, int nA, int iA, int isn, double fact)
{
    /* fact: multiplication factor for all terms but the first one*/
    double s = fact / (1.0 + nA);
    int i = 0;

    if (Ai == NULL)
    {
        for (i = 0; i < nA; i++)
        {
            Ar[i * iA] *= s;
        }
    }
    else
    {
        for (i = 0; i < nA; i++)
        {
            Ar[i * iA] *= s;
            Ai[i * iA] *= s;
        }

    }
}


void dst_scale_2D_array(double *Ar, double *Ai, int mA, int iA, int nA, int jA, int isn, double fact)
{
    int j = 0; /* loop variables */
    double s = fact / (1.0 + nA);

    if (Ai == NULL)
    {
        for (j = 0; j < nA; j++)
        {
            dst_scale_1D_array(&Ar[j * jA], NULL, mA, iA, isn, s);
        }
    }
    else
    {
        for (j = 0; j < nA; j++)
        {
            dst_scale_1D_array(&Ar[j * jA], &Ai[j * jA], mA, iA, isn, s);
        }
    }
}

int dst_scale_ND_array(double *Ar, double *Ai, int ndims, int *dims, int *incr, int isn, double fact)
{
    int i = 0;
    double s = 1.0;

    if (ndims == 2)
    {
        dst_scale_2D_array(Ar, Ai, dims[0], incr[0], dims[1], incr[1], isn, fact);
    }
    else if (ndims == 1)
    {
        dst_scale_1D_array(Ar, Ai, dims[0], incr[0], isn, fact);
    }
    else
    {
        /* Decompose recursively along the first array dimension
           A_scaled(i,:,...,:)=s1*scale(A(i,:,...,:))
           with
           s1 = 1/(n+1)
        */

        s = fact / (1.0 + dims[0]);

        if (Ai == NULL)
        {
            /* next indexes: s1(i>1)*/
            for (i = 0; i < dims[0]; i++)
            {
                dst_scale_ND_array(&Ar[i * incr[0]], NULL, ndims - 1, dims + 1, incr + 1, isn, s);
            }
        }
        else
        {
            for (i = 0; i < dims[0]; i++)
            {
                dst_scale_ND_array(&Ar[i * incr[0]], &Ai[i * incr[0]], ndims - 1, dims + 1, incr + 1, isn, s);
            }
        }
    }
    return 0;
}


int dst_scale_array(double *Ar, double *Ai, guru_dim_struct gdim, int isn)
{
    int * dims = NULL;
    int * incr = NULL;
    int *dims1 = NULL;
    int *incr1 = NULL;

    int i = 0, j = 0, k = 0;
    if (gdim.howmany_rank == 0)
    {
        switch (gdim.rank)
        {
            case 1:
                dst_scale_1D_array(Ar, Ai, gdim.dims[0].n, gdim.dims[0].is, isn, (double)1.0);
                return 0;
            case 2:
                dst_scale_2D_array(Ar, Ai, gdim.dims[0].n, gdim.dims[0].is, gdim.dims[1].n, gdim.dims[1].is, isn, (double)1.0);
                return 0;
            default: /*general N-D case*/
                if ((dims = (int *)MALLOC(sizeof(int) * gdim.rank)) == NULL)
                {
                    goto ERR;
                }
                if ((incr = (int *)MALLOC(sizeof(int) * gdim.rank)) == NULL)
                {
                    goto ERR;
                }
                for (i = 0; i < gdim.rank; i++)
                {
                    dims[i] = gdim.dims[i].n;
                    incr[i] = gdim.dims[i].is;
                }
                dst_scale_ND_array(Ar, Ai, gdim.rank, dims, incr, isn, (double)1.0);
        }
    }
    else
    {
        int m = 0;
        int hrank = gdim.howmany_rank;

        if ((dims1 = (int *)MALLOC(sizeof(int) * hrank)) == NULL)
        {
            goto ERR;
        }
        dims1[0] = gdim.howmany_dims[0].n;
        for (i = 1; i < hrank; i++)
        {
            dims1[i] = dims1[i - 1] * gdim.howmany_dims[i].n;
        }
        m = dims1[gdim.howmany_rank - 1];

        if ((incr1 = (int *)MALLOC(sizeof(int) * hrank)) == NULL)
        {
            goto ERR;
        }

        incr1[0] = gdim.howmany_dims[0].n * gdim.howmany_dims[0].is;
        for (i = 1; i < hrank; i++)
        {
            incr1[i] = incr1[i - 1] + (gdim.howmany_dims[i].n - 1) * gdim.howmany_dims[i].is;;
        }

        switch (gdim.rank)
        {
            case 1: /* multiple 1D dst */
                if (Ai == NULL)
                {
                    j = 0;
                    for (i = 1; i <= m; i++)
                    {
                        dst_scale_1D_array(Ar + j, NULL, gdim.dims[0].n, gdim.dims[0].is, isn, (double)1.0);
                        j += gdim.howmany_dims[0].is;
                        for (k = hrank - 2; k >= 0; k--)
                        {
                            if (i % dims1[k] == 0)
                            {
                                j += -incr1[k] + gdim.howmany_dims[k + 1].is;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    j = 0;
                    for (i = 1; i <= m; i++)
                    {
                        dst_scale_1D_array(Ar + j, Ai + j, gdim.dims[0].n, gdim.dims[0].is, isn, (double)1.0);
                        j += gdim.howmany_dims[0].is;
                        for (k = hrank - 2; k >= 0; k--)
                        {
                            if (i % dims1[k] == 0)
                            {
                                j += -incr1[k] + gdim.howmany_dims[k + 1].is;
                                break;
                            }
                        }
                    }
                }
                break;
            case 2: /* multiple 2D dst */
                if (Ai == NULL)
                {
                    j = 0;
                    for (i = 1; i <= m; i++)
                    {
                        dst_scale_2D_array(&Ar[j], NULL, gdim.dims[0].n, gdim.dims[0].is, gdim.dims[1].n, gdim.dims[1].is, isn, (double)1.0);
                        j += gdim.howmany_dims[0].is;
                        for (k = hrank - 2; k >= 0; k--)
                        {
                            if (i % dims1[k] == 0)
                            {
                                j += -incr1[k] + gdim.howmany_dims[k + 1].is;
                                break;
                            }
                        }
                    }
                }

                else
                {
                    j = 0;
                    for (i = 1; i <= m; i++)
                    {
                        dst_scale_2D_array(&Ar[j], &Ai[j], gdim.dims[0].n, gdim.dims[0].is, gdim.dims[1].n, gdim.dims[1].is, isn, (double)1.0);

                        j += gdim.howmany_dims[0].is;
                        for (k = hrank - 2; k >= 0; k--)
                        {
                            if (i % dims1[k] == 0)
                            {
                                j += -incr1[k] + gdim.howmany_dims[k + 1].is;
                                break;
                            }
                        }
                    }
                }
                break;
            default:  /* multiple ND dst */
                if ((dims = (int *)MALLOC(sizeof(int) * gdim.rank)) == NULL)
                {
                    goto ERR;
                }
                if ((incr = (int *)MALLOC(sizeof(int) * gdim.rank)) == NULL)
                {
                    goto ERR;
                }

                for (i = 0; i < gdim.rank; i++)
                {
                    dims[i] = gdim.dims[i].n;
                    incr[i] = gdim.dims[i].is;
                }
                j = 0;
                for (i = 1; i <= m; i++)
                {
                    if (Ai == NULL)
                    {
                        dst_scale_ND_array(Ar + j, NULL, gdim.rank, dims, incr, isn, (double)1.0);
                    }
                    else
                    {
                        dst_scale_ND_array(Ar + j, Ai + j, gdim.rank, dims, incr, isn, (double)1.0);
                    }

                    j += gdim.howmany_dims[0].is;
                    for (k = hrank - 2; k >= 0; k--)
                    {
                        if (i % dims1[k] == 0)
                        {
                            j += -incr1[k] + gdim.howmany_dims[k + 1].is;
                            break;
                        }
                    }
                }
        }

    }
    FREE(dims);
    FREE(incr);
    FREE(dims1);
    FREE(incr1);
    return 0;

ERR:
    FREE(dims);
    FREE(incr);
    FREE(dims1);
    FREE(incr1);
    return -1;
}
