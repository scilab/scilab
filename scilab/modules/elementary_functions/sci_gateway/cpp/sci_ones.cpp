/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 * Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
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
#include "int.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
}

using namespace types;

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_ones(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    Double* pOut = NULL;
    if (in.size() == 0)
    {
        out.push_back(new Double(1));
        return Function::OK;
    }
    else if (in.size() == 1)
    {
        types::GenericType* pIn = in[0]->getAs<types::GenericType>();
        int iDims = pIn->getDims();
        int* piDims = pIn->getDimsArray();

        // ones(:)
        if (pIn->getRows() == -1 && pIn->getCols() == -1)
        {
            Scierror(21, _("Invalid index.\n"));
            return types::Function::Error;
        }

        pOut = new Double(iDims, piDims);
    }
    else //size > 1
    {
        int iDims = static_cast<int>(in.size());
        int* piDims = new int[iDims];
        for (int i = 0 ; i < iDims ; i++)
        {
            if (in[i]->isGenericType() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Real scalar expected.\n"), "ones", i + 1);
                return Function::Error;
            }

            types::GenericType* pGTIn  = in[i]->getAs<types::GenericType>();
            if (pGTIn->isScalar() == false || pGTIn->isComplex())
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Real scalar expected.\n"), "ones", i + 1);
                return Function::Error;
            }

            switch (in[i]->getType())
            {
                case types::InternalType::RealDouble :
                    piDims[i] = static_cast<int>(in[i]->getAs<types::Double>()->get()[0]);
                    break;
                case types::InternalType::RealInt8 :
                    piDims[i] = static_cast<int>(in[i]->getAs<types::Int8>()->get()[0]);
                    break;
                case types::InternalType::RealUInt8 :
                    piDims[i] = static_cast<int>(in[i]->getAs<types::UInt8>()->get()[0]);
                    break;
                case types::InternalType::RealInt16 :
                    piDims[i] = static_cast<int>(in[i]->getAs<types::Int16>()->get()[0]);
                    break;
                case types::InternalType::RealUInt16 :
                    piDims[i] = static_cast<int>(in[i]->getAs<types::UInt16>()->get()[0]);
                    break;
                case types::InternalType::RealInt32 :
                    piDims[i] = in[i]->getAs<types::Int32>()->get()[0];
                    break;
                case types::InternalType::RealUInt32 :
                    piDims[i] = static_cast<int>(in[i]->getAs<types::UInt32>()->get()[0]);
                    break;
                case types::InternalType::RealInt64 :
                    piDims[i] = static_cast<int>(in[i]->getAs<types::Int64>()->get()[0]);
                    break;
                case types::InternalType::RealUInt64 :
                    piDims[i] = static_cast<int>(in[i]->getAs<types::UInt64>()->get()[0]);
                    break;
                default :
                    Scierror(999, _("%s: Wrong type for input argument #%d: Real scalar expected.\n"), "ones", i + 1);
                    return Function::Error;
            }
        }
        pOut = new Double(iDims, piDims);
        delete[] piDims;
    }

    pOut->setOnes();
    out.push_back(pOut);
    return Function::OK;
}
/*--------------------------------------------------------------------------*/
