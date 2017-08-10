/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
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


#include "scilabWrite.hxx"

extern "C"
{
#include <stdio.h>
#include "charEncoding.h"
#include "sci_malloc.h"
#include "diary.h"
#include "configvariable_interface.h"
}

static SCILAB_OUTPUT_METHOD _writer;

void setScilabOutputMethod(SCILAB_OUTPUT_METHOD writer)
{
    _writer = writer;
}

static void scilabPrint(const char* _pstText)
{
    wchar_t* pwstTemp = to_wide_string(_pstText);
    diaryWrite(pwstTemp, FALSE);
    FREE(pwstTemp);
    (*_writer)(const_cast<char*>(_pstText));
}

void scilabWrite(const char* _pstText)
{
    if (isPrintOutput())
    {
        scilabPrint(const_cast<char*>(_pstText));
    }
}

void scilabForcedWrite(const char* _pstText)
{
    scilabPrint(const_cast<char*>(_pstText));
}

void scilabWriteW(const wchar_t* _pwsText)
{
    if (isPrintOutput())
    {
        char* pstTemp = wide_string_to_UTF8(_pwsText);
        scilabWrite(pstTemp);
        FREE(pstTemp);
    }
}

void scilabForcedWriteW(const wchar_t* _pwsText)
{
    char* pstTemp = wide_string_to_UTF8(_pwsText);
    scilabForcedWrite(pstTemp);
    FREE(pstTemp);
}

void scilabError(const char* _pstText)
{
    if (isSilentError() == 0)
    {
        scilabPrint(const_cast<char*>(_pstText));
    }
}

void scilabErrorW(const wchar_t* _pwsText)
{
    if (isSilentError() == 0)
    {
        char* pstTemp = wide_string_to_UTF8(_pwsText);
        scilabPrint(pstTemp);
        FREE(pstTemp);
    }
}
