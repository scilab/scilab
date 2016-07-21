/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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

#include "fftw_common.hxx"
#include "overload.hxx"
#include "int.hxx"
#include "string.hxx"

extern "C"
{
#include "callfftw.h"
#include "localization.h"
#include "charEncoding.h"
#include "Scierror.h"

    extern int WITHMKL;
}
types::Function::ReturnValue fftw_common(std::wstring& name, types::typed_list &in, int _iRetCount, types::typed_list &out, fftw_gen func)
{
    int iRhs = static_cast<int>(in.size());

    wchar_t* option = nullptr;
    int isn = FFTW_FORWARD;
    WITHMKL = withMKL();
    int iopt = 0;


    char* s = wide_string_to_UTF8(name.data());
    std::string cname(s);
    FREE(s);

    int iMaxRhs = 4;
    if (name == L"fftw")
    {
        iMaxRhs = 5;
    }
    /****************************************
    * Basic constraints on rhs arguments  *
    ****************************************/

    /* check min/max lhs/rhs arguments of scilab function */
    if (in.size() < 1 || in.size() > iMaxRhs)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), cname.data(), 1, iMaxRhs);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), cname.data(), 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble() == false)
    {
        return Overload::generateNameAndCall(name, in, _iRetCount, out);
    }

    //check option, last parameter
    if (in.back()->isString())
    {
        types::String* pOption = in.back()->getAs<types::String>();
        if (pOption->isScalar())
        {
            option = pOption->get()[0];
            --iRhs;
        }
        else
        {
            Scierror(999, _("%s: Cannot allocate more memory.\n"), cname.data());
            return types::Function::Error;
        }
    }

    /********************  Checking if isn is given  ************************************************/
    if (iRhs != 1)
    {
        if (in[1]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), cname.data(), 2);
            return types::Function::Error;
        }

        types::Double* pWay = in[1]->getAs<types::Double>();
        if (pWay->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), cname.data(), 2);
            return types::Function::Error;
        }

        isn = static_cast<int>(pWay->get()[0]);
        if (isn != FFTW_FORWARD && isn != FFTW_BACKWARD)
        {
            Scierror(53, _("%s: Wrong value for input argument #%d: %d or %d expected.\n"), cname.data(), 2, FFTW_FORWARD, FFTW_BACKWARD);
            return types::Function::Error;
        }
    }

    if (option)
    {
        if (cname == "dct" || cname == "dst")
        {
            if (isn == FFTW_FORWARD)
            {
                if (option == name + L"1")
                {
                    iopt = 1;
                }
                else if (option == name + L"2")
                {
                    iopt = 2;
                }
                else if (option == name)
                {
                    iopt = 0;
                }
                else if (option == name + L"4")
                {
                    iopt = 4;
                }
                else
                {
                    std::string err;
                    err += "\"" + cname + "\",";
                    err += "\"" + cname + "1\",";
                    err += "\"" + cname + "2\",";
                    err += "\"" + cname + "4\"";
                    Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), cname.data(), in.size(), err.data());
                    return types::Function::Error;
                }
            }
            else
            {
                if (option == name + L"1")
                {
                    iopt = 1;
                }
                else if (option == name + L"3")
                {
                    iopt = 3;
                }
                else if (option == L"i" + name)
                {
                    iopt = 0;
                }
                else if (option == name + L"4")
                {
                    iopt = 4;
                }
                else
                {
                    std::string err;
                    err += "\"i" + cname + "\",";
                    err += "\"" + cname + "1\",";
                    err += "\"" + cname + "3\",";
                    err += "\"" + cname + "4\"";
                    Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), cname.data(), in.size(), err.data());
                    return types::Function::Error;
                }
            }
        }
        else //fftw
        {
            if (option == std::wstring(L"symmetric"))
            {
                iopt = 1;
            }
            else if (option == std::wstring(L"nonsymmetric"))
            {
                iopt = 2;
            }
            else
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), name.data(), iRhs, "\"symmetric\"", "\"nonsymmetric\"");
                return types::Function::Error;
            }
        }
    }

    switch (iRhs)
    {
        case 4:
        {
            /* dct(A ,sign ,dim, incr)*/
            return common_4args(cname, in, _iRetCount, out, func, isn, iopt);
            break;
        }
        case 3:
        {
            /* dct(A ,sign ,sel)*/
            return common_3args(cname, in, _iRetCount, out, func, isn, iopt);
            break;
        }
        default:
        {
            /* dct(A ,sign)*/
            return common_2args(cname, in, _iRetCount, out, func, isn, iopt);
            break;
        }
    }
}

