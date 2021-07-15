/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2020 - Stéphane Mottelet
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
/*--------------------------------------------------------------------------*/
#include "function.hxx"
#include "arrayof.hxx"
#include "overload.hxx"
#include "bool.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_issquare(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "issquare", 1);
        return types::Function::Error;
    }

    if (out.size() >  1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "issquare", 1);
        return types::Function::Error;
    }

    if (in[0]->isGenericType() && !in[0]->isList())
    {
        types::GenericType *pIn = in[0]->getAs<types::GenericType>();
        int iDims = pIn->getDims();
        bool bIsSquare = true;
        if (iDims == 2)
        {
            bIsSquare = pIn->getRows() == pIn->getCols() && pIn->getSize() != 0;
        }
        else
        {
            int *piDims = pIn->getDimsArray();
            int iRefDim = -1;
            int iNbDimsOne = 0;
            for (int i = 0; i < iDims; i++)
            {
                if (piDims[i] != 1)
                {
                    if (iRefDim == -1)
                    {
                        iRefDim = piDims[i];
                        continue;
                    }
                    if (piDims[i] != iRefDim)
                    {
                        bIsSquare = false;
                        break;
                    }
                }
                else
                {
                    iNbDimsOne++;
                }
            }
            //to exclude case like [4, 1, 1, 1]
            bIsSquare = bIsSquare && (iNbDimsOne < iDims-1);
        }
        out.push_back(new types::Bool(bIsSquare)); 
        return types::Function::OK;
    }
    else
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_issquare";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }
}
