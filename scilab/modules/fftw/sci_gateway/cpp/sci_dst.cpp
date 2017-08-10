/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 - INRIA - Serge STEER
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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

#include "fftw_gw.hxx"
#include "function.hxx"
#include "fftw_common.hxx"
extern "C"
{
#include "localization.h"
#include "charEncoding.h"
#include "Scierror.h"

    extern int WITHMKL;
    extern void C2F(dscal)(int *n, double *da, double *dx, int *incx); /* blas routine */
    extern void C2F(dset)(int *n, double *da, double *dx, int *incx); /* blas routine */
}
/*-----------------------------------------------------------------------------------*/
static int sci_dst_gen(const char *fname, types::Double* A, types::Double** O, int isn, guru_dim_struct gdim, int iopt);
/*-----------------------------------------------------------------------------------*/
types::Function::ReturnValue sci_dst(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    std::wstring name(L"dst");
    return fftw_common(name, in, _iRetCount, out, sci_dst_gen);
}
/*-----------------------------------------------------------------------------------*/
int sci_dst_gen(const char *fname, types::Double* A, types::Double** O, int isn, guru_dim_struct gdim, int iopt)
{
    *O = A->clone()->getAs<types::Double>();
    int ndimsA = (*O)->getDims();
    int *dimsA = (*O)->getDimsArray();
    double *Ar = (*O)->get();
    double *Ai = (*O)->getImg();

    /* Input  array variables */
    int  isrealA = (Ai == NULL), lA = 1;
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
        p = GetFFTWPlan(type, &gdim, Ar, NULL, Ar, NULL, getCurrentFftwFlags(), isn, kind, &errflag);

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

        p = GetFFTWPlan(type, &gdim, Ar, NULL, Ar, NULL, getCurrentFftwFlags(), isn, kind, &errflag);
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