types::Function::ReturnValue common_2args(std::string& name, types::typed_list &in, int _iRetCount, types::typed_list &out, fftw_gen func, int way, int opt)
{
    //getting the array A
    types::Double* A = in[0]->getAs<types::Double>();
    int ndimsA = A->getDims();
    int *dimsA = A->getDimsArray();

    /*FFTW specific library variable */
    guru_dim_struct gdim = {0, NULL, 0, NULL};

    /* local variable */
    int ndims = 0; /* number of non singleton dimensions */
    int first_nonsingleton = -1;
    int i = 0, j = 0;
    int prd = 1;

    /* ignore singleton dimensions */
    first_nonsingleton = -1;
    ndims = 0;
    for (i = 0; i < ndimsA; i++)
    {
        if (dimsA[i] > 1)
        {
            ndims++;
            if (first_nonsingleton < 0)
            {
                first_nonsingleton = i;
            }
        }
    }

    /* void or scalar input gives void output or scalar*/
    if (ndims == 0)
    {
        out.push_back(A);
        return types::Function::OK;
    }

    gdim.rank = ndims;
    if ((gdim.dims = (fftw_iodim *)MALLOC(sizeof(fftw_iodim) * gdim.rank)) == nullptr)
    {
        Scierror(999, _("%s: Cannot allocate more memory.\n"), name.data());
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return types::Function::Error;
    }

    j = 0;
    prd = 1;
    for (i = (first_nonsingleton); i < ndimsA; i++)
    {
        if (dimsA[i] > 1)
        {
            gdim.dims[j].n = dimsA[i];
            gdim.dims[j].is = prd;
            gdim.dims[j].os = prd;
            prd *= dimsA[i];
            j++;
        }
    }
    gdim.howmany_rank = 0;
    gdim.howmany_dims = nullptr;


    //fftw functions work "in place", so we need to clone input data.
    types::Double* D = nullptr;// A->clone()->getAs<types::Double>();
    if (!func(name.data(), A, &D, way, gdim, opt))
    {
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return types::Function::Error;
    }
    FREE(gdim.dims);
    FREE(gdim.howmany_dims);
    out.push_back(D);
    return types::Function::OK;
}

