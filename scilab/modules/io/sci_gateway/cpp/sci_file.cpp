/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2009-2010 - DIGITEO - Allan CORNET
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
#include "context.hxx"
#include "io_gw.hxx"
#include "filemanager.hxx"
#include "function.hxx"
#include "string.hxx"
#include "double.hxx"

extern "C"
{
#include "os_string.h"
#include "localization.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include "FileExist.h"
#include "mclose.h"
#include "mseek.h"

    extern int C2F(clunit)(int* , char const*, int*, int);
    extern int C2F(rewindinter)(int*);
    extern int C2F(backspaceinter)(int*);
    extern int C2F(readinter)(int*, char const*, int);
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_file_no_rhs(types::typed_list &in, int _iRetCount, types::typed_list &out);
types::Function::ReturnValue sci_file_one_rhs(types::typed_list &in, int _iRetCount, types::typed_list &out);
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_file(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pSAction = NULL;

    if (in.size() > 6)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "file", 0, 6);
        return types::Function::Error;
    }

    if (in.size() == 0)
    {
        return sci_file_no_rhs(in, _iRetCount, out);
    }

    if (in.size() == 1)
    {
        return sci_file_one_rhs(in, _iRetCount, out);
    }

    // get action
    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : string expected.\n"), "file", 1);
        return types::Function::Error;
    }

    pSAction = in[0]->getAs<types::String>();

    if (pSAction->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : A single string expected.\n"), "file", 1);
        return types::Function::Error;
    }

    if (strcmp(pSAction->get(0), "open") == 0)
    {
        types::String* pSPath   = NULL;
        types::String* pSOption = NULL;
        types::Double* pSRecl   = NULL;

        int iStatus = 0;
        int iAccess = 0;
        int iForm   = 0;
        int iRecl   = 0;

        int piMode[2] = {0, 0};

        // get path
        if (in[1]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : string expected.\n"), "file", 2);
            return types::Function::Error;
        }

        pSPath = in[1]->getAs<types::String>();

        if (pSPath->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A single string expected.\n"), "file", 2);
            return types::Function::Error;
        }

        // get optional inputs
        for (int i = 2; i < in.size(); i++)
        {
            if (in[i]->isString())
            {
                pSOption = in[i]->getAs<types::String>();
            }
            else if (i != 2 && in[i]->isDouble())
            {
                pSRecl = in[i]->getAs<types::Double>();
                if (pSRecl->isScalar() == false)
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "file", i + 1);
                    return types::Function::Error;
                }

                iRecl = (int)pSRecl->get(0);
                piMode[1] = iRecl;
                continue;
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d : string expected.\n"), "file", i + 1);
                return types::Function::Error;
            }

            if (pSOption->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d : A single string expected.\n"), "file", i + 1);
                return types::Function::Error;
            }

            if (strcmp(pSOption->get(0), "new") == 0)
            {
                iStatus = 0;
            }
            else if (strcmp(pSOption->get(0), "old") == 0)
            {
                iStatus = 1;
                // file must already exists.
                if (FileExist(pSPath->get(0)) == false)
                {
                    if (_iRetCount == 2)
                    {
                        out.push_back(types::Double::Empty());
                        out.push_back(new types::Double(240));
                        return types::Function::OK;
                    }
                    else
                    {
                        Scierror(240, _("%s: The file \"%s\" does not exist.\n"), "file", pSPath->get(0));
                        return types::Function::Error;
                    }
                }
            }
            else if (strcmp(pSOption->get(0), "scratch") == 0)
            {
                iStatus = 2;
            }
            else if (strcmp(pSOption->get(0), "unknown") == 0)
            {
                iStatus = 3;
            }
            else if (strcmp(pSOption->get(0), "sequential") == 0)
            {
                iAccess = 0;
            }
            else if (strcmp(pSOption->get(0), "direct") == 0)
            {
                iAccess = 1;
            }
            else if (strcmp(pSOption->get(0), "formatted") == 0)
            {
                iForm = 0;
            }
            else if (strcmp(pSOption->get(0), "unformatted") == 0)
            {
                iForm = 1;
            }
            else
            {
                Scierror(999, _("%s: Wrong value for input argument #%d.\n"), "file", i + 1);
                return types::Function::Error;
            }
        }

        piMode[0] = iStatus + 10 * (iAccess + 10 * iForm);
        int lunit = 0; // file unit. 0 mean we open the file by this name.
        int iErr = C2F(clunit)(&lunit, pSPath->get(0), piMode, (int)strlen(pSPath->get(0)));
        if (iErr)
        {
            if (_iRetCount == 1)
            {
                switch (iErr)
                {
                    case 65  :
                        Scierror(iErr, _("%s: %d logical unit already used.\n"), "file", lunit);
                        break;
                    case 66  :
                        Scierror(iErr, _("%s: Too many files opened!\n"), "file");
                        break;
                    case 67  :
                        Scierror(iErr, _("%s: Unknown file format.\n"), "file");
                        break;
                    case 240 :
                        Scierror(iErr, _("%s: File \"%s\" already exists or directory write access denied.\n"), "file", pSPath->get(0));
                        break;
                    case 241 :
                        Scierror(iErr, _("%s: File \"%s\" does not exist or read access denied.\n"), "file", pSPath->get(0));
                        break;
                    default  :
                        Scierror(iErr, _("%s: Can not open File \"%s\"\n"), "file", pSPath->get(0));
                }

                return types::Function::Error;
            }
            else
            {
                out.push_back(types::Double::Empty());
                out.push_back(new types::Double((double)iErr));
                return types::Function::OK;
            }
        }

        out.push_back(new types::Double((double)lunit));
        if (_iRetCount == 2)
        {
            out.push_back(new types::Double(0.0));
        }
    }
    else if (strcmp(pSAction->get(0), "close") == 0 ||
             strcmp(pSAction->get(0), "rewind") == 0 ||
             strcmp(pSAction->get(0), "backspace") == 0 ||
             strcmp(pSAction->get(0), "last") == 0)
    {
        if (_iRetCount != 1)
        {
            Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "file", 1);
            return types::Function::Error;
        }

        if (in.size() != 2)
        {
            Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "file", 2);
            return types::Function::Error;
        }

        if (in[1]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A matrix expected.\n"), "file", 2);
            return types::Function::Error;
        }

        types::Double* pDblFileUnit = in[1]->getAs<types::Double>();
        double* pdblUnit = pDblFileUnit->get();

        if (strcmp(pSAction->get(0), "close") == 0)
        {
            for (int i = 0; i < pDblFileUnit->getSize(); i++)
            {
                int iErr = mclose((int)(pdblUnit[i]));
                if (iErr)
                {
                    Scierror(999, _("%s: Cannot close file %d.\n"), "file", (int)pdblUnit[i]);
                    return types::Function::Error;
                }
            }
        }
        else if (strcmp(pSAction->get(0), "rewind") == 0)
        {
            int iFileUnit = (int)pdblUnit[0];
            types::File* pFile = FileManager::getFile(iFileUnit);

            if (pFile && pFile->getFileType() == 2)
            {
                mseek(iFileUnit, 0, SEEK_SET);
            }
            else if (pFile && pFile->getFileType() == 1)
            {
                C2F(rewindinter)(&iFileUnit);
            }
            else
            {
                Scierror(999, _("%s: Unknown file format.\n"), "file");
                return types::Function::Error;
            }
        }
        else if (strcmp(pSAction->get(0), "backspace") == 0)
        {
            int iFileUnit = (int)pdblUnit[0];
            types::File* pFile = FileManager::getFile(iFileUnit);

            if (pFile && pFile->getFileType() == 2)
            {
                Scierror(999, _("%s: Wrong input argument #%d.\n"), "file", 1);
                return types::Function::Error;
            }
            else if (pFile && pFile->getFileType() == 1)
            {
                C2F(backspaceinter)(&iFileUnit);
            }
            else
            {
                Scierror(67, _("%s: Unknown file format.\n"), "file");
                return types::Function::Error;
            }
        }
        else if (strcmp(pSAction->get(0), "last") == 0)
        {
            int iFileUnit = (int)pdblUnit[0];
            types::File* pFile = FileManager::getFile(iFileUnit);

            if (pFile && pFile->getFileType() == 2)
            {
                mseek(iFileUnit, 0, SEEK_END);
            }
            else if (pFile && pFile->getFileType() == 1)
            {
                int iErr = 0;
                while (iErr == 0)
                {
                    iErr = C2F(readinter)(&iFileUnit, "(a)", 1L);
                }

                if (iErr == 2)
                {
                    Scierror(999, _("%s: \n"), "file");
                    return types::Function::Error;
                }

                C2F(backspaceinter)(&iFileUnit);
            }
            else
            {
                Scierror(67, _("%s: Unknown file format.\n"), "file");
                return types::Function::Error;
            }
        }
    }
    else
    {
        Scierror(49, _("%s: Wrong value for input argument #%d: \"%s\", \"%s\", \"%s\", \"%s\", \"%s\" \n"), "file", 1, "open", "close", "rewind", "backspace", "last");
        return types::Function::Error;
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_file_no_rhs(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iCount = FileManager::getOpenedCount();
    if (iCount == 0)
    {
        for (int i = 0 ; i < _iRetCount ; i++)
        {
            out.push_back(types::Double::Empty());
        }
        return types::Function::OK;
    }

    int* piIds = FileManager::getIDs();
    if (piIds)
    {
        types::Double *pD = new types::Double(1, iCount);
        pD->setInt(piIds);
        out.push_back(pD);
        delete[] piIds;
    }

    if (_iRetCount > 1) /*types*/
    {
        char** pstTypes = FileManager::getTypesAsString();
        if (pstTypes != NULL)
        {
            types::String* pS = new types::String(1, iCount);
            pS->set(pstTypes);
            out.push_back(pS);
            for (int i = 0 ; i < iCount ; i++)
            {
                delete[] pstTypes[i];
            }
            delete[] pstTypes;
        }
    }

    if (_iRetCount > 2) /*names*/
    {
        char** pstNames = FileManager::getFilenames();
        if (pstNames != NULL)
        {
            types::String* pS = new types::String(1, iCount);
            pS->set(pstNames);
            out.push_back(pS);
            for (int i = 0 ; i < iCount ; i++)
            {
                delete[] pstNames[i];
            }
            delete[] pstNames;
        }
    }

    if (_iRetCount > 3) /* mod */
    {
        double* pdblModes = FileManager::getModes();
        if (pdblModes != NULL)
        {
            types::Double* pD = new types::Double(1, iCount);
            pD->set(pdblModes);
            out.push_back(pD);
            delete[] pdblModes;
        }
    }

    if (_iRetCount > 4) /* swap */
    {
        double* pdblSwaps = FileManager::getSwaps();
        if (pdblSwaps != NULL)
        {
            types::Double* pD = new types::Double(1, iCount);
            pD->set(pdblSwaps);
            out.push_back(pD);
            delete[] pdblSwaps;
        }
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_file_one_rhs(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in[0]->isDouble() == false || in[0]->getAs<types::Double>()->getSize() != 1)
    {
        Scierror(201, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "file", 1);
        return types::Function::Error;
    }

    types::Double* pD = in[0]->getAs<types::Double>();
    int iID = static_cast<int>(pD->getReal()[0]);

    //check if double value is an integer to exclude decimal values
    if (static_cast<double>(iID) != pD->getReal()[0])
    {
        Scierror(201, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "file", 1);
        return types::Function::Error;
    }

    types::File *pF = FileManager::getFile(iID);
    if (pF == NULL)
    {
        for (int i = 0 ; i < _iRetCount ; i++)
        {
            out.push_back(types::Double::Empty());
        }
        return types::Function::OK;
    }

    out.push_back(new types::Double(iID));
    if (_iRetCount > 1) /*type*/
    {
        types::String* pS = new types::String(pF->getFileTypeAsString().data());
        out.push_back(pS);
    }

    if (_iRetCount > 2) /*name*/
    {
        types::String* pS = new types::String(pF->getFilename().data());
        out.push_back(pS);
    }

    if (_iRetCount > 3) /* mod */
    {
        if (pF->getFileType() == 1)
        {
            out.push_back(new types::Double((double)pF->getFileFortranMode()));
        }
        else // if(pF->getFileType() == 2)
        {
            out.push_back(new types::Double((double)pF->getFileModeAsInt()));
        }
    }

    if (_iRetCount > 4) /* swap */
    {
        out.push_back(new types::Double(pF->getFileSwap()));
    }
    return types::Function::OK;
}
