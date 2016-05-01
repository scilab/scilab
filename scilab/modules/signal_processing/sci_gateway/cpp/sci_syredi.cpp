/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Antoine ELIAS
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

#include "signal_gw.hxx"
#include "double.hxx"
#include "function.hxx"

extern "C"
{
#include <math.h>
#include "sci_malloc.h"
#include "localization.h"
#include "Scierror.h"
#include "core_math.h"

    extern void C2F(syredi)(int *maxdeg, int *ityp, int *iapro,
                            double *om, double *adelp, double *adels,
                            /* outputs */
                            int *deg_count, int *zeros_count,
                            double *fact,
                            double *b2, double *b1, double *b0,
                            double *c1, double *c0,
                            double *zzr, double *zzi,
                            double *zpr, double *zpi,
                            int *ierr,
                            /* working buffers */
                            double *spr, double *spi,
                            double *pren, double *pimn,
                            double *zm, double *sm, double *rom,
                            /* v-- doutful types but whatever... */
                            double *nzero, double *nze);
}

enum filter_type
{
    low_pass = 1,
    high_pass = 2,
    band_pass = 3,
    stop_band = 4
};

enum design_type
{
    butterworth = 1,
    elliptic = 2,
    chebytchev1 = 3,
    chebytchev2 = 4
};

#define OUT_COUNT 18