types::Function::ReturnValue common_3args(std::string& name, types::typed_list &in, int _iRetCount, types::typed_list &out, fftw_gen func, int way, int opt)
{
    //getting the array A
    types::Double* A = in[0]->getAs<types::Double>();
    int ndimsA = A->getDims();
    int *dimsA = A->getDimsArray();

    int *Sel = nullptr;
    int rank = 0;

    /*FFTW specific library variable */
    guru_dim_struct gdim = {0, NULL, 0, NULL};
    /* local variable */
    int ndims = 0;
    int first_nonsingleton = -1;
    int ih = 0;
    int pd = 1; /* used to store prod(Dims(1:sel(k-1)))*/
    int pds = 1; /* used to store prod(Dims(sel(k-1):sel(k)))*/
    int i = 0, j = 0;

    /* ignore singleton dimensions */
    first_nonsingleton = -1;
    ndims = 0;
    for (i = 0; i < ndimsA; i++)
    {
        if (dimsA[i] > 1)
        {
            ndims++;
            if (first_nonsingleton < 0)
            {
                first_nonsingleton = i;
            }
        }
    }

    /* void or scalar input gives void output or scalar*/
    if (ndims == 0)
    {
        out.push_back(A);
        return types::Function::OK;
    }

    if (in[2]->isGenericType() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d.\n"), name.data(), 3);
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return types::Function::Error;
    }

    getVarAsDims(in[2], rank, Sel);

    /* size of Sel must be less than ndimsA */
    if (rank <= 0 || rank >= ndimsA)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Must be between %d and %d.\n"), name.data(), 3, 1, ndimsA - 1);
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        FREE(Sel);
        return types::Function::Error;
    }
    /* check values of Sel[i] */
    for (i = 0; i < rank; i++)
    {
        if (Sel[i] <= 0)
        {
            Scierror(999, _("%s: Wrong values for input argument #%d: Positive integers expected.\n"), name.data(), 3);
            FREE(gdim.dims);
            FREE(gdim.howmany_dims);
            FREE(Sel);
            return types::Function::Error;
        }
        if (Sel[i] > ndimsA)
        {
            Scierror(999, _("%s: Wrong values for input argument #%d: Elements must be less than %d.\n"), name.data(), 3, ndimsA);
            FREE(gdim.dims);
            FREE(gdim.howmany_dims);
            FREE(Sel);
            return types::Function::Error;
        }
        if (i > 0 && Sel[i] <= Sel[i - 1])
        {
            Scierror(999, _("%s: Wrong values for input argument #%d: Elements must be in increasing order.\n"), name.data(), 3);
            FREE(gdim.dims);
            FREE(gdim.howmany_dims);
            FREE(Sel);
            return types::Function::Error;
        }
    }

    /* Create  gdim struct */
    gdim.rank = rank;
    if ((gdim.dims = (fftw_iodim *)MALLOC(sizeof(fftw_iodim) * gdim.rank)) == nullptr)
    {
        Scierror(999, _("%s: Cannot allocate more memory.\n"), name.data());
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        FREE(Sel);
        return types::Function::Error;
    }

    pd = 1; /* used to store prod(Dims(1:sel(k-1)))*/
    pds = 1; /* used to store prod(Dims(sel(k-1):sel(k)))*/
    j = 0;
    for (i = 0; i < ndimsA; i++)
    {
        if (j >= gdim.rank)
        {
            break;
        }
        if (Sel[j] == i + 1)
        {
            gdim.dims[j].n = dimsA[i];
            gdim.dims[j].is = pd;
            gdim.dims[j].os = pd;
            j++;
        }
        pd *= dimsA[i];
    }
    /* Compute howmany_rank based on jumps in the Sel sequence */
    gdim.howmany_rank = 0;
    if ((Sel[0] != 1) && (Sel[0] != ndimsA))
    {
        gdim.howmany_rank++;
    }
    for (i = 1; i <= rank - 1; i++)
    {
        if (Sel[i] != Sel[i - 1] + 1)
        {
            /*check if all dimensions between Sel[i-1]+1 and Sel[i]-1 are
            equal to one, in this case they can be ignored and there is
            no jump*/
            for (j = Sel[i - 1] + 1; j <= Sel[i] - 1; j++)
            {
                if (dimsA[j - 1] != 1)
                {
                    gdim.howmany_rank++;
                    break;
                }
            }
        }
    }

    if ((Sel[rank - 1] != ndimsA) || (rank == 1))
    {
        gdim.howmany_rank++;
    }
    /* Fill the howmany_dims struct */
    if (gdim.howmany_rank > 0)
    {
        /* it must be the case */
        if ((gdim.howmany_dims = (fftw_iodim *)MALLOC(gdim.howmany_rank * sizeof(fftw_iodim))) == nullptr)
        {
            Scierror(999, _("%s: Cannot allocate more memory.\n"), name.data());
            FREE(gdim.dims);
            FREE(gdim.howmany_dims);
            FREE(Sel);
            return types::Function::Error;
        }
        pd = 1;
        for (j = 1; j <= (Sel[0] - 1); j++)
        {
            pd *= dimsA[j - 1];    /*prod(Dims(1:(sel(1)-1)))*/
        }
        ih = 0;
        if ((Sel[0] != 1) && (Sel[0] != ndimsA))
        {
            /* First seleted dimension */
            gdim.howmany_dims[ih].is = 1;
            gdim.howmany_dims[ih].os = 1;
            gdim.howmany_dims[ih].n = pd;
            ih++;
        }
        pd *= dimsA[Sel[0] - 1]; /*prod(Dims(1:sel(1)))*/
        for (i = 1; i <= rank - 1; i++)
        {
            /* intermediate selected dimensions */
            if (Sel[i] != Sel[i - 1] + 1)
            {
                pds = 1;
                for (j = (Sel[i - 1] + 1); j <= (Sel[i] - 1); j++)
                {
                    pds *= dimsA[j - 1];    /*prod(Dims(sel(i-1)+1:(sel(i)-1)))*/
                }
                /*check again if all dimensions between Sel[i-1]+1 and
                Sel[i]-1 are equal to one, in this case they can be
                ignored and there is no jump*/
                for (j = (Sel[i - 1] + 1); j <= (Sel[i] - 1); j++)
                {
                    if (dimsA[j - 1] != 1)
                    {
                        gdim.howmany_dims[ih].is = pd;
                        gdim.howmany_dims[ih].os = pd;
                        gdim.howmany_dims[ih].n = pds;
                        ih++;
                        break;
                    }
                }
            }
            pd *= pds * dimsA[Sel[i] - 1]; /*prod(Dims(1:sel(i)))*/
        }

        if (Sel[rank - 1] != ndimsA)
        {
            /* last selected dimension*/
            pds = 1;
            for (j = (Sel[rank - 1] + 1); j <= ndimsA; j++)
            {
                pds *= dimsA[j - 1];    /*prod(Dims(sel(i-1)+1:(sel(i)-1)))*/
            }
            gdim.howmany_dims[ih].is = pd;
            gdim.howmany_dims[ih].os = pd;
            gdim.howmany_dims[ih].n = pds;
            ih++;
        }
        else if (rank == 1)
        {
            /* the only selected dimension is the last one */
            gdim.howmany_dims[ih].is = 1;
            gdim.howmany_dims[ih].os = 1;
            gdim.howmany_dims[ih].n = pd / dimsA[Sel[0] - 1];
            ih++;
        }
    }

    //fftw functions work "in place", so we need to clone input data.
    types::Double* D = nullptr;// A->clone()->getAs<types::Double>();
    if (!func(name.data(), A, &D, way, gdim, opt))
    {
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        FREE(Sel);
        return types::Function::Error;
    }

    FREE(gdim.dims);
    FREE(gdim.howmany_dims);
    FREE(Sel);
    out.push_back(D);
    return types::Function::OK;
}

