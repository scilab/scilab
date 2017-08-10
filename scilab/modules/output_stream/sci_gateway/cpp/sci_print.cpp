/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2017 - Scilab Enterprises - ELIAS Antoine
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

#include "output_stream_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "double.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "expandPathVariable.h"
#include "mopen.h"
#include "mclose.h"
#include "mputl.h"
}

static const char fname[] = "print";

types::Function::ReturnValue sci_print(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int fid = 0;
    bool bCloseFile = false;
    wchar_t* newLine = L"";

    if (in.size() <= 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: at least %d expected.\n"), fname, 2);
        return types::Function::Error;
    }

    if (in[0]->isString())
    {
        types::String* pS = in[0]->getAs<types::String>();
        if (pS->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A single string expected.\n"), fname, 1);
            return types::Function::Error;
        }

        wchar_t *expandedFileName = expandPathVariableW(in[0]->getAs<types::String>()->get(0));

        int iErr = mopen(expandedFileName, L"wt", 0, &fid);

        if (iErr)
        {
            char* pst = wide_string_to_UTF8(expandedFileName);
            switch (iErr)
            {
                case MOPEN_NO_MORE_LOGICAL_UNIT:
                    Scierror(66, _("%s: Too many files opened!\n"), "mgetl");
                    break;
                case MOPEN_CAN_NOT_OPEN_FILE:
                    Scierror(999, _("%s: Cannot open file %s.\n"), "mgetl", pst);
                    break;
                case MOPEN_NO_MORE_MEMORY:
                    Scierror(999, _("%s: No more memory.\n"), "mgetl");
                    break;
                case MOPEN_INVALID_FILENAME:
                    Scierror(999, _("%s: invalid filename %s.\n"), "mgetl", pst);
                    break;
                default: //MOPEN_INVALID_STATUS
                    Scierror(999, _("%s: invalid status.\n"), "mgetl");
                    break;
            }

            FREE(pst);
            FREE(expandedFileName);
            return types::Function::Error;
        }
        FREE(expandedFileName);
        bCloseFile = true;

    }
    else if (in[0]->isDouble())
    {
        types::Double* pD = in[0]->getAs<types::Double>();
        if (pD->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, 1);
            return types::Function::Error;
        }

        fid = static_cast<int>(pD->get()[0]);
        //stdin
        if (fid == 5)
        {
            Scierror(999, _("%s: Wrong file descriptor: %d.\n"), fname, fid);
            return types::Function::Error;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real or String expected.\n"), fname, 1);
        return types::Function::Error;
    }

    mputl(fid, &newLine, 1);
    for (int i = (int)in.size() - 1; i > 0 ; --i)
    {
        std::wostringstream ostr;
        in[i]->toString(ostr);
        wchar_t* tmp = os_wcsdup(ostr.str().c_str());
        mputl(fid, &tmp, 1);
        FREE(tmp);
    }

    if (bCloseFile)
    {
        mclose(fid);
    }

    return types::Function::OK;
}