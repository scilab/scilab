/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "scilabWrite.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "promptmode.h"
#include <stdio.h>
#include "charEncoding.h"
#include "MALLOC.h"
#include "diary.h"
#include "sci_mode.h"
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
    if (isPromptShow())
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
    if (isPromptShow())
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
    if (getSilentError() == VERBOSE_ERROR)
    {
        scilabPrint(const_cast<char*>(_pstText));
    }
}

void scilabErrorW(const wchar_t* _pwsText)
{
    if (getSilentError() == VERBOSE_ERROR)
    {
        char* pstTemp = wide_string_to_UTF8(_pwsText);
        scilabPrint(pstTemp);
        FREE(pstTemp);
    }
}