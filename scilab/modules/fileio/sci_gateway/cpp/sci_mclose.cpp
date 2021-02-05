/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
 *  Copyright (C) 2012 - 2016 - Scilab Enterprises
 *  Copyright (C) 2021 - Stéphane MOTTELET
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "function.hxx"
#include "string.hxx"
#include "double.hxx"
#include "filemanager.hxx"
#include "fileio_gw.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "Scierror.h"
#include "localization.h"
#include "mclose.h"
#include "os_string.h"
#include "sciprint.h"
}

types::Function::ReturnValue sci_mclose(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblRet;
    int iRet = 0;

    if (in.size() == 0)
    {
        //close current file
        iRet = mcloseCurrentFile();
        pDblRet = new types::Double(static_cast<double>(iRet));
    }
    else if (in.size() == 1)
    {
        if (in[0]->isString())
        {
            types::String *pS = in[0]->getAs<types::String>();
            pDblRet = new types::Double(pS->getDims(),pS->getDimsArray());
            pDblRet->setZeros();

            if (pS->getSize()==1 && os_wcsicmp(pS->get(0), L"all") == 0)
            {
                iRet = mcloseAll();
                pDblRet->set(0,static_cast<double>(iRet));
            }
            else
            {
                for (int i=0; i < pS->getSize(); i++)
                {
                    if (FileManager::isOpened(pS->get(i)))
                    {
                        int iFileID = FileManager::getFileID(pS->get(i));
                        if (iFileID == -1)
                        {
                            char* pst = wide_string_to_UTF8(pS->get(i));
                            Scierror(999, _("%s: File not found: '%s'.\n"), "mclose", pst);
                            FREE(pst);
                            pDblRet->killMe();
                            return types::Function::Error;
                        }
                        iRet = mclose(iFileID);
                        pDblRet->set(i,static_cast<double>(iRet));
                    } else {
                         sciprint(_("%s: Cannot close file %ls: File is not active.\n"), "mclose", pS->get(i));
                    }
                }
            }
        }
        else if (in[0]->isDouble())
        {
            types::Double* pDblIn = in[0]->getAs<types::Double>();

            if (pDblIn->isComplex())
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "mclose", 1);
                return types::Function::Error;
            }

            pDblRet = new types::Double(pDblIn->getDims(),pDblIn->getDimsArray());
            pDblRet->setZeros();

            for (int i=0; i < pDblIn->getSize(); i++)
            {
                int iVal = static_cast<int>(pDblIn->get(i));
                switch (iVal)
                {
                    case 0: // stderr
                    case 5: // stdin
                    case 6: // stdout
                        Scierror(999, _("%s: Wrong file descriptor: %d.\n"), "mclose", iVal);
                        pDblRet->killMe();
                        return types::Function::Error;
                }
                iRet = mclose(iVal);
                pDblRet->set(i,static_cast<double>(iRet));
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A integer or string expected.\n"), "mclose", 1);
            return types::Function::Error;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d or %d expected.\n"), "mclose", 0, 1);
        return types::Function::Error;
    }

    out.push_back(pDblRet);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
