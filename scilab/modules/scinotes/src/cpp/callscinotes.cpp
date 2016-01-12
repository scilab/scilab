/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "SciNotes.hxx"

extern "C"
{
#include "callscinotes.h"
#include "charEncoding.h"
#include "getScilabJavaVM.h"
#include "charEncoding.h"
#include "sci_malloc.h"
#include "freeArrayOfString.h"
#include "getFullFilename.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_scinotes;
/*--------------------------------------------------------------------------*/
int callSciNotesW(char** _filenames, int _nbfiles)
{
    if ( (_filenames) && (_nbfiles > 0) )
    {
        int i = 0;
        for (i = 0; i < _nbfiles; i++)
        {
            if (_filenames[i])
            {
                char *cFullFilename = getFullFilename(_filenames[i]);
                if (cFullFilename)
                {
                    SciNotes::scinotes(getScilabJavaVM(), cFullFilename);
                    FREE(cFullFilename);
                    cFullFilename = NULL;
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

int callSciNotesWWithLineNumberAndFunction(char** _filenames, double* pdblLineNumber, char *functionName, int _nbfiles)
{
    if ( (_filenames) && (_nbfiles > 0) )
    {
        int i = 0;
        for (i = 0; i < _nbfiles; i++)
        {
            if (_filenames[i])
            {
                char *cFullFilename = getFullFilename(_filenames[i]);
                if (cFullFilename)
                {
                    SciNotes::scinotes(getScilabJavaVM(), cFullFilename, (int)pdblLineNumber[i], functionName);
                    FREE(cFullFilename);
                    cFullFilename = NULL;
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

int callSciNotesWWithOption(char** _filenames, char** option, int _nboptions, int _nbfiles)
{
    if ( (_filenames) && (_nbfiles > 0) )
    {
        int i = 0;
        for (i = 0; i < _nbfiles; i++)
        {
            if (_filenames[i])
            {
                char *cFullFilename = getFullFilename(_filenames[i]);
                if (cFullFilename)
                {
                    SciNotes::scinotes(getScilabJavaVM(), cFullFilename, option, _nboptions);
                    FREE(cFullFilename);
                    cFullFilename = NULL;
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
/*--------------------------------------------------------------------------*/
