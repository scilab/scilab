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