//local functions
double maximum(double* _pDblVal, int _iSize);
double minimum(double* _pDblVal, int _iSize);
bool isSortedAscending(double* _pdblVal, int _iSize);
void reshapeFilters(types::Double* _pDblInR, types::Double* _pDblInI, types::Double* _pDblOut);

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_syredi(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iMaxDeg                         = 64;
    types::Double *pDblType             = NULL;
    filter_type iType;

    types::Double *pDblAppro            = NULL;
    design_type iAppro;

    types::Double *pDblCutOff           = NULL;

    types::Double *pDblDeltaP           = NULL;
    double dblDeltaP                    = 0;

    types::Double *pDblDeltaS           = NULL;
    double dblDeltaS                    = 0;

    types::Double* pDblOut[OUT_COUNT];
    int piOutSize[OUT_COUNT]            = {32, 32, 32, 32, 32, 64, 64, 64, 64, 64, 64, 64, 64, 256, 256, 16, 64, 64};

    int iErr                            = 0;
    int iZeroCount                      = 0;
    int iDegCount                       = 0;

    types::Double* pDblFact             = NULL;
    double dblFact                      = 0;

    types::Double* pOut                 = NULL;

    //check input parameters
    if (in.size() != 5)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "syredi", 5);
        return types::Function::Error;
    }

    //check 1st input parameter : filter type ( 1 int )
    pDblType = in[0]->getAs<types::Double>();
    if (in[0]->isDouble() == false || pDblType->isScalar() == false || pDblType->isComplex() == true)
    {
        Scierror(999, _("%s: Wrong type for argument %d: Real scalar expected.\n"), "syredi", 1);
        return types::Function::Error;
    }

    iType = (filter_type)(int)in[0]->getAs<types::Double>()->get(0);

    //check 2nd input parameter : approximation type ( 1 int )
    pDblAppro = in[1]->getAs<types::Double>();
    if (in[1]->isDouble() == false || pDblAppro->isScalar() == false || pDblAppro->isComplex() == true)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Real scalar expected.\n"), "syredi", 2);
        return types::Function::Error;
    }

    iAppro = (design_type)(int)in[1]->getAs<types::Double>()->get(0);

    //check 3rd input parameter : cuttof frequencies ( 4-row vector )
    pDblCutOff = in[2]->getAs<types::Double>();
    if (in[2]->isDouble() == false || pDblCutOff->getSize() != 4 || pDblCutOff->getCols() != 4)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A %d-by-%d array expected.\n"), "syredi", 3, 1, 4);
        return types::Function::Error;
    }

    if (minimum(pDblCutOff->get(), pDblCutOff->getSize()) < 0 || minimum(pDblCutOff->get(), pDblCutOff->getSize()) > M_PI)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the interval [%s, %s].\n"), "syredi", 3, "0", "%pi");
        return types::Function::Error;
    }

    if ((iType == low_pass || iType == high_pass) && isSortedAscending(pDblCutOff->get(), 2) == false)
    {
        Scierror(999, _("%s: Wrong values for input argument #%d: Elements must be in increasing order.\n"), "syredi", 3);
        return types::Function::Error;
    }

    if ((iType == band_pass || iType == stop_band) && isSortedAscending(pDblCutOff->get(), 4) == false)
    {
        Scierror(999, _("%s: Wrong values for input argument #%d: Elements must be in increasing order.\n"), "syredi", 3);
        return types::Function::Error;
    }

    //check 4th input parameter : ripple in passband ( 0 < deltap < 1 )
    pDblDeltaP = in[3]->getAs<types::Double>();
    if (in[3]->isDouble() == false || pDblAppro->isScalar() == false || pDblAppro->isComplex() == true)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Real scalar expected.\n"), "syredi", 4);
        return types::Function::Error;
    }

    dblDeltaP = pDblDeltaP->get(0);

    //check 5th input parameter : ripple in stopband ( 0 < deltas < 1 )
    pDblDeltaS = in[4]->getAs<types::Double>();
    if (in[4]->isDouble() == false || pDblDeltaS->isScalar() == false || pDblDeltaS->isComplex() == true)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Real scalar expected.\n"), "syredi", 5);
        return types::Function::Error;
    }

    dblDeltaS = pDblDeltaS->get(0);


    //alloc temporary variables
    for (int i = 0 ; i < OUT_COUNT ; i++)
    {
        pDblOut[i] = new types::Double(1, piOutSize[i]);
    }

    //call math function
    C2F(syredi)(&iMaxDeg, (int*)&iType, (int*)&iAppro, pDblCutOff->get(), &dblDeltaP,
                &dblDeltaS, &iZeroCount, &iDegCount, &dblFact,
                pDblOut[0]->get(), pDblOut[1]->get(), pDblOut[2]->get(), pDblOut[3]->get(),
                pDblOut[4]->get(), pDblOut[5]->get(), pDblOut[6]->get(), pDblOut[7]->get(),
                pDblOut[8]->get(), &iErr, pDblOut[9]->get(), pDblOut[10]->get(), pDblOut[11]->get(),
                pDblOut[12]->get(), pDblOut[13]->get(), pDblOut[14]->get(), pDblOut[15]->get(),
                pDblOut[16]->get(), pDblOut[17]->get());

    if (iErr)
    {
        if (iErr == -7)
        {
            Scierror(999, _("%s: specs => invalid order filter.\n"), "syredi");
            return types::Function::Error;
        }
        else if (iErr == -9)
        {
            Scierror(999, _("%s: specs => too high order filter.\n"), "syredi");
            return types::Function::Error;
        }
        else
        {
            Scierror(999, _("%s: error in function syredi.\n"), "syredi");
            return types::Function::Error;
        }
    }

    //1st output : fact
    pDblFact = new types::Double(dblFact);
    out.push_back(pDblFact);

    //2nd output : b2
    pOut = new types::Double(1, iDegCount);
    pOut->set(pDblOut[0]->get());
    out.push_back(pOut);

    //3rd output : b1
    pOut = new types::Double(1, iDegCount);
    pOut->set(pDblOut[1]->get());
    out.push_back(pOut);

    //4th output : b0
    pOut = new types::Double(1, iDegCount);
    pOut->set(pDblOut[2]->get());
    out.push_back(pOut);

    //5th output : c1
    pOut = new types::Double(1, iDegCount);
    pOut->set(pDblOut[3]->get());
    out.push_back(pOut);

    //6th output : c0
    pOut = new types::Double(1, iDegCount);
    pOut->set(pDblOut[4]->get());
    out.push_back(pOut);

    //7th output : zeros
    pOut = new types::Double(1, iZeroCount, true);
    reshapeFilters(pDblOut[5], pDblOut[6], pOut);
    out.push_back(pOut);

    //8th output : poles
    pOut = new types::Double(1, iZeroCount, true);
    reshapeFilters(pDblOut[7], pDblOut[8], pOut);
    out.push_back(pOut);

    //clear temporary variables
    for (int i = 0 ; i < OUT_COUNT ; i++)
    {
        delete pDblOut[i];
    }

    return types::Function::OK;
}

