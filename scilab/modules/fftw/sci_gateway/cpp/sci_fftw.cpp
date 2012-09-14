/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 - INRIA - Serge STEER
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

#include "fftw_gw.hxx"
#include "function.hxx"
#include "fftw_common.hxx"

extern "C"
{
#include "callfftw.h"
#include "localization.h"
#include "charEncoding.h"
#include "Scierror.h"

    int WITHMKL = 0;
    extern void C2F(dscal)(int *n, double *da, double *dx, int *incx); /* blas routine */
    extern void C2F(dset)(int *n, double *da, double *dx, int *incx); /* blas routine */
}
/*--------------------------------------------------------------------------*/
enum Scaling
{
    Divide = -1,
    None = 0,
    Multiply = 1,
};
/*--------------------------------------------------------------------------*/
static int sci_fft_gen(const char *fname, types::Double* A, types::Double** O, int isn, guru_dim_struct gdim, int iopt);
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_fftw(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    std::wstring name(L"fftw");
    return fftw_common(name, in, _iRetCount, out, sci_fft_gen);
}
/*--------------------------------------------------------------------------*/
int sci_fft_gen(const char *fname, types::Double* A, types::Double** O, int isn, guru_dim_struct gdim, int iopt)
{
    types::Double* tmp = A->clone()->getAs<types::Double>();
    int ndimsA = tmp->getDims();
    int *dimsA = tmp->getDimsArray();
    double *Ar = tmp->get();
    double *Ai = tmp->getImg();

    /* Input  array variables */
    int  isrealA = (Ai == NULL), issymA = 1, lA = 1;
    /*for MKL*/
    int isrealA_save = isrealA;

    /*FFTW specific library variable */
    enum Scaling scale = None;
    enum Plan_Type type;
    fftw_plan p;

    /* input/output address for transform variables */
    double *ri = NULL, *ii = NULL, *ro = NULL, *io = NULL;

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


    if (iopt == 0)
    {
        /* automatically selected algorithm*/
        issymA = check_array_symmetry(Ar, Ai, gdim);
        if (issymA < 0)
        {
            Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
            return 0;
        }
    }
    else if (iopt == 1)
    {
        issymA = 1; /* user forces symmetry */
    }
    else
    {
        issymA = 0;
    }

    if (WITHMKL)
    {
        double dzero = 0.0;
        if (isrealA)
        {
            /*MKL does not implement the r2c nor r2r guru split methods, make A complex */
            if (issymA)
            {
                Ai = (double*)malloc(sizeof(double) * lA);
                C2F(dset)(&lA, &dzero, Ai, &one);
                //do not forget to release memory
                //Ai != null && isrealA == 1

                ///* result will be real, the imaginary part of A can be allocated alone */
                //sciErr = allocMatrixOfDouble(_pvCtx, *getNbInputArgument(_pvCtx) + 1, 1, lA, &Ai);
                //if (sciErr.iErr)
                //{
                //    Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
                //    return 0;
                //}
                //C2F(dset)(&lA, &dzero, Ai, &one);
            }
            else
            {
                Ai = (double*)malloc(sizeof(double) * lA);
                C2F(dset)(&lA, &dzero, Ai, &one);
                ///* result will be complex, realloc A for inplace computation */
                //sciErr = allocComplexArrayOfDouble(_pvCtx, *getNbInputArgument(_pvCtx) + 1, ndimsA, dimsA, &ri, &Ai);
                //if (sciErr.iErr)
                //{
                //    Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
                //    return 0;
                //}
                //C2F(dcopy)(&lA, Ar, &one, ri, &one);
                //Ar = ri;
                //C2F(dset)(&lA, &dzero, Ai, &one);
                //AssignOutputVariable(_pvCtx, 1) = nbInputArgument(_pvCtx) + 1;
                isrealA = 0;
            }
        }
    }

    /* Set pointers on real and imaginary part of the input */
    ri = Ar;
    ii = Ai;

    scale = None; /*no scaling needed */
    if (isn == FFTW_BACKWARD)
    {
        scale = Divide;
    }
    if (isrealA & !WITHMKL) /* To have type = C2C_PLAN*/
    {
        /*A is real */
        if (issymA)
        {
            ///*r2r =  isrealA &&  issymA*/
            ///* there is no general plan able to compute r2r transform so it is tranformed into
            //a R2c plan. The computed imaginary part will be zero*/
            double dzero = 0.0;
            *O = A->clone()->getAs<types::Double>();
            (*O)->setComplex(false);
            ro = (*O)->get();
            io = (double*)malloc(sizeof(double) * lA);
            C2F(dset)(&lA, &dzero, io, &one);
            type = R2C_PLAN;
        }
        else
        {
            /*r2c =  isrealA && ~issymA;*/
            /* transform cannot be done in place */
            *O = new types::Double(ndimsA, dimsA, true);
            ro = (*O)->get();
            io = (*O)->getImg();
            type = R2C_PLAN; /* fftw_plan_guru_split_dft_r2c plans for an FFTW_FORWARD transform*/
            if (isn == FFTW_BACKWARD)
            {
                /*transform problem into a FORWARD fft*/
                /*ifft(A)=conj(fft(A/N)) cas vect*/
                /* pre traitement A must be  divided by N cas vect*/
                /* post treatment result must conjugated */
            }
        }
    }
    else
    {
        *O = A->clone()->getAs<types::Double>();
        /* A is complex */
        if (!WITHMKL && issymA) /*result is real*/
        {
            /*c2r =  ~isrealA &&  issymA*/
            (*O)->setComplex(false);
            ro = (*O)->get();
            io = NULL;

            type = C2R_PLAN; /*fftw_plan_guru_split_dft_c2r plans for an FFTW_BACKWARD transform*/
            if (isn == FFTW_FORWARD)
            {
                /*transform problem into a BACKWARD fft : fft(A)=ifft(conj(A))*/
                double minusone = -1.0;
                C2F(dscal)(&lA, &minusone, ii, &one);
            }
        }
        else
        {
            /*c2c =  ~isrealA && ~issymA;*/
            /* use inplace transform*/
            isrealA = 0;
            type = C2C_PLAN; /*  fftw_plan_guru_split_dft plans for an FFTW_FORWARD transform*/
            if (isn == FFTW_BACKWARD)
            {
                /*transform problem into a FORWARD fft*/
                /* ifft(A) = %i*conj(fft(%i*conj(A)/N) */
                /* reverse input */
                ri = tmp->getImg();
                ii = tmp->get();
                /* reverse output */
                ro = (*O)->getImg();
                io = (*O)->get();
            }
            else
            {
                ro = (*O)->get();
                io = (*O)->getImg();
            }
        }
    }

    /* pre-treatment */
    if (scale != None)
    {
        double ak = 1.0;
        for (i = 0; i < gdim.rank; i++)
        {
            ak = ak * ((double)(gdim.dims[i].n));
        }
        if (scale == Divide)
        {
            ak = 1.0 / ak;
        }
        C2F(dscal)(&lA, &ak, ri, &one);
        if (isrealA == 0)
        {
            C2F(dscal)(&lA, &ak, ii, &one);
        }
    }

    if (!WITHMKL || gdim.howmany_rank <= 1)
    {
        /* Set Plan */
        p = GetFFTWPlan(type, &gdim, ri, ii, ro, io, getCurrentFftwFlags(), isn, (fftw_r2r_kind *)NULL, &errflag);
        if (errflag == 1)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }
        else if (errflag == 2)
        {
            Scierror(999, _("%s: Creation of requested fftw plan failed.\n"), fname);
            return 0;
        }
        /* execute FFTW plan */
        ExecuteFFTWPlan(type, p, ri, ii, ro, io);
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

        p = GetFFTWPlan(type, &gdim, ri, ii, ro, io, getCurrentFftwFlags(), isn, (fftw_r2r_kind *)NULL, &errflag);
        if (errflag == 1)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
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
            Scierror(999, _("%s: No more memory.\n"), fname);
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
            Scierror(999, _("%s: No more memory.\n"), fname);
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
            previous steps of sci_fft_gen*/
            ExecuteFFTWPlan(type, p, &ri[i], &ii[i], &ro[i], &io[i]);
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
    /* Post treatment */
    switch (type)
    {
        case R2R_PLAN:
            if (complete_array(ro, NULL, gdim) == -1)
            {
                Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
                return 0;
            }
            break;
        case C2R_PLAN:
            break;
        case R2C_PLAN:
            if (issymA)
            {
                /*R2C has been used to solve an r2r problem*/
                if (complete_array(ro, NULL, gdim) == -1)
                {
                    Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
                    return 0;
                }
            }
            else
            {
                if (complete_array(ro, io, gdim) == -1)
                {
                    Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
                    return 0;
                }
                if (isn == FFTW_BACKWARD)
                {
                    /*conjugate result */
                    double ak = -1.0;
                    C2F(dscal)(&lA, &ak, io, &one);
                }
            }
            break;
        case C2C_PLAN:
            if (WITHMKL && isrealA_save)
            {
                if (isn == FFTW_FORWARD)
                {
                    if (complete_array(ro, io, gdim) == -1)
                    {
                        Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
                        return 0;
                    }
                }
                else
                {
                    if (complete_array(io, ro, gdim) == -1)
                    {
                        Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
                        return 0;
                    }
                }
            }
            break;
    }

    if (tmp)
    {
        delete tmp;
    }
    return 1;
}
