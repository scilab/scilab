/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2005-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
* Copyright (C) 2005-2008 - INRIA - Pierrick MODE
* Copyright (C) 2007-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
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

#include "spreadsheet_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "double.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"
#include "xls.h"
#include "ole.h"
#include "ripole.h"
#include "expandPathVariable.h"
#include "mopen.h"
#include "sci_tmpdir.h"
#include "freeArrayOfString.h"
#include "FileExist.h"
}
/*--------------------------------------------------------------------------*/
static wchar_t* xls_basename(wchar_t* name);
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_xls_open(types::typed_list &in, int _iRetCount, types::typed_list &out)
{

#undef IN
#define max_char_xls_open 256

    // input
    types::String* pStrPath = NULL;
    wchar_t* filename_IN    = NULL;

    wchar_t TMP[max_char_xls_open];
    wchar_t* TMPDIR = NULL;

    char **sst          = NULL;
    char **Sheetnames   = NULL;

    int iId     = 0;
    int iSwap   = 0;
    int iErr    = 0;
    int ns      = 0;
    int* Abspos = NULL;
    int nsheets = 0;

    wchar_t sep[2];
#ifdef _MSC_VER
    sep[0] = L'\\';
#else
    sep[0] = L'/';
#endif
    sep[1] = L'\0';

    // *** check the minimal number of input args. ***
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "xls_open", 1);
        return types::Function::Error;
    }

    // *** check number of output args according the methode. ***
    if (_iRetCount != 4)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "xls_open", 4);
        return types::Function::Error;
    }

    // *** check type of input args and get it. ***
    // path
    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : string expected.\n"), "xls_open", 1);
        return types::Function::Error;
    }

    pStrPath = in[0]->getAs<types::String>();
    filename_IN = expandPathVariableW(pStrPath->get(0));

    // *** Perform operation. ***
    if (filename_IN)
    {
        /* bug 5615 */
        /* remove blank characters @ the end */
        int len = (int)wcslen(filename_IN);
        int i = 0;

        if (len >= 1)
        {
            for (i = len - 1; i >= 0  ; i--)
            {
                if (filename_IN[i] != ' ')
                {
                    break;
                }

                filename_IN[i] = '\0';
            }
        }

        if (FileExistW(filename_IN) == false)
        {
            char* pstFile = wide_string_to_UTF8(filename_IN);
            Scierror(999, _("The file %s does not exist.\n"), pstFile);
            FREE(pstFile);
            FREE(filename_IN);
            filename_IN = NULL;
            return types::Function::Error;
        }
    }
    else
    {
        Scierror(999, _("%s: Cannot read file name.\n"), "xls_open");
        return types::Function::Error;
    }

    TMPDIR = getTMPDIRW();
    wcscpy(TMP, TMPDIR);

    FREE(TMPDIR);
    TMPDIR = NULL;

    wcscat(TMP, sep);
    wcscat(TMP, xls_basename(filename_IN));

    char* tmp = wide_string_to_UTF8(TMP);
    char* filename_in = wide_string_to_UTF8(filename_IN);
    int result = ripole(filename_in, tmp, 0, 0);
    free(tmp);
    free(filename_in);

    if (result != OLE_OK)
    {
        char* pstFile = wide_string_to_UTF8(filename_IN);
        if (result == OLEER_NO_INPUT_FILE)
        {
            Scierror(999, _("%s: The file %s does not exist.\n"), "xls_open", pstFile);
        }
        else if (result == OLEER_NOT_OLE_FILE ||
                 result == OLEER_INSANE_OLE_FILE ||
                 result == OLEER_LOADFAT_BAD_BOUNDARY ||
                 result == OLEER_MINIFAT_READ_FAIL ||
                 result == OLEER_PROPERTIES_READ_FAIL)
        {
            Scierror(999, _("%s: File %s is not an ole2 file.\n"), "xls_open", pstFile);
        }
        else if (result == -1)
        {
            Scierror(999, _("%s: Cannot open file %s.\n"), "xls_open", pstFile);
        }

        FREE(filename_IN);
        filename_IN = NULL;
        FREE(pstFile);
        return types::Function::Error;
    }

    wcscat(TMP, sep);
    wcscat(TMP, L"Workbook");

    iErr = mopen(TMP, L"rb", iSwap, &iId);
    if (iErr != MOPEN_NO_ERROR)
    {
        sciprint(_("There is no xls stream in the ole2 file %ls.\n"), filename_IN);
        if (iErr == MOPEN_CAN_NOT_OPEN_FILE)
        {
            char* pstTMP = wide_string_to_UTF8(TMP);
            Scierror(999, _("%s: Cannot open file %s.\n"), "xls_open", pstTMP);
            FREE(pstTMP);
        }
        else if (iErr == MOPEN_INVALID_FILENAME)
        {
            Scierror(999, _("%s: invalid filename.\n"), "xls_open");
        }
        else //if(iErr == MOPEN_INVALID_STATUS)
        {
            Scierror(999, _("%s: invalid status.\n"), "xls_open");
        }

        FREE(filename_IN);
        return types::Function::Error;
    }

    FREE(filename_IN);
    filename_IN = NULL;

    xls_open(&iErr, &iId, &sst , &ns, &Sheetnames, &Abspos, &nsheets);

    /*return *err:
    0 = OK
    1 = not an OLE file
    2 = no Workbook included
    3 = memory allocation problem
    4 = incorrect file
    5 = not a BIFF8 xls file
    */
    switch (iErr)
    {
        case 0:
            /* OK */
            break;

        case 1:
            Scierror(999, _("%s: Not an ole2 file.\n"), "xls_open");
            return types::Function::Error;
            break;

        case 2:
            Scierror(999, _("%s: The file has no Workbook directory.\n"), "xls_open");
            return types::Function::Error;
            break;

        case 3:
            Scierror(999, _("%s: No more memory.\n"), "xls_open");
            return types::Function::Error;
            break;

        case 4:
            Scierror(990, _("%s: Incorrect or corrupted file.\n"), "xls_open");
            return types::Function::Error;
            break;

        case 5:
            Scierror(999, _("%s: Only BIFF8 file format is handled.\n"), "xls_open");
            return types::Function::Error;
            break;

        default :
            break;
    }

    // *** Return result in Scilab. ***
    types::Double* pDblId = new types::Double(static_cast<double>(iId));
    out.push_back(pDblId);

    if (ns)
    {
        /* Create a typed list to return the properties */
        types::String* pStrSst = new types::String(1, ns);
        for (int i = 0; i < ns; i++)
        {
            wchar_t* wcssst = to_wide_string(sst[i]);
            pStrSst->set(i, wcssst);
            free(wcssst);
        }
        freeArrayOfString(sst, ns);
        out.push_back(pStrSst);
    }
    else
    {
        out.push_back(types::Double::Empty());
    }

    if (nsheets)
    {
        /* Create a typed list to return the properties */
        types::String* pStrSheets = new types::String(1, nsheets);
        for (int i = 0; i < nsheets; i++)
        {
            wchar_t* wcssheet = to_wide_string(Sheetnames[i]);
            pStrSheets->set(i, wcssheet);
            free(wcssheet);
        }
        freeArrayOfString(Sheetnames, nsheets);
        out.push_back(pStrSheets);

        if (Abspos)
        {
            types::Double* pDblAbsPos = new types::Double(1, nsheets);
            for (int i = 0; i < nsheets; i++)
            {
                pDblAbsPos->set(i, static_cast<double>(Abspos[i]));
            }
            out.push_back(pDblAbsPos);
            free(Abspos);
            Abspos = NULL;
        }
        else
        {
            out.push_back(types::Double::Empty());
        }
    }
    else
    {
        out.push_back(types::Double::Empty());
        out.push_back(types::Double::Empty());
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
static wchar_t* xls_basename(wchar_t* name)
{
    wchar_t* base = NULL;
#ifdef _MSC_VER
    base = wcsrchr (name, L'\\');
#else
    base = wcsrchr (name, L'/');
#endif
    return base ? base + 1 : name;
}
/*--------------------------------------------------------------------------*/
