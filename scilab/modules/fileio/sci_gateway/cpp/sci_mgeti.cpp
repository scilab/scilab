/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2021 - ESI Group - Clément DAVID
*
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
#include "filemanager.hxx"
#include "fileio_gw.hxx"
#include "string.hxx"
#include "double.hxx"
#include "int.hxx"
#include "file.hxx"
#include "function.hxx"

#include <memory>

extern "C"
{
#include "mget.h"
#include "localization.h"
#include "Scierror.h"
#include "charEncoding.h"
#include "os_string.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_mgeti(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    std::wstring stType(L"i");
    const wchar_t* pstType = stType.data();
    int iSize       = 0;
    int iFile       = -1; //default file : last opened file
    int iErr        = 0;

    if (in.size() < 1 || in.size() > 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "mgeti", 1, 3);
        return types::Function::Error;
    }

    //check parameter 1
    if (in[0]->isDouble() == false || in[0]->getAs<types::Double>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A positive integer value expected.\n"), "mgeti", 1);
        return types::Function::Error;
    }

    types::Double* pDoubleTest = in[0]->getAs<types::Double>();
    if ((pDoubleTest->get(0) != (int)pDoubleTest->get(0)) || (pDoubleTest->get(0) < 0))
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: A positive integer value expected.\n"), "mgeti", 1);
        return types::Function::Error;
    }

    iSize = static_cast<int>(in[0]->getAs<types::Double>()->get(0));

    if (in.size() >= 2)
    {
        //export format
        if (in[1]->isString() == false || in[1]->getAs<types::String>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "mgeti", 2);
            return types::Function::Error;
        }

        pstType = in[1]->getAs<types::String>()->get(0);
    }

    if (in.size() == 3)
    {
        if (in[2]->isDouble() == false || in[2]->getAs<types::Double>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A integer expected.\n"), "mgeti", 3);
            return types::Function::Error;
        }

        iFile = static_cast<int>(in[2]->getAs<types::Double>()->get(0));
    }

    switch (iFile)
    {
        case 0: // stderr
        case 6: // stdout
            Scierror(999, _("%s: Wrong file descriptor: %d.\n"), "mgeti", iFile);
            return types::Function::Error;
    }

    types::File* pFile = FileManager::getFile(iFile);
    // file opened with fortran open function
    if (pFile == NULL || pFile->getFileType() == 1)
    {
        Scierror(999, _("%s: Wrong file descriptor: %d.\n"), "mgeti", iFile);
        return types::Function::Error;
    }

    types::GenericType* pGT;
    void* pData;
    char* pType = wide_string_to_UTF8(pstType);
    switch (pType[0])
    {
        case 'u':
            switch (pType[1])
            {
                case 'l':
                {
                    auto* pIT = new types::UInt64(1, iSize);
                    pData = pIT->get();
                    pGT = pIT;
                    break;
                }
                case 'i':
                {
                    auto* pIT = new types::UInt32(1, iSize);
                    pData = pIT->get();
                    pGT = pIT;
                    break;
                }
                case 's':
                {
                    auto* pIT = new types::UInt16(1, iSize);
                    pData = pIT->get();
                    pGT = pIT;
                    break;
                }
                case 'c':
                {
                    auto* pIT = new types::UInt8(1, iSize);
                    pData = pIT->get();
                    pGT = pIT;
                    break;
                }
            }
            break;
        case 'd':
        {
            auto* pIT = new types::Double(1, iSize);
            pData = pIT->get();
            pGT = pIT;
            break;
        }
        case 'f':
        {
            auto* pIT = new types::Double(1, iSize);
            pData = pIT->get();
            pGT = pIT;
            break;
        }
        case 'l':
        {
            auto* pIT = new types::Int64(1, iSize);
            pData = pIT->get();
            pGT = pIT;
            break;
        }
        case 'i':
        {
            auto* pIT = new types::Int32(1, iSize);
            pData = pIT->get();
            pGT = pIT;
            break;
        }
        case 's':
        {
            auto* pIT = new types::Int16(1, iSize);
            pData = pIT->get();
            pGT = pIT;
            break;
        }
        case 'c':
        {
            auto* pIT = new types::Int8(1, iSize);
            pData = pIT->get();
            pGT = pIT;
            break;
        }
        default:
            FREE(pType);
            Scierror(999, _("%s: Wrong value for input argument #%d: Incorrect conversion code; ""c"", ""s"", ""i"" or ""l"" expected.\n"), "mgeti", 3);
            return types::Function::Error;
    }

    C2F(mgetnc)(&iFile, pData, &iSize, pType, &iErr);
    FREE(pType);
    if (iErr > 0)
    {
        pGT->killMe();
        return types::Function::Error;
    }

    if (iErr < 0) //no error
    {
        int iNewSize = (-iErr) - 1;
        if (iNewSize < iSize)
        {
            //read data are smaller then expected size, shrink
            pGT->resize(1, iNewSize);
        }
    }
    out.push_back(pGT);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
