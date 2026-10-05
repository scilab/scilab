/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
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

static SCILAB_OUTPUT_METHOD _writer_output = NULL;
static SCILAB_OUTPUT_METHOD _writer_error = NULL;
static SCILAB_PRINT_METHOD _writer_print = NULL;

void setScilabPrintMethod(SCILAB_PRINT_METHOD writer)
{
    _writer_print = writer;
}

/* legacy setters: the host registering last wins, so they disable a previously set severity-aware writer */
void setScilabOutputMethod(SCILAB_OUTPUT_METHOD writer)
{
    _writer_output = writer;
    _writer_error = writer;
    _writer_print = NULL;
}

void setScilabErrorStreamMethod(SCILAB_OUTPUT_METHOD writer)
{
    _writer_error = writer;
    _writer_print = NULL;
}

void setScilabOutputStreamMethod(SCILAB_OUTPUT_METHOD writer)
{
    _writer_output = writer;
    _writer_print = NULL;
}

void scilab_print(const char* _pstText, int _iSeverity)
{
    wchar_t* pwstTemp = to_wide_string(_pstText);
    diaryWrite(pwstTemp, FALSE);
    FREE(pwstTemp);

    if (_writer_print)
    {
        (*_writer_print)(_pstText, _iSeverity);
    }
    else if (_iSeverity == SCI_MSG_ERROR && _writer_error)
    {
        (*_writer_error)(_pstText);
    }
    else if (_writer_output)
    {
        (*_writer_output)(_pstText);
    }
}

void scilab_printW(const wchar_t* _pwstText, int _iSeverity)
{
    if (_pwstText && _pwstText[0] == L'\0')
    {
        return;
    }

    char* pstTemp = wide_string_to_UTF8(_pwstText);
    scilab_print(pstTemp, _iSeverity);
    FREE(pstTemp);
}

void scilabWrite(const char* _pstText)
{
    if (isPrintOutput())
    {
        scilab_print(_pstText, SCI_MSG_OUTPUT);
    }
}

void scilabForcedWrite(const char* _pstText)
{
    scilab_print(_pstText, SCI_MSG_OUTPUT);
}

void scilabWriteW(const wchar_t* _pwsText)
{
    if (isPrintOutput())
    {
        if (_pwsText && _pwsText[0] == L'\0')
        {
            return;
        }

        char* pstTemp = wide_string_to_UTF8(_pwsText);
        scilabWrite(pstTemp);
        FREE(pstTemp);
    }
}

void scilabForcedWriteW(const wchar_t* _pwsText)
{
    if (_pwsText && _pwsText[0] == L'\0')
    {
        return;
    }

    char* pstTemp = wide_string_to_UTF8(_pwsText);
    scilabForcedWrite(pstTemp);
    FREE(pstTemp);
}

void scilabError(const char* _pstText)
{
    if (isSilentError() == 0)
    {
        scilab_print(_pstText, SCI_MSG_ERROR);
    }
}

void scilabErrorW(const wchar_t* _pwsText)
{
    if (isSilentError() == 0)
    {
        char* pstTemp = wide_string_to_UTF8(_pwsText);
        scilab_print(pstTemp, SCI_MSG_ERROR);
        FREE(pstTemp);
    }
}

void scilabWarning(const char* _pstText)
{
    scilab_print(_pstText, SCI_MSG_WARNING);
}

void scilabWarningW(const wchar_t* _pwsText)
{
    if (_pwsText && _pwsText[0] == L'\0')
    {
        return;
    }

    char* pstTemp = wide_string_to_UTF8(_pwsText);
    scilab_print(pstTemp, SCI_MSG_WARNING);
    FREE(pstTemp);
}