types::Function::ReturnValue common_4args(std::string& name, types::typed_list &in, int _iRetCount, types::typed_list &out, fftw_gen func, int way, int opt)
{
    //getting the array A
    types::Double* A = in[0]->getAs<types::Double>();
    int ndimsA = A->getDims();
    int *dimsA = A->getDimsArray();

    /* Input  array variables */
    int *Dim1 = nullptr;
    int ndims = 0;
    int *Incr = nullptr;
    int nincr = 0;

    /*FFTW specific library variable */
    guru_dim_struct gdim = {0, NULL, 0, NULL};
    /* input/output address for transform variables */

    /* local variable */
    int *Dim = nullptr, *Sel = nullptr;
    int pd = 1;
    int pds = 1;
    int nd = 0;
    int rank = 0;
    int i = 0, j = 0, k = 0, lA = 1;

    for (i = 0; i < ndimsA; i++)
    {
        lA *= dimsA[i];
    }

    /* void or scalar input gives void output or scalar*/
    if (lA == 0)
    {
        out.push_back(A);
        return types::Function::OK;
    }

    if (in[2]->isGenericType() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d.\n"), name.data(), 3);
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return types::Function::Error;
    }

    getVarAsDims(in[2], ndims, Dim1);
    /* check values of Dim1[i] */
    pd = 1;
    for (i = 0; i < ndims; i++)
    {
        if (Dim1[i] <= 1)
        {
            Scierror(999, _("%s: Wrong values for input argument #%d: Elements must be greater than %d.\n"), name.data(), 3, 1);
            FREE(Dim1);
            FREE(gdim.dims);
            FREE(gdim.howmany_dims);
            return types::Function::Error;
        }
        pd *= Dim1[i];
    }
    if (pd > lA)
    {
        Scierror(999, _("%s: Wrong values for input argument #%d: Must be less than %d.\n"), name.data(), 3, lA);
        FREE(Dim1);
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return types::Function::Error;
    }
    if (lA % pd)
    {
        Scierror(999, _("%s: Wrong values for input argument #%d: Must be a divisor of %d.\n"), name.data(), 3, lA);
        FREE(Dim1);
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return types::Function::Error;
    }

    if (in[3]->isGenericType() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d.\n"), name.data(), 4);
        FREE(Dim1);
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return types::Function::Error;
    }

    getVarAsDims(in[3], nincr, Incr);

    if (nincr != ndims)
    {
        Scierror(999, _("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"), name.data(), 3, 4);
        FREE(Dim1);
        FREE(Incr);
        FREE(Dim);
        FREE(Sel);
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return types::Function::Error;
    }

    /* check values of Incr[i] */
    if (Incr[0] <= 0)
    {
        Scierror(999, _("%s: Wrong values for input argument #%d: Positive integers expected.\n"), name.data(), 4);
        FREE(Dim1);
        FREE(Incr);
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return types::Function::Error;
    }
    for (i = 0; i < ndims; i++)
    {
        if (lA % Incr[i])
        {
            Scierror(999, _("%s: Wrong values for input argument #%d: Elements must be divisors of %d.\n"), name.data(), 3, lA);
            FREE(Dim1);
            FREE(Incr);
            FREE(gdim.dims);
            FREE(gdim.howmany_dims);
            return types::Function::Error;
        }
        if (i > 0 && (Incr[i] <= Incr[i - 1]))
        {
            Scierror(999, _("%s: Wrong values for input argument #%d: Elements must be in increasing ""order.\n"), name.data(), 4);
            FREE(Dim1);
            FREE(Incr);
            FREE(gdim.dims);
            FREE(gdim.howmany_dims);
            return types::Function::Error;
        }
    }
    if ((Dim = (int *)MALLOC((2 * ndims + 1) * sizeof(int))) == nullptr)
    {
        Scierror(999, _("%s: Cannot allocate more memory.\n"), name.data());
        FREE(Dim1);
        FREE(Incr);
        FREE(Dim);
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return types::Function::Error;
    }
    if ((Sel = (int *)MALLOC((ndims) * sizeof(int))) == nullptr)
    {
        Scierror(999, _("%s: Cannot allocate more memory.\n"), name.data());
        FREE(Dim1);
        FREE(Incr);
        FREE(Dim);
        FREE(Sel);
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return types::Function::Error;
    }


    /*Transform  Dim1 and Incr into Dim and Sel and check validity*/

    nd = 0;
    pd = 1;
    if (Incr[0] != 1)
    {
        Dim[nd++] = Incr[0];
        pd *= Incr[0];
    }
    Dim[nd++] = Dim1[0];
    pd *= Dim1[0];
    Sel[0] = nd;

    for (k = 1; k < ndims; k++)
    {
        if (Incr[k] % pd != 0)
        {
            Scierror(999, _("%s: Incompatible input arguments #%d and #%d.\n"), name.data(), 3, 4);
            FREE(Dim1);
            FREE(Incr);
            FREE(Dim);
            FREE(Sel);
            FREE(gdim.dims);
            FREE(gdim.howmany_dims);
            return types::Function::Error;
        }
        if (Incr[k] != pd)
        {
            Dim[nd++] = (int)(Incr[k] / pd);
            pd = Incr[k];
        }
        Dim[nd++] = Dim1[k];
        pd *= Dim1[k];
        Sel[k] = nd;
    }
    if (pd < lA)
    {
        if (lA % pd != 0)
        {
            Scierror(999, _("%s: Incompatible input arguments #%d and #%d.\n"), name.data(), 3, 4);
            FREE(Dim1);
            FREE(Incr);
            FREE(Dim);
            FREE(Sel);
            FREE(gdim.dims);
            FREE(gdim.howmany_dims);
            return types::Function::Error;
        }
        Dim[nd++] = (int)(lA / pd);
    }

    rank = ndims;
    ndims = nd;
    /* now  same algorithm than sci_dct_3args applies */
    /* Create  gdim struct */
    gdim.rank = rank;
    if ((gdim.dims = (fftw_iodim *)MALLOC(sizeof(fftw_iodim) * gdim.rank)) == nullptr)
    {
        Scierror(999, _("%s: Cannot allocate more memory.\n"), name.data());
        FREE(Dim1);
        FREE(Incr);
        FREE(Dim);
        FREE(Sel);
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return types::Function::Error;
    }

    pd = 1; /* used to store prod(Dims(1:sel(k-1)))*/
    pds = 1; /* used to store prod(Dims(sel(k-1):sel(k)))*/
    j = 0;
    for (i = 0; i < ndims; i++)
    {
        if (j >= gdim.rank)
        {
            break;
        }
        if (Sel[j] == i + 1)
        {
            gdim.dims[j].n = Dim[i];
            gdim.dims[j].is = pd;
            gdim.dims[j].os = pd;
            j++;
        }
        pd *= Dim[i];
    }
    /* Compute howmany_rank based on jumps in the Sel sequence */
    gdim.howmany_rank = 0;
    if ((Sel[0] != 1) && (Sel[0] != ndims))
    {
        gdim.howmany_rank++;
    }

    for (i = 1; i <= rank - 1; i++)
    {
        if (Sel[i] != Sel[i - 1] + 1)
        {
            /*check if all dimensions between Sel[i-1]+1 and Sel[i]-1 are
            equal to one, in this case they can be ignored and there is
            no jump*/
            for (j = Sel[i - 1] + 1; j <= Sel[i] - 1; j++)
            {
                if (Dim[j - 1] != 1)
                {
                    gdim.howmany_rank++;
                    break;
                }
            }
        }
    }
    if ((Sel[rank - 1] != ndims) || (rank == 1))
    {
        gdim.howmany_rank++;
    }
    /* Fill the howmany_dims struct */
    if (gdim.howmany_rank > 0)
    {
        /* it must be the case */
        int ih = 0;

        if ((gdim.howmany_dims = (fftw_iodim *)MALLOC(gdim.howmany_rank * sizeof(fftw_iodim))) == nullptr)
        {
            Scierror(999, _("%s: Cannot allocate more memory.\n"), name.data());
            FREE(Dim1);
            FREE(Incr);
            FREE(Dim);
            FREE(Sel);
            FREE(gdim.dims);
            FREE(gdim.howmany_dims);
            return types::Function::Error;
        }
        pd = 1;
        for (j = 1; j <= (Sel[0] - 1); j++)
        {
            pd *= Dim[j - 1];    /*prod(Dims(1:(sel(1)-1)))*/
        }
        ih = 0;
        if ((Sel[0] != 1) && (Sel[0] != ndims))
        {
            /* First seleted dimension */
            gdim.howmany_dims[ih].is = 1;
            gdim.howmany_dims[ih].os = 1;
            gdim.howmany_dims[ih].n = pd;
            ih++;
        }
        pd *= Dim[Sel[0] - 1]; /*prod(Dims(1:sel(1)))*/

        for (i = 1; i <= rank - 1; i++)
        {
            /* intermediate selected dimensions */
            if (Sel[i] != Sel[i - 1] + 1)
            {
                pds = 1;
                for (j = (Sel[i - 1] + 1); j <= (Sel[i] - 1); j++)
                {
                    pds *= Dim[j - 1];    /*prod(Dims(sel(i-1)+1:(sel(i)-1)))*/
                }
                /*check again if all dimensions between Sel[i-1]+1 and
                Sel[i]-1 are equal to one, in this case they can be
                ignored and there is no jump*/
                for (j = (Sel[i - 1] + 1); j <= (Sel[i] - 1); j++)
                {
                    if (Dim[j - 1] != 1)
                    {
                        gdim.howmany_dims[ih].is = pd;
                        gdim.howmany_dims[ih].os = pd;
                        gdim.howmany_dims[ih].n = pds;
                        ih++;
                        break;
                    }
                }
            }

            pd *= pds * Dim[Sel[i] - 1]; /*prod(Dims(1:sel(i)))*/
        }

        if (Sel[rank - 1] != ndims)
        {
            /* last selected dimension*/
            pds = 1;
            for (j = (Sel[rank - 1] + 1); j <= ndims; j++)
            {
                pds *= Dim[j - 1];    /*prod(Dims(sel(i-1)+1:(sel(i)-1)))*/
            }
            gdim.howmany_dims[ih].is = pd;
            gdim.howmany_dims[ih].os = pd;
            gdim.howmany_dims[ih].n = pds;
            ih++;
        }
        else if (rank == 1) /* the only selected dimension is the last one */
        {
            gdim.howmany_dims[ih].is = 1;
            gdim.howmany_dims[ih].os = 1;
            gdim.howmany_dims[ih].n = pd / Dim[Sel[0] - 1];
            ih++;
        }
    }

    //fftw functions work "in place", so we need to clone input data.
    types::Double* D = nullptr;// A->clone()->getAs<types::Double>();
    if (!func(name.data(), A, &D, way, gdim, opt))
    {
        FREE(Dim1);
        FREE(Incr);
        FREE(Dim);
        FREE(Sel);
        FREE(gdim.dims);
        FREE(gdim.howmany_dims);
        return types::Function::Error;
    }

    FREE(Dim1);
    FREE(Incr);
    FREE(Dim);
    FREE(Sel);
    FREE(gdim.dims);
    FREE(gdim.howmany_dims);

    out.push_back(D);
    return types::Function::OK;
}

