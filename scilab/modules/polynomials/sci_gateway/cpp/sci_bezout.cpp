/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Cedric DELAMARRE
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
#include "polynomials_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "polynom.hxx"
#include "overload.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
    extern void C2F(recbez)(double*, int*, double*, int*, double*, int*, double*, double*);
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_bezout(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    bool bComplex     = false;
    double* pdblIn[2] = {NULL, NULL};
    int piDegree[2]   = {0, 0};
    double dblErr     = 0;
    int iOne          = 1;

    std::wstring wstrName = L"";

    if (in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "bezout", 2);
        return types::Function::Error;
    }

    if (_iRetCount != 2 && _iRetCount != 3)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "bezout", 2, 3);
        return types::Function::Error;
    }

    // get input arguments
    for (int i = 0; i < in.size(); i++)
    {
        if (in[i]->isPoly() == false && in[i]->isDouble() == false)
        {
            std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_bezout";
            return Overload::call(wstFuncName, in, _iRetCount, out);
        }

        types::GenericType* pGT = in[i]->getAs<types::GenericType>();

        if (pGT->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real scalar expected.\n"), "bezout", i + 1);
            return types::Function::Error;
        }

        if (pGT->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), "bezout", i + 1);
            return types::Function::Error;
        }

        if (in[i]->isDouble())
        {
            pdblIn[i] = in[i]->getAs<types::Double>()->get();
        }
        else // polynom
        {
            types::Polynom* pPolyIn = in[i]->getAs<types::Polynom>();
            if (wstrName != L"" && wstrName != pPolyIn->getVariableName())
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: A polynomial '%ls' expected.\n"), "bezout", i + 1, wstrName.c_str());
                return types::Function::Error;
            }

            wstrName    = pPolyIn->getVariableName();
            pdblIn[i]   = pPolyIn->get(0)->get();
            piDegree[i] = pPolyIn->get(0)->getRank();
        }
    }

    // perform operation
    int iMaxRank     = std::max(piDegree[0], piDegree[1]) + 1;
    int iMinRank     = std::min(piDegree[0], piDegree[1]) + 1;
    double* pdblWork = new double[10 * iMaxRank + 3 * iMaxRank * iMaxRank];
    double* pdblOut  = new double[2 * (piDegree[0] + piDegree[1] + 2) + iMinRank + 3];
    int ipb[6];

    C2F(recbez)(pdblIn[0], piDegree, pdblIn[1], piDegree + 1, pdblOut, ipb, pdblWork, &dblErr);
    delete[] pdblWork;

    // create result
    int np = ipb[1] - ipb[0];
    double* pdblSP = NULL;
    types::SinglePoly* pSP = new types::SinglePoly(&pdblSP, np - 1);
    memcpy(pdblSP, pdblOut + ipb[0] - 1, np * sizeof(double));

    if (wstrName == L"")
    {
        wstrName = L"s";
    }

    types::Polynom* pPolyGCD = new types::Polynom(wstrName, 1, 1);
    pPolyGCD->set(0, pSP);
    delete pSP;

    types::Polynom* pPolyU = new types::Polynom(wstrName, 2, 2);
    for (int i = 0; i < 4; i++)
    {
        int ii     = i + 1;
        int iRankU = ipb[ii + 1] - ipb[ii];
        double* pdblU = NULL;
        types::SinglePoly* pSPU = new types::SinglePoly(&pdblU, iRankU - 1);
        memcpy(pdblU, pdblOut + ipb[ii] - 1, iRankU * sizeof(double));
        pPolyU->set(i, pSPU);
        delete pSPU;
    }

    delete[] pdblOut;

    // return result
    out.push_back(pPolyGCD);
    out.push_back(pPolyU);

    if (_iRetCount == 3)
    {
        out.push_back(new types::Double(dblErr));
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

