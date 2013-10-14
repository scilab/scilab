/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Antoine ELIAS
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "fileio_gw.hxx"
#include "string.hxx"
#include "mlist.hxx"
#include "filemanager.hxx"
#include "double.hxx"
#include "function.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"
#include "mgetl.h"
#include "do_xxscanf.h"
#include "scanf_functions.h"
#include "StringConvert.h"
#include "mtell.h"
#include "mseek.h"
}

types::Function::ReturnValue sci_mfscanf(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iFile                   = -1; //default file : last opened file
    int size                    = (int)in.size();
    int iNiter                  = 1;
    int iErr                    = 0;
    wchar_t* wcsFormat          = NULL;
    int dimsArray[2]            = {1, 1};
    std::vector<types::InternalType*>* pIT = new std::vector<types::InternalType*>();

    int args        = 0;
    int nrow        = 0;
    int ncol        = 0;
    int retval      = 0;
    int retval_s    = 0;
    int rowcount    = 0;
    rec_entry buf[MAXSCAN];
    entry *data;
    sfdir type[MAXSCAN], type_s[MAXSCAN];

    if (size < 2 || size > 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "mfscanf", 2, 3);
        return types::Function::Error;
    }

    if (size == 3)
    {
        if (in[0]->isDouble() == false || in[0]->getAs<types::Double>()->isScalar() == false || in[0]->getAs<types::Double>()->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A Real expected.\n"), "mfscanf", 1);
            return types::Function::Error;
        }
        iNiter = static_cast<int>(in[0]->getAs<types::Double>()->get(0));
    }

    if (in[size - 2]->isDouble() == false || in[size - 2]->getAs<types::Double>()->isScalar() == false || in[size - 2]->getAs<types::Double>()->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A Real expected.\n"), "mfscanf", size - 1);
        return types::Function::Error;
    }

    if (in[size - 1]->isString() == false || in[size - 1]->getAs<types::String>()->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A String expected.\n"), "mfscanf", size);
        return types::Function::Error;
    }

    iFile = static_cast<int>(in[size - 2]->getAs<types::Double>()->get(0));
    switch (iFile)
    {
        case 0: // stderr
        case 6: // stdout
            Scierror(999, _("%s: Wrong file descriptor: %d.\n"), "mfscanf", iFile);
            return types::Function::Error;
        default :
            break;
    }

    wcsFormat = in[size - 1]->getAs<types::String>()->get(0);
    StringConvertW(wcsFormat);

    types::File* pFile = FileManager::getFile(iFile);
    if (pFile == NULL)
    {
        Scierror(999, _("%s: Cannot read file %d.\n"), "mfscanf", iFile);
        return types::Function::Error;
    }

    // file opened with fortran open function
    if (pFile->getFileType() == 1)
    {
        Scierror(999, _("%s: Wrong file descriptor: %d.\n"), "mfscanf", iFile);
        return types::Function::Error;
    }

    FILE* fDesc = pFile->getFiledesc();
    nrow = iNiter;
    while ((iNiter == -1) || (rowcount < iNiter))
    {
        // get current position in file
        int iCurrentPos = mtell(iFile);

        // get data
        int err = do_xxscanf(L"mfscanf", fDesc, wcsFormat, &args, NULL, &retval, buf, type);
        if (err < 0)
        {
            return types::Function::Error;
        }

        err = Store_Scan(&nrow, &ncol, type_s, type, &retval, &retval_s, buf, &data, rowcount, args);
        if (err == DO_XXPRINTF_MEM_LACK)
        {
            Free_Scan(rowcount, ncol, type_s, &data);
            Scierror(999, _("%s: No more memory.\n"), "mfscanf");
            return types::Function::Error;
        }
        else if (err == DO_XXPRINTF_MISMATCH)
        {
            // go back to the last position
            // only if this line is not empty
            if (args)
            {
                mseek(iFile, iCurrentPos, SEEK_SET);
            }

            break;
        }

        rowcount++;

        // EOF reached
        if (retval == -1)
        {
            break;
        }
    }

    unsigned int uiFormatUsed = 0;
    for (int i = 0 ; i < ncol ; i++)
    {
        switch (type_s[i])
        {
            case SF_C:
            case SF_S:
            {
                types::String* ps = new types::String(rowcount, 1);
                for (int j = 0 ; j < rowcount ; j++)
                {
                    ps->set(j, data[i + ncol * j].s);
                }
                pIT->push_back(ps);
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
                types::Double* p = new types::Double(rowcount, 1);
                for (int j = 0; j < rowcount; j++)
                {
                    p->set(j, data[i + ncol * j].d);
                }
                pIT->push_back(p);
                uiFormatUsed |= (1 << 2);
            }
            break;
        }
    }

    int sizeOfVector = (int)pIT->size();
    if (_iRetCount > 1)
    {
        types::Double* pDouble = new types::Double(2, dimsArray);
        pDouble->set(0, retval_s);
        out.push_back(pDouble);

        for (int i = 0; i < sizeOfVector; i++)
        {
            out.push_back((*pIT)[i]);
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
                int sizeOfString = (*pIT)[0]->getAs<types::String>()->getRows();
                int dimsArrayOfRes[2] = {sizeOfString, sizeOfVector};
                types::String* pString = new types::String(2, dimsArrayOfRes);
                for (int i = 0; i < sizeOfVector; i++)
                {
                    for (int j = 0; j < sizeOfString; j++)
                    {
                        pString->set(i * sizeOfString + j, (*pIT)[i]->getAs<types::String>()->get(j));
                    }
                }
                out.push_back(pString);
            }
            break;
            case (1 << 2) :
            {
                int sizeOfDouble = (*pIT)[0]->getAs<types::Double>()->getRows();
                int dimsArrayOfRes[2] = {sizeOfDouble, sizeOfVector};
                types::Double* pDouble = new types::Double(2, dimsArrayOfRes);
                for (int i = 0 ; i < sizeOfVector; i++)
                {
                    for (int j = 0 ; j < sizeOfDouble; j++)
                    {
                        pDouble->set(i * sizeOfDouble + j, (*pIT)[i]->getAs<types::Double>()->get(j));
                    }
                }
                out.push_back(pDouble);
            }
            break;
            default :
            {
                std::vector<types::InternalType*>* pITTemp = new std::vector<types::InternalType*>();
                pITTemp->push_back((*pIT)[0]);

                // sizeOfVector always > 1
                for (int i = 1 ; i < sizeOfVector ; i++) // concatenates the Cells. ex : [String 4x1] [String 4x1] = [String 4x2]
                {
                    if (pITTemp->back()->getType() == (*pIT)[i]->getType())
                    {
                        switch (pITTemp->back()->getType())
                        {
                            case types::InternalType::RealString :
                            {
                                int iRows               = pITTemp->back()->getAs<types::String>()->getRows();
                                int iCols               = pITTemp->back()->getAs<types::String>()->getCols();
                                int arrayOfType[2]      = {iRows, iCols + 1};
                                types::String* pType    = new types::String(2, arrayOfType);

                                for (int k = 0 ; k < pITTemp->back()->getAs<types::String>()->getSize() ; k++)
                                {
                                    pType->set(k, pITTemp->back()->getAs<types::String>()->get(k));
                                }

                                for (int k = 0; k < (*pIT)[i]->getAs<types::String>()->getSize() ; k++)
                                {
                                    pType->set(iRows * iCols + k, (*pIT)[i]->getAs<types::String>()->get(k));
                                }
                                pITTemp->pop_back();
                                pITTemp->push_back(pType);
                            }
                            break;
                            case types::InternalType::RealDouble :
                            {
                                int iRows               = pITTemp->back()->getAs<types::Double>()->getRows();
                                int iCols               = pITTemp->back()->getAs<types::Double>()->getCols();
                                int arrayOfType[2]      = {iRows, iCols + 1};
                                types::Double* pType    = new types::Double(2, arrayOfType);

                                pType->set(pITTemp->back()->getAs<types::Double>()->get());
                                for (int k = 0; k < (*pIT)[i]->getAs<types::Double>()->getSize() ; k++)
                                {
                                    pType->set(iRows * iCols + k, (*pIT)[i]->getAs<types::Double>()->get(k));
                                }
                                pITTemp->pop_back();
                                pITTemp->push_back(pType);
                            }
                            break;
                            default :
                                return types::Function::Error;
                        }
                    }
                    else
                    {
                        pITTemp->push_back((*pIT)[i]);
                    }
                }

                types::MList* pMList = new types::MList();
                pMList->append(new types::String(L"cblock"));
                for (int i = 0 ; i < pITTemp->size() ; i++)
                {
                    pMList->append((*pITTemp)[i]);
                }
                out.push_back(pMList);

                //                int dimsArrayOfCell[2] = {1, (int)pITTemp->size()};
                //                types::Cell* pCell = new types::Cell(2, dimsArrayOfCell);
                //                for (int i = 0 ; i < pITTemp->size() ; i++)
                //                {
                //                    pCell->set(i, (*pITTemp)[i]);
                //                }
                //                out.push_back(pCell);
            }
        }
    }
    Free_Scan(rowcount, ncol, type_s, &data);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

