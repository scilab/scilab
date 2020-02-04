/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2018- Stéphane MOTTELET
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "double.hxx"
#include "function.hxx"
#include "int.hxx"
#include "overload.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

template<typename T>
int convertTypeToInt32(T* pVal)
{
    typename T::type TVal = pVal->get(0);
    if (TVal >= INT_MAX)
    {
        return INT_MAX;
    }
    else
    {
        return TVal < 0 ? 0 :  (static_cast<int> (TVal));
    }
}

int convertToSize(types::InternalType *pIT);

bool fillRange(double* pdblOut, double* pdblMin, double* pdblMax, int iRows, int iCols);

/* ==================================================================== */
types::Function::ReturnValue sci_linspace(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iCols = 100;
    types::Double* pDblOut;

    if (in.size() != 2 && in.size() != 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "linspace", 2, 3);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected."), "linspace", 1);
        return types::Function::Error;
    }

    types::Double* pDbl[2];
    for (int i = 0; i < 2; i++)
    {
        if (in[i]->isDouble())
        {
            pDbl[i] = in[i]->getAs<types::Double>();
        }
        else
        {
            // other types -> overload
            std::wstring wstFuncName = L"%" + in[i]->getShortTypeStr() + L"_linspace";
            return Overload::call(wstFuncName, in, _iRetCount, out);
        }
    }

    // Check dimensions are the same
    int  iDims0  = pDbl[0]->getDims();
    int* piDims0 = pDbl[0]->getDimsArray();
    int  iDims1  = pDbl[1]->getDims();
    int* piDims1 = pDbl[1]->getDimsArray();
    if (iDims0 != iDims1)
    {
        Scierror(999, _("%s: Arguments %d and %d must have same dimensions.\n"), "linspace", 1, 2);
        return types::Function::Error;
    }
    for (int i = 0; i < iDims0; i++)
    {
        if (piDims0[i] != piDims1[i])
        {
            Scierror(999, _("%s: Arguments %d and %d must have same dimensions.\n"), "linspace", 1, 2);
            return types::Function::Error;
        }
    }

    if (in.size() == 3)
    {
        if (in[2]->isGenericType() && in[2]->getAs<types::GenericType>()->isScalar() &&
            (in[2]->isInt() || (in[2]->isDouble() && in[2]->getAs<types::Double>()->isComplex() == false) ))
        {
            if (in[2]->isDouble())
            {
                double dblCols = in[2]->getAs<types::Double>()->get(0);
                if (std::floor(dblCols) != dblCols)
                {
                    Scierror(999, _("%s: Argument #%d: An integer value expected.\n"), "linspace",3);
                    return types::Function::Error;
                }
            }
            iCols = convertToSize(in[2]);
        }
        else
        {
            Scierror(999, _("%s: Argument #%d: An integer value expected.\n"), "linspace", 3);
            return types::Function::Error;
        }
    }

    if (iCols == INT_MAX)
    {
         Scierror(999, _("%s: Wrong values for input argument #%d: Must be less than %d.\n"),"linspace",3,INT_MAX);
         return types::Function::Error;
    }

    if (iCols == 0 || (pDbl[0]->getSize() == 0))
    {
        // empty matrix case
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }

    // generation is done by considering array as a column vector
    int iRows =  pDbl[0]->getSize();
    // pDblOut is resized later
    pDblOut = new types::Double(iRows, iCols);

    if (!fillRange(pDblOut->get(), pDbl[0]->get(), pDbl[1]->get(), iRows, iCols))
    {
        // if Infs or NaNs
        pDblOut->killMe();
        return types::Function::Error;
    }

    if (pDbl[0]->isComplex() || pDbl[1]->isComplex())
    {
        int iReal;
        for (iReal = 0; iReal < 2; iReal++)
        {
            if (!pDbl[iReal]->isComplex())
            {
                pDbl[iReal] = pDbl[iReal]->clone();
                pDbl[iReal]->setComplex(true);
                break;
            }
        }
        // Complexify pDblOut
        pDblOut->setComplex(true);
        bool status = fillRange(pDblOut->getImg(), pDbl[0]->getImg(), pDbl[1]->getImg(), iRows, iCols);
        if (iReal < 2)
        {
            pDbl[iReal]->killMe();
        }
        if (status != true) // if Infs or NaNs
        {
            pDblOut->killMe();
            return types::Function::Error;
        }
    }

    int *piNewDims = new int[iDims0 + 1];
    // keep the first dimension unchanged
    piNewDims[0] = piDims0[0];
    int iDim = 1;
    for (int i = 1; i < iDims0; i++)
    {
        // squeeze subsequent single dimensions
        if (piDims0[i] > 1)
        {
            piNewDims[iDim++] = piDims0[i];
        }
    }
    // add the suplementary dimension
    piNewDims[iDim++] = iCols;
    // reshape the matrix:
    pDblOut->reshape(piNewDims, iDim);
    out.push_back(pDblOut);

    delete[] piNewDims;
    return types::Function::OK;
}

bool fillRange(double* pdblOut, double* pdblMin, double* pdblMax, int iRows, int iCols)
{
    double* step = new double[iRows];
    for (int j = 0, k = (iCols - 1) * iRows; j < iRows; j++)
    {
        step[j] = (pdblMax[j] - pdblMin[j]) / (iCols - 1);
        // checking Infs and NaNs
        int indInfOrNan = std::isinf(pdblMin[j]) || std::isnan(pdblMin[j]) ? 1 : 0;
        indInfOrNan = indInfOrNan == 0 ? (std::isinf(pdblMax[j]) || std::isnan(pdblMax[j]) ? 2 : 0) : indInfOrNan;
        if (indInfOrNan > 0)
        {
            delete[] step;
            Scierror(999, _("%s: Argument #%d: %%nan and %%inf values are forbidden.\n"), "linspace", indInfOrNan);
            return false;
        }
        // last column is enforced (http://bugzilla.scilab.org/10966)
        pdblOut[k++] = pdblMax[j];
    }
    // doing the linear range generation
    for (int i = 0; i < iCols - 1; i++)
    {
        for (int j = 0; j < iRows; j++)
        {
            *(pdblOut++) = pdblMin[j] + i * step[j];
        }
    }
    delete[] step;

    return true;
}

int convertToSize(types::InternalType *pIT)
{
    switch (pIT->getType())
    {
        case types::InternalType::ScilabDouble:
        {
            return convertTypeToInt32(pIT->getAs<types::Double>());
        }
        case types::InternalType::ScilabInt8:
        {
            return convertTypeToInt32(pIT->getAs<types::Int8>());
        }
        case types::InternalType::ScilabInt16:
        {
            return convertTypeToInt32(pIT->getAs<types::Int16>());
        }
        case types::InternalType::ScilabInt32:
        {
            return convertTypeToInt32(pIT->getAs<types::Int32>());
        }
        case types::InternalType::ScilabInt64:
        {
            return convertTypeToInt32(pIT->getAs<types::Int64>());
        }
        case types::InternalType::ScilabUInt8:
        {
            return convertTypeToInt32(pIT->getAs<types::UInt8>());
        }
        case types::InternalType::ScilabUInt16:
        {
            return convertTypeToInt32(pIT->getAs<types::UInt16>());
        }
        case types::InternalType::ScilabUInt32:
        {
            return convertTypeToInt32(pIT->getAs<types::UInt32>());
        }
        case types::InternalType::ScilabUInt64:
        {
            return convertTypeToInt32(pIT->getAs<types::UInt64>());
        }
        default:
            return 0;
    }
}

