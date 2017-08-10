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

#include "configvariable.hxx"
#include "callable.hxx"
#include "double.hxx"

extern "C"
{
#include "scischur.h"
#include "schurSelect.h"
#include "sciprint.h"
}

/*--------------------------------------------------------------------------*/
static double* allocDgeesWorkspace(int iCols, int iDim, int* allocated);
static doublecomplex* allocZgeesWorkspace(int iCols, int iDim, int* allocated);
static double* allocDggesWorkspace(int iCols, int iDim, int* allocated);
static doublecomplex* allocZggesWorkspace(int iCols, int iDim, int* allocated);
/*--------------------------------------------------------------------------*/
int schurSelect(types::Double** _pDblIn, types::Double** _pDblOut, bool _bIsComplex, bool _bIsDiscrete, bool _bIsContinu, ConfigVariable::EntryPointStr* pStrFunction)
{
    int info                    = 0;
    int* pBwork                 = NULL;
    int iWorksize               = 0;
    double* pRwork              = NULL;
    doublecomplex* pCplxWork    = NULL;
    int iDim                    = 0;
    int iCols                   = _pDblIn[0]->getCols();
    types::Callable* pCall      = ConfigVariable::getSchurFunction();

    doublecomplex* pDataInDoublecomplex[2]     = {NULL, NULL};
    doublecomplex* pDataOutDoublecomplex[2]    = {NULL, NULL};

    pBwork = (int*)MALLOC((_pDblIn[1] ? 2 : 1) * iCols * sizeof(int));
    if (pBwork == NULL)
    {
        return -1;
    }

    const char* jobL = _pDblOut[0]  ? "V" : "N";
    const char* jobR = _pDblOut[1]  ? "V" : "N";
    const char* sort = (pStrFunction || pCall || _bIsDiscrete || _bIsContinu) ? "S" : "N";

    if (_pDblIn[1] == NULL && _bIsComplex == false)
    {
        //dgees
        double* pWR = NULL;
        double* pWI = NULL;
        
        if ((pWR = (double*)MALLOC(iCols * sizeof(double))) == NULL)
        {
            FREE(pBwork);
            return -1;
        }

        if ((pWI = (double*)MALLOC(iCols * sizeof(double))) == NULL)
        {
            FREE(pWR);
            FREE(pBwork);
            return -1;
        }

        if ((pRwork = allocDgeesWorkspace(iCols, iDim, &iWorksize)) == NULL)
        {
            FREE(pWR);
            FREE(pWI);
            FREE(pBwork);
            return -1;
        }

        if (_bIsDiscrete)
        {
            C2F(dgees)(jobL, sort, schur_sb02mw, &iCols, _pDblIn[0]->getReal(), &iCols, &iDim, pWR, pWI, _pDblOut[0]->get(), &iCols, pRwork, &iWorksize, pBwork, &info);
        }
        else if (_bIsContinu)
        {
            C2F(dgees)(jobL, sort, schur_sb02mv, &iCols, _pDblIn[0]->getReal(), &iCols, &iDim, pWR, pWI, _pDblOut[0]->get(), &iCols, pRwork, &iWorksize, pBwork, &info);
        }
        else if (pCall)
        {
            C2F(dgees)(jobL, sort, schur_dgees, &iCols, _pDblIn[0]->getReal(), &iCols, &iDim, pWR, pWI, _pDblOut[0]->get(), &iCols, pRwork, &iWorksize, pBwork, &info);
        }
        else if (pStrFunction)
        {
            C2F(dgees)(jobL, sort, (schur_dgees_t)pStrFunction->functionPtr, &iCols, _pDblIn[0]->getReal(), &iCols, &iDim, pWR, pWI, _pDblOut[0]->get(), &iCols, pRwork, &iWorksize, pBwork, &info);
        }
        else
        {
            C2F(dgees)(jobL, sort, NULL, &iCols, _pDblIn[0]->getReal(), &iCols, &iDim, pWR, pWI, _pDblOut[0]->get(), &iCols, pRwork, &iWorksize, pBwork, &info);
        }

        if (_pDblOut[2])
        {
            _pDblOut[2]->set(0, (double)iDim);
        }

        FREE(pWR);
        FREE(pWI);
        FREE(pRwork);

        if (info < 0)
        {
            sciprint(_("Argument %d in dgees had an illegal value.\n"), -info);
        }
        else if (info > 0 && info < iCols)
        {
            sciprint(_("The QR algorithm failed to compute all the eigenvalues.\n"));
        }
        else if (info == iCols + 1)
        {
            sciprint(_("The eigenvalues could not be reordered because some eigenvalues were too close to separate (the problem is very ill-conditioned).\n"));
        }
        else if (info == iCols + 2)
        {
            sciprint(_("After reordering, roundoff changed values of some complex eigenvalues so that leading eigenvalues in the Schur form no longer satisfy SELECT=.TRUE. This could also be caused by underflow due to scaling.\n"));
        }
    }
    else if (_pDblIn[1] == NULL && _bIsComplex)
    {
        //zgees
        doublecomplex* pW = NULL;
        
        if ((pRwork = (double*)MALLOC(iCols * sizeof(double))) == NULL)
        {
            FREE(pBwork);
            return -1;
        }

        if ((pW = (doublecomplex*)MALLOC(iCols * sizeof(doublecomplex))) == NULL)
        {
            FREE(pRwork);
            FREE(pBwork);
            return -1;
        }

        if ((pCplxWork = allocZgeesWorkspace(iCols, iDim, &iWorksize)) == NULL)
        {
            FREE(pW);
            FREE(pRwork);
            FREE(pBwork);
            return -1;
        }

        pDataInDoublecomplex[0] = oGetDoubleComplexFromPointer(_pDblIn[0]->getReal(), _pDblIn[0]->getImg(), _pDblIn[0]->getSize());
        pDataOutDoublecomplex[0] = oGetDoubleComplexFromPointer(_pDblOut[0]->getReal(), _pDblOut[0]->getImg(), _pDblOut[0]->getSize());

        if (_bIsDiscrete)
        {
            C2F(zgees)(jobL, sort, schur_zb02mw, &iCols, pDataInDoublecomplex[0], &iCols, &iDim, pW, pDataOutDoublecomplex[0], &iCols, pCplxWork, &iWorksize, pRwork, pBwork, &info);
        }
        else if (_bIsContinu)
        {
            C2F(zgees)(jobL, sort, schur_zb02mv, &iCols, pDataInDoublecomplex[0], &iCols, &iDim, pW, pDataOutDoublecomplex[0], &iCols, pCplxWork, &iWorksize, pRwork, pBwork, &info);
        }
        else if (pCall)
        {
            C2F(zgees)(jobL, sort, schur_zgees, &iCols, pDataInDoublecomplex[0], &iCols, &iDim, pW, pDataOutDoublecomplex[0], &iCols, pCplxWork, &iWorksize, pRwork, pBwork, &info);
        }
        else if (pStrFunction)
        {
            C2F(zgees)(jobL, sort, (schur_zgees_t)pStrFunction->functionPtr, &iCols, pDataInDoublecomplex[0], &iCols, &iDim, pW, pDataOutDoublecomplex[0], &iCols, pCplxWork, &iWorksize, pRwork, pBwork, &info);
        }
        else
        {
            C2F(zgees)(jobL, sort, NULL, &iCols, pDataInDoublecomplex[0], &iCols, &iDim, pW, pDataOutDoublecomplex[0], &iCols, pCplxWork, &iWorksize, pRwork, pBwork, &info);
        }

        if (_pDblOut[2])
        {
            _pDblOut[2]->set(0, (double)iDim);
        }

        vGetPointerFromDoubleComplex(pDataInDoublecomplex[0], _pDblIn[0]->getSize(), _pDblIn[0]->getReal(), _pDblIn[0]->getImg());
        vGetPointerFromDoubleComplex(pDataOutDoublecomplex[0], _pDblOut[0]->getSize(), _pDblOut[0]->getReal(), _pDblOut[0]->getImg());

        FREE(pW);
        FREE(pRwork);
        FREE(pCplxWork);
        vFreeDoubleComplexFromPointer(pDataInDoublecomplex[0]);
        vFreeDoubleComplexFromPointer(pDataOutDoublecomplex[0]);

        if (info < 0)
        {
            sciprint(_("Argument %d in zgees had an illegal value.\n"), -info);
        }
        else if (info > 0 && info < iCols)
        {
            sciprint(_("The QR algorithm failed to compute all the eigenvalues.\n"));
        }
        else if (info == iCols + 1)
        {
            sciprint(_("The eigenvalues could not be reordered because some eigenvalues were too close to separate (the problem is very ill-conditioned).\n"));
        }
        else if (info == iCols + 2)
        {
            sciprint(_("After reordering, roundoff changed values of some complex eigenvalues so that leading eigenvalues in the Schur form no longer satisfy SELECT=.TRUE. This could also be caused by underflow due to scaling.\n"));
        }
    }
    else if (_pDblIn[1] && _bIsComplex == false)
    {
        //dgges
        double* pAlphaR = NULL;
        double* pAlphaI = NULL;
        double* pBeta = NULL;
        
        if ((pAlphaR = (double*)MALLOC(iCols * sizeof(double))) == NULL)
        {
            FREE(pBwork);
            return -1;
        }

        if ((pAlphaI = (double*)MALLOC(iCols * sizeof(double))) == NULL)
        {
            FREE(pAlphaR);
            FREE(pBwork);
            return -1;
        }

        if ((pBeta = (double*)MALLOC(iCols * sizeof(double))) == NULL)
        {
            FREE(pAlphaI);
            FREE(pAlphaR);
            FREE(pBwork);
            return -1;
        }

        if ((pRwork = allocDggesWorkspace(iCols, iDim, &iWorksize)) == NULL)
        {
            FREE(pBeta);
            FREE(pAlphaI);
            FREE(pAlphaR);
            FREE(pBwork);
            return -1;
        }

        if (_bIsDiscrete)
        {
            C2F(dgges)(jobL, jobR, sort, schur_sb02ox, &iCols, _pDblIn[0]->getReal(), &iCols, _pDblIn[1]->getReal(), &iCols, &iDim, pAlphaR, pAlphaI, pBeta, _pDblOut[0]->get(), &iCols, _pDblOut[1]->get(), &iCols, pRwork, &iWorksize, pBwork, &info);
        }
        else if (_bIsContinu)
        {
            C2F(dgges)(jobL, jobR, sort, schur_sb02ow, &iCols, _pDblIn[0]->getReal(), &iCols, _pDblIn[1]->getReal(), &iCols, &iDim, pAlphaR, pAlphaI, pBeta, _pDblOut[0]->get(), &iCols, _pDblOut[1]->get(), &iCols, pRwork, &iWorksize, pBwork, &info);
        }
        else if (pCall)
        {
            C2F(dgges)(jobL, jobR, sort, schur_dgges, &iCols, _pDblIn[0]->getReal(), &iCols, _pDblIn[1]->getReal(), &iCols, &iDim, pAlphaR, pAlphaI, pBeta, _pDblOut[0]->get(), &iCols, _pDblOut[1]->get(), &iCols, pRwork, &iWorksize, pBwork, &info);
        }
        else if (pStrFunction)
        {
            C2F(dgges)(jobL, jobR, sort, (schur_dgges_t)pStrFunction->functionPtr, &iCols, _pDblIn[0]->getReal(), &iCols, _pDblIn[1]->getReal(), &iCols, &iDim, pAlphaR, pAlphaI, pBeta, _pDblOut[0]->get(), &iCols, _pDblOut[1]->get(), &iCols, pRwork, &iWorksize, pBwork, &info);
        }
        else
        {
            C2F(dgges)(jobL, jobR, sort, NULL, &iCols, _pDblIn[0]->getReal(), &iCols, _pDblIn[1]->getReal(), &iCols, &iDim, pAlphaR, pAlphaI, pBeta, _pDblOut[0]->get(), &iCols, _pDblOut[1]->get(), &iCols, pRwork, &iWorksize, pBwork, &info);
        }

        if (_pDblOut[2])
        {
            _pDblOut[2]->set(0, (double)iDim);
        }

        FREE(pAlphaR);
        FREE(pAlphaI);
        FREE(pBeta);
        FREE(pRwork);

        if (info < 0)
        {
            sciprint(_("Argument %d in dgges had an illegal value.\n"), -info);
        }
        else if (info > 0 && info < iCols)
        {
            sciprint(_("The QZ iteration failed. (A,E) are not in Schur form.\n"));
        }
        else if (info == iCols + 1)
        {
            sciprint(_("Other than QZ iteration failed in DHGEQZ.\n"));
        }
        else if (info == iCols + 2)
        {
            sciprint(_("After reordering, roundoff changed values of some complex eigenvalues so that leading eigenvalues in the Schur form no longer satisfy SELECT=.TRUE. This could also be caused by underflow due to scaling.\n"));
        }
        else if (info == iCols + 3)
        {
            sciprint(_("Reordering failed in DTGSEN.\n"));
        }
    }
    else if (_pDblIn[1] && _bIsComplex)
    {
        //zgges
        doublecomplex* pAlpha = NULL;
        doublecomplex* pBeta = NULL;

        if ((pAlpha = (doublecomplex*)MALLOC(iCols * sizeof(doublecomplex))) == NULL)
        {
            FREE(pBwork);
            return -1;
        }

        if ((pBeta = (doublecomplex*)MALLOC(iCols * sizeof(doublecomplex))) == NULL)
        {
            FREE(pAlpha);
            FREE(pBwork);
            return -1;
        }

        if ((pRwork = (double*) MALLOC(8 * iCols * sizeof(double))) == NULL)
        {
            FREE(pBeta);
            FREE(pAlpha);
            FREE(pBwork);
            return -1;
        }

        if ((pCplxWork = allocZggesWorkspace(iCols, iDim, &iWorksize)) == NULL)
        {
            FREE(pRwork);
            FREE(pBeta);
            FREE(pAlpha);
            FREE(pBwork);
            return -1;
        }

        pDataInDoublecomplex[0] = oGetDoubleComplexFromPointer(_pDblIn[0]->getReal(), _pDblIn[0]->getImg(), _pDblIn[0]->getSize());
        pDataOutDoublecomplex[0] = oGetDoubleComplexFromPointer(_pDblOut[0]->getReal(), _pDblOut[0]->getImg(), _pDblOut[0]->getSize());
        pDataInDoublecomplex[1] = oGetDoubleComplexFromPointer(_pDblIn[1]->getReal(), _pDblIn[1]->getImg(), _pDblIn[1]->getSize());
        pDataOutDoublecomplex[1] = oGetDoubleComplexFromPointer(_pDblOut[1]->getReal(), _pDblOut[1]->getImg(), _pDblOut[1]->getSize());

        if (_bIsDiscrete)
        {
            C2F(zgges)(jobL, jobR, sort, schur_zb02ox, &iCols, pDataInDoublecomplex[0], &iCols, pDataInDoublecomplex[1], &iCols, &iDim, pAlpha, pBeta, pDataOutDoublecomplex[0], &iCols, pDataOutDoublecomplex[1], &iCols, pCplxWork, &iWorksize, pRwork, pBwork, &info);
        }
        else if (_bIsContinu)
        {
            C2F(zgges)(jobL, jobR, sort, schur_zb02ow, &iCols, pDataInDoublecomplex[0], &iCols, pDataInDoublecomplex[1], &iCols, &iDim, pAlpha, pBeta, pDataOutDoublecomplex[0], &iCols, pDataOutDoublecomplex[1], &iCols, pCplxWork, &iWorksize, pRwork, pBwork, &info);
        }
        else if (pCall)
        {
            C2F(zgges)(jobL, jobR, sort, schur_zgges, &iCols, pDataInDoublecomplex[0], &iCols, pDataInDoublecomplex[1], &iCols, &iDim, pAlpha, pBeta, pDataOutDoublecomplex[0], &iCols, pDataOutDoublecomplex[1], &iCols, pCplxWork, &iWorksize, pRwork, pBwork, &info);
        }
        else if (pStrFunction)
        {
            C2F(zgges)(jobL, jobR, sort, (schur_zgges_t)pStrFunction->functionPtr, &iCols, pDataInDoublecomplex[0], &iCols, pDataInDoublecomplex[1], &iCols, &iDim, pAlpha, pBeta, pDataOutDoublecomplex[0], &iCols, pDataOutDoublecomplex[1], &iCols, pCplxWork, &iWorksize, pRwork, pBwork, &info);
        }
        else
        {
            C2F(zgges)(jobL, jobR, sort, NULL, &iCols, pDataInDoublecomplex[0], &iCols, pDataInDoublecomplex[1], &iCols, &iDim, pAlpha, pBeta, pDataOutDoublecomplex[0], &iCols, pDataOutDoublecomplex[1], &iCols, pCplxWork, &iWorksize, pRwork, pBwork, &info);
        }

        if (_pDblOut[2])
        {
            _pDblOut[2]->set(0, (double)iDim);
        }

        vGetPointerFromDoubleComplex(pDataInDoublecomplex[0], _pDblIn[0]->getSize(), _pDblIn[0]->getReal(), _pDblIn[0]->getImg());
        vGetPointerFromDoubleComplex(pDataOutDoublecomplex[0], _pDblOut[0]->getSize(), _pDblOut[0]->getReal(), _pDblOut[0]->getImg());
        vGetPointerFromDoubleComplex(pDataInDoublecomplex[1], _pDblIn[1]->getSize(), _pDblIn[1]->getReal(), _pDblIn[1]->getImg());
        vGetPointerFromDoubleComplex(pDataOutDoublecomplex[1], _pDblOut[1]->getSize(), _pDblOut[1]->getReal(), _pDblOut[1]->getImg());

        FREE(pRwork);
        FREE(pCplxWork);
        vFreeDoubleComplexFromPointer(pDataInDoublecomplex[0]);
        vFreeDoubleComplexFromPointer(pDataOutDoublecomplex[0]);
        vFreeDoubleComplexFromPointer(pDataInDoublecomplex[1]);
        vFreeDoubleComplexFromPointer(pDataOutDoublecomplex[1]);

        if (info < 0)
        {
            sciprint(_("Argument %d in zgges had an illegal value.\n"), -info);
        }
        else if (info > 0 && info < iCols)
        {
            sciprint(_("The QZ iteration failed. (A,E) are not in Schur form.\n"));
            info = 0;
        }
        else if (info == iCols + 1)
        {
            sciprint(_("Other than QZ iteration failed in ZHGEQZ.\n"));
        }
        else if (info == iCols + 2)
        {
            sciprint(_("After reordering, roundoff changed values of some complex eigenvalues so that leading eigenvalues in the Schur form no longer satisfy SELECT=.TRUE. This could also be caused by underflow due to scaling.\n"));
            info = 0;
        }
        else if (info == iCols + 3)
        {
            sciprint(_("Reordering failed in ZTGSEN.\n"));
            info = 0;
        }
    }

    FREE(pBwork);
    return info;
}
/*--------------------------------------------------------------------------*/
static double* allocDgeesWorkspace(int iCols, int iDim, int* allocated)
{
    int info;
    int query = -1;
    double optim;
    double* ret = NULL;

    C2F(dgees)("V", "N", NULL, &iCols, NULL, &iCols, &iDim, NULL, NULL, NULL, &iCols, &optim, &query, NULL, &info);

    *allocated = (int)optim;
    ret = (double*) MALLOC(*allocated * sizeof(double));

    if (!ret)
    {
        *allocated = 3 * iCols;
        ret = (double*) MALLOC(*allocated * sizeof(double));

        if (!ret)
        {
            *allocated = 0;
        }
    }
    return ret;
}

