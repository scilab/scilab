/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
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
