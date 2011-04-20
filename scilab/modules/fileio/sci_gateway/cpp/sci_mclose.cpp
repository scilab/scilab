/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "function.hxx"
#include "string.hxx"
#include "filemanager.hxx"
#include "fileio_gw.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "mclose.h"
#include "os_wcsicmp.h"
}
#include "stdio.h"

types::Function::ReturnValue sci_mclose(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iRet = 0;
    if(in.size() == 0)
    {//close current file
        iRet = mcloseCurrentFile();
    }
    else if(in.size() == 1)
    {
        if(in[0]->isString())
        {
            types::String *pS = in[0]->getAs<types::String>();
            if(pS->getSize() != 1)
            {
                ScierrorW(999,_W("%ls: Wrong type for input argument #%d: A String expected.\n"), L"mclose", 1);
                return types::Function::Error;
            }

            if(FileManager::isOpened(pS->get(0)))
            {
                int iFileID = FileManager::getFileID(pS->get(0));
                if(iFileID == -1)
                {
                    ScierrorW(999, _W("%ls: File not found: '%ls'.\n"), L"mclose", pS->get(0));
                    return types::Function::Error;
                }
                iRet = mclose(iFileID);
            }
            else if(os_wcsicmp(pS->get(0), L"all") == 0)
            {
                iRet = mcloseAll();
            }
            else
            {
                ScierrorW(999, _W("%ls: Wrong input arguments: '%ls' expected.\n"), L"mclose", L"all");
                return types::Function::Error;
            }
        }
        else if(in[0]->isDouble())
        {
            types::Double* pD = in[0]->getAs<types::Double>();
            if(pD->getSize() != 1 || pD->isComplex())
            {
                ScierrorW(999,_W("%ls: Wrong type for input argument #%d: A real expected.\n"), L"mclose", 1);
                return types::Function::Error;
            }

            int iVal = static_cast<int>(pD->get(0));
            switch (iVal)
            {
            case 0: // stderr
            case 5: // stdin
            case 6: // stdout
                ScierrorW(999, _W("%ls: Wrong file descriptor: %d.\n"), L"mclose", iVal);
                return types::Function::Error;
            }
            iRet = mclose(iVal);
        }
        else
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A integer or string expected.\n"), L"mclose", 1);
            return types::Function::Error;
        }
    }
    else
    {
        ScierrorW(999, _W("%ls: Wrong number of input arguments: %d or %d expected.\n"), L"mclose", 0, 1);
        return types::Function::Error;
    }

    types::Double* pD = new types::Double(static_cast<double>(iRet));
    out.push_back(pD);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
