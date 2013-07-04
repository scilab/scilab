/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SciNotes.hxx"

extern "C"
{
#include "callscinotes.h"
#include "charEncoding.h"
#include "getScilabJavaVM.h"
#include "charEncoding.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "getFullFilename.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_scinotes;
/*--------------------------------------------------------------------------*/
int callSciNotesW(wchar_t **_wcfilenames, int _nbfiles)
{
    if ( (_wcfilenames) && (_nbfiles > 0) )
    {
        int i = 0;
        for (i = 0; i < _nbfiles; i++)
        {
            if (_wcfilenames[i])
            {
                wchar_t *wcFullFilename = getFullFilenameW(_wcfilenames[i]);
                if (wcFullFilename)
                {
                    char *fullFilename = wide_string_to_UTF8(wcFullFilename);
                    if (fullFilename)
                    {
                        SciNotes::scinotes(getScilabJavaVM(), fullFilename);
                        FREE(fullFilename);
                        fullFilename = NULL;
                    }

                    FREE(wcFullFilename);
                    wcFullFilename = NULL;
                }
            }
        }
    }
    else
    {
        SciNotes::scinotes(getScilabJavaVM());
    }
    return 0;
}

int callSciNotesWWithLineNumberAndFunction(wchar_t **_wcfilenames, double* pdblLineNumber, char *functionName, int _nbfiles)
{
    if ( (_wcfilenames) && (_nbfiles > 0) )
    {
        int i = 0;
        for (i = 0; i < _nbfiles; i++)
        {
            if (_wcfilenames[i])
            {
                wchar_t *wcFullFilename = getFullFilenameW(_wcfilenames[i]);
                if (wcFullFilename)
                {
                    char *fullFilename = wide_string_to_UTF8(wcFullFilename);
                    if (fullFilename)
                    {
                        SciNotes::scinotes(getScilabJavaVM(), fullFilename, (int)pdblLineNumber[i], functionName);
                        FREE(fullFilename);
                        fullFilename = NULL;
                    }

                    FREE(wcFullFilename);
                    wcFullFilename = NULL;
                }
            }
        }
    }
    else
    {
        SciNotes::scinotes(getScilabJavaVM());
    }
    return 0;
}

int callSciNotesWWithOption(wchar_t **_wcfilenames, wchar_t** option, int _nboptions, int _nbfiles)
{
    if ( (_wcfilenames) && (_nbfiles > 0) )
    {
        int i = 0;
        char ** opt = NULL;
        if (_nboptions)
        {
            opt = new char*[_nboptions];
            for (int i = 0; i < _nboptions; i++)
            {
                opt[i] = wide_string_to_UTF8(option[i]);
            }
        }

        for (i = 0; i < _nbfiles; i++)
        {
            if (_wcfilenames[i])
            {
                wchar_t *wcFullFilename = getFullFilenameW(_wcfilenames[i]);
                if (wcFullFilename)
                {
                    char *fullFilename = wide_string_to_UTF8(wcFullFilename);
                    if (fullFilename)
                    {
                        SciNotes::scinotes(getScilabJavaVM(), fullFilename, opt, _nboptions);
                        FREE(fullFilename);
                        fullFilename = NULL;
                    }

                    FREE(wcFullFilename);
                    wcFullFilename = NULL;
                }
            }
        }
        FREE(opt);
    }
    else
    {
        SciNotes::scinotes(getScilabJavaVM());
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
