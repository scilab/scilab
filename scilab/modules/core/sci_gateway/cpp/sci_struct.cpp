/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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

#include "core_gw.hxx"
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

types::Function::ReturnValue sci_struct_gw(types::typed_list &in, int _piRetCount, types::typed_list &out)
{
    int* piDimsRef  = NULL;
    int iDimsRef    = 0;

    /* Check number of input arguments: must be even */
    if (in.size() % 2 != 0)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): An even number is expected.\n"), "struct");
        return types::Function::Error;
    }

    //no input parameter
    if (in.size() == 0)
    {
        out.push_back(new types::Struct());
        return types::Function::OK;
    }

    /* First check if all fields are Strings */
    types::typed_list::iterator itInput;
    for (itInput = in.begin() ; itInput != in.end() ; itInput += 2)
    {
        if ((*itInput)->isString() == false || (*itInput)->getAs<types::String>()->getSize() != 1)
        {
            Scierror(999, _("%s: Field names must be strings.\n"), "struct");
            return types::Function::Error;
        }
    }

    /* Second check if dimensions of data are good*/
    for (itInput = in.begin() + 1; itInput != in.end() ; ((itInput + 1) != in.end()) ? itInput += 2 : itInput += 1)
    {
        if ((*itInput)->isCell() && (*itInput)->getAs<types::Cell>()->isScalar() == false)
        {
            types::Cell* pCell = (*itInput)->getAs<types::Cell>();
            if (piDimsRef == NULL)
            {
                iDimsRef    = pCell->getDims();
                piDimsRef   = pCell->getDimsArray();
            }
            else
            {
                if (iDimsRef == pCell->getDims())
                {
                    int* piDims = pCell->getDimsArray();
                    for (int i = 0 ; i < iDimsRef ; i++)
                    {
                        if (piDims[i] != piDimsRef[i])
                        {
                            Scierror(999, _("%s: Arguments must be scalar or must have same dimensions.\n"), "struct");
                            return types::Function::Error;
                        }
                    }
                }
                else
                {
                    Scierror(999, _("%s: Arguments must be scalar or must have same dimensions.\n"), "struct");
                    return types::Function::Error;
                }
            }
        }
    }

    types::Struct *pOut = NULL;

    if (piDimsRef)
    {
        pOut = new types::Struct(iDimsRef, piDimsRef);
    }
    else
    {
        pOut = new types::Struct(1, 1);
    }

    types::InternalType *pFieldValue = NULL;
    for (itInput = in.begin() ; itInput != in.end() ; itInput += 2)
    {
        //for each field
        std::wstring wstField((*itInput)->getAs<types::String>()->get(0));
        types::InternalType* pData = (*(itInput + 1));

        //add field in struct
        pOut->addField(wstField);

        if (pData->isCell())
        {
            //non scalar cell dispatch cell data in each SingleStruct
            types::Cell* pCell = pData->getAs<types::Cell>();
            if (pCell->isScalar())
            {
                for (int i = 0 ; i < pOut->getSize() ; i++)
                {
                    pOut->get(i)->set(wstField, pCell->get(0));
                }
            }
            else
            {
                for (int i = 0 ; i < pOut->getSize() ; i++)
                {
                    pOut->get(i)->set(wstField, pCell->get(i));
                }
            }
        }
        else
        {
            //others
            for (int i = 0 ; i < pOut->getSize() ; i++)
            {
                pOut->get(i)->set(wstField, pData);
            }
        }
    }

    out.push_back(pOut);

    return types::Function::OK;
}
