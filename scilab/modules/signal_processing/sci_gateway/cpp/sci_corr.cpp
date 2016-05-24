/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Antoine ELIAS
* Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
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

#include "signal_gw.hxx"
#include "double.hxx"
#include "string.hxx"
#include "internal.hxx"
#include "function.hxx"
#include "signalprocessingfunctions.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"

}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_corr(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    //check input parameters
    if (in.size() < 2 || in.size() > 5)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "corr", 2, 5);
        return types::Function::Error;
    }

    //call format
    if (in[0]->isString())
    {
        types::String* pS = in[0]->getAs<types::String>();
        if (pS->getSize() == 1 && pS->get(0)[0] == L'f')
        {
            //[cov,mean]=corr('fft',xmacro,[ymacro],n,sect)

            int iErr        = 0;
            int iSect       = 0;
            int iOutSize    = 0;
            int iTotalSize  = 0;
            int iSize       = 0;
            int iMode       = 0;

            double* xa = NULL;
            double* xi = NULL;
            double* xr = NULL;
            double* zr = NULL;
            double* zi = NULL;

            char *dx = NULL;
            char *dy = NULL;
            bool bOK = false;

            //check input parameters
            if (in.size() < 4 || in.size() > 5)
            {
                Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "corr", 4, 5);
                return types::Function::Error;
            }

            //get parameter sect
            int iPos = (int)(in.size() - 1);
            if (in[iPos]->isDouble() == false || in[iPos]->getAs<types::Double>()->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "corr", iPos + 1);
                return types::Function::Error;
            }

            iOutSize = (int)in[iPos]->getAs<types::Double>()->get(0);
            iSect = iOutSize * 2;

            //get parameter n
            iPos--;
            if (in[iPos]->isDouble() == false || in[iPos]->getAs<types::Double>()->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "corr", iPos + 1);
                return types::Function::Error;
            }

            iTotalSize = (int)in[iPos]->getAs<types::Double>()->get(0);

            Signalprocessingfunctions* spFunctionsManager = new Signalprocessingfunctions(L"corr");
            Signalprocessing::addSignalprocessingfunctions(spFunctionsManager);

            //get xmacro
            if (in[1]->isCallable())
            {
                spFunctionsManager->setDgetx(in[1]->getAs<types::Callable>());
            }
            else if (in[1]->isString())
            {
                spFunctionsManager->setDgetx(in[1]->getAs<types::String>());
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "corr", iPos + 1);
                return types::Function::Error;
            }

            iMode = 2;

            if (in.size() == 5)
            {
                //get ymacro
                if (in[2]->isCallable())
                {
                    spFunctionsManager->setDgety(in[2]->getAs<types::Callable>());
                }
                else if (in[2]->isString())
                {
                    spFunctionsManager->setDgety(in[2]->getAs<types::String>());
                }
                else
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "corr", iPos + 2);
                    return types::Function::Error;
                }

                iMode = 3;
            }

            xa = new double[iSect];
            xr = new double[iSect];
            xi = new double[iSect];
            zr = new double[iSect / 2 + 1];
            zi = new double[iSect / 2 + 1];
            C2F(cmpse2)(&iSect, &iTotalSize, &iMode, (void*) dgetx_f, (void*) dgety_f, xa, xr, xi, zr, zi, &iErr);

            delete[] xi;
            delete[] zr;
            delete[] zi;

            if (iErr > 0)
            {
                delete[] xa;
                delete[] xr;
                Scierror(999, _("fft call : needs power of two!"));
                return types::Function::Error;
            }

            types::Double *pDblOut1 = new types::Double(1, iOutSize);
            pDblOut1->set(xa);
            delete[] xa;
            out.push_back(pDblOut1);

            types::Double *pDblOut2 = new types::Double(1, iMode - 1);
            pDblOut2->set(xr);
            delete[] xr;
            out.push_back(pDblOut2);

            return types::Function::OK;
        }
        else if (pS->getSize() == 1 && pS->get(0)[0] == L'u')
        {
            types::Double* pDblIn1 = NULL;
            types::Double* pDblIn2 = NULL;
            types::Double* pDblIn3 = NULL;
            types::Double* pDblIn4 = NULL;

            int iErr = 0;
            int mnx = 0;
            int mny = 0;
            int mfft = 0;
            int nbx = 0;
            int ichaud = 0;
            int iMode = 0;

            double* x = NULL;
            double* xu = NULL;
            double* xui = NULL;
            double* w = NULL;
            double* wi = NULL;
            double* y = NULL;
            double* yi = NULL;

            if (in[1]->isDouble() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Matrix expected.\n"), "corr" , 2);
                return types::Function::Error;
            }

            pDblIn1 = in[1]->getAs<types::Double>();
            if (pDblIn1->isComplex())
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Real matrix expected.\n"), "corr" , 2);
                return types::Function::Error;
            }

            mnx = pDblIn1->getRows() * pDblIn1->getCols();

            x = pDblIn1->get();


            if (in[2]->isDouble() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Matrix expected.\n"), "corr" , 3);
                return types::Function::Error;
            }

            pDblIn2 = in[2]->getAs<types::Double>();

            mny = pDblIn2->getRows() * pDblIn2->getCols();

            if (mnx == mny)
            {
                iMode = 1;
                if (pDblIn2->isComplex())
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: Real matrix expected.\n"), "corr" , 3);
                    return types::Function::Error;
                }

                y = pDblIn2->get();
            }

            if (iMode == 0)
            {
                mfft = mny;
                if (pDblIn2->isComplex() == false)
                {
                    double* wtempo = NULL;
                    w = new double[pDblIn2->getSize()];
                    wi = new double[mfft];
                    memset(wi, 0x00, sizeof(double) * mfft);

                    wtempo = pDblIn2->get();
                    memcpy(w, wtempo, sizeof(double) * pDblIn2->getSize());
                }
                else
                {
                    double* wtempo = NULL;
                    double* witempo = NULL;
                    w = new double[pDblIn2->getSize()];
                    wi = new double[pDblIn2->getSize()];

                    wtempo = pDblIn2->getReal();
                    witempo = pDblIn2->getImg();

                    memcpy(w, wtempo, sizeof(double) * pDblIn2->getSize());
                    memcpy(wi, witempo, sizeof(double) * pDblIn2->getSize());
                }


                if (in.size() == 4)
                {
                    pDblIn3 = in[3]->getAs<types::Double>();
                    if (pDblIn3->isComplex())
                    {
                        Scierror(999, _("%s: Wrong type for input argument #%d: Real matrix expected.\n"), "corr" , 4);
                        delete[] wi;
                        delete[] w;
                        return types::Function::Error;
                    }

                    xui = new double[mfft * 2];
                    double* xutempo = NULL;
                    xutempo = pDblIn3->get();
                    xu = new double[mfft * 2];
                    memset(xu, 0x00, sizeof(double) * mfft * 2);
                    memcpy(xu, xutempo, sizeof(double) * pDblIn3->getSize());

                    nbx =  pDblIn3->getSize();
                    ichaud = 1;
                }
                else
                {
                    xu = new double[mfft * 2];
                    xui = new double[mfft * 2];
                }

                yi = new double[mny];
                C2F(cmpse3)(&mfft, &mnx, &iMode, x, yi, xu, xui, w, wi, &iErr, &ichaud, &nbx);
                if (iErr > 0)
                {
                    delete[] xu;
                    delete[] xui;
                    delete[] wi;
                    delete[] w;
                    Scierror(999, _("fft call : needs power of two!"));
                    return types::Function::Error;
                }

            }
            else
            {
                pDblIn3 = in[3]->getAs<types::Double>();
                mfft  =   pDblIn3->getRows() * pDblIn3->getCols();
                if (pDblIn3->isComplex() == false)
                {
                    wi = new double[mfft];
                    memset(wi, 0x00, sizeof(double) * mfft);

                    w = new double[pDblIn3->getSize()];
                    double* wtempo = NULL;
                    wtempo = pDblIn3->get();
                    memcpy(w, wtempo, sizeof(double) * pDblIn3->getSize());

                }
                else
                {
                    double* wtempo = NULL;
                    double* witempo = NULL;
                    w = new double[pDblIn3->getSize()];
                    wi = new double[pDblIn3->getSize()];

                    wtempo = pDblIn3->getReal();
                    witempo = pDblIn3->getImg();

                    memcpy(w, wtempo, sizeof(double) * pDblIn3->getSize());
                    memcpy(wi, witempo, sizeof(double) * pDblIn3->getSize());
                }
                if (in.size() == 5)
                {
                    pDblIn4 = in[4]->getAs<types::Double>();
                    nbx = pDblIn4->getSize();
                    double* xutempo = NULL;
                    xutempo = pDblIn4->get();
                    xu = new double[mfft * 2];
                    memset(xu, 0x00, sizeof(double) * mfft * 2);
                    memcpy(xu, xutempo, sizeof(double) * pDblIn4->getSize());
                    ichaud = 1;

                    xui = new double[mfft * 2];
                }
                else
                {
                    xu = new double[mfft * 2];
                    xui = new double[mfft * 2];
                }

                C2F(cmpse3)(&mfft, &mnx, &iMode, x, y, xu, xui, w, wi, &iErr, &ichaud, &nbx);
                if (iErr > 0)
                {
                    delete[] xu;
                    delete[] xui;
                    delete[] wi;
                    delete[] w;
                    Scierror(999, _("fft call : needs power of two!"));
                    return types::Function::Error;
                }

            }

            types::Double *pDblOut1 = NULL;
            pDblOut1 = new types::Double(1, mfft, true);
            pDblOut1->set(w);
            pDblOut1->setImg(wi);
            out.push_back(pDblOut1);

            if (_iRetCount == 2)
            {
                types::Double *pDblOut2 = NULL;
                pDblOut2 = new types::Double(1, mfft / 2);

                for (int i = 0; i < mfft / 2; i++)
                {
                    xui[i] = x[mnx - mfft / 2 + i];
                }

                pDblOut2->set(xui);
                out.push_back(pDblOut2);

            }
            delete[] w;
            delete[] wi;
            delete[] xui;
            delete[] xu;
            return types::Function::OK;

        }
        else
        {
            //error
            Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "corr", 1, "'fft', 'update'");
            return types::Function::Error;
        }
    }
    else
    {
        //usual case [cov,mean]=corr(x,[y],nlags)
        int iErr                        = 0;
        int iCorrelation                = 0;
        types::Double* pDblX            = NULL;
        types::Double* pDblY            = NULL;
        types::Double* pDblCorrelation  = NULL;
        types::Double* pDblMean         = NULL;
        int iSize                       = 0;
        double pdblMean[2];

        //check input parameters
        if (in.size() < 2 || in.size() > 3)
        {
            Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "corr", 2, 3);
            return types::Function::Error;
        }

        //get last parameter nlags
        int iPos = (int)(in.size() - 1);
        if (in[iPos]->isDouble() == false || in[iPos]->getAs<types::Double>()->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "corr", iPos + 1);
            return types::Function::Error;
        }

        iCorrelation = (int)in[iPos]->getAs<types::Double>()->get(0);

        if (in.size() == 3)
        {
            if (in[1]->isDouble() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Matrix expected.\n"), "corr" , 2);
                return types::Function::Error;
            }

            pDblY = in[1]->getAs<types::Double>();

            if (in[0]->isDouble() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Matrix expected.\n"), "corr" , 1);
                return types::Function::Error;
            }

            pDblX = in[0]->getAs<types::Double>();

            if (pDblX->getSize() != pDblY->getSize())
            {
                Scierror(60, _("%s: Wrong size for argument: Incompatible dimensions.\n"), "corr");
                return types::Function::Error;
            }
        }
        else
        {
            if (in[0]->isDouble() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Matrix expected.\n"), "corr" , 1);
                return types::Function::Error;
            }

            pDblX = in[0]->getAs<types::Double>();
            pDblY = pDblX;
        }

        iSize = pDblX->getSize();
        pDblCorrelation = new types::Double(1, iCorrelation);
        C2F(tscccf)(pDblX->get(), pDblY->get(), &iSize, pDblCorrelation->get(), pdblMean, &iCorrelation, &iErr);
        if (iErr == -1)
        {
            delete pDblCorrelation;
            Scierror(999, _("%s: Too many coefficients are required.\n"), "corr");
            return types::Function::Error;
        }

        out.push_back(pDblCorrelation);

        if (_iRetCount == 2)
        {
            if (in.size() == 3)
            {
                pDblMean = new types::Double(1, 2);
            }
            else
            {
                pDblMean = new types::Double(1, 1);
            }

            pDblMean->set(pdblMean);
            out.push_back(pDblMean);
        }
    }

    return types::Function::OK;
}