/* returns the maximum of the values array (with length elements) */
double maximum(double* _pDblVal, int _iSize)
{
    double dblMax = 0;
    if (_iSize < 1)
    {
        return dblMax;
    }

    dblMax = _pDblVal[0];
    for (int i = 1 ; i < _iSize ; i++)
    {
        dblMax = std::max(_pDblVal[i], dblMax);
    }
    return dblMax;
}

/* returns the minimum of the values array (with length elements) */
double minimum(double* _pDblVal, int _iSize)
{
    double dblMin = 0;
    if (_iSize < 1)
    {
        return dblMin;
    }

    dblMin = _pDblVal[0];
    for (int i = 1 ; i < _iSize ; i++)
    {
        dblMin = std::min(_pDblVal[i], dblMin);
    }
    return dblMin;
}

bool isSortedAscending(double* _pdblVal, int _iSize)
{
    for (int i = 1 ; i < _iSize ; i++)
    {
        if (_pdblVal[i - 1] > _pdblVal[i])
        {
            return false;
        }
    }
    return true;
}

int syredi_buffered(/* inputs */
    enum filter_type filter, enum design_type design,
    double cutoff_frequencies[4], double ripple_passband, double ripple_stopband,
    /* outputs */
    int *zeros_count, int *deg_count, double *fact,
    double b2[], double b1[], double b0[],
    double c1[], double c0[],
    double zzr[], double zzi[],
    double zpr[], double zpi[])
{
#define BUFFERS_COUNT 9
    const int buffers_count = BUFFERS_COUNT;
    int buffers_sizes[BUFFERS_COUNT] = {64, 64, 64, 64, 256, 256, 16, 64, 64};
    double *buffers[BUFFERS_COUNT];
#undef BUFFERS_COUNT
    int maxdeg = 64, error = 1, i;

    for (i = 0 ; i < buffers_count ; ++i)
    {
        buffers[i] = (double *)MALLOC(buffers_sizes[i] * sizeof(double));
        if (buffers[i] == NULL)
        {
            break;
        }
    }
    if (i == buffers_count)
    {
        C2F(syredi)(/* inputs */
            &maxdeg, (int *) &filter, (int *) &design,
            cutoff_frequencies, &ripple_passband, &ripple_stopband,
            /* outputs */
            zeros_count, deg_count, fact,
            b2, b1, b0, c1, c0,
            zzr, zzi, zpr, zpi,
            &error,
            /* buffers */
            buffers[0], buffers[1], buffers[2], buffers[3],
            buffers[4], buffers[5],
            buffers[6],
            buffers[7], buffers[8]);
    }
    for (i-- ; i >= 0 ; i--)
    {
        FREE(buffers[i]);
        buffers[i] = NULL;
    }
    return error;
}

void reshapeFilters(types::Double* _pDblInR, types::Double* _pDblInI, types::Double* _pDblOut)
{
    int iSize           = _pDblOut->getSize();
    double* pdblInR     = _pDblInR->getReal();
    double* pdblInI     = _pDblInI->getReal();
    double* pdblOutR    = _pDblOut->getReal();
    double* pdblOutI    = _pDblOut->getImg();

    for (int i = 0, j = 0 ; j < iSize ; i++, j++)
    {
        if (pdblInI[i] == 0)
        {
            pdblOutR[j] = pdblInR[i];
            pdblOutI[j] = 0;
        }
        else
        {
            pdblOutR[j] = pdblInR[i];
            pdblOutI[j] = pdblInI[i];
            j++;
            pdblOutR[j] = pdblInR[i];
            pdblOutI[j] = - pdblInI[i];
        }
    }
}
