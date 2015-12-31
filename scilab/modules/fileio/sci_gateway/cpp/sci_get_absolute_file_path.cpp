/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Digiteo - Cedric DELAMARRE
 *
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "funcmanager.hxx"
#include "fileio_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "filemanager.hxx"

extern "C"
{
#include <stdio.h>
#include <string.h>
#include "sci_malloc.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_get_absolute_file_path(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    char* fileName = NULL;
    char** filesOpened = NULL;
    char* temp = NULL;
    char* path = NULL;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "get_absolute_file_path", 1);
        return types::Function::Error;
    }

    if (in[0]->isString() == false || in[0]->getAs<types::String>()->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A String expected.\n"), "get_absolute_file_path", 1);
        return types::Function::Error;
    }

    fileName = in[0]->getAs<types::String>()->get(0);
    filesOpened = FileManager::getFilenames();

    for (int i = FileManager::getOpenedCount() - 1; i >= 0; --i)
    {
        temp = strstr(filesOpened[i], fileName);
        if (temp)
        {
            int iSize = (int)(temp - filesOpened[i]);
            if (strlen(filesOpened[i]) == strlen(fileName) + iSize)
            {
                path = (char*)MALLOC((iSize + 1) * sizeof(char));
                memcpy(path, filesOpened[i], iSize * sizeof(char));
                path[iSize] = '\0';
                types::String* pStringOut = new types::String(path);
                FREE(path);
                out.push_back(pStringOut);
                freeArrayOfString(filesOpened, FileManager::getOpenedCount());
                return types::Function::OK;
            }
            else
            {
                temp = NULL;
            }
        }
    }

    freeArrayOfString(filesOpened, FileManager::getOpenedCount());

    if (temp == NULL)
    {
        Scierror(999, _("%s: The file %s is not opened in scilab.\n"), "get_absolute_file_path", fileName);
        return types::Function::Error;
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
