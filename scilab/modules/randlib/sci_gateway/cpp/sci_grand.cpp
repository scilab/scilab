/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
* Copyright (C) 2014 - Scilab Enterprises - Sylvain GENIN
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
#include "randlib_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "configvariable.hxx"
#include "int.hxx"
#include "polynom.hxx"
#include "sparse.hxx"
#include "overload.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"
#include "grand.h"
#include "clcg4.h"
#include "others_generators.h"
}
/*--------------------------------------------------------------------------*/

template<class U>
void sci_grand_prm(int iNumIter, U *pIn, types::InternalType** pOut);

types::Function::ReturnValue sci_grand(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    enum
    {
        MT, KISS, CLCG4, CLCG2, URAND
    };

    //  names at the scilab level
    const wchar_t* names_gen[6] = { L"mt", L"kiss", L"clcg4", L"clcg2", L"urand" };

    types::String* pStrMethod = NULL;
    types::String* pStrGenOrPhr = NULL;

    std::vector<types::Double*> vectpDblInput;

    int iStrPos = 0;
    int iPos = 0;
    int meth = -1;
    int iRows = -1;
    int iCols = -1;
    int iNumIter = -1;


    int current_base_gen = ConfigVariable::getCurrentBaseGen();

    // *** check the maximal number of input args. ***
    /* if (in.size() > 6)
    {
    Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "grand", 1, 6);
    return types::Function::Error;
    }*/

    // *** check number of output args. ***
    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "grand", 1);
        return types::Function::Error;
    }

    // *** find the method string. ***
    for (int i = 0; i < in.size(); i++)
    {
        if (in[i]->isString())
        {
            pStrMethod = in[i]->getAs<types::String>();
            iStrPos = i;
            break;
        }
    }

    if (pStrMethod == NULL)
    {
        for (int i = 0; i < in.size(); i++)
        {
            if (in[i]->isDouble() == false)
            {
                std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_grand";
                return Overload::call(wstFuncName, in, _iRetCount, out);
            }
        }

        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "grand", in.size() + 1);
        return types::Function::Error;
    }

    int iDims = iStrPos > 1 ? iStrPos : 2;
    int *itab = new int[iDims];

    //all variables are matrix
    itab[0] = 1;
    itab[1] = 1;

    wchar_t* wcsMeth = pStrMethod->get(0);
    int iNumInputArg = 5;
    if (wcscmp(wcsMeth, L"bet") == 0) // beta
    {
        meth = 1;
    }
    else if (wcscmp(wcsMeth, L"bin") == 0) // binomial
    {
        meth = 2;
    }
    else if (wcscmp(wcsMeth, L"nbn") == 0) // negative binomial
    {
        meth = 3;
    }
    else if (wcscmp(wcsMeth, L"chi") == 0) // chisquare
    {
        iNumInputArg = 4;
        meth = 4;
    }
    else if (wcscmp(wcsMeth, L"nch") == 0) // non central chisquare
    {
        meth = 5;
    }
    else if (wcscmp(wcsMeth, L"exp") == 0) // exponential
    {
        iNumInputArg = 4;
        meth = 6;
    }
    else if (wcscmp(wcsMeth, L"f") == 0) // F variance ratio
    {
        meth = 7;
    }
    else if (wcscmp(wcsMeth, L"nf") == 0) // non central F variance ratio
    {
        iNumInputArg = 6;
        meth = 8;
    }
    else if (wcscmp(wcsMeth, L"gam") == 0) // gamma
    {
        meth = 9;
    }
    else if (wcscmp(wcsMeth, L"nor") == 0) // Gauss Laplace (normal)
    {
        meth = 10;
    }
    else if (wcscmp(wcsMeth, L"mn") == 0) // multivariate gaussian (multivariate normal)
    {
        iNumInputArg = 4;
        meth = 11;
    }
    else if (wcscmp(wcsMeth, L"geom") == 0) // geometric
    {
        iNumInputArg = 4;
        meth = 12;
    }
    else if (wcscmp(wcsMeth, L"markov") == 0) // markov
    {
        iNumInputArg = 4;
        meth = 13;
    }
    else if (wcscmp(wcsMeth, L"mul") == 0) // multinomial
    {
        iNumInputArg = 4;
        meth = 14;
    }
    else if (wcscmp(wcsMeth, L"poi") == 0) // Poisson
    {
        iNumInputArg = 4;
        meth = 15;
    }
    else if (wcscmp(wcsMeth, L"prm") == 0) // random permutations
    {
        iNumInputArg = 3;
        meth = 16;
    }
    else if (wcscmp(wcsMeth, L"def") == 0) // uniform (def)
    {
        iNumInputArg = 3;
        meth = 17;
    }
    else if (wcscmp(wcsMeth, L"unf") == 0) // uniform (unf)
    {
        meth = 18;
    }
    else if (wcscmp(wcsMeth, L"uin") == 0) // uniform (uin)
    {
        meth = 19;
    }
    else if (wcscmp(wcsMeth, L"lgi") == 0) // uniform (lgi)
    {
        iNumInputArg = 3;
        meth = 20;
    }
    else if (wcscmp(wcsMeth, L"getgen") == 0) // getgen
    {
        iNumInputArg = 1;
        meth = 21;
    }
    else if (wcscmp(wcsMeth, L"setgen") == 0) // setgen
    {
        iNumInputArg = 2;
        meth = 22;
    }
    else if (wcscmp(wcsMeth, L"getsd") == 0) // getsd
    {
        iNumInputArg = 1;
        meth = 23;
    }
    else if (wcscmp(wcsMeth, L"setsd") == 0) // setsd
    {
        switch (current_base_gen)
        {
            case MT:
                iNumInputArg = 2;
                break;
            case KISS:
                iNumInputArg = 5;
                break;
            case CLCG2:
                iNumInputArg = 3;
                break;
            case CLCG4:
                iNumInputArg = 5;
                break;
            case URAND:
                iNumInputArg = 2;
                break;
        }
        meth = 24;
    }
    else if (wcscmp(wcsMeth, L"phr2sd") == 0) // phr2sd
    {
        iNumInputArg = 2;
        meth = 25;
    }
    else if (wcscmp(wcsMeth, L"setcgn") == 0) // setcgn
    {
        iNumInputArg = 2;
        meth = 26;
    }
    else if (wcscmp(wcsMeth, L"getcgn") == 0) // getcgn
    {
        iNumInputArg = 1;
        meth = 27;
    }
    else if (wcscmp(wcsMeth, L"initgn") == 0) // initgn
    {
        iNumInputArg = 2;
        meth = 28;
    }
    else if (wcscmp(wcsMeth, L"setall") == 0) // setall
    {
        iNumInputArg = 5;
        meth = 29;
    }
    else if (wcscmp(wcsMeth, L"advnst") == 0) // advnst
    {
        iNumInputArg = 2;
        meth = 30;
    }
    else
    {
        char* pstMeth = wide_string_to_UTF8(wcsMeth);
        Scierror(78, _("%s: Wrong method for input argument #%d: %s unknown.\n"), "grand", iStrPos + 1, pstMeth);
        FREE(pstMeth);
        delete[] itab;
        return types::Function::Error;
    }

    // *** get arguments before methode string. ***
    if (meth == 11 || meth == 13 || meth == 14 || meth == 16) // grand(n, "...", ...
    {
        if (iStrPos != 1)
        {
            Scierror(999, _("%s: Wrong position for input argument #%d : Must be in position %d.\n"), "grand", iStrPos + 1, 2);
            delete[] itab;
            return types::Function::Error;
        }

        if (in[iPos]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), "grand", iPos + 1);
            delete[] itab;
            return types::Function::Error;
        }

        types::Double* pDblTemp = in[iPos]->getAs<types::Double>();

        if (pDblTemp->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "grand", iPos + 1);
            delete[] itab;
            return types::Function::Error;
        }

        iNumIter = (int)pDblTemp->get(0);
        iPos++;
    }
    else if (meth < 21) // grand(m, n, "...", ... || grand(matrix, "...", ...
    {

        /*if (iStrPos != 1 && iStrPos != 2)
        {
        Scierror(999, _("%s: Wrong position for input argument #%d : Must be in position %d or %d.\n"), "grand", iStrPos + 1, 2, 3);
        return types::Function::Error;
        }*/

        std::vector<types::Double*> vectpDblTemp;
        for (int i = 0; i < iStrPos; i++)
        {
            if (in[iPos]->isDouble() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "grand", iPos + 1);
                delete[] itab;
                return types::Function::Error;
            }

            vectpDblTemp.push_back(in[iPos]->getAs<types::Double>());

            if (iStrPos == 3 && vectpDblTemp[i]->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "grand", iPos + 1);
                delete[] itab;
                return types::Function::Error;
            }
            iPos++;
        }

        //get number of dimensions to output
        for (int i = 0; i < iStrPos; i++)
        {
            itab[i] = static_cast<int>(vectpDblTemp[i]->get(0));
        }
    }

    if (in[iPos]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : string expected.\n"), "grand", iPos + 1);
        delete[] itab;
        return types::Function::Error;
    }
    iPos++; // method string has been already got.

    // *** get arguments after methode string. ***
    if (meth == 22 || meth == 25) // setgen || phr2sd
    {
        if (in[iPos]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : string expected.\n"), "grand", iPos + 1);
            delete[] itab;
            return types::Function::Error;
        }

        pStrGenOrPhr = in[iPos]->getAs<types::String>();

        if (pStrGenOrPhr->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d : Only one string expected.\n"), "grand", iPos + 1);
            delete[] itab;
            return types::Function::Error;
        }
    }
    else if (meth == 16)
    {
        if (in.size() > 3)
        {
            Scierror(999, _("Missing vect for random permutation\n"));
            delete[] itab;
            return types::Function::Error;
        }
    }
    else
    {

        for (int i = iPos; i < in.size(); i++)
        {
            if (in[i]->isDouble() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "grand", iPos + 1);
                delete[] itab;
                return types::Function::Error;
            }

            vectpDblInput.push_back(in[i]->getAs<types::Double>());
        }
    }

    // *** perform operation in according method string and return result. ***

    types::Double* pDblOut = NULL;

    if (iStrPos >= 2)
    {
        int iProdiTab = 1;
        for (int i = 0; i < iStrPos; i++)
        {
            iProdiTab = iProdiTab * itab[i];
        }

        if (iProdiTab == 0)
        {
            pDblOut = types::Double::Empty();
        }
        else if ((itab[1] != itab[0]) && (itab[0] <= -1))
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: Positive scalar expected.\n"), "grand", 1);
            delete[] itab;
            return types::Function::Error;
        }
        else if ((itab[1] != itab[0]) && (itab[1] <= -1))
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: Positive scalar expected.\n"), "grand", 2);
            delete[] itab;
            return types::Function::Error;
        }
        else
        {
            pDblOut = new types::Double(iDims, itab);
        }
    }
    else
    {

        types::Double* pDblIn = in[0]->getAs<types::Double>();
        if (meth == 14)//'mul'
        {
            int* iDimsArraytempo = new int[2];
            iDimsArraytempo[0] = in[3]->getAs<types::Double>()->getSize() + 1;
            iDimsArraytempo[1] = iNumIter;
            pDblOut = new types::Double(pDblIn->getDims(), iDimsArraytempo);
        }
        else
        {
            pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray());
        }

    }

    delete[] itab;

    switch (meth)
    {
        case 1: // beta
        {
            double minlog   = 1.e-37;

            for (int i = 0; i < 2; i++)
            {
                if (vectpDblInput[i]->isScalar() == false)
                {
                    delete pDblOut;
                    Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "grand", i + 4);
                    return types::Function::Error;
                }

                if (vectpDblInput[i]->get(0) < minlog)
                {
                    delete pDblOut;
                    Scierror(999, _("%s: Wrong value for input argument #%d : At least %lf expected.\n"), "grand", iPos + 1, minlog);
                    return types::Function::Error;
                }
            }

            for (int i = 0; i < pDblOut->getSize(); i++)
            {
                pDblOut->set(i, C2F(genbet)(vectpDblInput[0]->get(), vectpDblInput[1]->get()));
            }

            out.push_back(pDblOut);
            break;
        }
        case 2: // binomial
        case 3: // negative binomial
        {
            for (int i = 0; i < 2; i++)
            {
                if (vectpDblInput[i]->isScalar() == false)
                {
                    delete pDblOut;
                    Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "grand", i + 4);
                    return types::Function::Error;
                }
            }

            if (vectpDblInput[0]->get(0) < 0.0) // N
            {
                delete pDblOut;
                Scierror(999, _("%s: Wrong value for input argument #%d : Positive integer expected.\n"), "grand", 4);
                return types::Function::Error;
            }

            if (vectpDblInput[1]->get(0) < 0.0 || vectpDblInput[1]->get(0) > 1.0) // p
            {
                delete pDblOut;
                Scierror(999, _("%s: Wrong value for input argument #%d : A value expected.\n"), "grand", 5);
                return types::Function::Error;
            }

            int N = static_cast<int>(vectpDblInput[0]->get(0));
            if (meth == 2)
            {
                for (int i = 0; i < pDblOut->getSize(); i++)
                {
                    pDblOut->set(i, static_cast<double>(C2F(ignbin)(&N, vectpDblInput[1]->get())));
                }
            }
            else // meth == 3
            {
                for (int i = 0; i < pDblOut->getSize(); i++)
                {
                    pDblOut->set(i, static_cast<double>(C2F(ignnbn)(&N, vectpDblInput[1]->get())));
                }
            }

            out.push_back(pDblOut);
            break;
        }
        case 4: // chisquare
        {
            if (vectpDblInput[0]->isScalar() == false)
            {
                delete pDblOut;
                Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "grand", 4);
                return types::Function::Error;
            }

            if (vectpDblInput[0]->get(0) <= 0.0) // Df
            {
                delete pDblOut;
                Scierror(999, _("%s: Wrong value for input argument #%d : Positive no null value expected.\n"), "grand", 4);
                return types::Function::Error;
            }

            for (int i = 0; i < pDblOut->getSize(); i++)
            {
                pDblOut->set(i, C2F(genchi)(vectpDblInput[0]->get()));
            }

            out.push_back(pDblOut);
            break;
        }
        case 5: // non central chisquare
        {
            for (int i = 0; i < 2; i++)
            {
                if (vectpDblInput[i]->isScalar() == false)
                {
                    delete pDblOut;
                    Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "grand", i + 4);
                    return types::Function::Error;
                }
            }

            if (vectpDblInput[0]->get(0) < 1.0) // Df
            {
                delete pDblOut;
                Scierror(999, _("%s: Wrong value for input argument #%d : value > 1.0 expected.\n"), "grand", 4);
                return types::Function::Error;
            }

            if (vectpDblInput[1]->get(0) < 0.0) // Xnon
            {
                delete pDblOut;
                Scierror(999, _("%s: Wrong value for input argument #%d : Positive value expected.\n"), "grand", 5);
                return types::Function::Error;
            }

            for (int i = 0; i < pDblOut->getSize(); i++)
            {
                pDblOut->set(i, C2F(gennch)(vectpDblInput[0]->get(), vectpDblInput[1]->get()));
            }

            out.push_back(pDblOut);
            break;
        }
        case 6: // exponential
        {
            if (vectpDblInput[0]->isScalar() == false)
            {
                delete pDblOut;
                Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "grand", 4);
                return types::Function::Error;
            }

            if (vectpDblInput[0]->get(0) < 0.0) // Av
            {
                delete pDblOut;
                Scierror(999, _("%s: Wrong value for input argument #%d : Positive value expected.\n"), "grand", 4);
                return types::Function::Error;
            }

            for (int i = 0; i < pDblOut->getSize(); i++)
            {
                pDblOut->set(i, C2F(genexp)(vectpDblInput[0]->get()));
            }

            out.push_back(pDblOut);
            break;
        }
        case 7: // F variance ratio
        {
            for (int i = 0; i < 2; i++)
            {
                if (vectpDblInput[i]->isScalar() == false)
                {
                    delete pDblOut;
                    Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "grand", i + 4);
                    return types::Function::Error;
                }

                if (vectpDblInput[i]->get(0) <= 0.0) // Dfn Dfd
                {
                    delete pDblOut;
                    Scierror(999, _("%s: Wrong value for input argument #%d : Positive no null value expected.\n"), "grand", i + 4);
                    return types::Function::Error;
                }
            }

            for (int i = 0; i < pDblOut->getSize(); i++)
            {
                pDblOut->set(i, C2F(genf)(vectpDblInput[0]->get(), vectpDblInput[1]->get()));
            }

            out.push_back(pDblOut);
            break;
        }
        case 8: // non F variance ratio
        {
            for (int i = 0; i < 3; i++)
            {
                if (vectpDblInput[i]->isScalar() == false)
                {
                    delete pDblOut;
                    Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "grand", i + 4);
                    return types::Function::Error;
                }
            }

            if (vectpDblInput[0]->get(0) < 1.0) // Dfn
            {
                delete pDblOut;
                Scierror(999, _("%s: Wrong value for input argument #%d : value > 1.0 expected.\n"), "grand", 4);
                return types::Function::Error;
            }

            if (vectpDblInput[1]->get(0) <= 0.0) // Dfd
            {
                delete pDblOut;
                Scierror(999, _("%s: Wrong value for input argument #%d : Positive non null value expected.\n"), "grand", 5);
                return types::Function::Error;
            }

            if (vectpDblInput[2]->get(0) < 0.0) // Xnon
            {
                delete pDblOut;
                Scierror(999, _("%s: Wrong value for input argument #%d : Positive value expected.\n"), "grand", 6);
                return types::Function::Error;
            }

            for (int i = 0; i < pDblOut->getSize(); i++)
            {
                pDblOut->set(i, C2F(gennf)(vectpDblInput[0]->get(), vectpDblInput[1]->get(), vectpDblInput[2]->get()));
            }

            out.push_back(pDblOut);
            break;
        }
        case 9: // gamma
        {
            for (int i = 0; i < 2; i++)
            {
                if (vectpDblInput[i]->isScalar() == false)
                {
                    delete pDblOut;
                    Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "grand", i + 4);
                    return types::Function::Error;
                }

                if (vectpDblInput[i]->get(0) <= 0.0)
                {
                    delete pDblOut;
                    Scierror(999, _("%s: Wrong value for input argument #%d : Positive non null value expected.\n"), "grand", i + 4);
                    return types::Function::Error;
                }
            }

            for (int i = 0; i < pDblOut->getSize(); i++)
            {
                //** WARNING ** : order is changed in parameters for
                //compatibility between Rand(...'gam',..) and cdfgam

                pDblOut->set(i, C2F(gengam)(vectpDblInput[1]->get(), vectpDblInput[0]->get()));
            }

            out.push_back(pDblOut);
            break;
        }
        case 10: // Gauss Laplace (normal)
        {
            for (int i = 0; i < 2; i++)
            {
                if (vectpDblInput[i]->isScalar() == false)
                {
                    delete pDblOut;
                    Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "grand", i + 4);
                    return types::Function::Error;
                }
            }

            if (vectpDblInput[1]->get(0) < 0.0)
            {
                delete pDblOut;
                Scierror(999, _("%s: Wrong value for input argument #%d : Positive value expected.\n"), "grand", 5);
                return types::Function::Error;
            }

            for (int i = 0; i < pDblOut->getSize(); i++)
            {
                pDblOut->set(i, C2F(gennor)(vectpDblInput[0]->get(), vectpDblInput[1]->get()));
            }

            out.push_back(pDblOut);
            break;
        }
        case 11: // multivariate gaussian (multivariate normal)
        {
            if (vectpDblInput[0]->getCols() != 1 || vectpDblInput[0]->getSize() == 0)
            {
                delete pDblOut;
                Scierror(999, _("%s: Wrong size for input argument #%d : A matrix of size m x 1 expected.(m > 0)\n"), "grand", 4);
                return types::Function::Error;
            }

            if (vectpDblInput[0]->getRows() != vectpDblInput[1]->getRows())
            {
                delete pDblOut;
                Scierror(999, _("%s: Wrong size for input arguments #%d and #%d: Mean and Cov have incompatible dimensions.\n"), "grand", 4, 5);
                return types::Function::Error;
            }

            if (vectpDblInput[1]->getRows() != vectpDblInput[1]->getCols())
            {
                delete pDblOut;
                Scierror(999, _("%s: Wrong size for input argument #%d : A square symmetric positive definite matrix expected.\n"), "grand", 5);
                return types::Function::Error;
            }

            int ierr = 0;
            int size = vectpDblInput[0]->getRows();
            int mp   = (size * (size + 3)) / 2 + 1;

            delete pDblOut;
            types::Double* pDblOut = new types::Double(size, iNumIter);
            double* work = new double[size];
            double* param = new double[mp];

            types::Double* pDblMean = vectpDblInput[0]->clone()->getAs<types::Double>();
            types::Double* pDblCov  = vectpDblInput[1]->clone()->getAs<types::Double>();

            C2F(setgmn)(pDblMean->get(), pDblCov->get(), &size, &size, param, &ierr);

            delete pDblMean;
            delete pDblCov;

            if (ierr == 1)
            {
                delete[] work;
                delete[] param;
                delete pDblOut;
                Scierror(999, _("%s: setgmn return with state %d.\n"), "grand", ierr);
                return types::Function::Error;
            }

            for (int i = 0; i < iNumIter; i++)
            {
                C2F(genmn)(param, pDblOut->get() + (size * i), work);
            }

            delete[] work;
            delete[] param;

            out.push_back(pDblOut);
            break;
        }
        case 12: // geometric
        {
            double pmin = 1.3e-307;

            if (vectpDblInput[0]->isScalar() == false)
            {
                delete pDblOut;
                Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "grand", 4);
                return types::Function::Error;
            }

            if (vectpDblInput[0]->get(0) < pmin || vectpDblInput[0]->get(0) > 1.0)
            {
                delete pDblOut;
                Scierror(999, _("%s: Wrong value for input argument #%d : Must be between %lf and %d.\n"), "grand", 4, pmin, 1);
                return types::Function::Error;
            }

            for (int i = 0; i < pDblOut->getSize(); i++)
            {
                pDblOut->set(i, igngeom(vectpDblInput[0]->get(0)));
            }

            out.push_back(pDblOut);
            break;
        }
        case 13: // markov
        {
            if ( vectpDblInput[0]->getRows() != vectpDblInput[0]->getCols() &&
                    vectpDblInput[0]->getRows() != 1)
            {
                delete pDblOut;
                Scierror(999, _("%s: Wrong size for input argument #%d : A square matrix or a row vector expected.\n"), "grand", 4);
                return types::Function::Error;
            }

            if (vectpDblInput[1]->getSize() == 0)
            {
                delete pDblOut;
                Scierror(999, _("%s: Wrong size for input argument #%d: No empty matrix expected.\n"), "grand", 5);
                return types::Function::Error;
            }

            int sizeOfX0 = vectpDblInput[1]->getSize();

            for ( int i = 0; i < sizeOfX0; i++)
            {
                if (vectpDblInput[1]->get(i) < 1 || (vectpDblInput[1]->get(i) - 1) >= vectpDblInput[0]->getCols())
                {
                    Scierror(999, _("%s: X0(%d) must be in the range [1,%d[.\n"), "grand", i + 1, vectpDblInput[0]->getCols() + 1);
                    return types::Function::Error;
                }
            }

            int size = vectpDblInput[0]->getSize() + vectpDblInput[0]->getCols();
            double* work = new double[(size * sizeof(double))];

            for (int i = 0; i < vectpDblInput[0]->getRows(); i++)
            {
                double ptot = 0.0;
                for (int j = 0; j < vectpDblInput[0]->getCols(); j++)
                {
                    int position = vectpDblInput[0]->getRows() * j + i;

                    if (vectpDblInput[0]->get(position) < 0 || vectpDblInput[0]->get(position) > 1)
                    {
                        delete pDblOut;
                        Scierror(999, _("%s: Wrong value for input argument #%d: P(%d,%d) must be in the range [0 1].\n"), "grand", i + 1, j + 1);
                        delete[] work;
                        return types::Function::Error;
                    }

                    ptot += vectpDblInput[0]->get(position);
                }

                if (fabs(ptot - 1.0) > 1e-8)
                {
                    delete pDblOut;
                    Scierror(999, _("%s: Sum of P(%d,1:%d)=%lf ~= 1.\n"), "grand", i + 1, vectpDblInput[0]->getCols(), ptot);
                    delete[] work;
                    return types::Function::Error;
                }
            }
            // ** Computing the cumulative sum of the P matrix **

            for (int i = 0; i < vectpDblInput[0]->getRows(); i++)
            {
                double cumsum = 0.0;
                work[i] = 0.0;
                for (int j = 1; j < vectpDblInput[0]->getCols() + 1; j++)
                {
                    cumsum += vectpDblInput[0]->get(vectpDblInput[0]->getRows() * (j - 1) + i);
                    work[vectpDblInput[0]->getRows() * j + i] = cumsum;
                }
            }

            for (int i = 0; i < vectpDblInput[1]->getSize(); i++)
            {
                int iCur = static_cast<int>(vectpDblInput[1]->get(i)) - 1;
                for (int j = 0; j < iNumIter; j++)
                {
                    int niv = 0;
                    double rr = C2F(ranf)();

                    if (vectpDblInput[0]->getRows() == 1)
                    {
                        iCur = 0;
                    }

                    while (  rr >= work[iCur + vectpDblInput[0]->getRows() * niv] &&
                             niv < (vectpDblInput[0]->getRows() + 1))
                    {
                        niv++;
                    }

                    // ** projection to avoid boundaries **
                    niv = std::max(std::min(niv, vectpDblInput[0]->getCols()), 1);
                    pDblOut->set(vectpDblInput[1]->getSize() * j + i, static_cast<double>(niv));
                    iCur = niv - 1;
                }
            }

            out.push_back(pDblOut);
            delete[] work;
            break;
        }
        case 14: // multinomial
        {
            if (vectpDblInput[0]->isScalar() == false)
            {
                delete pDblOut;
                Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "grand", 4);
                return types::Function::Error;
            }

            if (vectpDblInput[0]->get(0) < 0)
            {
                delete pDblOut;
                Scierror(999, _("%s: Wrong value for input argument #%d: A positive scalar expected.\n"), "grand", 4);
                return types::Function::Error;
            }

            if (vectpDblInput[1]->getCols() != 1 || vectpDblInput[1]->getRows() <= 0)
            {
                delete pDblOut;
                Scierror(999, _("%s: Wrong size for input argument #%d: A column vector expected.\n"), "grand", 5);
                return types::Function::Error;
            }

            double ptot = 0.0;
            int ncat = vectpDblInput[1]->getRows() + 1;

            for (int i = 0; i < vectpDblInput[1]->getCols(); i++)
            {
                if (vectpDblInput[1]->get(i) < 0.0 || vectpDblInput[1]->get(i) > 1.0)
                {
                    delete pDblOut;
                    Scierror(999, _("%s: Wrong value for input argument #%d: P(%d) must be in the range [0 1].\n"), "grand", 5, i + 1);
                    return types::Function::Error;
                }
                ptot += vectpDblInput[1]->get(i);
            }

            if (ptot > 1.0)
            {
                delete pDblOut;
                Scierror(999, _("%s: Wrong value for input argument #%d: Sum of P(i) > 1.\n"), "grand", 5);
                return types::Function::Error;
            }

            int* piP = new int[vectpDblInput[0]->getSize()];
            int* piOut = new int[pDblOut->getSize()];

            for (int i = 0; i < vectpDblInput[0]->getSize(); i++)
            {
                piP[i] = static_cast<int>(vectpDblInput[0]->get(i));
            }

            for (int i = 0; i < iNumIter; i++)
            {
                C2F(genmul)(piP, vectpDblInput[1]->get(), &ncat,  + &piOut[ncat * i]);
            }

            for (int i = 0; i < pDblOut->getSize(); i++)
            {
                pDblOut->set(i, static_cast<double>(piOut[i]));
            }

            delete piP;
            delete piOut;

            out.push_back(pDblOut);
            break;
        }
        case 15: // Poisson
        {
            if (vectpDblInput[0]->isScalar() == false)
            {
                delete pDblOut;
                Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "grand", 4);
                return types::Function::Error;
            }

            if (vectpDblInput[0]->get(0) < 0.0)
            {
                delete pDblOut;
                Scierror(999, _("%s: Wrong value for input argument #%d : Positive value expected.\n"), "grand", 4);
                return types::Function::Error;
            }

            for (int i = 0; i < pDblOut->getSize(); i++)
            {
                pDblOut->set(i, static_cast<double>(C2F(ignpoi)(vectpDblInput[0]->get())));
            }

            out.push_back(pDblOut);
            break;
        }
        case 16: // random permutations
        {
            delete pDblOut;
            types::InternalType* pITOut = NULL;

            // get dims and isComplex
            int iDims = 0;
            int* piDimsArray = NULL;
            bool bIsComplex = false;
            if (in[2]->isGenericType())
            {
                types::GenericType* pGT = in[2]->getAs<types::GenericType>();
                iDims = pGT->getDims();
                piDimsArray = pGT->getDimsArray();
                bIsComplex = pGT->isComplex();
            }

            switch (in[2]->getType())
            {
                case types::InternalType::ScilabInt8:
                    pITOut = new types::Int8(iDims, piDimsArray);
                    sci_grand_prm(iNumIter, in[2]->getAs<types::Int8>(), &pITOut);
                    break;
                case types::InternalType::ScilabUInt8:
                    pITOut = new types::UInt8(iDims, piDimsArray);
                    sci_grand_prm(iNumIter, in[2]->getAs<types::UInt8>(), &pITOut);
                    break;
                case types::InternalType::ScilabInt16:
                    pITOut = new types::Int16(iDims, piDimsArray);
                    sci_grand_prm(iNumIter, in[2]->getAs<types::Int16>(), &pITOut);
                    break;
                case types::InternalType::ScilabUInt16:
                    pITOut = new types::UInt16(iDims, piDimsArray);
                    sci_grand_prm(iNumIter, in[2]->getAs<types::UInt16>(), &pITOut);
                    break;
                case types::InternalType::ScilabInt32:
                    pITOut = new types::Int32(iDims, piDimsArray);
                    sci_grand_prm(iNumIter, in[2]->getAs<types::Int32>(), &pITOut);
                    break;
                case types::InternalType::ScilabUInt32:
                    pITOut = new types::UInt32(iDims, piDimsArray);
                    sci_grand_prm(iNumIter, in[2]->getAs<types::UInt32>(), &pITOut);
                    break;
                case types::InternalType::ScilabInt64:
                    pITOut = new types::Int64(iDims, piDimsArray);
                    sci_grand_prm(iNumIter, in[2]->getAs<types::Int64>(), &pITOut);
                    break;
                case types::InternalType::ScilabUInt64:
                    pITOut = new types::UInt64(iDims, piDimsArray);
                    sci_grand_prm(iNumIter, in[2]->getAs<types::UInt64>(), &pITOut);
                    break;
                case types::InternalType::ScilabDouble:
                    pITOut = new types::Double(iDims, piDimsArray, bIsComplex);
                    sci_grand_prm(iNumIter, in[2]->getAs<types::Double>(), &pITOut);
                    break;
                case types::InternalType::ScilabBool:
                    pITOut = new types::Bool(iDims, piDimsArray);
                    sci_grand_prm(iNumIter, in[2]->getAs<types::Bool>(), &pITOut);
                    break;
                case types::InternalType::ScilabString:
                    pITOut = new types::String(iDims, piDimsArray);
                    sci_grand_prm(iNumIter, in[2]->getAs<types::String>(), &pITOut);
                    break;
                case types::InternalType::ScilabPolynom:
                    pITOut = new types::Polynom(in[2]->getAs<types::Polynom>()->getVariableName(), iDims, piDimsArray);
                    sci_grand_prm(iNumIter, in[2]->getAs<types::Polynom>(), &pITOut);
                    break;
                case types::InternalType::ScilabSparse:
                {
                    std::complex<double> cplxDbl;
                    types::InternalType* pITOutTempo = NULL;
                    types::Double* pDblTempo = NULL;
                    types::Sparse* pSP = in[2]->getAs<types::Sparse>();
                    int isize = pSP->getSize();
                    pITOut = new types::Sparse(pSP->getRows(), pSP->getCols(), pSP->isComplex());
                    pDblTempo = new types::Double(isize, 1, pSP->isComplex());
                    pITOutTempo = new types::Double(isize, iNumIter, pSP->isComplex());

                    if (pDblTempo->isComplex())
                    {
                        for (int i = 0; i < isize; i++)
                        {
                            cplxDbl = in[2]->getAs<types::Sparse>()->getImg(i);
                            pDblTempo->set(i, cplxDbl.real());
                            pDblTempo->set(i, cplxDbl.imag());
                        }
                    }
                    else
                    {
                        for (int i = 0; i < isize; i++)
                        {
                            pDblTempo->set(i, in[2]->getAs<types::Sparse>()->get(i));
                        }
                    }
                    sci_grand_prm(iNumIter, pDblTempo, &pITOutTempo);

                    if (pDblTempo->isComplex())
                    {
                        for (int i = 0; i < isize; i++)
                        {
                            cplxDbl.real(pITOutTempo->getAs<types::Double>()->get(i));
                            cplxDbl.imag(pITOutTempo->getAs<types::Double>()->getImg(i));
                            pITOutTempo->getAs<types::Sparse>()->set(i, cplxDbl);
                        }
                    }
                    else
                    {
                        for (int i = 0; i < isize; i++)
                        {
                            pITOut->getAs<types::Sparse>()->set(i, pITOutTempo->getAs<types::Double>()->get(i));
                        }
                    }

                    delete pITOutTempo;
                    delete pDblTempo;
                    break;
                }
                default:
                {
                    Scierror(999, _("%s: Wrong type for input argument: Matrix (full or sparse) or Hypermatrix of Reals, Complexes, Integers, Booleans, Strings or Polynomials expected.\n"), "grand");
                    return types::Function::Error;
                }
            }

            out.push_back(pITOut);
            break;
        }
        case 17: // uniform (def)
        {
            for (int i = 0; i < pDblOut->getSize(); i++)
            {
                pDblOut->set(i, C2F(ranf)());
            }

            out.push_back(pDblOut);
            break;
        }
        case 18: // uniform (unf)
        {
            for (int i = 0; i < 2; i++)
            {
                if (vectpDblInput[i]->isScalar() == false)
                {
                    delete pDblOut;
                    Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "grand", i + 4);
                    return types::Function::Error;
                }
            }

            double low  = vectpDblInput[0]->get(0);
            double high = vectpDblInput[1]->get(0);

            if (low > high)
            {
                delete pDblOut;
                Scierror(999, _("%s: Wrong value for input arguments #%d and #%d: Low < High expected.\n"), "grand", 4, 5);
                return types::Function::Error;
            }

            for (int i = 0; i < pDblOut->getSize(); i++)
            {
                pDblOut->set(i, low + (high - low) * C2F(ranf)());
            }

            out.push_back(pDblOut);
            break;
        }
        case 19: // uniform (uin)
        {
            for (int i = 0; i < 2; i++)
            {
                if (vectpDblInput[i]->isScalar() == false)
                {
                    delete pDblOut;
                    Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "grand", i + 4);
                    return types::Function::Error;
                }
            }

            int low  = static_cast<int>(vectpDblInput[0]->get(0));
            int high = static_cast<int>(vectpDblInput[1]->get(0));

            if (low > high)
            {
                delete pDblOut;
                Scierror(999, _("%s: Wrong value for input arguments #%d and #%d: Low < High expected.\n"), "grand", 4, 5);
                return types::Function::Error;
            }

            if ( low  != vectpDblInput[0]->get(0) ||
                    high != vectpDblInput[1]->get(0) ||
                    (high - low + 1) > 2147483561)
            {
                delete pDblOut;
                Scierror(999, _("%s: Wrong value for input arguments #%d and #%d: Low and High must be integers and (high - low + 1) <=  2147483561.\n"), "grand", 4, 5);
                return types::Function::Error;
            }

            for (int i = 0; i < pDblOut->getSize(); i++)
            {
                pDblOut->set(i, C2F(ignuin)(vectpDblInput[0]->get(), vectpDblInput[1]->get()));
            }

            out.push_back(pDblOut);
            break;
        }
        case 20: // uniform (lgi)
        {
            for (int i = 0; i < pDblOut->getSize(); i++)
            {
                pDblOut->set(i, static_cast<double>(ignlgi()));
            }

            out.push_back(pDblOut);
            break;
        }
        case 21: // getgen
        {
            delete pDblOut;
            types::String* pStrOut = new types::String(names_gen[current_base_gen]);
            out.push_back(pStrOut);
            break;
        }
        case 22: // setgen
        {
            delete pDblOut;
            wchar_t* wcsGen = pStrGenOrPhr->get(0);

            if (wcscmp(wcsGen, L"mt") == 0)
            {
                ConfigVariable::setCurrentBaseGen(MT);
            }
            else if (wcscmp(wcsGen, L"kiss") == 0)
            {
                ConfigVariable::setCurrentBaseGen(KISS);
            }
            else if (wcscmp(wcsGen, L"clcg4") == 0)
            {
                ConfigVariable::setCurrentBaseGen(CLCG4);
            }
            else if (wcscmp(wcsGen, L"clcg2") == 0)
            {
                ConfigVariable::setCurrentBaseGen(CLCG2);
            }
            else if (wcscmp(wcsGen, L"urand") == 0)
            {
                ConfigVariable::setCurrentBaseGen(URAND);
            }
            else
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: '%s', '%s', '%s', '%s', '%s' or '%s' expected.\n"), "grand", 2, "mt", "kiss", "clcg4", "clcg2", "urand");
                return types::Function::Error;
            }

            int current_gen = ConfigVariable::getCurrentBaseGen();
            types::String* pStrOut = new types::String(names_gen[current_gen]);
            out.push_back(pStrOut);
            break;
        }
        case 23: // getsd
        {
            switch (current_base_gen)
            {
                case MT:
                {
                    pDblOut = new types::Double(625, 1);
                    get_state_mt(pDblOut->get());
                    break;
                }
                case KISS:
                {
                    pDblOut = new types::Double(4, 1);
                    get_state_kiss(pDblOut->get());
                    break;
                }
                case CLCG4:
                {
                    pDblOut = new types::Double(4, 1);
                    int current_clcg4 = ConfigVariable::getCurrentClcg4();
                    get_state_clcg4(current_clcg4, pDblOut->get());
                    break;
                }
                case CLCG2:
                {
                    pDblOut = new types::Double(2, 1);
                    get_state_clcg2(pDblOut->get());
                    break;
                }
                case URAND:
                {
                    pDblOut = new types::Double(1, 1);
                    get_state_urand(pDblOut->get());
                    break;
                }
            }

            out.push_back(pDblOut);
            break;
        }
        case 24: // setsd
        {
            delete pDblOut;
            int ierr = 0;
            switch (current_base_gen)
            {
                case MT:
                {
                    if (vectpDblInput[0]->isScalar())
                    {
                        ierr = set_state_mt_simple(vectpDblInput[0]->get(0));
                    }
                    else if (vectpDblInput[0]->getSize() == 625)
                    {
                        ierr = set_state_mt(vectpDblInput[0]->get());
                    }
                    else
                    {
                        Scierror(999, _("%s: Wrong size for input argument #%d : A scalar or a vector of size %d expected.\n"), "grand", 4, 625);
                        return types::Function::Error;
                    }

                    break;
                }
                case KISS:
                case CLCG4:
                {
                    for (int i = 0; i < 4; i++)
                    {
                        if (vectpDblInput[i]->isScalar() == false)
                        {
                            Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "grand", i + 2);
                            return types::Function::Error;
                        }
                    }

                    if (current_base_gen == KISS)
                    {
                        ierr = set_state_kiss(  vectpDblInput[0]->get(0),
                                                vectpDblInput[1]->get(0),
                                                vectpDblInput[2]->get(0),
                                                vectpDblInput[3]->get(0));
                    }
                    else // CLCG4
                    {
                        ierr = set_seed_clcg4(  ConfigVariable::getCurrentClcg4(),
                                                vectpDblInput[0]->get(0),
                                                vectpDblInput[1]->get(0),
                                                vectpDblInput[2]->get(0),
                                                vectpDblInput[3]->get(0));
                    }

                    break;
                }
                case CLCG2:
                {
                    for (int i = 0; i < 2; i++)
                    {
                        if (vectpDblInput[i]->isScalar() == false)
                        {
                            Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "grand", i + 2);
                            return types::Function::Error;
                        }
                    }

                    ierr = set_state_clcg2(vectpDblInput[0]->get(0), vectpDblInput[1]->get(0));
                    break;
                }
                case URAND:
                {
                    if (vectpDblInput[0]->isScalar() == false)
                    {
                        Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "grand", 2);
                        return types::Function::Error;
                    }

                    ierr = set_state_urand(vectpDblInput[0]->get(0));
                    break;
                }
            }

            if (ierr == 0)
            {
                Scierror(999, _("%s: Wrong value for the last %d input argument(s).\n"), "grand", in.size() - 1);
                return types::Function::Error;
            }

            break;
        }
        case 25: // phr2sd
        {
            delete pDblOut;
            if (pStrGenOrPhr->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d : One string expected.\n"), "grand", 2);
                return types::Function::Error;
            }

            types::Double* pDblOut = new types::Double(1, 2);
            int size = (int)wcslen(pStrGenOrPhr->get(0));
            int piOut[2];
            char* strPhr = wide_string_to_UTF8(pStrGenOrPhr->get(0));

            C2F(phrtsd)(strPhr, &size, piOut, piOut + 1, size);

            pDblOut->set(0, static_cast<double>(piOut[0]));
            pDblOut->set(1, static_cast<double>(piOut[1]));

            FREE(strPhr);
            out.push_back(pDblOut);
            break;
        }
        case 26: // setcgn
        {
            delete pDblOut;
            if (current_base_gen != CLCG4)
            {
                sciprint(_("The %s option affects only the %s generator\n"), "setcgn", "clcg4");
            }

            if (vectpDblInput[0]->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "grand", 2);
                return types::Function::Error;
            }

            if (vectpDblInput[0]->get(0) < 0 || vectpDblInput[0]->get(0) > Maxgen)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d : Must be between %d and %d.\n"), "grand", 0, Maxgen);
                return types::Function::Error;
            }

            ConfigVariable::setCurrentClcg4(static_cast<int>(vectpDblInput[0]->get(0)));
            double dOut = static_cast<double>(ConfigVariable::getCurrentClcg4());
            out.push_back(new types::Double(dOut));
            break;
        }
        case 27: // getcgn
        {
            delete pDblOut;
            double dOut = static_cast<double>(ConfigVariable::getCurrentClcg4());
            out.push_back(new types::Double(dOut));
            break;
        }
        case 28: // initgn
        {
            delete pDblOut;
            SeedType where;
            if (current_base_gen != CLCG4)
            {
                sciprint(_("The %s option affects only the %s generator\n"), "initgn", "clcg4");
            }

            if (vectpDblInput[0]->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "grand", 2);
                return types::Function::Error;
            }

            if ( vectpDblInput[0]->get(0) != 0 &&
                    vectpDblInput[0]->get(0) != -1 &&
                    vectpDblInput[0]->get(0) != 1)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d : Must be between %d, %d or %d.\n"), "grand", 2, -1, 0, 1);
                return types::Function::Error;
            }

            where = (SeedType)(int)(vectpDblInput[0]->get(0) + 1);
            init_generator_clcg4(ConfigVariable::getCurrentClcg4(), where);
            out.push_back(vectpDblInput[0]);
            break;
        }
        case 29: // setall
        {
            delete pDblOut;
            if (current_base_gen != CLCG4)
            {
                sciprint(_("The %s option affects only the %s generator\n"), "setall", "clcg4");
            }

            for (int i = 0; i < 4; i++)
            {
                if (vectpDblInput[i]->isScalar() == false)
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "grand", i + 2);
                    return types::Function::Error;
                }
            }

            int ierr = set_initial_seed_clcg4(  vectpDblInput[0]->get(0),
                                                vectpDblInput[1]->get(0),
                                                vectpDblInput[2]->get(0),
                                                vectpDblInput[3]->get(0));
            if (ierr == 0)
            {
                Scierror(999, _("%s: Wrong value for the last %d input argument(s).\n"), "grand", 4);
                return types::Function::Error;
            }

            out.push_back(pStrMethod);
            break;
        }
        case 30: // advnst
        {
            delete pDblOut;
            if (current_base_gen != CLCG4)
            {
                sciprint(_("The %s option affects only the %s generator\n"), "advnst", "clcg4");
            }

            if (vectpDblInput[0]->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "grand", 2);
                return types::Function::Error;
            }

            int k = static_cast<int>(vectpDblInput[0]->get(0));

            if (k < 1)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d : Must be > %d.\n"), "grand", 2, 0);
                return types::Function::Error;
            }

            advance_state_clcg4(ConfigVariable::getCurrentClcg4(), k);
            out.push_back(new types::Double(static_cast<double>(k)));
            break;
        }
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

