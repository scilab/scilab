/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "core_gw.hxx"
#include "types.hxx"
#include "function.hxx"
#include "string.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "md5.h"
#include "localization.h"
#include "Scierror.h"
#include "charEncoding.h"
#include "isdir.h"
#include "expandPathVariable.h"
}

types::Function::ReturnValue sci_getmd5(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    bool bStringMode = false;
    char* pstPath = NULL;

    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "getmd5", 1);
        return types::Function::Error;
    }

    if (in.size() != 1 && in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "getmd5", 1, 2);
        return types::Function::Error;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type of input argument #%d: String expected.\n"), "getmd5", 1);
        return types::Function::Error;
    }

    if (in.size() == 2 && in[1]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type of input argument #%d: String expected.\n"), "getmd5", 2);
        return types::Function::Error;
    }

    if (in.size() == 2)
    {
        if (wcscmp(in[1]->getAs<types::String>()->get(0), L"string") == 0)
        {
            bStringMode = true;
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: \"%s\" expected.\n"), "getmd5", 2, "string");
            return types::Function::Error;
        }
    }

    types::String *pIn = in[0]->getAs<types::String>();
    types::String *pOutput = new types::String(pIn->getRows(), pIn->getCols());

    for (int i = 0 ; i < pIn->getSize() ; ++i)
    {
        wchar_t *wcsCurrentIn = pIn->get(i);
        wchar_t *pstMD5 = NULL;

        if (bStringMode)
        {
            pstPath = wide_string_to_UTF8(wcsCurrentIn);
            pstMD5 = to_wide_string(md5_str(pstPath));
        }
        else
        {
            FILE *fp = NULL;
            wchar_t *real_path = NULL;

            /* Replaces SCI, ~, HOME, TMPDIR by the real path */
            real_path = expandPathVariableW(wcsCurrentIn);
            pstPath = wide_string_to_UTF8(real_path);

            /* bug 4469 */
            if (isdirW(real_path))
            {
                Scierror(999, _("%s: The file %s does not exist.\n"), "getmd5", pstPath);
                FREE(pstPath);
                delete pOutput;
                FREE(real_path);
                return types::Function::Error;
            }

            wcfopen(fp, pstPath, "rb");

            if (fp)
            {
                char* pstrFile = md5_file(fp);
                pstMD5 = to_wide_string(pstrFile);
                fclose(fp);
                FREE(pstrFile);
            }
            else
            {
                Scierror(999, _("%s: The file %s does not exist.\n"), "getmd5", pstPath);
                FREE(pstPath);
                delete pOutput;
                FREE(real_path);
                return types::Function::Error;
            }

            FREE(pstPath);
            FREE(real_path);
        }

        pOutput->set(i, pstMD5);
        FREE(pstMD5);
    }

    out.push_back(pOutput);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