void getVarAsDims(types::InternalType* t, int& rank, int*& Sel)
{
    switch (t->getType())
    {
        case types::InternalType::ScilabDouble:
        {
            getVarAsDims(t->getAs<types::Double>(), rank, Sel);
            break;
        }
        case types::InternalType::ScilabInt8:
        {
            getVarAsDims(t->getAs<types::Int8>(), rank, Sel);
            break;
        }
        case types::InternalType::ScilabInt16:
        {
            getVarAsDims(t->getAs<types::Int16>(), rank, Sel);
            break;
        }
        case types::InternalType::ScilabInt32:
        {
            getVarAsDims(t->getAs<types::Int32>(), rank, Sel);
            break;
        }
        case types::InternalType::ScilabInt64:
        {
            getVarAsDims(t->getAs<types::Int64>(), rank, Sel);
            break;
        }
        case types::InternalType::ScilabUInt8:
        {
            getVarAsDims(t->getAs<types::UInt8>(), rank, Sel);
            break;
        }
        case types::InternalType::ScilabUInt16:
        {
            getVarAsDims(t->getAs<types::UInt16>(), rank, Sel);
            break;
        }
        case types::InternalType::ScilabUInt32:
        {
            getVarAsDims(t->getAs<types::UInt32>(), rank, Sel);
            break;
        }
        case types::InternalType::ScilabUInt64:
        {
            getVarAsDims(t->getAs<types::UInt64>(), rank, Sel);
            break;
        }
    }
}

template<class T>
void getVarAsDims(T* t, int& dims, int*& pdims)
{
    dims = t->getSize();
    pdims = new int[dims];
    for (int i = 0; i < dims; ++i)
    {
        pdims[i] = static_cast<int>(t->get(i));
    }
}
