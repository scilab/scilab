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
#ifndef __CALLSCINOTES_H__
#define __CALLSCINOTES_H__
/*--------------------------------------------------------------------------*/
#include <wchar.h>
#include "dynlib_scinotes.h"
/*--------------------------------------------------------------------------*/
/**
* call SciNotes from C (Wide Char as input filesname)
* used by Windows for localized files name
* @param[in] _wcfilenames wide char encoding (filenames to open)
* @param[in] size of _wcfilenames
* @return 0
*/
SCINOTES_IMPEXP int callSciNotesW(wchar_t **_wcfilenames, int _nbfiles);
SCINOTES_IMPEXP int callSciNotesWWithLineNumberAndFunction(wchar_t **_wcfilenames, double* pdblLineNumber, char *functionName, int _nbfiles);
SCINOTES_IMPEXP int callSciNotesWWithOption(wchar_t **_wcfilenames, wchar_t** option, int _nboptions, int _nbfiles);
/*--------------------------------------------------------------------------*/
#endif /* __CALLSCINOTES_H__ */
