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
#include <string.h>
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
/* --- Output capture infrastructure --- */
static bool _capturing = false;
static bool _captureSilent = false;
static char* _captureBuffer = NULL;
static size_t _captureLen = 0;
static size_t _captureCap = 0;
static SCILAB_OUTPUT_METHOD _savedWriter = NULL;
static SCILAB_OUTPUT_METHOD _savedError = NULL;
static SCILAB_PRINT_METHOD _savedPrint = NULL;

static void captureAppend(const char* text)
{
    if (text == NULL) return;
    size_t len = strlen(text);
    if (len == 0) return;

    if (_captureLen + len + 1 > _captureCap)
    {
        size_t newCap = (_captureCap == 0) ? 4096 : _captureCap;
        while (newCap < _captureLen + len + 1) newCap *= 2;
        char* newBuf = (char*)REALLOC(_captureBuffer, newCap);
        if (newBuf == NULL) return;
        _captureBuffer = newBuf;
        _captureCap = newCap;
    }
    memcpy(_captureBuffer + _captureLen, text, len);
    _captureLen += len;
    _captureBuffer[_captureLen] = '\0';
}

static void captureWriter(const char* text)
{
    captureAppend(text);
    if (!_captureSilent && _savedWriter)
    {
        _savedWriter(text);
    }
}

static void captureErrorWriter(const char* text)
{
    captureAppend(text);
    if (!_captureSilent && _savedError)
    {
        _savedError(text);
    }
}

static void capturePrintWriter(const char* text, int severity)
{
    captureAppend(text);
    if (!_captureSilent && _savedPrint)
    {
        _savedPrint(text, severity);
    }
}

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

/* --- Output capture API --- */

void scilabStartOutputCapture(int silent)
{
    if (_capturing)
    {
        /* Already capturing: reset the buffer */
        _captureLen = 0;
        if (_captureBuffer) _captureBuffer[0] = '\0';
        _captureSilent = (silent != 0);
        return;
    }

    _capturing = true;
    _captureSilent = (silent != 0);
    _captureLen = 0;
    _captureCap = 0;
    _captureBuffer = NULL;

    /* Save current writers and redirect to capture */
    _savedWriter = _writer_output;
    _savedError = _writer_error;
    _savedPrint = _writer_print;
    _writer_output = captureWriter;
    _writer_error = captureErrorWriter;
    /* scilab_print prefers the severity-aware writer, so it must be intercepted too */
    _writer_print = _savedPrint ? capturePrintWriter : NULL;
}

char* scilabStopOutputCapture(void)
{
    if (!_capturing)
    {
        return NULL;
    }

    _capturing = false;

    /* Restore original writers */
    _writer_output = _savedWriter;
    _writer_error = _savedError;
    _writer_print = _savedPrint;
    _savedWriter = NULL;
    _savedError = NULL;
    _savedPrint = NULL;

    /* Return captured buffer (caller must free) */
    char* result = _captureBuffer;
    _captureBuffer = NULL;
    _captureLen = 0;
    _captureCap = 0;

    return result;
}
