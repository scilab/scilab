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


#include "yaspio.hxx"

extern "C"
{
#include "promptmode.h"
#include <stdio.h>
#include "charEncoding.h"
#include "MALLOC.h"
#include "diary.h"
#include "prompt.h"
}

static YASP_OUTPUT _writer;
static YASP_INPUT _reader;

void setYaspOutputMethod(YASP_OUTPUT writer)
{
    _writer = writer;
}

void setYaspInputMethod(YASP_INPUT reader)
{
    _reader = reader;
}

static void YaspPrint(const char* _pstText)
{
    wchar_t* pwstTemp = to_wide_string(_pstText);
    diaryWrite(pwstTemp, FALSE);
    FREE(pwstTemp);
    (*_writer)(const_cast<char*>(_pstText));
}

char *YaspRead()
{
    //call reader
    char* pstTemp = (*_reader)();

    //add prompt to diary
    static char pstPrompt[PROMPT_SIZE_MAX];
    GetCurrentPrompt(pstPrompt);
    wchar_t* pwstPrompt = to_wide_string(pstPrompt);
    diaryWrite(pwstPrompt, TRUE);
    FREE(pwstPrompt);

    //add input to diary
    wchar_t* pwstIn = to_wide_string(pstTemp);
    diaryWriteln(pwstIn, TRUE);
    FREE(pwstIn);

    return pstTemp;
}

void YaspWrite(const char* _pstText)
{
    if(isPromptShow())
    {
        YaspPrint(const_cast<char*>(_pstText));
    }
}

void YaspForcedWrite(const char* _pstText)
{
    YaspPrint(const_cast<char*>(_pstText));
}

void YaspWriteW(const wchar_t* _pwsText)
{
    if(isPromptShow())
    {
        char* pstTemp = wide_string_to_UTF8(_pwsText);
        YaspWrite(pstTemp);
        FREE(pstTemp);
    }
}

void YaspForcedWriteW(const wchar_t* _pwsText)
{
    char* pstTemp = wide_string_to_UTF8(_pwsText);
    YaspForcedWrite(pstTemp);
    FREE(pstTemp);
}

void YaspError(const char* _pstText)
{
    if(getSilentError() == VERBOSE_ERROR)
    {
        YaspPrint(const_cast<char*>(_pstText));
    }
}

void YaspErrorW(const wchar_t* _pwsText)
{
    if(getSilentError() == VERBOSE_ERROR)
    {
        char* pstTemp = wide_string_to_UTF8(_pwsText);
        YaspPrint(pstTemp);
        FREE(pstTemp);
    }
}