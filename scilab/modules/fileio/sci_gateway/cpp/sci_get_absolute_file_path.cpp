/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Digiteo - Cedric DELAMARRE
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
