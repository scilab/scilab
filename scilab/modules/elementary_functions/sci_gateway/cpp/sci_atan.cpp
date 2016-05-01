/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Cedric DELAMARRE
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

#include <cmath>

#include "elem_func_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "overload.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "Scierror.h"
#include "sciprint.h"
#include "localization.h"
#include "tan.h"
#include "abs.h"
    int C2F(watan)(double*, double*, double*, double*);
}

/*
clear a;nb = 2500;a = rand(nb, nb);tic();atan(a);toc
clear a;nb = 2500;a = rand(nb, nb) + 0.5;tic();atan(a);toc
clear a;nb = 2500;a = rand(nb, nb); a = a + a *%i;tic();atan(a);toc
*/

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_atan(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblX   = NULL;
    types::Double* pDblY   = NULL;
    types::Double* pDblOut = NULL;

    if (in.size() < 1 || in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "atan", 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "atan", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble() == false)
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_atan";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    pDblX = in[0]->getAs<types::Double>();

    if (in.size() == 1)
    {
        if (pDblX->isComplex())
        {
            pDblOut = new types::Double(pDblX->getDims(), pDblX->getDimsArray(), true);
            double* pXR = pDblX->get();
            double* pXI = pDblX->getImg();
            double* pOR = pDblOut->get();
            double* pOI = pDblOut->getImg();

            int size = pDblX->getSize();
            bool msg = true;
            for (int i = 0; i < size; i++)
            {
                if (msg && pXR[i] == 0 && std::abs(pXI[i]) == 1)
                {
                    if (ConfigVariable::getIeee() == 0)
                    {
                        Scierror(999, _("%s: Wrong value for input argument #%d : Singularity of the function.\n"), "atan", 1);
                        return types::Function::Error;
                    }
                    else if (msg && ConfigVariable::getIeee() == 1)
                    {
                        if (ConfigVariable::getWarningMode())
                        {
                            sciprint(_("%s: Warning: Wrong value for input argument #%d : Singularity of the function.\n"), "atan", 1);
                        }

                        msg = false;
                    }
                }

                C2F(watan)(pXR + i, pXI + i, pOR + i, pOI + i);
            }
        }
        else
        {
            pDblOut = new types::Double(pDblX->getDims(), pDblX->getDimsArray(), false);
            double* pXR = pDblX->get();
            double* pOR = pDblOut->get();
            int size = pDblX->getSize();

            for (int i = 0; i < size; i++)
            {
                pOR[i] = std::atan(pXR[i]);
            }
        }
    }
    else // in.size() == 2
    {
        pDblY = in[1]->getAs<types::Double>();

        if (pDblX->isComplex() || pDblY->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A real matrix expected.\n"), "atan", 2);
            return types::Function::Error;
        }

        if (pDblX->getSize() != pDblY->getSize())
        {
            Scierror(999, _("%s: Wrong size for input argument #%d and #%d: Same size expected.\n"), "atan", 1, 2);
            return types::Function::Error;
        }

        pDblOut = new types::Double(pDblX->getDims(), pDblX->getDimsArray(), false);
        double* pXR = pDblX->get();
        double* pYR = pDblY->get();
        double* pOR = pDblOut->get();
        int size = pDblX->getSize();

        for (int i = 0; i < size; i++)
        {
            pOR[i] =  std::atan2(pXR[i], pYR[i]);
        }
    }

    out.push_back(pDblOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
