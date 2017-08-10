/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Cedric DELAMARRE
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
#include "cacsd_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "polynom.hxx"

extern "C"
{
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
#include "dmp2pm.h"

    extern void C2F(rtitr)( int*, int*, int*, double*, int*, int*, double*, int*, int*, double*,
                            double*, int*, double*, double*, int*, int*, int*, double*, int*);

}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_rtitr(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblUp = NULL;
    types::Double* pDblYp = NULL;

    double* pdblUp      = NULL;
    double* pdblYp      = NULL;
    double* pdblU       = NULL;
    double** pdblNum    = NULL;
    double** pdblDen    = NULL;
    int* piRankDen      = NULL;
    int* piRankNum      = NULL;
    int iMaxRankNum     = 0;
    int iMaxRankDen     = 0;

    int iRowsDen = 0;
    int iSizeDen = 0;
    int iRowsNum = 0;
    int iColsNum = 0;
    int iSizeNum = 0;
    int iRowsU   = 0;
    int iColsU   = 0;
    int iOne     = 1;
    int iJob     = 1;
    int iErr     = 0;

    if (in.size() != 3 && in.size() != 5)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d or %d expected.\n"), "rtitr", 3, 5);
        return types::Function::Error;
    }

    if (_iRetCount < 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "rtitr", 1);
        return types::Function::Error;
    }

    /*** get inputs arguments ***/
    // get up and yp
    if (in.size() == 5)
    {
        if (in[4]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "rtitr", 5);
            return types::Function::Error;
        }

        pDblYp = in[4]->getAs<types::Double>();

        if (pDblYp->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "rtitr", 5);
            return types::Function::Error;
        }

        pdblYp = pDblYp->get();

        if (in[3]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "rtitr", 4);
            return types::Function::Error;
        }

        pDblUp = in[3]->getAs<types::Double>();

        if (pDblUp->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "rtitr", 4);
            return types::Function::Error;
        }

        pdblUp = pDblUp->get();

        iJob = 2;
    }

    // get Num
    if (in[0]->isDouble())
    {
        types::Double* pDblNum = in[0]->getAs<types::Double>();
        if (pDblNum->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "rtitr", 1);
            return types::Function::Error;
        }

        iSizeNum    = pDblNum->getSize();
        iRowsNum    = pDblNum->getRows();
        iColsNum    = pDblNum->getCols();
        iMaxRankNum = 0;
        pdblNum     = new double*[iSizeNum];
        double* pdbl = pDblNum->get();

        for (int i = 0; i < iSizeNum; i++)
        {
            pdblNum[i] = pdbl + i;
        }
    }
    else if (in[0]->isPoly())
    {
        types::Polynom* pPolyNum = in[0]->getAs<types::Polynom>();
        if (pPolyNum->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real polynom expected.\n"), "rtitr", 1);
            return types::Function::Error;
        }

        iSizeNum    = pPolyNum->getSize();
        iRowsNum    = pPolyNum->getRows();
        iColsNum    = pPolyNum->getCols();
        iMaxRankNum = pPolyNum->getMaxRank();
        piRankNum   = new int[iSizeNum];
        pPolyNum->getRank(piRankNum);

        pdblNum     = new double*[iSizeNum];
        for (int i = 0; i < iSizeNum; i++)
        {
            pdblNum[i] = pPolyNum->get(i)->get();
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix or polynom expected.\n"), "rtitr", 1);
        return types::Function::Error;
    }

    // get Den
    if (in[1]->isDouble())
    {
        types::Double* pDblDen = in[1]->getAs<types::Double>();
        if (pDblDen->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "rtitr", 2);
            delete[] pdblNum;
            delete[] piRankNum;
            return types::Function::Error;
        }

        if (pDblDen->getRows() != pDblDen->getCols())
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A square matrix expected.\n"), "rtitr", 2);
            delete[] pdblNum;
            delete[] piRankNum;
            return types::Function::Error;
        }

        iSizeDen    = pDblDen->getSize();
        iRowsDen    = pDblDen->getRows();
        iMaxRankDen = 0;
        pdblDen     = new double*[iSizeDen];
        double* pdbl = pDblDen->get();

        for (int i = 0; i < iSizeDen; i++)
        {
            pdblDen[i] = pdbl + i;
        }
    }
    else if (in[1]->isPoly())
    {
        types::Polynom* pPolyDen = in[1]->getAs<types::Polynom>();
        if (pPolyDen->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real polynom expected.\n"), "rtitr", 2);
            delete[] pdblNum;
            delete[] piRankNum;
            return types::Function::Error;
        }

        if (pPolyDen->getRows() != pPolyDen->getCols())
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A square matrix expected.\n"), "rtitr", 2);
            delete[] pdblNum;
            delete[] piRankNum;
            return types::Function::Error;
        }

        iSizeDen    = pPolyDen->getSize();
        iRowsDen    = pPolyDen->getRows();
        iMaxRankDen = pPolyDen->getMaxRank();
        piRankDen   = new int[iSizeDen];
        pPolyDen->getRank(piRankDen);

        pdblDen     = new double*[iSizeDen];
        for (int i = 0; i < iSizeDen; i++)
        {
            pdblDen[i] = pPolyDen->get(i)->get();
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix or polynom expected.\n"), "rtitr", 2);
        delete[] pdblNum;
        delete[] piRankNum;
        return types::Function::Error;
    }

    // get u
    if (in[2]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "rtitr", 3);
        delete[] pdblDen;
        delete[] pdblNum;
        delete[] piRankNum;
        delete[] piRankDen;
        return types::Function::Error;
    }

    types::Double* pDblU = in[2]->getAs<types::Double>();

    if (pDblU->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "rtitr", 3);
        delete[] pdblDen;
        delete[] pdblNum;
        delete[] piRankNum;
        delete[] piRankDen;
        return types::Function::Error;
    }

    iRowsU = pDblU->getRows();
    iColsU = pDblU->getCols();

    if (iRowsDen != iRowsNum || iColsNum != iRowsU)
    {
        Scierror(60, _("%s: Wrong size for argument: Incompatible dimensions.\n"), "rtitr");
        delete[] pdblDen;
        delete[] pdblNum;
        delete[] piRankNum;
        delete[] piRankDen;
        return types::Function::Error;
    }

    pdblU = pDblU->get();

    // check size of input argument 4 and 5
    if (in.size() == 5)
    {
        if (pDblYp->getRows() != iRowsDen && pDblYp->getRows() != 0)
        {
            Scierror(60, _("%s: Wrong size for argument: Incompatible dimensions.\n"), "rtitr");
            delete[] pdblDen;
            delete[] pdblNum;
            delete[] piRankNum;
            delete[] piRankDen;
            return types::Function::Error;
        }

        if (pDblYp->getCols() != iMaxRankDen)
        {
            Scierror(60, _("%s: Wrong size for argument: Incompatible dimensions.\n"), "rtitr");
            delete[] pdblDen;
            delete[] pdblNum;
            delete[] piRankNum;
            delete[] piRankDen;
            return types::Function::Error;
        }

        if (pDblUp->getRows() != iColsNum && pDblUp->getRows() != 0)
        {
            Scierror(60, _("%s: Wrong size for argument: Incompatible dimensions.\n"), "rtitr");
            delete[] pdblDen;
            delete[] pdblNum;
            delete[] piRankNum;
            delete[] piRankDen;
            return types::Function::Error;
        }

        if (pDblUp->getCols() != iMaxRankDen)
        {
            Scierror(60, _("%s: Wrong size for argument: Incompatible dimensions.\n"), "rtitr");
            delete[] pdblDen;
            delete[] pdblNum;
            delete[] piRankNum;
            delete[] piRankDen;
            return types::Function::Error;
        }
    }

    /*** perform operations ***/
    // output
    int iRowsOut            = iColsU + iMaxRankDen - iMaxRankNum;
    types::Double* pDblOut  = new types::Double(iRowsDen, iRowsOut);
    double* pdblOut         = pDblOut->get();

    // working array
    double* pdblWork    = new double[iRowsDen];
    int* piWork         = new int[iRowsDen];

    // converted a matrix of polynom to a polynomial matrix
    double* pdblDenMat = dmp2pm(pdblDen, iSizeDen, piRankDen, iMaxRankDen);
    double* pdblNumMat = dmp2pm(pdblNum, iSizeNum, piRankNum, iMaxRankNum);

    // perform operations
    C2F(rtitr)(&iRowsU, &iRowsDen, &iColsU, pdblNumMat, &iRowsDen, &iMaxRankNum, pdblDenMat, &iRowsDen, &iMaxRankDen,
               pdblUp, pdblU, &iRowsU, pdblYp, pdblOut, &iRowsDen, &iJob, piWork, pdblWork, &iErr);

    // check error
    if (iErr)
    {
        if (iErr == 1)
        {
            char strValue[256];
            sprintf(strValue, "%lf", pdblWork[0]);
            sciprint(_("Warning :\n"));
            sciprint(_("matrix is close to singular or badly scaled. rcond = %s\n"), strValue);
            iErr = 0;
        }
        else if (iErr == 2)
        {
            Scierror(19, _("Problem is singular.\n"), "rtitr");
        }
    }

    // free memory
    free(pdblDenMat);
    free(pdblNumMat);
    delete[] pdblWork;
    delete[] piWork;
    delete[] pdblDen;
    delete[] pdblNum;

    if (piRankDen)
    {
        delete[] piRankDen;
    }

    if (piRankNum)
    {
        delete[] piRankNum;
    }

    /*** retrun output arguments ***/
    if (iErr)
    {
        return types::Function::Error;
    }

    out.push_back(pDblOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
