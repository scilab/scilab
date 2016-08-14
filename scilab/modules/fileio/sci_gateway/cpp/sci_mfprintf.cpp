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
#include "filemanager.hxx"
#include "fileio_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "scilab_sprintf.hxx"
#include "overload.hxx"
#include "scilabWrite.hxx"

extern "C"
{
#include <stdio.h>
#include "localization.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include "configvariable_interface.h"
#include "mputl.h"
}
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
static BOOL forceSTDERRredirect = TRUE;
#endif

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_mfprintf(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    BOOL isSTD                      = FALSE;
    BOOL isSTDErr                   = FALSE;
    int iFile                       = -1; //default file : last opened file
    unsigned int iNumberPercent     = 0;
    unsigned int iNumberCols        = 0;
    int nbrOfLines                  = 0;
    int ifileMode                   = 0;
    wchar_t* wcsInput               = NULL;
    wchar_t** wcsStringToWrite      = NULL;

    if (in.size() < 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): At least %d expected.\n"), "mfprintf", 2);
        return types::Function::Error;
    }

    if (in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A Real expected.\n"), "mfprintf", 1);
        return types::Function::Error;
    }

    types::Double* pFileId = in[0]->getAs<types::Double>();
    if (pFileId->isScalar() == false || pFileId->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A Real expected.\n"), "mfprintf", 1);
        return types::Function::Error;
    }

    if (in[1]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A String expected.\n"), "mfprintf", 2);
        return types::Function::Error;
    }

    types::String* pFileStr = in[1]->getAs<types::String>();
    if (pFileStr->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A String expected.\n"), "mfprintf", 2);
        return types::Function::Error;
    }

    for (unsigned int i = 2 ; i < in.size() ; i++)
    {
        if (in[i]->isDouble() == false && in[i]->isString() == false)
        {
            std::wstring wstFuncName = L"%" + in[i]->getShortTypeStr() + L"_mfprintf";
            return Overload::call(wstFuncName, in, _iRetCount, out);
        }
    }

    // checking ID of file
    iFile = static_cast<int>(pFileId->get(0));

    if (FileManager::getFile(iFile) == NULL)
    {
        Scierror(999, _("%s: Wrong file descriptor: %d.\n"), "mfprintf", iFile);
        return types::Function::Error;
    }

    switch (iFile)
    {
        case 0:
#ifdef _MSC_VER
            if ((getScilabMode()  == SCILAB_STD) && (forceSTDERRredirect == TRUE))
            {
                //  Console redirect stderr --> CONOUT$
                freopen("CONOUT$", "wb", stderr);
                forceSTDERRredirect = FALSE;
            }
#endif
            isSTDErr = TRUE;
            break;
        case 6:
            isSTD = TRUE;
            break;
        case 5:
            Scierror(999, _("%s: Wrong file descriptor: %d.\n"), "mfprintf", iFile);
            return types::Function::Error;
        default:
            isSTD = FALSE;
            types::File* pFile = FileManager::getFile(iFile);
            // file opened with fortran open function
            if (!pFile || pFile->getFileType() == 1)
            {
                Scierror(999, _("%s: Wrong file descriptor: %d.\n"), "mfprintf", iFile);
                return types::Function::Error;
            }
            ifileMode = pFile->getFileModeAsInt();
            break;
    }

    /* checks file mode */
    /* bug 3898 */
    /* read only attrib 1xx*/
    if ((ifileMode >= 100) && (ifileMode < 200) && ((ifileMode % 100) < 10) /* check that it is not r+ */ && !isSTD)
    {
        Scierror(999, _("%s: Wrong file mode: READ only.\n"), "mfprintf");
        return types::Function::Error;
    }

    // Checking input string to write in file
    int iNewLine = 0;
    wcsInput = pFileStr->get(0);
    wcsStringToWrite = scilab_sprintf("mfprintf", wcsInput, in, &nbrOfLines, &iNewLine);

    if (isSTD)
    {
        for (int i = 0; i < nbrOfLines; i++)
        {
            if (isSTDErr)
            {
                std::wcerr << wcsStringToWrite[i];
            }
            else
            {
                scilabForcedWriteW(wcsStringToWrite[i]);
            }
            scilabForcedWriteW(L"\n");
        }
    }
    else
    {
        int iRet = mputl(iFile, wcsStringToWrite, nbrOfLines, (BOOL)iNewLine); // FALSE = don't add the "\n" at the end.
        if (iRet)
        {
            Scierror(999, _("%s: Error while writing in file: disk full or deleted file.\n"), "mprintf");
            for (int i = 0; i < nbrOfLines; i++)
            {
                FREE(wcsStringToWrite[i]);
            }
            FREE(wcsStringToWrite);
            return types::Function::Error;
        }
    }

    for (int i = 0; i < nbrOfLines; i++)
    {
        FREE(wcsStringToWrite[i]);
    }

    FREE(wcsStringToWrite);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
