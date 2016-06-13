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
#include "scilabWrite.hxx"
#include "cell.hxx"
#include "function.hxx"
#include "double.hxx"
#include "configvariable.hxx"
#include "threadmanagement.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "localization.h"
#include "Scierror.h"
#include "do_xxscanf.h"
#include "scanf_functions.h"
#include "scilabRead.h"
}

types::Function::ReturnValue sci_mscanf(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int size                    = (int)in.size();
    int iNiter                  = 1;
    wchar_t* wcsFormat          = NULL;
    wchar_t* wcsRead            = NULL;
    int dimsArray[2]            = {1, 1};
    std::vector<types::InternalType*> pIT;

    int args        = 0;
    int nrow        = 0;
    int ncol        = 0;
    int retval      = 0;
    int retval_s    = 0;
    int rowcount    = -1;
    rec_entry buf[MAXSCAN];
    entry *data = NULL;
    sfdir type[MAXSCAN] = {NONE};
    sfdir type_s[MAXSCAN] = {NONE};

    if (size < 1 || size > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "mscanf", 1, 2);
        return types::Function::Error;
    }

    if (size == 2)
    {
        if (in[0]->isDouble() == false || in[0]->getAs<types::Double>()->isScalar() == false || in[0]->getAs<types::Double>()->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A Real expected.\n"), "mscanf", 1);
            return types::Function::Error;
        }
        iNiter = static_cast<int>(in[0]->getAs<types::Double>()->get(0));
        if (iNiter < 0)
        {
            iNiter = 1;
        }
    }

    if (in[size - 1]->isString() == false || in[size - 1]->getAs<types::String>()->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A String expected.\n"), "mscanf", size);
        return types::Function::Error;
    }

    wcsFormat = in[size - 1]->getAs<types::String>()->get(0);
    nrow = iNiter;
    while (++rowcount < iNiter)
    {
        if ((iNiter >= 0) && (rowcount >= iNiter))
        {
            break;
        }

        // get data
        // The console thread must not parse the next console input.
        ConfigVariable::setScilabCommand(0);

        // Get the console input filled by the console thread.
        char* pcConsoleReadStr = ConfigVariable::getConsoleReadStr();
        ThreadManagement::SendConsoleExecDoneSignal();
        while (pcConsoleReadStr == NULL)
        {
            pcConsoleReadStr = ConfigVariable::getConsoleReadStr();
            ThreadManagement::SendConsoleExecDoneSignal();
        }

        // reset flag to default value
        ConfigVariable::setScilabCommand(1);

        wcsRead = to_wide_string(pcConsoleReadStr);
        FREE(pcConsoleReadStr);
        int err = do_xxscanf(L"sscanf", (FILE *)0, wcsFormat, &args, wcsRead, &retval, buf, type);
        FREE(wcsRead);
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
                    Scierror(999, _("%s: Data mismatch.\n"), "mscanf");
                    return types::Function::Error;

                case DO_XXPRINTF_MEM_LACK:
                    Free_Scan(rowcount, ncol, type_s, &data);
                    Scierror(999, _("%s: No more memory.\n"), "mscanf");
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
                pIT.push_back(ps);
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
                pIT.push_back(p);
                uiFormatUsed |= (1 << 2);
            }
            break;
            case NONE:
                break;
        }
    }

    int sizeOfVector = (int)pIT.size();
    if (_iRetCount > 1)
    {
        types::Double* pDouble = new types::Double(2, dimsArray);
        pDouble->set(0, retval);
        out.push_back(pDouble);

        for (int i = 0; i < sizeOfVector; i++)
        {
            out.push_back(pIT[i]);
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
            out.push_back(new types::String(L""));
            return types::Function::OK;
        }

        switch (uiFormatUsed)
        {
            case (1 << 1) :
            {
                int sizeOfString = pIT[0]->getAs<types::String>()->getRows();
                int dimsArrayOfRes[2] = {sizeOfString, sizeOfVector};
                types::String* pString = new types::String(2, dimsArrayOfRes);
                for (int i = 0; i < sizeOfVector; i++)
                {
                    for (int j = 0; j < sizeOfString; j++)
                    {
                        pString->set(i * sizeOfString + j, pIT[i]->getAs<types::String>()->get(j));
                    }
                }
                out.push_back(pString);
            }
            break;
            case (1 << 2) :
            {
                int sizeOfDouble = pIT[0]->getAs<types::Double>()->getRows();
                int dimsArrayOfRes[2] = {sizeOfDouble, sizeOfVector};
                types::Double* pDouble = new types::Double(2, dimsArrayOfRes);
                for (int i = 0; i < sizeOfVector; i++)
                {
                    for (int j = 0; j < sizeOfDouble; j++)
                    {
                        pDouble->set(i * sizeOfDouble + j, pIT[i]->getAs<types::Double>()->get(j));
                    }
                }
                out.push_back(pDouble);
            }
            break;
            default :
            {
                std::vector<types::InternalType*> pITTemp = std::vector<types::InternalType*>();
                pITTemp.push_back(pIT[0]);

                // sizeOfVector always > 1
                for (int i = 1; i < sizeOfVector; i++) // concatenates the Cells. ex : [String 4x1] [String 4x1] = [String 4x2]
                {
                    if (pITTemp.back()->getType() == pIT[i]->getType())
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
                                for (int k = 0; k < pIT[i]->getAs<types::String>()->getSize(); k++)
                                {
                                    pType->set(iRows * iCols + k, pIT[i]->getAs<types::String>()->get(k));
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
                                for (int k = 0; k < pIT[i]->getAs<types::Double>()->getSize(); k++)
                                {
                                    pType->set(iRows * iCols + k, pIT[i]->getAs<types::Double>()->get(k));
                                }
                                pITTemp.back();
                                pITTemp.push_back(pType);
                            }
                            break;
                            default :
                                return types::Function::Error;
                        }
                    }
                    else
                    {
                        pITTemp.push_back(pIT[i]);
                    }
                }

                int dimsArrayOfCell[2] = {1, (int)pITTemp.size()};
                types::Cell* pCell = new types::Cell(2, dimsArrayOfCell);
                for (int i = 0; i < dimsArrayOfCell[1]; i++)
                {
                    pCell->set(i, pITTemp[i]);
                }
                out.push_back(pCell);
            }
        }
    }
    Free_Scan(rowcount, ncol, type_s, &data);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