doublecomplex* allocZgeesWorkspace(int iCols, int iDim, int* allocated)
{
    int info;
    int query = -1;
    doublecomplex optim;
    doublecomplex* ret = NULL;

    C2F(zgees)("V", "N", NULL, &iCols, NULL, &iCols, &iDim, NULL, NULL, &iCols, &optim, &query, NULL, NULL, &info);

    *allocated = (int)optim.r;
    ret = (doublecomplex*) MALLOC(*allocated * sizeof(doublecomplex));

    if (!ret)
    {
        *allocated = 2 * iCols;
        ret = (doublecomplex*) MALLOC(*allocated * sizeof(doublecomplex));
        if (!ret)
        {
            *allocated = 0;
        }
    }
    return ret;
}

static double* allocDggesWorkspace(int iCols, int iDim, int* allocated)
{
    int info;
    int query = -1;
    double optim;
    double* ret = NULL;

    C2F(dgges)("V", "V", "N", NULL, &iCols, NULL, &iCols, NULL, &iCols, &iDim, NULL, NULL, NULL, NULL, &iCols, NULL, &iCols, &optim, &query, NULL, &info);

    *allocated = (int)optim;
    ret = (double*)MALLOC(*allocated * sizeof(double));

    if (!ret)
    {
        *allocated = 8 * iCols + 16;
        ret = (double*)MALLOC(*allocated * sizeof(double));

        if (!ret)
        {
            *allocated = 0;
        }
    }
    return ret;
}

static doublecomplex* allocZggesWorkspace(int iCols, int iDim, int* allocated)
{
    int info;
    int query = -1;
    doublecomplex optim;
    doublecomplex* ret = NULL;

    C2F(zgges)("V", "V", "N", NULL, &iCols, NULL, &iCols, NULL, &iCols, &iDim, NULL, NULL, NULL, &iCols, NULL, &iCols, &optim, &query, NULL, NULL, &info);

    *allocated = (int)optim.r;
    ret = (doublecomplex*) MALLOC(*allocated * sizeof(doublecomplex));

    if (!ret)
    {
        *allocated = 2 * iCols;
        ret = (doublecomplex*) MALLOC(*allocated * sizeof(doublecomplex));
        if (!ret)
        {
            *allocated = 0;
        }
    }
    return ret;
}
/*--------------------------------------------------------------------------*/
