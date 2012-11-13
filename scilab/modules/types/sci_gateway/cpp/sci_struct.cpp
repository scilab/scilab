/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "types_gw.hxx"
#include "function.hxx"
#include "struct.hxx"
#include "string.hxx"
#include "cell.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
}

using namespace types;

Function::ReturnValue sci_struct(typed_list &in, int _piRetCount, typed_list &out)
{
    int* piDimsRef  = NULL;
    int iDimsRef    = 0;

    /* Check number of input arguments: must be even */
    if (in.size() % 2 != 0)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): An even number is expected.\n"), "struct");
        return Function::Error;
    }

    //no input parameter
    if(in.size() == 0)
    {
        out.push_back(new Struct());
        return Function::OK;
    }

    /* First check if all fields are Strings */
    typed_list::iterator itInput;
    for (itInput = in.begin() ; itInput != in.end() ; itInput += 2)
    {
        if ((*itInput)->isString() == false || (*itInput)->getAs<String>()->getSize() != 1)
        {
            Scierror(999, _("%s: Field names must be strings.\n"), "struct");
            return Function::Error;
        }
    }

    /* Second check if dimensions of data are good*/
    for (itInput = in.begin() + 1; itInput != in.end() ; ((itInput+1) != in.end()) ? itInput += 2 : itInput += 1)
    {
        if((*itInput)->getAs<Cell>() && (*itInput)->getAs<Cell>()->isScalar() == false)
        {
            Cell* pCell = (*itInput)->getAs<Cell>();
            if(piDimsRef == NULL)
            {
                iDimsRef    = pCell->getDims();
                piDimsRef   = pCell->getDimsArray();
            }
            else
            {
                if(iDimsRef == pCell->getDims())
                {
                    int* piDims = pCell->getDimsArray();
                    for(int i = 0 ; i < iDimsRef ; i++)
                    {
                        if(piDims[i] != piDimsRef[i])
                        {
                            Scierror(999, _("%s: Arguments must be scalar or must have same dimensions.\n"), "struct");
                            return Function::Error;
                        }
                    }
                }
                else
                {
                    Scierror(999, _("%s: Arguments must be scalar or must have same dimensions.\n"), "struct");
                    return Function::Error;
                }
            }
        }
    }

    Struct *pOut = NULL;

    if(piDimsRef)
    {
        pOut = new Struct(iDimsRef, piDimsRef);
    }
    else
    {
        pOut = new Struct(1,1);
    }

    InternalType *pFieldValue = NULL;
    for (itInput = in.begin() ; itInput != in.end() ; itInput += 2)
    {//for each field
        std::wstring wstField((*itInput)->getAs<String>()->get(0));
        InternalType* pData = (*(itInput + 1));

        //add field in struct
        pOut->addField(wstField);

        if(pData->isCell())
        {//non scalar cell dispatch cell data in each SingleStruct
            Cell* pCell = pData->getAs<Cell>();
            if(pCell->isScalar())
            {
                for(int i = 0 ; i < pOut->getSize() ; i++)
                {
                    pOut->get(i)->set(wstField, pCell->get(0));
                }
            }
            else
            {
                for(int i = 0 ; i < pOut->getSize() ; i++)
                {
                    pOut->get(i)->set(wstField, pCell->get(i));
                }
            }
        }
        else
        {//others
            for(int i = 0 ; i < pOut->getSize() ; i++)
            {
                pOut->get(i)->set(wstField, pData);
            }
        }
    }

    out.push_back(pOut);

    return Function::OK;
}