template<class U>
void sci_grand_prm(int iNumIter, U *pIn, types::InternalType** pOut)
{
    U* pUTempo = NULL;
    types::InternalType* pITTempo = NULL;
    int* piDimsArray = NULL;
    int Dims = 0;

    if ((pIn->getCols() == 1) && (pIn->getDims() == 2))
    {
        pOut[0]->getAs<U>()->resize(pIn->getRows(), iNumIter);
        pUTempo = pIn;
    }
    else if ((pIn->getRows() == 1) && (pIn->getDims() == 2))
    {
        pIn->transpose(pITTempo);
        pOut[0]->getAs<U>()->resize(iNumIter, pIn->getCols());
        pUTempo = pITTempo->getAs<U>();
    }
    else
    {
        piDimsArray = pOut[0]->getAs<U>()->getDimsArray();
        Dims = pOut[0]->getAs<U>()->getDims();
        piDimsArray[Dims] = iNumIter;
        pOut[0]->getAs<U>()->resize(piDimsArray, Dims + 1);
        pUTempo = pIn;
    }

    int isize = pUTempo->getSize();

    types::Double* pDblOut = new types::Double(isize, iNumIter, pUTempo->isComplex());

    for (int i = 0; i < iNumIter; i++)
    {
        for (int j = 0; j < isize; j++)
        {
            pDblOut->set(isize * i + j, j);
        }
        C2F(genprm)(pDblOut->get() + (isize * i), &isize);
    }

    if ((pIn->getCols() != 1) && (pIn->getRows() == 1) && (pIn->getDims() == 2))
    {
        pDblOut->transpose(pITTempo);
        delete pDblOut;
        pDblOut = pITTempo->getAs<types::Double>();
    }

    if (pUTempo->isComplex() && pUTempo->isPoly() == false)
    {
        for (int i = 0; i < pOut[0]->getAs<U>()->getSize(); i++)
        {
            pOut[0]->getAs<U>()->set(i , pIn->get(static_cast<int>(pDblOut->get(i))));
            pOut[0]->getAs<U>()->setImg(i, pIn->getImg(static_cast<int>(pDblOut->get(i))));
        }
    }
    else
    {
        for (int i = 0; i < pOut[0]->getAs<U>()->getSize(); i++)
        {
            pOut[0]->getAs<U>()->set(i, pIn->get(static_cast<int>(pDblOut->get(i))));
        }
    }

    if ((pIn->getCols() != 1) && (pIn->getRows() == 1) && (pIn->getDims() == 2))
    {
        delete pUTempo;
    }

    delete pDblOut;
}

/*--------------------------------------------------------------------------*/
