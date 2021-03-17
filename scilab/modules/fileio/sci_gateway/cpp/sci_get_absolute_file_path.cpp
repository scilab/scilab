/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Digiteo - Cedric DELAMARRE
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 * Copyright (C) 2020 - Stéphane MOTTELET
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
#include "configvariable.hxx"
#include <filesystem>

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
    wchar_t* wcsFileName = NULL;
    wchar_t** wcsFilesOpened = NULL;
    wchar_t* wcsTemp = NULL;
    wchar_t* wcsPath = NULL;

    if (in.size() > 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): at most %d expected.\n"), "get_absolute_file_path", 1);
        return types::Function::Error;
    }

    if (in.size() == 0)
    {
        if (_iRetCount > 2)
        {
            Scierror(77, _("%s: Wrong number of output argument(s): at most %d expected.\n"), "get_absolute_file_path", 2);
            return types::Function::Error;
        }

        const std::vector<ConfigVariable::WhereEntry>& where = ConfigVariable::getWhere();

        for (auto it = where.rbegin(); it != where.rend(); ++it)
        {
            if (it->m_file_name != NULL && it->m_file_name->length() > 0)
            {
                std::filesystem::path p = std::filesystem::path(it->m_file_name->c_str());
                std::filesystem::path filepath = p.parent_path() / "";
                out.push_back(new types::String(filepath.c_str()));
                if (_iRetCount == 2)
                {
                    out.push_back(new types::String(p.filename().c_str()));
                }
                // taking only the more recently pushed filename allows
                // to cope with nested script execution.
                break;
            }
        }

        if (out.size() > 0)
        {
            return types::Function::OK;
        }
        else
        {
            Scierror(999, _("%s: Without arguments, this function must be called from a script.\n"), "get_absolute_file_path");
            return types::Function::Error;
        }
    }

    if (in[0]->isString() == false || in[0]->getAs<types::String>()->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A String expected.\n"), "get_absolute_file_path", 1);
        return types::Function::Error;
    }

    wcsFileName = in[0]->getAs<types::String>()->get(0);
    wcsFilesOpened = FileManager::getFilenames();

    for (int i = FileManager::getOpenedCount() - 1; i >= 0; --i)
    {
        wcsTemp = wcsstr(wcsFilesOpened[i], wcsFileName);
        if (wcsTemp)
        {
            int iSize = (int)(wcsTemp - wcsFilesOpened[i]);
            if (wcslen(wcsFilesOpened[i]) == wcslen(wcsFileName) + iSize)
            {
                wcsPath = (wchar_t*)MALLOC((iSize + 1) * sizeof(wchar_t));
                memcpy(wcsPath, wcsFilesOpened[i], iSize * sizeof(wchar_t));
                wcsPath[iSize] = L'\0';
                types::String* pStringOut = new types::String(wcsPath);
                FREE(wcsPath);
                out.push_back(pStringOut);
                freeArrayOfWideString(wcsFilesOpened, FileManager::getOpenedCount());
                return types::Function::OK;
            }
            else
            {
                wcsTemp = NULL;
            }
        }
    }

    freeArrayOfWideString(wcsFilesOpened, FileManager::getOpenedCount());

    char* pstFile = wide_string_to_UTF8(wcsFileName);
    Scierror(999, _("%s: The file %s is not opened in scilab.\n"), "get_absolute_file_path", pstFile);
    FREE(pstFile);
    return types::Function::Error;
}
/*--------------------------------------------------------------------------*/
