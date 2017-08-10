/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
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

#include "interpolation_gw.hxx"
#include "function.hxx"
#include "double.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"
#include "configvariable_interface.h"
#include "interpolation_functions.h"
#include "interpolation.h"
#include "basic_functions.h" // vDset
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_lsq_splin(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    // input
    types::Double* pDblXd = NULL;
    types::Double* pDblYd = NULL;
    types::Double* pDblWd = NULL;
    types::Double* pDblX  = NULL;

    // output
    types::Double* pDblY = NULL;
    types::Double* pDblD = NULL;

    double dOne     = 1.0;
    int iOne        = 1;
    int sizeOfXd    = 0;
    int sizeOfX     = 0;
    int ierr        = 0;

    // *** check the minimal number of input args. ***
    if (in.size() < 3 || in.size() > 4)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "lsq_splin", 3, 4);
        return types::Function::Error;
    }

    // *** check number of output args according the methode. ***
    if (_iRetCount > 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "lsq_splin", 1, 2);
        return types::Function::Error;
    }

    // *** check type of input args and get it. ***
    // xd
    if (in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), "lsq_splin", 1);
        return types::Function::Error;
    }

    pDblXd = in[0]->getAs<types::Double>();
    sizeOfXd = pDblXd->getSize();
    // yd
    if (in[1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), "lsq_splin", 2);
        return types::Function::Error;
    }

    pDblYd = in[1]->getAs<types::Double>();

    if ( pDblXd->getSize() < 4 ||
            pDblXd->getCols() != pDblYd->getCols() ||
            pDblXd->getRows() != pDblYd->getRows() ||
            (pDblXd->getCols() != 1 && pDblXd->getRows() != 1))
    {
        Scierror(999, _("%s: Wrong size for input arguments #%d and #%d: Same size expected.\n"), "lsq_splin", 1, 2);
        return types::Function::Error;
    }

    // wd
    int iPos = 2;
    if (in.size() == 4)
    {
        if (in[iPos]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), "lsq_splin", iPos + 1);
            return types::Function::Error;
        }

        pDblWd = in[iPos]->getAs<types::Double>();

        if (pDblWd->getRows() != pDblXd->getRows() || pDblWd->getCols() != pDblXd->getCols())
        {
            Scierror(999, _("%s: Wrong size for input arguments #%d and #%d: Same size expected.\n"), "lsq_splin", 1, 3);
            return types::Function::Error;
        }
        iPos++;
    }

    // x
    if (in[iPos]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), "lsq_splin", iPos + 1);
        return types::Function::Error;
    }

    pDblX = in[iPos]->getAs<types::Double>();
    sizeOfX = pDblX->getSize();

    if (pDblX->getSize() < 2 || (pDblX->getCols() != 1 && pDblX->getRows() != 1))
    {
        Scierror(999, _("%s: Wrong size for input argument #%d : A vector of size 2 expected.\n"), "lsq_splin", iPos + 1);
        return types::Function::Error;
    }

    if (good_order(pDblX->get(), pDblX->getSize()) == false) /* verify strict increasing abscissae */
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Not (strictly) increasing or +-inf detected.\n"), "lsq_splin", iPos + 1);
        return types::Function::Error;
    }

    // *** Perform operation. ***
    pDblY = new types::Double(pDblX->getRows(), pDblX->getCols());
    pDblD = new types::Double(pDblX->getRows(), pDblX->getCols());

    if (in.size() == 3)
    {
        pDblWd = new types::Double(pDblXd->getRows(), pDblXd->getCols());
        vDset(sizeOfXd, dOne, pDblWd->get(), iOne);  /* set all the weight = 1  */
    }

    int mworkSize = 7 * sizeOfX + 18;
    double* mwork = new double[mworkSize];

    C2F(spfit)(pDblXd->get(), pDblYd->get(), pDblWd->get(), &sizeOfXd, pDblX->get(), &sizeOfX, pDblY->get(), pDblD->get(), mwork, &ierr);

    delete[] mwork;

    if (in.size() == 3)
    {
        delete pDblWd;
        pDblWd = NULL;
    }

    if (ierr == -1)
    {
        Scierror(999, _("%s: Not enough points for the fit.\n"), "lsq_plin");
        return types::Function::Error;
    }
    else if (ierr == 1 && getWarningMode())
    {
        sciprint(_("%ls: Warning: Rank deficiency of the least square matrix.\n"), "lsq_splin");
    }

    // *** Return result in Scilab. ***
    out.push_back(pDblY);
    out.push_back(pDblD);

    return types::Function::OK;
}

