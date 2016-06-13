/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Antoine ELIAS
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
#include "fileio_gw.hxx"
#include "string.hxx"
#include "mlist.hxx"
#include "function.hxx"
#include "double.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "do_xxscanf.h"
#include "scanf_functions.h"
}

types::Function::ReturnValue sci_msscanf(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int size                    = (int)in.size();
    int iNiter                  = 1;
    wchar_t* wcsFormat          = NULL;
    types::String* pStrRead     = NULL;
    int dimsArray[2]            = {1, 1};
    std::vector<types::InternalType*> IT;

    int args        = 0;
    int nrow        = 0;
    int ncol        = 0;
    int retval      = 0;
    int retval_s    = 0;
    int rowcount    = -1;
    rec_entry buf[MAXSCAN];
    entry *data = NULL;
    sfdir type[MAXSCAN]   = {NONE};
    sfdir type_s[MAXSCAN] = {NONE};

    if (size < 2 || size > 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "msscanf", 2, 3);
        return types::Function::Error;
    }

    if (size == 3)
    {
        if (in[0]->isDouble() == false || in[0]->getAs<types::Double>()->isScalar() == false || in[0]->getAs<types::Double>()->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A Real expected.\n"), "msscanf", 1);
            return types::Function::Error;
        }
        iNiter = static_cast<int>(in[0]->getAs<types::Double>()->get(0));
    }

    if (in[size - 2]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A Strings expected.\n"), "msscanf", size - 1);
        return types::Function::Error;
    }

    if (in[size - 1]->isString() == false || in[size - 1]->getAs<types::String>()->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A String expected.\n"), "msscanf", size);
        return types::Function::Error;
    }

    pStrRead  = in[size - 2]->getAs<types::String>();
    if (iNiter == -1)
    {
        iNiter = pStrRead->getRows();
    }
    wcsFormat = in[size - 1]->getAs<types::String>()->get(0);
    nrow = iNiter;
    while (++rowcount < iNiter)
    {
        if ((iNiter >= 0) && (rowcount >= iNiter))
        {
            break;
        }
        int err = do_xxscanf(L"sscanf", (FILE *)0, wcsFormat, &args, pStrRead->get(rowcount), &retval, buf, type);
        if (err == DO_XXPRINTF_MISMATCH)
        {
            break;
        }
        if (err < 0)
        {
            return types::Function::Error;
        }
        err = Store_Scan(&nrow, &ncol, type_s, type, &retval, &retval_s, buf, &data, rowcount, args);
        if (err < 0)
        {
            switch (err)
            {
                case DO_XXPRINTF_MISMATCH:
                    Free_Scan(rowcount, ncol, type_s, &data);
                    Scierror(999, _("%s: Data mismatch.\n"), "msscanf");
                    return types::Function::Error;

                case DO_XXPRINTF_MEM_LACK:
                    Free_Scan(rowcount, ncol, type_s, &data);
                    Scierror(999, _("%s: No more memory.\n"), "msscanf");
                    return types::Function::Error;
            }
        }
    }

    unsigned int uiFormatUsed = 0;
    for (int i = 0; i < ncol; i++)
    {
        switch ( type_s[i] )
        {
            case SF_C:
            case SF_S:
            {
                types::String* ps = new types::String(iNiter, 1);
                for (int j = 0; j < iNiter; j++)
                {
                    ps->set(j, data[i + ncol * j].s);
                }

                IT.push_back(ps);
                uiFormatUsed |= (1 << 1);
            }
            break;
            case SF_LUI:
            case SF_SUI:
            case SF_UI:
            case SF_LI:
            case SF_SI:
            case SF_I:
            case SF_LF:
            case SF_F:
            {
                types::Double* p = new types::Double(iNiter, 1);
                for (int j = 0; j < iNiter; j++)
                {
                    p->set(j, data[i + ncol * j].d);
                }

                IT.push_back(p);
                uiFormatUsed |= (1 << 2);
            }
            break;
            case NONE:
                break;
        }
    }

    int sizeOfVector = (int)IT.size();
    if (_iRetCount > 1)
    {
        out.push_back(new types::Double((double)retval));

        for (int i = 0; i < sizeOfVector; i++)
        {
            out.push_back(IT[i]);
        }
        for (int i = sizeOfVector + 1; i < _iRetCount; i++)
        {
            out.push_back(types::Double::Empty());
        }
    }
    else
    {
        if (sizeOfVector == 0)
        {
            out.push_back(types::Double::Empty());
            return types::Function::OK;
        }

        switch (uiFormatUsed)
        {
            case (1 << 1) :
            {
                int sizeOfString = IT[0]->getAs<types::String>()->getRows();
                int dimsArrayOfRes[2] = {sizeOfString, sizeOfVector};
                types::String* pString = new types::String(2, dimsArrayOfRes);
                for (int i = 0; i < sizeOfVector; i++)
                {
                    for (int j = 0; j < sizeOfString; j++)
                    {
                        pString->set(i * sizeOfString + j, IT[i]->getAs<types::String>()->get(j));
                    }
                }
                out.push_back(pString);
            }
            break;
            case (1 << 2) :
            {
                int sizeOfDouble = IT[0]->getAs<types::Double>()->getRows();
                int dimsArrayOfRes[2] = {sizeOfDouble, sizeOfVector};
                types::Double* pDouble = new types::Double(2, dimsArrayOfRes);
                for (int i = 0; i < sizeOfVector; i++)
                {
                    types::Double* pdbl = IT[i]->getAs<types::Double>();
                    double* dbl = pdbl->get();
                    for (int j = 0; j < sizeOfDouble; j++)
                    {
                        pDouble->set(i * sizeOfDouble + j, dbl[j]);
                    }

                    pdbl->killMe();
                }
                out.push_back(pDouble);
            }
            break;
            default :
            {
                std::vector<types::InternalType*> pITTemp = std::vector<types::InternalType*>();
                pITTemp.push_back(IT[0]);

                // sizeOfVector always > 1
                for (int i = 1; i < sizeOfVector; i++) // concatenates the Cells. ex : [String 4x1] [String 4x1] = [String 4x2]
                {
                    if (pITTemp.back()->getType() == IT[i]->getType())
                    {
                        switch (pITTemp.back()->getType())
                        {
                            case types::InternalType::ScilabString :
                            {
                                int iRows               = pITTemp.back()->getAs<types::String>()->getRows();
                                int iCols               = pITTemp.back()->getAs<types::String>()->getCols();
                                int arrayOfType[2]      = {iRows, iCols + 1};
                                types::String* pType    = new types::String(2, arrayOfType);

                                for (int k = 0; k < pITTemp.back()->getAs<types::String>()->getSize(); k++)
                                {
                                    pType->set(k, pITTemp.back()->getAs<types::String>()->get(k));
                                }
                                for (int k = 0; k < IT[i]->getAs<types::String>()->getSize(); k++)
                                {
                                    pType->set(iRows * iCols + k, IT[i]->getAs<types::String>()->get(k));
                                }
                                pITTemp.pop_back();
                                pITTemp.push_back(pType);
                            }
                            break;
                            case types::InternalType::ScilabDouble :
                            {
                                int iRows               = pITTemp.back()->getAs<types::Double>()->getRows();
                                int iCols               = pITTemp.back()->getAs<types::Double>()->getCols();
                                int arrayOfType[2]      = {iRows, iCols + 1};
                                types::Double* pType    = new types::Double(2, arrayOfType);

                                pType->set(pITTemp.back()->getAs<types::Double>()->get());
                                for (int k = 0; k < IT[i]->getAs<types::Double>()->getSize(); k++)
                                {
                                    pType->set(iRows * iCols + k, IT[i]->getAs<types::Double>()->get(k));
                                }
                                pITTemp.pop_back();
                                pITTemp.push_back(pType);
                            }
                            break;
                            default :
                                return types::Function::Error;
                        }
                    }
                    else
                    {
                        pITTemp.push_back(IT[i]);
                    }
                }

                types::MList* pMList = new types::MList();
                pMList->append(new types::String(L"cblock"));
                for (int i = 0 ; i < pITTemp.size() ; i++)
                {
                    pMList->append(pITTemp[i]);
                }
                out.push_back(pMList);

                //                int dimsArrayOfCell[2] = {1, (int)pITTemp.size()};
                //                types::Cell* pCell = new types::Cell(2, dimsArrayOfCell);
                //                for (int i = 0; i < pITTemp.size(); i++)
                //                {
                //                    pCell->set(i, pITTemp[i]);
                //                }
                //                out.push_back(pCell);
            }
        }
    }
    Free_Scan(rowcount, ncol, type_s, &data);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
