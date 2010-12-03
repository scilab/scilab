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
#include "promptmode.h"

extern "C"
{
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

char *YaspRead()
{
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

void YaspWrite(const char* text)
{
    int iMode =  getPromptMode();
    char sz[256];

    //sprintf(sz, "getPromptMode() -> %d", iMode);
    //MessageBoxA(NULL, sz, NULL, 0);

    if(iMode != PROMPTMODE_SILENT)
    {
        wchar_t* pwstTemp = to_wide_string(text);
        diaryWrite(pwstTemp, FALSE);
        FREE(pwstTemp);
        (*_writer)(const_cast<char*>(text));
    }
}

void YaspWriteW(const wchar_t* text)
{
    char* pstTemp = wide_string_to_UTF8(text);
    YaspWrite(pstTemp);
    FREE(pstTemp);
}
