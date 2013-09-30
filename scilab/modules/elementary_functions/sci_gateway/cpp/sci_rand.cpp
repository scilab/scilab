/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "elem_func_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "basic_functions.h"
}

#define Ran1		siRandSave		//old C2F(com).ran[0]
#define Ran2		siRandType		//old C2F(com).ran[1]

const wchar_t g_pwstConfigInfo[] = {L"info"};
const wchar_t g_pwstConfigSeed[] = {L"seed"};

const wchar_t g_pwstTypeUniform[] = {L"uniform"};
const wchar_t g_pwstTypeNormal[] = {L"normal"};

int setRandType(wchar_t _wcType);
double getNextRandValue(int _iRandType, int* _piRandSave, int _iForceInit);

types::Function::ReturnValue sci_rand(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    static int siRandType = 0;
    static int siRandSave = 0;
    static int iForceInit	= 0;

    int iSizeIn = (int)in.size();

    if (iSizeIn == 0 || iSizeIn == -1)
    {
        //rand or rand()
        double dblRand = getNextRandValue(siRandType, &siRandSave, 0);
        out.push_back(new types::Double(dblRand));
        return types::Function::OK;
    }

    if (in[0]->isString())
    {
        //rand("xxx")
        types::String* pS = in[0]->getAs<types::String>();
        if (pS->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), "rand", 1);
            return types::Function::Error;
        }

        wchar_t* pwstKey = pS->get(0);

        if (pwstKey[0] == g_pwstConfigInfo[0])
        {
            //info
            if (iSizeIn > 1)
            {
                Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "rand", 1);
                return types::Function::Error;
            }

            if (siRandType == 0)
            {
                out.push_back(new types::String(g_pwstTypeUniform));
            }
            else
            {
                out.push_back(new types::String(g_pwstTypeNormal));
            }
        }
        else if (pwstKey[0] == g_pwstConfigSeed[0])
        {
            //seed
            if (iSizeIn == 1)
            {
                //get
                out.push_back(new types::Double(siRandSave));
            }
            else if (iSizeIn == 2)
            {
                if (in[1]->isDouble() == false || in[1]->getAs<types::Double>()->isScalar() == false)
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "rand", 2);
                    return types::Function::Error;
                }

                siRandSave = (int)Max(in[1]->getAs<types::Double>()->get(0), 0);
                iForceInit = 1;
            }
            else
            {
                Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "rand", 2);
                return types::Function::Error;
            }
        }
        else
        {
            siRandType = setRandType(pwstKey[0]);
        }
    }
    else if (in[0]->isDouble())
    {
        int iRandSave = siRandType;
        if (in[iSizeIn - 1]->isString())
        {
            //uniform ou normal
            types::String* pS = in[iSizeIn - 1]->getAs<types::String>();
            if (pS->getSize() != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), "rand", iSizeIn);
                return types::Function::Error;
            }

            //set randomize law
            iRandSave = siRandType;
            siRandType = setRandType(pS->get(0)[0]);
            iSizeIn--;
        }

        if (iSizeIn == 1)
        {
            //rand(X) or rand(X, "")
            types::Double* pD = in[0]->getAs<types::Double>();

            // rand(:)
            if (pD->getRows() == -1 && pD->getCols() == -1)
            {
                Scierror(21, _("Invalid index.\n"));
                return types::Function::Error;
            }

            types::Double* pOut = new types::Double(pD->getDims(), pD->getDimsArray(), pD->isComplex());

            double* pReal = pOut->getReal();
            for (int i = 0 ; i < pOut->getSize() ; i++)
            {
                pReal[i] = getNextRandValue(siRandType, &siRandSave, iForceInit);
            }

            if (pD->isComplex())
            {
                double* pImg = pOut->getImg();
                for (int i = 0 ; i < pOut->getSize() ; i++)
                {
                    pImg[i] = getNextRandValue(siRandType, &siRandSave, iForceInit);
                }
            }

            out.push_back(pOut);

            //retore previous law
            siRandType = iRandSave;
        }
        else
        {
            int iDims = iSizeIn;
            int *piDims = new int[iDims];

            //check others parameter type and size
            for (int i = 0 ; i < iSizeIn ; i++)
            {
                if (in[i]->isDouble() == false || in[i]->getAs<types::Double>()->isScalar() == false)
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: Real scalar expected.\n"), "rand" , i + 1);
                    return types::Function::Error;
                }

                piDims[i] = (int)in[i]->getAs<types::Double>()->get(0);
            }

            types::Double* pOut = new types::Double(iDims, piDims);
            delete[] piDims;

            double* pd = pOut->get();
            for (int i = 0 ; i < pOut->getSize() ; i++)
            {
                pd[i] = getNextRandValue(siRandType, &siRandSave, iForceInit);
                iForceInit = 0;
            }
            out.push_back(pOut);
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string or a real expected.\n"), "rand", 1);
        return types::Function::Error;
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
double getNextRandValue(int _iRandType, int* _piRandSave, int _iForceInit)
{
    static int siInit       = TRUE;
    static double sdblImg   = 0;
    static double sdblR     = 0;
    double dblReal          = 0;
    double dblVal           = 0;
    double dblTemp          = 2;

    if (_iForceInit)
    {
        siInit = TRUE;
    }

    if (_iRandType == 0)
    {
        dblVal = durands(_piRandSave);
    }
    else
    {
        if (siInit == TRUE)
        {
            while (dblTemp > 1)
            {
                dblReal	= 2 * durands(_piRandSave) - 1;
                sdblImg	= 2 * durands(_piRandSave) - 1;
                dblTemp = dblReal * dblReal + sdblImg * sdblImg;
            }
            sdblR   = dsqrts(-2 * dlogs(dblTemp) / dblTemp);
            dblVal  = dblReal * sdblR;
        }
        else
        {
            dblVal = sdblImg * sdblR;
        }
        siInit = !siInit;
    }
    return dblVal;
}

int setRandType(wchar_t _wcType)
{
    if (_wcType == L'g' || _wcType == L'n')
    {
        return 1;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
