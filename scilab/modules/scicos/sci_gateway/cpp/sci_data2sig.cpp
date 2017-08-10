/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
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

#include <string>

#include "gw_scicos.hxx"

#include "types.hxx"
#include "internal.hxx"
#include "double.hxx"
#include "int.hxx"
#include "struct.hxx"
#include "function.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}

/*--------------------------------------------------------------------------*/
/* B=data2sig(A[,step])
*
* Put data in a 'scicos' signal structure.
* If A is a vector, then it will be understood
* as a scalar that evolves along time.
* If A is a matrix, then it will be understood
* as a vector that evolves along time.
* If A is an hypermatrix, then it will be understood
* as a matrix that evolves along time.
*
* Usage :
* -->a(:,:,4)=([7 8 9;10 11 12]);
* -->b=data2sig(a,0.1)
* b  =
*   values: [2x3x4 hypermat]
*   time: [4x1 constant]
* -->b.time
* ans  =
*    0.
*    0.1.
*    0.2.
*    0.3.
*
* Inputs
*   A    : vector,matrix or hypermatrix(with 3 dimensions)
*          of real/complex or integers numbers,
*
*   step : an optional argument that gives the time step
*          for the output time field.
*          If not given, then the time step is 1.
*
* Output
*   B    : a scilab structure that defines a signal :
*          B.values :  A input,
*          B.time   :  the samples time in the form of
*                      a columun vector. The size of that
*                      vector is given by the A input.
*
* initial rev 08/11/07, Alan
*
*/
/*--------------------------------------------------------------------------*/

static const std::string funname = "data2sig";

static const std::wstring Values(L"values");
static const std::wstring Time(L"time");

types::Function::ReturnValue sci_data2sig(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1 && in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), funname.data(), 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), funname.data(), 1);
        return types::Function::Error;
    }

    // Preparing the output
    types::Struct* B = new types::Struct(1, 1);

    // Add 'B' fields
    B->addField(Values);
    B->addField(Time);


    // Values
    // Extracting input #1 and its last dimension
    int  iDims;
    int* pDims;
    types::InternalType* A;
    switch (in[0]->getType())
    {
        case types::InternalType::ScilabDouble :
        {
            A = in[0]->getAs<types::Double>();
            iDims = A->getAs<types::Double>()->getDims();
            pDims = A->getAs<types::Double>()->getDimsArray();
            break;
        }
        case types::InternalType::ScilabInt8 :
        {
            A = in[0]->getAs<types::Int8>();
            iDims = A->getAs<types::Int8>()->getDims();
            pDims = A->getAs<types::Int8>()->getDimsArray();
            break;
        }
        case types::InternalType::ScilabUInt8 :
        {
            A = in[0]->getAs<types::UInt8>();
            iDims = A->getAs<types::UInt8>()->getDims();
            pDims = A->getAs<types::UInt8>()->getDimsArray();
            break;
        }
        case types::InternalType::ScilabInt16 :
        {
            A = in[0]->getAs<types::Int16>();
            iDims = A->getAs<types::Int16>()->getDims();
            pDims = A->getAs<types::Int16>()->getDimsArray();
            break;
        }
        case types::InternalType::ScilabUInt16 :
        {
            A = in[0]->getAs<types::UInt16>();
            iDims = A->getAs<types::UInt16>()->getDims();
            pDims = A->getAs<types::UInt16>()->getDimsArray();
            break;
        }
        case types::InternalType::ScilabInt32 :
        {
            A = in[0]->getAs<types::Int32>();
            iDims = A->getAs<types::Int32>()->getDims();
            pDims = A->getAs<types::Int32>()->getDimsArray();
            break;
        }
        case types::InternalType::ScilabUInt32 :
        {
            A = in[0]->getAs<types::UInt32>();
            iDims = A->getAs<types::UInt32>()->getDims();
            pDims = A->getAs<types::UInt32>()->getDimsArray();
            break;
        }
        default :
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A real, complex or integer matrix expected.\n"), funname.data(), 1);
            B->killMe();
            return types::Function::Error;
        }
    }
    if (iDims > 3)
    {
        Scierror(888, _("%s: Wrong size for input argument #%d: Only hypermatrices with 3 dimensions are allowed.\n"), funname.data(), 1);
        B->killMe();
        return types::Function::Error;
    }
    // The 'time' vector will have as many lines as the last dimension of 'A'
    const int lastDim = pDims[iDims - 1];
    B->get(0)->set(Values, A);


    // Time
    double defaultStep = 1;
    if (in.size() == 2)
    {
        // Retrieve input #2 if it is given
        if (!in[1]->isDouble())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A real expected.\n"), funname.data(), 2);
            B->killMe();
            return types::Function::Error;
        }
        types::Double* step = in[1]->getAs<types::Double>();
        if (!step->isScalar())
        {
            Scierror(999, _("%s: Wrong size for input argument #%d : A scalar expected.\n"), funname.data(), 2);
            B->killMe();
            return types::Function::Error;
        }
        if (step->isComplex())
        {
            Scierror(999, _("%s: Wrong value for input argument #%d : A real value expected.\n"), funname.data(), 2);
            B->killMe();
            return types::Function::Error;
        }
        defaultStep = step->get(0);
    }

    types::Double* timeValues = new types::Double(lastDim, 1);
    for (int i = 0; i < lastDim; ++i)
    {
        timeValues->set(i, i * defaultStep);
    }
    B->get(0)->set(Time, timeValues);


    out.push_back(B);
    return types::Function::OK;
}
